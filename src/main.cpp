#include <Arduino.h>
#include "ble/ble_server.h"
#include "display/amoled_driver.h"
#include "display/ui_manager.h"
#include "sensors/imu_handler.h"
#include "config/settings.h"

// Global objects
BLEServer bleServer;
AmoledDriver display;
UIManager ui;
IMUHandler imu;
Settings config;

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32-S3 HUD Navigation Starting...");
    
    // Initialize configuration
    config.init();
    
    // Initialize display
    if (!display.init()) {
        Serial.println("Failed to initialize display!");
        return;
    }
    
    // Initialize UI manager
    ui.init(&display);
    ui.showStartupScreen();
    
    // Initialize sensors
    imu.init();
    
    // Initialize BLE server
    bleServer.init();
    bleServer.startAdvertising();
    
    Serial.println("ESP32-S3 HUD initialized successfully!");
}

void loop() {
    // Update UI
    ui.update();
    
    // Check for BLE data
    if (bleServer.hasNewData()) {
        NavigationData navData = bleServer.getNavigationData();
        ui.updateNavigation(navData);
    }
    
    // Check sensor data for rotation
    if (imu.hasNewData()) {
        float rotation = imu.getRotation();
        ui.setRotation(rotation);
    }
    
    delay(50); // 20 FPS update rate
}