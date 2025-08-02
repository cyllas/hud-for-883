#include "settings.h"

// Preference keys
const char* Settings::PREF_NAMESPACE = "hud_settings";
const char* Settings::KEY_BRIGHTNESS = "brightness";
const char* Settings::KEY_ROTATION = "rotation";
const char* Settings::KEY_AUTO_ROTATION = "auto_rot";
const char* Settings::KEY_NIGHT_MODE = "night_mode";
const char* Settings::KEY_SHOW_SPEED_LIMIT = "show_speed";
const char* Settings::KEY_SHOW_DISTANCE = "show_dist";
const char* Settings::KEY_SHOW_INSTRUCTION = "show_instr";
const char* Settings::KEY_DISTANCE_UNIT = "dist_unit";
const char* Settings::KEY_RECONNECT_DELAY = "reconnect";
const char* Settings::KEY_AUTO_CONNECT = "auto_conn";
const char* Settings::KEY_SLEEP_TIMEOUT = "sleep_time";
const char* Settings::KEY_WAKE_ON_DATA = "wake_data";

Settings::Settings() : initialized(false) {
}

Settings::~Settings() {
    if (initialized) {
        preferences.end();
    }
}

bool Settings::init() {
    Serial.println("Initializing settings...");
    
    if (!preferences.begin(PREF_NAMESPACE, false)) {
        Serial.println("Failed to initialize preferences");
        return false;
    }
    
    initialized = true;
    
    // Load settings from flash or use defaults
    load();
    
    Serial.println("Settings initialized successfully");
    return true;
}

bool Settings::load() {
    if (!initialized) return false;
    
    Serial.println("Loading settings from flash...");
    
    currentSettings.brightness = preferences.getUChar(KEY_BRIGHTNESS, currentSettings.brightness);
    currentSettings.rotation = preferences.getUChar(KEY_ROTATION, currentSettings.rotation);
    currentSettings.autoRotation = preferences.getBool(KEY_AUTO_ROTATION, currentSettings.autoRotation);
    currentSettings.nightMode = preferences.getBool(KEY_NIGHT_MODE, currentSettings.nightMode);
    currentSettings.showSpeedLimit = preferences.getBool(KEY_SHOW_SPEED_LIMIT, currentSettings.showSpeedLimit);
    currentSettings.showDistance = preferences.getBool(KEY_SHOW_DISTANCE, currentSettings.showDistance);
    currentSettings.showInstruction = preferences.getBool(KEY_SHOW_INSTRUCTION, currentSettings.showInstruction);
    currentSettings.distanceUnit = preferences.getUChar(KEY_DISTANCE_UNIT, currentSettings.distanceUnit);
    currentSettings.reconnectDelay = preferences.getUShort(KEY_RECONNECT_DELAY, currentSettings.reconnectDelay);
    currentSettings.autoConnect = preferences.getBool(KEY_AUTO_CONNECT, currentSettings.autoConnect);
    currentSettings.sleepTimeout = preferences.getUShort(KEY_SLEEP_TIMEOUT, currentSettings.sleepTimeout);
    currentSettings.wakeonData = preferences.getBool(KEY_WAKE_ON_DATA, currentSettings.wakeonData);
    
    Serial.println("Settings loaded successfully");
    return true;
}

bool Settings::save() {
    if (!initialized) return false;
    
    Serial.println("Saving settings to flash...");
    
    preferences.putUChar(KEY_BRIGHTNESS, currentSettings.brightness);
    preferences.putUChar(KEY_ROTATION, currentSettings.rotation);
    preferences.putBool(KEY_AUTO_ROTATION, currentSettings.autoRotation);
    preferences.putBool(KEY_NIGHT_MODE, currentSettings.nightMode);
    preferences.putBool(KEY_SHOW_SPEED_LIMIT, currentSettings.showSpeedLimit);
    preferences.putBool(KEY_SHOW_DISTANCE, currentSettings.showDistance);
    preferences.putBool(KEY_SHOW_INSTRUCTION, currentSettings.showInstruction);
    preferences.putUChar(KEY_DISTANCE_UNIT, currentSettings.distanceUnit);
    preferences.putUShort(KEY_RECONNECT_DELAY, currentSettings.reconnectDelay);
    preferences.putBool(KEY_AUTO_CONNECT, currentSettings.autoConnect);
    preferences.putUShort(KEY_SLEEP_TIMEOUT, currentSettings.sleepTimeout);
    preferences.putBool(KEY_WAKE_ON_DATA, currentSettings.wakeonData);
    
    Serial.println("Settings saved successfully");
    return true;
}

void Settings::reset() {
    if (!initialized) return;
    
    Serial.println("Resetting settings to defaults...");
    
    preferences.clear();
    currentSettings = HUDSettings(); // Reset to defaults
    save();
}

// Setters with automatic save
void Settings::setBrightness(uint8_t brightness) {
    currentSettings.brightness = constrain(brightness, 0, 255);
    save();
}

void Settings::setRotation(uint8_t rotation) {
    currentSettings.rotation = rotation % 4;
    save();
}

void Settings::setAutoRotation(bool enabled) {
    currentSettings.autoRotation = enabled;
    save();
}

void Settings::setNightMode(bool enabled) {
    currentSettings.nightMode = enabled;
    save();
}

void Settings::setShowSpeedLimit(bool show) {
    currentSettings.showSpeedLimit = show;
    save();
}

void Settings::setShowDistance(bool show) {
    currentSettings.showDistance = show;
    save();
}

void Settings::setShowInstruction(bool show) {
    currentSettings.showInstruction = show;
    save();
}

void Settings::setDistanceUnit(uint8_t unit) {
    currentSettings.distanceUnit = unit % 2;
    save();
}

void Settings::setReconnectDelay(uint16_t delay) {
    currentSettings.reconnectDelay = max(delay, (uint16_t)1000);
    save();
}

void Settings::setAutoConnect(bool enabled) {
    currentSettings.autoConnect = enabled;
    save();
}

void Settings::setSleepTimeout(uint16_t timeout) {
    currentSettings.sleepTimeout = max(timeout, (uint16_t)30);
    save();
}

void Settings::setWakeOnData(bool enabled) {
    currentSettings.wakeonData = enabled;
    save();
}

void Settings::toggleNightMode() {
    setNightMode(!currentSettings.nightMode);
}

void Settings::toggleAutoRotation() {
    setAutoRotation(!currentSettings.autoRotation);
}