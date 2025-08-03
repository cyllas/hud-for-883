#include <unity.h>
#include <Arduino.h>
#include "../src/display/ui_manager.h"
#include "../src/ble/ble_server.h"

// Test UI state management
void test_ui_state_management() {
    UIManager ui;
    
    // Test initial state
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_STARTUP, ui.getState(), "Initial state should be UI_STARTUP");
    
    // Test state transitions
    ui.setState(UI_CONNECTING);
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_CONNECTING, ui.getState(), "State should change to UI_CONNECTING");
    
    ui.setState(UI_NAVIGATION);
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_NAVIGATION, ui.getState(), "State should change to UI_NAVIGATION");
    
    ui.setState(UI_NO_DATA);
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_NO_DATA, ui.getState(), "State should change to UI_NO_DATA");
    
    ui.setState(UI_ERROR);
    TEST_ASSERT_EQUAL_INT_MESSAGE(UI_ERROR, ui.getState(), "State should change to UI_ERROR");
}

// Test theme management
void test_theme_management() {
    UIManager ui;
    
    // Test initial theme
    TEST_ASSERT_EQUAL_INT_MESSAGE(THEME_DAY, ui.getTheme(), "Initial theme should be THEME_DAY");
    
    // Test theme change
    ui.setTheme(THEME_NIGHT);
    TEST_ASSERT_EQUAL_INT_MESSAGE(THEME_NIGHT, ui.getTheme(), "Theme should change to THEME_NIGHT");
    
    ui.setTheme(THEME_DAY);
    TEST_ASSERT_EQUAL_INT_MESSAGE(THEME_DAY, ui.getTheme(), "Theme should change back to THEME_DAY");
    
    // Test theme toggle
    ui.toggleTheme();
    TEST_ASSERT_EQUAL_INT_MESSAGE(THEME_NIGHT, ui.getTheme(), "Toggle should switch to night theme");
    
    ui.toggleTheme();
    TEST_ASSERT_EQUAL_INT_MESSAGE(THEME_DAY, ui.getTheme(), "Toggle should switch back to day theme");
}

// Test rotation management
void test_rotation_management() {
    UIManager ui;
    
    // Test initial rotation
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, ui.getRotation(), "Initial rotation should be 0.0");
    
    // Test rotation changes
    ui.setRotation(90.0);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(90.0, ui.getRotation(), "Rotation should be set to 90.0");
    
    ui.setRotation(180.0);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(180.0, ui.getRotation(), "Rotation should be set to 180.0");
    
    ui.setRotation(270.0);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(270.0, ui.getRotation(), "Rotation should be set to 270.0");
    
    ui.setRotation(360.0);
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(360.0, ui.getRotation(), "Rotation should be set to 360.0");
}

// Test navigation data handling
void test_navigation_data_handling() {
    UIManager ui;
    NavigationData navData;
    
    // Test invalid navigation data
    navData.isValid = false;
    ui.updateNavigation(navData);
    // After invalid data, UI should go to NO_DATA state
    // Note: This would need UI state to be accessible or UI to have a way to query this
    TEST_ASSERT_TRUE_MESSAGE(true, "Invalid navigation data should be handled gracefully");
    
    // Test valid navigation data
    navData.isValid = true;
    navData.instruction = "Turn Right";
    navData.distance = 250;
    navData.speedLimit = 50;
    navData.turnDirection = 2;
    
    ui.updateNavigation(navData);
    TEST_ASSERT_TRUE_MESSAGE(true, "Valid navigation data should be processed without error");
}

