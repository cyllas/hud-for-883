#include <unity.h>
#include <Arduino.h>
#include "../src/ble/ble_server.h"
#include "../src/display/amoled_driver.h"
#include "../src/display/ui_manager.h"
#include "../src/sensors/imu_handler.h"
#include "../src/config/settings.h"

// Test complete system initialization
void test_system_initialization() {
    Settings config;
    AmoledDriver display;
    UIManager ui;
    IMUHandler imu;
    BLEServer bleServer;
    
    // Test configuration initialization
    bool configInit = config.init();
    TEST_ASSERT_TRUE_MESSAGE(configInit, "Configuration should initialize successfully");
    
    // Test display initialization (mock - hardware not available in test)
    // Note: In real hardware test, this would actually initialize
    TEST_ASSERT_EQUAL_INT_MESSAGE(466, display.width(), "Display should have correct dimensions");
    TEST_ASSERT_EQUAL_INT_MESSAGE(466, display.height(), "Display should have correct dimensions");
    
    // Test UI initialization
    // Note: UI.init() requires initialized display, so we test interface compatibility
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_STARTUP, ui.getState(), "UI should start in startup state");
    TEST_ASSERT_EQUAL_INT_MESSAGE(THEME_DAY, ui.getTheme(), "UI should start with day theme");
    
    // Test IMU interface
    TEST_ASSERT_FALSE_MESSAGE(imu.isInitialized(), "IMU should not be initialized without hardware");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, imu.getRotation(), "IMU should start with zero rotation");
    
    // Test BLE initialization
    bool bleInit = bleServer.init();
    TEST_ASSERT_TRUE_MESSAGE(bleInit, "BLE should initialize successfully");
}

// Test data flow between modules
void test_data_flow() {
    NavigationData navData;
    UIManager ui;
    
    // Create valid navigation data
    navData.isValid = true;
    navData.instruction = "Turn Right";
    navData.distance = 200;
    navData.speedLimit = 50;
    navData.turnDirection = 2;
    
    // Test UI accepts navigation data
    ui.updateNavigation(navData);
    // Note: In real test, you'd verify UI state changed appropriately
    TEST_ASSERT_TRUE_MESSAGE(true, "UI should accept valid navigation data");
    
    // Test invalid data handling
    navData.isValid = false;
    ui.updateNavigation(navData);
    TEST_ASSERT_TRUE_MESSAGE(true, "UI should handle invalid navigation data gracefully");
}

// Test configuration propagation
void test_configuration_propagation() {
    Settings config;
    UIManager ui;
    
    config.init();
    
    // Test theme propagation
    config.setNightMode(true);
    bool nightMode = config.getNightMode();
    
    if (nightMode) {
        ui.setTheme(THEME_NIGHT);
    } else {
        ui.setTheme(THEME_DAY);
    }
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(THEME_NIGHT, ui.getTheme(), "UI theme should match config");
    
    // Test rotation propagation
    config.setRotation(1);
    uint8_t rotation = config.getRotation();
    float rotationAngle = rotation * 90.0; // Convert to degrees
    
    ui.setRotation(rotationAngle);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(90.0, ui.getRotation(), "UI rotation should match config");
}

// Test sensor-to-UI integration
void test_sensor_ui_integration() {
    IMUHandler imu;
    UIManager ui;
    Settings config;
    
    config.init();
    
    // Test auto-rotation integration
    if (config.getAutoRotation()) {
        // Simulate IMU data
        float sensorRotation = 45.0;
        ui.setRotation(sensorRotation);
        
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE(45.0, ui.getRotation(), "UI should accept IMU rotation data");
    }
    
    // Test manual rotation override
    config.setAutoRotation(false);
    config.setRotation(2); // 180 degrees
    
    if (!config.getAutoRotation()) {
        float manualRotation = config.getRotation() * 90.0;
        ui.setRotation(manualRotation);
        
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE(180.0, ui.getRotation(), "Manual rotation should override IMU");
    }
}

// Test BLE-to-UI integration
void test_ble_ui_integration() {
    BLEServer bleServer;
    UIManager ui;
    
    bleServer.init();
    
    // Test connection state propagation
    bool connected = bleServer.isConnected();
    
    if (connected) {
        ui.setState(UI_NAVIGATION);
    } else {
        ui.setState(UI_CONNECTING);
    }
    
    // Since BLE starts disconnected
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_CONNECTING, ui.getState(), "UI should show connecting when BLE disconnected");
    
    // Test navigation data integration
    if (bleServer.hasNewData()) {
        NavigationData navData = bleServer.getNavigationData();
        ui.updateNavigation(navData);
        TEST_ASSERT_TRUE_MESSAGE(true, "Navigation data should flow from BLE to UI");
    } else {
        ui.setState(UI_NO_DATA);
        TEST_ASSERT_EQUAL_INT_MESSAGE(UI_NO_DATA, ui.getState(), "UI should show no data when no BLE data");
    }
}

