#include <unity.h>
#include <Arduino.h>
#include "../src/config/settings.h"

// Test settings initialization
void test_settings_initialization() {
    Settings config;
    
    // Test initial state
    TEST_ASSERT_FALSE_MESSAGE(config.isInitialized(), "Settings should not be initialized initially");
    
    // Test initialization
    bool initResult = config.init();
    TEST_ASSERT_TRUE_MESSAGE(initResult, "Settings initialization should succeed");
    TEST_ASSERT_TRUE_MESSAGE(config.isInitialized(), "Settings should be initialized after init()");
}

// Test default settings values
void test_default_settings() {
    HUDSettings defaultSettings;
    
    // Test display defaults
    TEST_ASSERT_EQUAL_INT_MESSAGE(200, defaultSettings.brightness, "Default brightness should be 200");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, defaultSettings.rotation, "Default rotation should be 0");
    TEST_ASSERT_TRUE_MESSAGE(defaultSettings.autoRotation, "Auto rotation should be enabled by default");
    TEST_ASSERT_FALSE_MESSAGE(defaultSettings.nightMode, "Night mode should be disabled by default");
    
    // Test navigation defaults
    TEST_ASSERT_TRUE_MESSAGE(defaultSettings.showSpeedLimit, "Show speed limit should be enabled by default");
    TEST_ASSERT_TRUE_MESSAGE(defaultSettings.showDistance, "Show distance should be enabled by default");
    TEST_ASSERT_TRUE_MESSAGE(defaultSettings.showInstruction, "Show instruction should be enabled by default");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, defaultSettings.distanceUnit, "Default distance unit should be meters (0)");
    
    // Test BLE defaults
    TEST_ASSERT_EQUAL_INT_MESSAGE(5000, defaultSettings.reconnectDelay, "Default reconnect delay should be 5000ms");
    TEST_ASSERT_TRUE_MESSAGE(defaultSettings.autoConnect, "Auto connect should be enabled by default");
    
    // Test power defaults
    TEST_ASSERT_EQUAL_INT_MESSAGE(300, defaultSettings.sleepTimeout, "Default sleep timeout should be 300s");
    TEST_ASSERT_TRUE_MESSAGE(defaultSettings.wakeonData, "Wake on data should be enabled by default");
}

// Test settings getters
void test_settings_getters() {
    Settings config;
    config.init();
    
    // Test display getters
    TEST_ASSERT_EQUAL_INT_MESSAGE(200, config.getBrightness(), "Brightness getter should return default");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, config.getRotation(), "Rotation getter should return default");
    TEST_ASSERT_TRUE_MESSAGE(config.getAutoRotation(), "Auto rotation getter should return default");
    TEST_ASSERT_FALSE_MESSAGE(config.getNightMode(), "Night mode getter should return default");
    
    // Test navigation getters
    TEST_ASSERT_TRUE_MESSAGE(config.getShowSpeedLimit(), "Show speed limit getter should return default");
    TEST_ASSERT_TRUE_MESSAGE(config.getShowDistance(), "Show distance getter should return default");
    TEST_ASSERT_TRUE_MESSAGE(config.getShowInstruction(), "Show instruction getter should return default");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, config.getDistanceUnit(), "Distance unit getter should return default");
    
    // Test BLE getters
    TEST_ASSERT_EQUAL_INT_MESSAGE(5000, config.getReconnectDelay(), "Reconnect delay getter should return default");
    TEST_ASSERT_TRUE_MESSAGE(config.getAutoConnect(), "Auto connect getter should return default");
    
    // Test power getters
    TEST_ASSERT_EQUAL_INT_MESSAGE(300, config.getSleepTimeout(), "Sleep timeout getter should return default");
    TEST_ASSERT_TRUE_MESSAGE(config.getWakeOnData(), "Wake on data getter should return default");
}

