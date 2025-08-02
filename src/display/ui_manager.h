#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include "amoled_driver.h"
#include "../ble/ble_server.h"

enum UIState {
    UI_STARTUP,
    UI_CONNECTING,
    UI_NAVIGATION,
    UI_NO_DATA,
    UI_ERROR
};

enum UITheme {
    THEME_DAY,
    THEME_NIGHT
};

class UIManager {
private:
    AmoledDriver* display;
    UIState currentState;
    UITheme currentTheme;
    NavigationData lastNavData;
    float currentRotation;
    
    // Display positions for round screen
    int16_t centerX, centerY;
    int16_t radius;
    
    // Colors for current theme
    uint16_t bgColor, textColor, accentColor, warningColor;
    
    void updateThemeColors();
    void drawBackground();
    void drawConnectionStatus(bool connected);
    void drawSpeedLimit(int speedLimit);
    void drawDistance(int distance);
    void drawInstruction(const String& instruction);
    void drawTurnDirection(int direction);
    
    // Text rendering helpers
    void drawCenteredText(const String& text, int16_t y, uint8_t size, uint16_t color);
    void drawTextInArc(const String& text, int16_t centerX, int16_t centerY, int16_t radius, float startAngle, uint8_t size, uint16_t color);
    
public:
    UIManager();
    ~UIManager();
    
    bool init(AmoledDriver* disp);
    void update();
    
    // State management
    void setState(UIState state);
    UIState getState() const { return currentState; }
    
    // Theme management
    void setTheme(UITheme theme);
    UITheme getTheme() const { return currentTheme; }
    void toggleTheme();
    
    // Display updates
    void showStartupScreen();
    void showConnectingScreen();
    void updateNavigation(const NavigationData& navData);
    void showNoDataScreen();
    void showErrorScreen(const String& error);
    
    // Rotation support
    void setRotation(float rotation);
    float getRotation() const { return currentRotation; }
};