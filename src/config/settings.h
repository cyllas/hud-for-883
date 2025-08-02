#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <Preferences.h>

struct HUDSettings {
    // Display settings
    uint8_t brightness;      // 0-255
    uint8_t rotation;        // 0-3 (0, 90, 180, 270 degrees)
    bool autoRotation;       // Enable auto-rotation based on IMU
    bool nightMode;          // Day/night theme
    
    // Navigation settings
    bool showSpeedLimit;     // Show speed limit circle
    bool showDistance;       // Show distance to next turn
    bool showInstruction;    // Show turn instruction text
    uint8_t distanceUnit;    // 0=meters, 1=feet
    
    // BLE settings
    uint16_t reconnectDelay; // Delay between reconnection attempts (ms)
    bool autoConnect;        // Auto-connect to last known device
    
    // Power management
    uint16_t sleepTimeout;   // Timeout before sleep (seconds)
    bool wakeonData;         // Wake up when new navigation data arrives
    
    // Default constructor with sensible defaults
    HUDSettings() {
        brightness = 200;
        rotation = 0;
        autoRotation = true;
        nightMode = false;
        showSpeedLimit = true;
        showDistance = true;
        showInstruction = true;
        distanceUnit = 0; // meters
        reconnectDelay = 5000;
        autoConnect = true;
        sleepTimeout = 300; // 5 minutes
        wakeonData = true;
    }
};

class Settings {
private:
    Preferences preferences;
    HUDSettings currentSettings;
    bool initialized;
    
    // Keys for preferences storage
    static const char* PREF_NAMESPACE;
    static const char* KEY_BRIGHTNESS;
    static const char* KEY_ROTATION;
    static const char* KEY_AUTO_ROTATION;
    static const char* KEY_NIGHT_MODE;
    static const char* KEY_SHOW_SPEED_LIMIT;
    static const char* KEY_SHOW_DISTANCE;
    static const char* KEY_SHOW_INSTRUCTION;
    static const char* KEY_DISTANCE_UNIT;
    static const char* KEY_RECONNECT_DELAY;
    static const char* KEY_AUTO_CONNECT;
    static const char* KEY_SLEEP_TIMEOUT;
    static const char* KEY_WAKE_ON_DATA;
    
public:
    Settings();
    ~Settings();
    
    bool init();
    bool isInitialized() const { return initialized; }
    
    // Settings management
    bool load();
    bool save();
    void reset();
    
    // Getters
    const HUDSettings& getSettings() const { return currentSettings; }
    uint8_t getBrightness() const { return currentSettings.brightness; }
    uint8_t getRotation() const { return currentSettings.rotation; }
    bool getAutoRotation() const { return currentSettings.autoRotation; }
    bool getNightMode() const { return currentSettings.nightMode; }
    bool getShowSpeedLimit() const { return currentSettings.showSpeedLimit; }
    bool getShowDistance() const { return currentSettings.showDistance; }
    bool getShowInstruction() const { return currentSettings.showInstruction; }
    uint8_t getDistanceUnit() const { return currentSettings.distanceUnit; }
    uint16_t getReconnectDelay() const { return currentSettings.reconnectDelay; }
    bool getAutoConnect() const { return currentSettings.autoConnect; }
    uint16_t getSleepTimeout() const { return currentSettings.sleepTimeout; }
    bool getWakeOnData() const { return currentSettings.wakeonData; }
    
    // Setters
    void setBrightness(uint8_t brightness);
    void setRotation(uint8_t rotation);
    void setAutoRotation(bool enabled);
    void setNightMode(bool enabled);
    void setShowSpeedLimit(bool show);
    void setShowDistance(bool show);
    void setShowInstruction(bool show);
    void setDistanceUnit(uint8_t unit);
    void setReconnectDelay(uint16_t delay);
    void setAutoConnect(bool enabled);
    void setSleepTimeout(uint16_t timeout);
    void setWakeOnData(bool enabled);
    
    // Convenience methods
    void toggleNightMode();
    void toggleAutoRotation();
};