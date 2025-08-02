#include "imu_handler.h"
#include <math.h>

IMUHandler::IMUHandler() : i2c(nullptr), initialized(false), hasNewData(false),
                           accelScale(1.0/16384.0), gyroScale(1.0/131.0), 
                           rotationOffset(0.0), currentRotation(0.0), 
                           lastUpdateTime(0) {
}

IMUHandler::~IMUHandler() {
}

bool IMUHandler::init() {
    Serial.println("Initializing IMU sensor...");
    
    // Initialize I2C with pins for ESP32-S3 Touch AMOLED (Official Waveshare pinout)
    i2c = &Wire;
    i2c->begin(47, 48); // SDA=GPIO47, SCL=GPIO48
    i2c->setClock(400000); // 400kHz
    
    delay(10);
    
    // Check WHO_AM_I register
    uint8_t whoami = readRegister(QMI8658_WHO_AM_I);
    if (whoami != 0x05) {
        Serial.printf("IMU WHO_AM_I failed: 0x%02X (expected 0x05)\n", whoami);
        return false;
    }
    
    // Reset the device
    writeRegister(QMI8658_CTRL1, 0x60);
    delay(10);
    
    // Configure accelerometer
    // CTRL2: Accelerometer enable, ±2g range, 100Hz ODR
    writeRegister(QMI8658_CTRL2, 0x05);
    
    // Configure gyroscope  
    // CTRL3: Gyroscope enable, ±500dps range, 100Hz ODR
    writeRegister(QMI8658_CTRL3, 0x55);
    
    // Enable accelerometer and gyroscope
    writeRegister(QMI8658_CTRL7, 0x03);
    
    delay(50);
    
    // Initial calibration
    calibrate();
    
    initialized = true;
    lastUpdateTime = millis();
    
    Serial.println("IMU sensor initialized successfully");
    return true;
}

bool IMUHandler::writeRegister(uint8_t reg, uint8_t value) {
    i2c->beginTransmission(QMI8658_I2C_ADDR);
    i2c->write(reg);
    i2c->write(value);
    return (i2c->endTransmission() == 0);
}

uint8_t IMUHandler::readRegister(uint8_t reg) {
    i2c->beginTransmission(QMI8658_I2C_ADDR);
    i2c->write(reg);
    i2c->endTransmission(false);
    
    i2c->requestFrom(QMI8658_I2C_ADDR, 1);
    if (i2c->available()) {
        return i2c->read();
    }
    return 0;
}

int16_t IMUHandler::readRegister16(uint8_t regLow, uint8_t regHigh) {
    uint8_t low = readRegister(regLow);
    uint8_t high = readRegister(regHigh);
    return (int16_t)((high << 8) | low);
}

bool IMUHandler::readData() {
    if (!initialized) return false;
    
    // Read accelerometer data
    int16_t ax = readRegister16(QMI8658_AX_L, QMI8658_AX_H);
    int16_t ay = readRegister16(QMI8658_AY_L, QMI8658_AY_H);
    int16_t az = readRegister16(QMI8658_AZ_L, QMI8658_AZ_H);
    
    // Read gyroscope data
    int16_t gx = readRegister16(QMI8658_GX_L, QMI8658_GX_H);
    int16_t gy = readRegister16(QMI8658_GY_L, QMI8658_GY_H);
    int16_t gz = readRegister16(QMI8658_GZ_L, QMI8658_GZ_H);
    
    // Convert to physical units
    currentData.accelX = ax * accelScale;
    currentData.accelY = ay * accelScale;
    currentData.accelZ = az * accelScale;
    
    currentData.gyroX = gx * gyroScale;
    currentData.gyroY = gy * gyroScale;
    currentData.gyroZ = gz * gyroScale;
    
    // Calculate rotation
    unsigned long now = millis();
    float deltaTime = (now - lastUpdateTime) / 1000.0;
    if (deltaTime > 0) {
        updateRotation(currentData.gyroZ, deltaTime);
        lastUpdateTime = now;
    }
    
    currentData.rotation = currentRotation;
    currentData.isValid = true;
    hasNewData = true;
    
    return true;
}

void IMUHandler::updateRotation(float gyroZ, float deltaTime) {
    // Integrate gyroscope data to get rotation angle
    currentRotation += (gyroZ - rotationOffset) * deltaTime;
    
    // Keep rotation within 0-360 degrees
    while (currentRotation < 0) currentRotation += 360.0;
    while (currentRotation >= 360.0) currentRotation -= 360.0;
    
    // Optional: Use accelerometer for drift correction
    float tiltAngle = calculateTiltAngle(currentData.accelX, currentData.accelY, currentData.accelZ);
    if (abs(tiltAngle) < 30.0) { // Only correct if device is relatively upright
        // Simple complementary filter for drift correction
        currentRotation = 0.98 * currentRotation + 0.02 * tiltAngle;
    }
}

float IMUHandler::calculateTiltAngle(float accelX, float accelY, float accelZ) {
    // Calculate tilt angle from accelerometer data
    return atan2(accelY, accelX) * 180.0 / PI;
}

IMUData IMUHandler::getData() {
    hasNewData = false;
    return currentData;
}

float IMUHandler::getRotation() {
    hasNewData = false;
    return currentRotation;
}

void IMUHandler::resetRotation() {
    currentRotation = 0.0;
}

void IMUHandler::calibrate() {
    if (!initialized) return;
    
    Serial.println("Calibrating IMU... keep device still");
    
    float gyroZSum = 0;
    int samples = 100;
    
    for (int i = 0; i < samples; i++) {
        int16_t gz = readRegister16(QMI8658_GZ_L, QMI8658_GZ_H);
        gyroZSum += gz * gyroScale;
        delay(10);
    }
    
    rotationOffset = gyroZSum / samples;
    
    Serial.printf("IMU calibration complete. Gyro Z offset: %.3f\n", rotationOffset);
}