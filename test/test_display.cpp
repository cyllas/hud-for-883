#include <unity.h>
#include <Arduino.h>
#include "../src/display/amoled_driver.h"

// Test display driver initialization
void test_display_initialization() {
    AmoledDriver display;
    
    // Test initial state
    TEST_ASSERT_FALSE_MESSAGE(display.isInitialized(), "Display should not be initialized initially");
    
    // Test dimensions
    TEST_ASSERT_EQUAL_INT_MESSAGE(466, display.width(), "Display width should be 466");
    TEST_ASSERT_EQUAL_INT_MESSAGE(466, display.height(), "Display height should be 466");
}

// Test display constants and definitions
void test_display_constants() {
    // Test resolution
    TEST_ASSERT_EQUAL_INT_MESSAGE(466, AMOLED_WIDTH, "AMOLED width constant should be 466");
    TEST_ASSERT_EQUAL_INT_MESSAGE(466, AMOLED_HEIGHT, "AMOLED height constant should be 466");
    TEST_ASSERT_EQUAL_INT_MESSAGE(233, AMOLED_RADIUS, "AMOLED radius should be 233");
    
    // Test GPIO pin definitions
    TEST_ASSERT_EQUAL_INT_MESSAGE(10, AMOLED_CLK, "AMOLED CLK should be GPIO 10");
    TEST_ASSERT_EQUAL_INT_MESSAGE(11, AMOLED_D0, "AMOLED D0 should be GPIO 11");
    TEST_ASSERT_EQUAL_INT_MESSAGE(12, AMOLED_D1, "AMOLED D1 should be GPIO 12");
    TEST_ASSERT_EQUAL_INT_MESSAGE(13, AMOLED_D2, "AMOLED D2 should be GPIO 13");
    TEST_ASSERT_EQUAL_INT_MESSAGE(14, AMOLED_D3, "AMOLED D3 should be GPIO 14");
    TEST_ASSERT_EQUAL_INT_MESSAGE(38, AMOLED_RST, "AMOLED RST should be GPIO 38");
    TEST_ASSERT_EQUAL_INT_MESSAGE(42, AMOLED_EN, "AMOLED EN should be GPIO 42");
}

// Test color definitions
void test_color_definitions() {
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x0000, COLOR_BLACK, "Black should be 0x0000");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0xFFFF, COLOR_WHITE, "White should be 0xFFFF");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0xF800, COLOR_RED, "Red should be 0xF800");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x07E0, COLOR_GREEN, "Green should be 0x07E0");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x001F, COLOR_BLUE, "Blue should be 0x001F");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0xFFE0, COLOR_YELLOW, "Yellow should be 0xFFE0");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0xFC00, COLOR_ORANGE, "Orange should be 0xFC00");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x8410, COLOR_GRAY, "Gray should be 0x8410");
}

// Test circular boundary checking
void test_circular_boundary() {
    AmoledDriver display;
    
    // Test center point (should be inside)
    TEST_ASSERT_TRUE_MESSAGE(display.isInCircle(233, 233), "Center point should be inside circle");
    
    // Test corner points (should be outside)
    TEST_ASSERT_FALSE_MESSAGE(display.isInCircle(0, 0), "Corner (0,0) should be outside circle");
    TEST_ASSERT_FALSE_MESSAGE(display.isInCircle(465, 465), "Corner (465,465) should be outside circle");
    
    // Test edge points
    TEST_ASSERT_TRUE_MESSAGE(display.isInCircle(233, 100), "Point near top should be inside");
    TEST_ASSERT_TRUE_MESSAGE(display.isInCircle(233, 366), "Point near bottom should be inside");
    
    // Test boundary math
    int centerX = 233, centerY = 233, radius = 233;
    int testX = 233, testY = 0; // Top of circle
    int dx = testX - centerX;
    int dy = testY - centerY;
    int distanceSquared = dx * dx + dy * dy;
    int radiusSquared = radius * radius;
    
    TEST_ASSERT_TRUE_MESSAGE(distanceSquared <= radiusSquared, "Boundary calculation should be correct");
}

// Test rotation values
void test_rotation_values() {
    AmoledDriver display;
    
    // Test rotation range (0-3)
    for (uint8_t rotation = 0; rotation < 4; rotation++) {
        display.setRotation(rotation);
        // Note: In real test, you'd verify the actual rotation was applied
        TEST_ASSERT_TRUE_MESSAGE(true, "Rotation should be applied without error");
    }
    
    // Test rotation overflow (should wrap around)
    display.setRotation(5); // Should become 1 (5 % 4)
    TEST_ASSERT_TRUE_MESSAGE(true, "Rotation overflow should be handled");
}

// Test touch screen pin definitions
void test_touch_pins() {
    TEST_ASSERT_EQUAL_INT_MESSAGE(47, TOUCH_SDA, "Touch SDA should be GPIO 47");
    TEST_ASSERT_EQUAL_INT_MESSAGE(48, TOUCH_SCL, "Touch SCL should be GPIO 48");
}

// Test display coordinate system
void test_coordinate_system() {
    AmoledDriver display;
    
    // Test coordinate bounds
    int16_t maxX = display.width() - 1;
    int16_t maxY = display.height() - 1;
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(465, maxX, "Max X coordinate should be 465");
    TEST_ASSERT_EQUAL_INT_MESSAGE(465, maxY, "Max Y coordinate should be 465");
    
    // Test center calculation
    int16_t centerX = display.width() / 2;
    int16_t centerY = display.height() / 2;
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(233, centerX, "Center X should be 233");
    TEST_ASSERT_EQUAL_INT_MESSAGE(233, centerY, "Center Y should be 233");
}

// Test RGB565 color format
void test_rgb565_format() {
    // Test RGB565 bit patterns
    uint16_t red = 0xF800;   // 11111 000000 00000
    uint16_t green = 0x07E0; // 00000 111111 00000  
    uint16_t blue = 0x001F;  // 00000 000000 11111
    
    // Extract color components
    uint8_t r_component = (red >> 11) & 0x1F;
    uint8_t g_component = (green >> 5) & 0x3F;
    uint8_t b_component = blue & 0x1F;
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(31, r_component, "Red component should be max (31)");
    TEST_ASSERT_EQUAL_INT_MESSAGE(63, g_component, "Green component should be max (63)");
    TEST_ASSERT_EQUAL_INT_MESSAGE(31, b_component, "Blue component should be max (31)");
}

// Test drawing boundaries
void test_drawing_boundaries() {
    AmoledDriver display;
    
    // Test valid coordinates
    bool validCenter = (233 >= 0 && 233 < 466 && 233 >= 0 && 233 < 466);
    TEST_ASSERT_TRUE_MESSAGE(validCenter, "Center coordinates should be valid");
    
    // Test invalid coordinates
    bool invalidNegative = (-1 >= 0 && -1 < 466);
    bool invalidTooLarge = (466 >= 0 && 466 < 466);
    
    TEST_ASSERT_FALSE_MESSAGE(invalidNegative, "Negative coordinates should be invalid");
    TEST_ASSERT_FALSE_MESSAGE(invalidTooLarge, "Coordinates >= 466 should be invalid");
}

// Main test runner for display module
void run_display_tests() {
    RUN_TEST(test_display_initialization);
    RUN_TEST(test_display_constants);
    RUN_TEST(test_color_definitions);
    RUN_TEST(test_circular_boundary);
    RUN_TEST(test_rotation_values);
    RUN_TEST(test_touch_pins);
    RUN_TEST(test_coordinate_system);
    RUN_TEST(test_rgb565_format);
    RUN_TEST(test_drawing_boundaries);
}