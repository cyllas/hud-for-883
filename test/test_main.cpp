#include <unity.h>
#include <Arduino.h>

// Test runner function
void setUp(void) {
    // Set up code before each test
}

void tearDown(void) {
    // Clean up code after each test
}

// Include all test modules
void run_ble_tests();
void run_display_tests(); 
void run_ui_tests();
void run_imu_tests();
void run_settings_tests();
void run_integration_tests();

void setup() {
    delay(2000); // Wait for Serial Monitor
    
    UNITY_BEGIN();
    
    // Run all test suites
    run_ble_tests();
    run_display_tests();
    run_ui_tests(); 
    run_imu_tests();
    run_settings_tests();
    run_integration_tests();
    
    UNITY_END();
}

void loop() {
    // Empty loop for unit tests
}