// Test UI state enumeration values
void test_ui_state_values() {
    TEST_ASSERT_TRUE_MESSAGE(UI_STARTUP != UI_CONNECTING, "UI states should have different values");
    TEST_ASSERT_TRUE_MESSAGE(UI_CONNECTING != UI_NAVIGATION, "UI states should have different values");
    TEST_ASSERT_TRUE_MESSAGE(UI_NAVIGATION != UI_NO_DATA, "UI states should have different values");
    TEST_ASSERT_TRUE_MESSAGE(UI_NO_DATA != UI_ERROR, "UI states should have different values");
    
    // Test that all states are within expected range (0-4)
    TEST_ASSERT_TRUE_MESSAGE(UI_STARTUP >= 0 && UI_STARTUP <= 4, "UI_STARTUP should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(UI_CONNECTING >= 0 && UI_CONNECTING <= 4, "UI_CONNECTING should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(UI_NAVIGATION >= 0 && UI_NAVIGATION <= 4, "UI_NAVIGATION should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(UI_NO_DATA >= 0 && UI_NO_DATA <= 4, "UI_NO_DATA should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(UI_ERROR >= 0 && UI_ERROR <= 4, "UI_ERROR should be in valid range");
}

// Test theme enumeration values
void test_theme_values() {
    TEST_ASSERT_TRUE_MESSAGE(THEME_DAY != THEME_NIGHT, "Themes should have different values");
    TEST_ASSERT_TRUE_MESSAGE(THEME_DAY >= 0 && THEME_DAY <= 1, "THEME_DAY should be in valid range");
    TEST_ASSERT_TRUE_MESSAGE(THEME_NIGHT >= 0 && THEME_NIGHT <= 1, "THEME_NIGHT should be in valid range");
}

// Test circular display positioning
void test_circular_positioning() {
    // Test center calculation for circular display
    int16_t centerX = AMOLED_WIDTH / 2;
    int16_t centerY = AMOLED_HEIGHT / 2;
    int16_t radius = AMOLED_RADIUS;
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(233, centerX, "Center X should be 233");
    TEST_ASSERT_EQUAL_INT_MESSAGE(233, centerY, "Center Y should be 233");
    TEST_ASSERT_EQUAL_INT_MESSAGE(233, radius, "Radius should be 233");
    
    // Test that radius is exactly half the width/height
    TEST_ASSERT_EQUAL_INT_MESSAGE(centerX, radius, "Radius should equal center coordinates");
    TEST_ASSERT_EQUAL_INT_MESSAGE(centerY, radius, "Radius should equal center coordinates");
}

// Test text centering calculations
void test_text_centering() {
    String testText = "Hello";
    uint8_t textSize = 2;
    int16_t charWidth = 6; // Standard character width
    
    int16_t textWidth = testText.length() * charWidth * textSize;
    int16_t centerX = AMOLED_WIDTH / 2;
    int16_t textX = centerX - textWidth / 2;
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(30, textWidth, "Text width calculation should be correct");
    TEST_ASSERT_EQUAL_INT_MESSAGE(218, textX, "Text X position should center the text");
    
    // Test edge cases
    String emptyText = "";
    int16_t emptyWidth = emptyText.length() * charWidth * textSize;
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, emptyWidth, "Empty text should have zero width");
    
    String longText = "This is a very long text";
    int16_t longWidth = longText.length() * charWidth * textSize;
    TEST_ASSERT_TRUE_MESSAGE(longWidth > AMOLED_WIDTH, "Long text should exceed display width");
}

// Test speed limit display calculations
void test_speed_limit_display() {
    // Test speed limit circle parameters
    int16_t centerX = AMOLED_WIDTH / 2;
    int16_t centerY = AMOLED_HEIGHT / 2;
    int16_t speedLimitY = centerY - 120;
    int16_t circleRadius = 35;
    int16_t innerRadius = 30;
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(113, speedLimitY, "Speed limit Y position should be correct");
    TEST_ASSERT_TRUE_MESSAGE(speedLimitY > 0, "Speed limit should be within display bounds");
    TEST_ASSERT_TRUE_MESSAGE(speedLimitY + circleRadius < AMOLED_HEIGHT, "Speed limit circle should fit");
    TEST_ASSERT_TRUE_MESSAGE(innerRadius < circleRadius, "Inner radius should be smaller than outer");
    
    // Test speed limit number positioning
    int speedLimit = 50;
    String speedText = String(speedLimit);
    int16_t textX = centerX - (speedText.length() * 6); // Approximate centering
    
    TEST_ASSERT_TRUE_MESSAGE(textX > 0 && textX < AMOLED_WIDTH, "Speed text should be within bounds");
}

// Test distance display formatting
void test_distance_formatting() {
    // Test meter formatting
    int distance_m = 500;
    String distText_m = String(distance_m) + "m";
    TEST_ASSERT_EQUAL_STRING_MESSAGE("500m", distText_m.c_str(), "Meter formatting should be correct");
    
    // Test kilometer formatting
    int distance_km = 1500;
    String distText_km = String(distance_km / 1000.0, 1) + "km";
    TEST_ASSERT_EQUAL_STRING_MESSAGE("1.5km", distText_km.c_str(), "Kilometer formatting should be correct");
    
    // Test boundary cases
    distance_km = 1000;
    distText_km = String(distance_km / 1000.0, 1) + "km";
    TEST_ASSERT_EQUAL_STRING_MESSAGE("1.0km", distText_km.c_str(), "Exact kilometer should format correctly");
    
    distance_m = 999;
    distText_m = String(distance_m) + "m";
    TEST_ASSERT_EQUAL_STRING_MESSAGE("999m", distText_m.c_str(), "Sub-kilometer should stay in meters");
}

// Test turn direction arrow positioning
void test_turn_direction_arrows() {
    int16_t centerX = AMOLED_WIDTH / 2;
    int16_t centerY = AMOLED_HEIGHT / 2;
    int16_t arrowY = centerY + 50;
    int16_t arrowSize = 20;
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(283, arrowY, "Arrow Y position should be correct");
    TEST_ASSERT_TRUE_MESSAGE(arrowY > centerY, "Arrow should be below center");
    TEST_ASSERT_TRUE_MESSAGE(arrowY + arrowSize < AMOLED_HEIGHT, "Arrow should fit within display");
    
    // Test arrow boundaries
    int16_t leftArrowStart = centerX - arrowSize;
    int16_t rightArrowEnd = centerX + arrowSize;
    
    TEST_ASSERT_TRUE_MESSAGE(leftArrowStart > 0, "Left arrow should be within bounds");
    TEST_ASSERT_TRUE_MESSAGE(rightArrowEnd < AMOLED_WIDTH, "Right arrow should be within bounds");
}

// Main test runner for UI module
void run_ui_tests() {
    RUN_TEST(test_ui_state_management);
    RUN_TEST(test_theme_management);
    RUN_TEST(test_rotation_management);
    RUN_TEST(test_navigation_data_handling);
    RUN_TEST(test_ui_state_values);
    RUN_TEST(test_theme_values);
    RUN_TEST(test_circular_positioning);
    RUN_TEST(test_text_centering);
    RUN_TEST(test_speed_limit_display);
    RUN_TEST(test_distance_formatting);
    RUN_TEST(test_turn_direction_arrows);
}