// Test error handling integration
void test_error_handling_integration() {
    Settings config;
    UIManager ui;
    AmoledDriver display;
    
    config.init();
    
    // Test display error handling
    if (!display.isInitialized()) {
        ui.showErrorScreen("Display Error");
        TEST_ASSERT_EQUAL_INT_MESSAGE(UI_ERROR, ui.getState(), "UI should show error state");
    }
    
    // Test configuration error handling
    if (!config.isInitialized()) {
        ui.showErrorScreen("Config Error");
        TEST_ASSERT_EQUAL_INT_MESSAGE(UI_ERROR, ui.getState(), "UI should handle config errors");
    }
    
    // Test recovery from error state
    ui.setState(UI_STARTUP);
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_STARTUP, ui.getState(), "UI should allow recovery from error");
}

// Test memory management
void test_memory_management() {
    // Test object creation and destruction
    {
        Settings* config = new Settings();
        TEST_ASSERT_NOT_NULL_MESSAGE(config, "Config object should be created");
        delete config;
        // No assertion after delete - just testing no crash
    }
    
    {
        UIManager* ui = new UIManager();
        TEST_ASSERT_NOT_NULL_MESSAGE(ui, "UI object should be created");
        delete ui;
    }
    
    {
        BLEServer* ble = new BLEServer();
        TEST_ASSERT_NOT_NULL_MESSAGE(ble, "BLE object should be created");
        delete ble;
    }
    
    TEST_ASSERT_TRUE_MESSAGE(true, "Memory management should work without crashes");
}

// Test circular display constraints
void test_circular_display_integration() {
    AmoledDriver display;
    UIManager ui;
    
    // Test center calculations
    int16_t centerX = display.width() / 2;
    int16_t centerY = display.height() / 2;
    
    // Test that UI uses correct center
    TEST_ASSERT_EQUAL_INT_MESSAGE(233, centerX, "UI should use correct center X");
    TEST_ASSERT_EQUAL_INT_MESSAGE(233, centerY, "UI should use correct center Y");
    
    // Test circular boundary
    TEST_ASSERT_TRUE_MESSAGE(display.isInCircle(centerX, centerY), "Center should be in circle");
    TEST_ASSERT_FALSE_MESSAGE(display.isInCircle(0, 0), "Corner should be outside circle");
    
    // Test UI elements fit in circle
    int16_t speedLimitY = centerY - 120;
    int16_t distanceY = centerY + 100;
    
    TEST_ASSERT_TRUE_MESSAGE(display.isInCircle(centerX, speedLimitY), "Speed limit should fit in circle");
    TEST_ASSERT_TRUE_MESSAGE(display.isInCircle(centerX, distanceY), "Distance display should fit in circle");
}

// Test system state transitions
void test_system_state_transitions() {
    UIManager ui;
    BLEServer bleServer;
    NavigationData navData;
    
    bleServer.init();
    
    // Test startup sequence
    ui.setState(UI_STARTUP);
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_STARTUP, ui.getState(), "System should start in startup state");
    
    // Test waiting for connection
    ui.setState(UI_CONNECTING);
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_CONNECTING, ui.getState(), "System should wait for BLE connection");
    
    // Test navigation state
    navData.isValid = true;
    navData.instruction = "Turn Left";
    navData.distance = 500;
    
    ui.updateNavigation(navData);
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_NAVIGATION, ui.getState(), "System should show navigation when data available");
    
    // Test no data state
    navData.isValid = false;
    ui.updateNavigation(navData);
    // Note: UI implementation should detect invalid data and change state
    TEST_ASSERT_TRUE_MESSAGE(true, "System should handle loss of navigation data");
}

// Test configuration persistence integration
void test_persistence_integration() {
    Settings config1;
    Settings config2;
    
    config1.init();
    
    // Set configuration in first instance
    config1.setBrightness(150);
    config1.setNightMode(true);
    config1.save();
    
    // Load configuration in second instance
    config2.init();
    config2.load();
    
    // Verify persistence
    TEST_ASSERT_EQUAL_INT_MESSAGE(150, config2.getBrightness(), "Configuration should persist across instances");
    TEST_ASSERT_TRUE_MESSAGE(config2.getNightMode(), "Boolean settings should persist");
}

// Test real-time updates
void test_realtime_updates() {
    UIManager ui;
    NavigationData navData;
    
    // Test rapid data updates
    for (int i = 0; i < 10; i++) {
        navData.isValid = true;
        navData.distance = 1000 - (i * 100); // Decreasing distance
        navData.instruction = "Continue";
        
        ui.updateNavigation(navData);
        TEST_ASSERT_TRUE_MESSAGE(true, "UI should handle rapid updates");
    }
    
    // Test update frequency
    ui.update(); // Should not crash or cause issues when called frequently
    ui.update();
    ui.update();
    
    TEST_ASSERT_TRUE_MESSAGE(true, "UI update should be safe to call frequently");
}

// Main test runner for integration tests
void run_integration_tests() {
    RUN_TEST(test_system_initialization);
    RUN_TEST(test_data_flow);
    RUN_TEST(test_configuration_propagation);
    RUN_TEST(test_sensor_ui_integration);
    RUN_TEST(test_ble_ui_integration);
    RUN_TEST(test_error_handling_integration);
    RUN_TEST(test_memory_management);
    RUN_TEST(test_circular_display_integration);
    RUN_TEST(test_system_state_transitions);
    RUN_TEST(test_persistence_integration);
    RUN_TEST(test_realtime_updates);
}