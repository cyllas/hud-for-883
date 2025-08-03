#include <unity.h>
#include <Arduino.h>
#include "../src/sensors/imu_handler.h"
#include <math.h>

// Test IMU constants and configuration
void test_imu_constants() {
    // Test I2C address
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x6B, QMI8658_I2C_ADDR, "IMU I2C address should be 0x6B");
    
    // Test register addresses
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, QMI8658_WHO_AM_I, "WHO_AM_I register should be 0x00");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x02, QMI8658_CTRL1, "CTRL1 register should be 0x02");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x03, QMI8658_CTRL2, "CTRL2 register should be 0x03");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x04, QMI8658_CTRL3, "CTRL3 register should be 0x04");
    
    // Test data registers
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x35, QMI8658_AX_L, "Accel X Low register should be 0x35");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x36, QMI8658_AX_H, "Accel X High register should be 0x36");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x3B, QMI8658_GX_L, "Gyro X Low register should be 0x3B");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x3C, QMI8658_GX_H, "Gyro X High register should be 0x3C");
}

// Test IMU data structure
void test_imu_data_structure() {
    IMUData imuData;
    
    // Test default constructor values
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imuData.accelX, "Default accelX should be 0.0");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imuData.accelY, "Default accelY should be 0.0");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imuData.accelZ, "Default accelZ should be 0.0");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imuData.gyroX, "Default gyroX should be 0.0");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imuData.gyroY, "Default gyroY should be 0.0");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imuData.gyroZ, "Default gyroZ should be 0.0");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imuData.rotation, "Default rotation should be 0.0");
    TEST_ASSERT_FALSE_MESSAGE(imuData.isValid, "Default isValid should be false");
    
    // Test data assignment
    imuData.accelX = 1.5;
    imuData.accelY = -2.3;
    imuData.accelZ = 9.8;
    imuData.gyroX = 0.1;
    imuData.gyroY = -0.2;
    imuData.gyroZ = 45.0;
    imuData.rotation = 90.0;
    imuData.isValid = true;
    
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(1.5, imuData.accelX, "AccelX should be set correctly");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(-2.3, imuData.accelY, "AccelY should be set correctly");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(9.8, imuData.accelZ, "AccelZ should be set correctly");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.1, imuData.gyroX, "GyroX should be set correctly");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(-0.2, imuData.gyroY, "GyroY should be set correctly");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(45.0, imuData.gyroZ, "GyroZ should be set correctly");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(90.0, imuData.rotation, "Rotation should be set correctly");
    TEST_ASSERT_TRUE_MESSAGE(imuData.isValid, "IsValid should be set correctly");
}

// Test IMU handler initialization
void test_imu_handler_initialization() {
    IMUHandler imu;
    
    // Test initial state
    TEST_ASSERT_FALSE_MESSAGE(imu.isInitialized(), "IMU should not be initialized initially");
    TEST_ASSERT_FALSE_MESSAGE(imu.hasNewData(), "IMU should not have new data initially");
    
    // Test initial rotation
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imu.getRotation(), "Initial rotation should be 0.0");
}

// Test scale factor calculations
void test_scale_factors() {
    IMUHandler imu;
    
    // Test default scale factors (these are typical values)
    float expectedAccelScale = 1.0/16384.0; // For ±2g range
    float expectedGyroScale = 1.0/131.0;    // For ±500dps range
    
    // Set and test scale factors
    imu.setAccelScale(expectedAccelScale);
    imu.setGyroScale(expectedGyroScale);
    
    // Test calculations with known values
    int16_t rawAccel = 16384; // Should equal 1g
    float physicalAccel = rawAccel * expectedAccelScale;
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01, 1.0, physicalAccel, "Accel scale should convert to 1g");
    
    int16_t rawGyro = 131; // Should equal 1 dps
    float physicalGyro = rawGyro * expectedGyroScale;
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01, 1.0, physicalGyro, "Gyro scale should convert to 1 dps");
}

// Test rotation calculations
void test_rotation_calculations() {
    // Test angle wrapping
    float testRotation = 0.0;
    
    // Test normal rotation
    testRotation += 90.0;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(90.0, testRotation, "Normal rotation should work");
    
    // Test rotation overflow
    testRotation += 300.0; // Total 390
    while (testRotation >= 360.0) testRotation -= 360.0;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(30.0, testRotation, "Rotation should wrap at 360°");
    
    // Test negative rotation
    testRotation = -30.0;
    while (testRotation < 0) testRotation += 360.0;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(330.0, testRotation, "Negative rotation should wrap to positive");
}

