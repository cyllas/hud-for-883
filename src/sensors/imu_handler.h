#ifndef IMU_HANDLER_H
#define IMU_HANDLER_H

#include <Arduino.h>
#include <Wire.h>

// QMI8658 IMU sensor I2C address and registers
#define QMI8658_I2C_ADDR    0x6B
#define QMI8658_WHO_AM_I    0x00
#define QMI8658_REVISION    0x01
#define QMI8658_CTRL1       0x02
#define QMI8658_CTRL2       0x03
#define QMI8658_CTRL3       0x04
#define QMI8658_CTRL4       0x05
#define QMI8658_CTRL5       0x06
#define QMI8658_CTRL6       0x07
#define QMI8658_CTRL7       0x08
#define QMI8658_STATUSINT   0x2D
#define QMI8658_STATUS0     0x2E
#define QMI8658_STATUS1     0x2F

// Accelerometer data registers
#define QMI8658_AX_L        0x35
#define QMI8658_AX_H        0x36
#define QMI8658_AY_L        0x37
#define QMI8658_AY_H        0x38
#define QMI8658_AZ_L        0x39
#define QMI8658_AZ_H        0x3A

// Gyroscope data registers
#define QMI8658_GX_L        0x3B
#define QMI8658_GX_H        0x3C
#define QMI8658_GY_L        0x3D
#define QMI8658_GY_H        0x3E
#define QMI8658_GZ_L        0x3F
#define QMI8658_GZ_H        0x40

struct IMUData {
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float rotation;  // Calculated rotation angle
    bool isValid;
    
    IMUData() : accelX(0), accelY(0), accelZ(0), 
                gyroX(0), gyroY(0), gyroZ(0), 
                rotation(0), isValid(false) {}
};

class IMUHandler {
private:
    TwoWire* i2c;
    IMUData currentData;
    bool initialized;
    bool hasNewData;
    
    // Calibration values
    float accelScale;
    float gyroScale;
    float rotationOffset;
    
    // Internal state for rotation calculation
    float currentRotation;
    unsigned long lastUpdateTime;
    
    // I2C communication
    bool writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    int16_t readRegister16(uint8_t regLow, uint8_t regHigh);
    
    // Data processing
    void updateRotation(float gyroZ, float deltaTime);
    float calculateTiltAngle(float accelX, float accelY, float accelZ);
    
public:
    IMUHandler();
    ~IMUHandler();
    
    bool init();
    bool isInitialized() const { return initialized; }
    
    // Data reading
    bool readData();
    bool hasNewData() const { return hasNewData; }
    IMUData getData();
    
    // Rotation specific functions
    float getRotation();
    void resetRotation();
    void calibrate();
    
    // Configuration
    void setAccelScale(float scale) { accelScale = scale; }
    void setGyroScale(float scale) { gyroScale = scale; }
    void setRotationOffset(float offset) { rotationOffset = offset; }
};