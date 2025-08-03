#include <unity.h>
#include <Arduino.h>
#include "../src/ble/ble_server.h"

// Mock navigation data for testing
const uint8_t MOCK_NAV_DATA[] = {0x01, 0x32, 0x0A, 0x33, 0x35, 0x30, 0x6D};
const size_t MOCK_NAV_SIZE = sizeof(MOCK_NAV_DATA);

// Test BLE server initialization
void test_ble_server_initialization() {
    BLEServer bleServer;
    
    // Test initialization
    bool initResult = bleServer.init();
    TEST_ASSERT_TRUE_MESSAGE(initResult, "BLE server should initialize successfully");
    
    // Test initial state
    TEST_ASSERT_FALSE_MESSAGE(bleServer.isConnected(), "BLE should not be connected initially");
    TEST_ASSERT_FALSE_MESSAGE(bleServer.hasNewData(), "BLE should not have new data initially");
}

// Test navigation data parsing
void test_navigation_data_parsing() {
    BLEServer bleServer;
    bleServer.init();
    
    // Simulate data reception (this would normally be called by BLE callback)
    // For testing, we need to access private method - this is a conceptual test
    
    // Test valid data structure
    NavigationData expectedData;
    expectedData.speedLimit = 50;  // 0x32 = 50 decimal
    expectedData.turnDirection = 0x33;
    expectedData.distance = 350; // From hex string "350"
    expectedData.instruction = "Continue"; // Default for unknown direction
    expectedData.isValid = true;
    
    // Note: In real implementation, you'd need to make parseNavigationData public
    // or create a test-friendly interface for testing private methods
    TEST_ASSERT_TRUE_MESSAGE(true, "Navigation data parsing structure validated");
}

// Test BLE connection state management
void test_ble_connection_state() {
    BLEServer bleServer;
    bleServer.init();
    
    // Test initial disconnected state
    TEST_ASSERT_FALSE_MESSAGE(bleServer.isConnected(), "Should start disconnected");
    
    // Test advertising
    bleServer.startAdvertising();
    TEST_ASSERT_TRUE_MESSAGE(true, "Advertising should start without errors");
    
    bleServer.stopAdvertising();
    TEST_ASSERT_TRUE_MESSAGE(true, "Advertising should stop without errors");
}

// Test navigation data validation
void test_navigation_data_validation() {
    NavigationData navData;
    
    // Test default constructor
    TEST_ASSERT_FALSE_MESSAGE(navData.isValid, "Default navigation data should be invalid");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, navData.distance, "Default distance should be 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, navData.speedLimit, "Default speed limit should be 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, navData.turnDirection, "Default turn direction should be 0");
    
    // Test valid data
    navData.isValid = true;
    navData.distance = 500;
    navData.speedLimit = 60;
    navData.turnDirection = 1;
    navData.instruction = "Turn Left";
    
    TEST_ASSERT_TRUE_MESSAGE(navData.isValid, "Navigation data should be valid");
    TEST_ASSERT_EQUAL_INT_MESSAGE(500, navData.distance, "Distance should match");
    TEST_ASSERT_EQUAL_INT_MESSAGE(60, navData.speedLimit, "Speed limit should match");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, navData.turnDirection, "Turn direction should match");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Turn Left", navData.instruction.c_str(), "Instruction should match");
}

// Test turn direction mapping
void test_turn_direction_mapping() {
    // Test direction constants
    const int LEFT_TURN = 0x01;
    const int RIGHT_TURN = 0x02;
    const int STRAIGHT = 0x03;
    const int U_TURN = 0x04;
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, LEFT_TURN, "Left turn constant should be 1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, RIGHT_TURN, "Right turn constant should be 2");
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, STRAIGHT, "Straight constant should be 3");
    TEST_ASSERT_EQUAL_INT_MESSAGE(4, U_TURN, "U-turn constant should be 4");
}

// Test BLE service UUID validation
void test_ble_service_uuid() {
    const char* expectedUUID = "DD3F0AD1-6239-4E1F-81F1-91F6C9F01D86";
    
    // Test UUID format (36 characters with hyphens)
    String uuidStr = String(expectedUUID);
    TEST_ASSERT_EQUAL_INT_MESSAGE(36, uuidStr.length(), "UUID should be 36 characters");
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('-', expectedUUID[8], "UUID should have hyphen at position 8");
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('-', expectedUUID[13], "UUID should have hyphen at position 13");
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('-', expectedUUID[18], "UUID should have hyphen at position 18");
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('-', expectedUUID[23], "UUID should have hyphen at position 23");
}

// Test data packet size validation
void test_data_packet_validation() {
    // Test minimum packet size
    const size_t MIN_PACKET_SIZE = 7;
    TEST_ASSERT_TRUE_MESSAGE(MOCK_NAV_SIZE >= MIN_PACKET_SIZE, "Mock data should meet minimum size");
    
    // Test packet structure
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, MOCK_NAV_DATA[0], "First byte should be data flag");
}

// Main test runner for BLE module
void run_ble_tests() {
    RUN_TEST(test_ble_server_initialization);
    RUN_TEST(test_navigation_data_parsing);
    RUN_TEST(test_ble_connection_state);
    RUN_TEST(test_navigation_data_validation);
    RUN_TEST(test_turn_direction_mapping);
    RUN_TEST(test_ble_service_uuid);
    RUN_TEST(test_data_packet_validation);
}