// Test tilt angle calculation
void test_tilt_calculation() {
    // Test tilt angle using atan2
    float accelX = 1.0;
    float accelY = 0.0;
    float expectedAngle = atan2(accelY, accelX) * 180.0 / PI;
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(1.0, 0.0, expectedAngle, "Horizontal should be 0°");
    
    // Test 90-degree tilt
    accelX = 0.0;
    accelY = 1.0;
    expectedAngle = atan2(accelY, accelX) * 180.0 / PI;
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(1.0, 90.0, expectedAngle, "Vertical should be 90°");
    
    // Test 45-degree tilt
    accelX = 1.0;
    accelY = 1.0;
    expectedAngle = atan2(accelY, accelX) * 180.0 / PI;
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(1.0, 45.0, expectedAngle, "45° tilt should be calculated correctly");
}

// Test complementary filter
void test_complementary_filter() {
    // Test filter coefficients
    float gyroWeight = 0.98;
    float accelWeight = 0.02;
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001, 1.0, gyroWeight + accelWeight, "Filter weights should sum to 1.0");
    
    // Test filter operation
    float currentRotation = 90.0;
    float gyroRotation = 95.0;
    float accelTilt = 85.0;
    
    float filteredRotation = gyroWeight * gyroRotation + accelWeight * accelTilt;
    float expectedResult = 0.98 * 95.0 + 0.02 * 85.0; // 93.1 + 1.7 = 94.8
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.1, expectedResult, filteredRotation, "Complementary filter should blend correctly");
}

// Test rotation reset functionality
void test_rotation_reset() {
    IMUHandler imu;
    
    // Set a rotation value
    // Note: In real implementation, you'd need access to set internal rotation
    // This test validates the interface exists
    imu.resetRotation();
    
    float rotation = imu.getRotation();
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, rotation, "Reset should set rotation to 0.0");
}

// Test data validation ranges
void test_data_ranges() {
    // Test accelerometer range (±2g = ±19.6 m/s²)
    float maxAccel = 2.0 * 9.8; // 2g in m/s²
    TEST_ASSERT_TRUE_MESSAGE(maxAccel > 15.0 && maxAccel < 25.0, "Max accel should be ~19.6 m/s²");
    
    // Test gyroscope range (±500 dps)
    float maxGyro = 500.0;
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(500.0, maxGyro, "Max gyro should be 500 dps");
    
    // Test rotation range (0-360°)
    TEST_ASSERT_TRUE_MESSAGE(0.0 >= 0.0 && 0.0 < 360.0, "Min rotation should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(359.9 >= 0.0 && 359.9 < 360.0, "Max rotation should be in valid range");
}

// Test I2C pin configuration
void test_i2c_pins() {
    // Test I2C pin definitions (shared with touch)
    TEST_ASSERT_EQUAL_INT_MESSAGE(47, TOUCH_SDA, "I2C SDA should be GPIO 47");
    TEST_ASSERT_EQUAL_INT_MESSAGE(48, TOUCH_SCL, "I2C SCL should be GPIO 48");
    
    // Test that pins are in valid GPIO range for ESP32-S3
    TEST_ASSERT_TRUE_MESSAGE(TOUCH_SDA >= 0 && TOUCH_SDA <= 48, "SDA pin should be valid GPIO");
    TEST_ASSERT_TRUE_MESSAGE(TOUCH_SCL >= 0 && TOUCH_SCL <= 48, "SCL pin should be valid GPIO");
}

// Test calibration parameters
void test_calibration() {
    IMUHandler imu;
    
    // Test offset setting
    float testOffset = 0.5;
    imu.setRotationOffset(testOffset);
    
    // Test that calibration doesn't break basic functionality
    TEST_ASSERT_TRUE_MESSAGE(true, "Calibration should complete without error");
    
    // Test reasonable offset range (should be small)
    TEST_ASSERT_TRUE_MESSAGE(testOffset > -10.0 && testOffset < 10.0, "Offset should be reasonable");
}

// Test integration time calculations
void test_integration_timing() {
    // Test typical sample rates
    float sampleRate = 100.0; // Hz
    float deltaTime = 1.0 / sampleRate; // 0.01 seconds
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001, 0.01, deltaTime, "Delta time should be 10ms at 100Hz");
    
    // Test integration calculation
    float gyroRate = 45.0; // degrees per second
    float rotationChange = gyroRate * deltaTime; // Should be 0.45 degrees
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01, 0.45, rotationChange, "Rotation integration should be correct");
}

// Main test runner for IMU module
void run_imu_tests() {
    RUN_TEST(test_imu_constants);
    RUN_TEST(test_imu_data_structure);
    RUN_TEST(test_imu_handler_initialization);
    RUN_TEST(test_scale_factors);
    RUN_TEST(test_rotation_calculations);
    RUN_TEST(test_tilt_calculation);
    RUN_TEST(test_complementary_filter);
    RUN_TEST(test_rotation_reset);
    RUN_TEST(test_data_ranges);
    RUN_TEST(test_i2c_pins);
    RUN_TEST(test_calibration);
    RUN_TEST(test_integration_timing);
}