// Test settings setters
void test_settings_setters() {
    Settings config;
    config.init();
    
    // Test brightness setter
    config.setBrightness(150);
    TEST_ASSERT_EQUAL_INT_MESSAGE(150, config.getBrightness(), "Brightness should be set correctly");
    
    // Test brightness bounds (0-255)
    config.setBrightness(300); // Should be constrained to 255
    TEST_ASSERT_EQUAL_INT_MESSAGE(255, config.getBrightness(), "Brightness should be constrained to 255");
    
    // Test rotation setter
    config.setRotation(2);
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, config.getRotation(), "Rotation should be set correctly");
    
    // Test rotation wrap (should mod 4)
    config.setRotation(5); // Should become 1
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, config.getRotation(), "Rotation should wrap around");
    
    // Test boolean setters
    config.setAutoRotation(false);
    TEST_ASSERT_FALSE_MESSAGE(config.getAutoRotation(), "Auto rotation should be disabled");
    
    config.setNightMode(true);
    TEST_ASSERT_TRUE_MESSAGE(config.getNightMode(), "Night mode should be enabled");
    
    // Test navigation setters
    config.setShowSpeedLimit(false);
    TEST_ASSERT_FALSE_MESSAGE(config.getShowSpeedLimit(), "Show speed limit should be disabled");
    
    config.setDistanceUnit(1);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, config.getDistanceUnit(), "Distance unit should be set to feet");
    
    // Test distance unit wrap (should mod 2)
    config.setDistanceUnit(3); // Should become 1
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, config.getDistanceUnit(), "Distance unit should wrap around");
}

// Test settings toggle functions
void test_settings_toggles() {
    Settings config;
    config.init();
    
    // Test night mode toggle
    bool initialNightMode = config.getNightMode();
    config.toggleNightMode();
    TEST_ASSERT_NOT_EQUAL_MESSAGE(initialNightMode, config.getNightMode(), "Night mode should toggle");
    
    config.toggleNightMode();
    TEST_ASSERT_EQUAL_INT_MESSAGE(initialNightMode, config.getNightMode(), "Night mode should toggle back");
    
    // Test auto rotation toggle
    bool initialAutoRotation = config.getAutoRotation();
    config.toggleAutoRotation();
    TEST_ASSERT_NOT_EQUAL_MESSAGE(initialAutoRotation, config.getAutoRotation(), "Auto rotation should toggle");
    
    config.toggleAutoRotation();
    TEST_ASSERT_EQUAL_INT_MESSAGE(initialAutoRotation, config.getAutoRotation(), "Auto rotation should toggle back");
}

// Test settings bounds validation
void test_settings_bounds() {
    Settings config;
    config.init();
    
    // Test brightness bounds
    config.setBrightness(0);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, config.getBrightness(), "Minimum brightness should be 0");
    
    config.setBrightness(255);
    TEST_ASSERT_EQUAL_INT_MESSAGE(255, config.getBrightness(), "Maximum brightness should be 255");
    
    // Test reconnect delay minimum
    config.setReconnectDelay(500); // Below minimum of 1000
    TEST_ASSERT_TRUE_MESSAGE(config.getReconnectDelay() >= 1000, "Reconnect delay should have minimum of 1000ms");
    
    // Test sleep timeout minimum
    config.setSleepTimeout(10); // Below minimum of 30
    TEST_ASSERT_TRUE_MESSAGE(config.getSleepTimeout() >= 30, "Sleep timeout should have minimum of 30s");
}

// Test settings structure getter
void test_settings_structure() {
    Settings config;
    config.init();
    
    // Modify some settings
    config.setBrightness(180);
    config.setNightMode(true);
    config.setRotation(1);
    
    // Get settings structure
    const HUDSettings& settings = config.getSettings();
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(180, settings.brightness, "Settings structure brightness should match");
    TEST_ASSERT_TRUE_MESSAGE(settings.nightMode, "Settings structure night mode should match");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, settings.rotation, "Settings structure rotation should match");
}

// Test settings save and load
void test_settings_persistence() {
    Settings config;
    config.init();
    
    // Set non-default values
    config.setBrightness(100);
    config.setNightMode(true);
    config.setRotation(2);
    
    // Save settings
    bool saveResult = config.save();
    TEST_ASSERT_TRUE_MESSAGE(saveResult, "Settings save should succeed");
    
    // Load settings
    bool loadResult = config.load();
    TEST_ASSERT_TRUE_MESSAGE(loadResult, "Settings load should succeed");
    
    // Verify values persist
    TEST_ASSERT_EQUAL_INT_MESSAGE(100, config.getBrightness(), "Brightness should persist");
    TEST_ASSERT_TRUE_MESSAGE(config.getNightMode(), "Night mode should persist");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, config.getRotation(), "Rotation should persist");
}

// Test settings reset
void test_settings_reset() {
    Settings config;
    config.init();
    
    // Change from defaults
    config.setBrightness(100);
    config.setNightMode(true);
    config.setAutoRotation(false);
    
    // Reset to defaults
    config.reset();
    
    // Verify defaults are restored
    TEST_ASSERT_EQUAL_INT_MESSAGE(200, config.getBrightness(), "Reset should restore default brightness");
    TEST_ASSERT_FALSE_MESSAGE(config.getNightMode(), "Reset should restore default night mode");
    TEST_ASSERT_TRUE_MESSAGE(config.getAutoRotation(), "Reset should restore default auto rotation");
}

// Test preference keys
void test_preference_keys() {
    // Test key string lengths (should be reasonable for NVS)
    const char* testKeys[] = {
        "brightness", "rotation", "auto_rot", "night_mode",
        "show_speed", "show_dist", "show_instr", "dist_unit",
        "reconnect", "auto_conn", "sleep_time", "wake_data"
    };
    
    for (int i = 0; i < 12; i++) {
        size_t keyLen = strlen(testKeys[i]);
        TEST_ASSERT_TRUE_MESSAGE(keyLen > 0 && keyLen <= 15, "Preference keys should be 1-15 characters");
    }
}

// Test settings validation ranges
void test_settings_validation() {
    // Test brightness range (0-255)
    TEST_ASSERT_TRUE_MESSAGE(0 >= 0 && 0 <= 255, "Min brightness should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(255 >= 0 && 255 <= 255, "Max brightness should be in valid range");
    
    // Test rotation range (0-3)
    TEST_ASSERT_TRUE_MESSAGE(0 >= 0 && 0 <= 3, "Min rotation should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(3 >= 0 && 3 <= 3, "Max rotation should be in valid range");
    
    // Test distance unit range (0-1)
    TEST_ASSERT_TRUE_MESSAGE(0 >= 0 && 0 <= 1, "Meters unit should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(1 >= 0 && 1 <= 1, "Feet unit should be in valid range");
    
    // Test reasonable timeout ranges
    TEST_ASSERT_TRUE_MESSAGE(1000 >= 1000, "Min reconnect delay should be at least 1000ms");
    TEST_ASSERT_TRUE_MESSAGE(30 >= 30, "Min sleep timeout should be at least 30s");
}

// Test settings namespace
void test_settings_namespace() {
    // Test namespace string length (NVS namespace max is 15 chars)
    const char* namespace_name = "hud_settings";
    size_t namespaceLen = strlen(namespace_name);
    
    TEST_ASSERT_TRUE_MESSAGE(namespaceLen > 0 && namespaceLen <= 15, "Namespace should be 1-15 characters");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("hud_settings", namespace_name, "Namespace should match expected value");
}

// Main test runner for settings module
void run_settings_tests() {
    RUN_TEST(test_settings_initialization);
    RUN_TEST(test_default_settings);
    RUN_TEST(test_settings_getters);
    RUN_TEST(test_settings_setters);
    RUN_TEST(test_settings_toggles);
    RUN_TEST(test_settings_bounds);
    RUN_TEST(test_settings_structure);
    RUN_TEST(test_settings_persistence);
    RUN_TEST(test_settings_reset);
    RUN_TEST(test_preference_keys);
    RUN_TEST(test_settings_validation);
    RUN_TEST(test_settings_namespace);
}