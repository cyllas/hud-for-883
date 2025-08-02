#include "ui_manager.h"
#include <math.h>

UIManager::UIManager() : display(nullptr), currentState(UI_STARTUP), 
                         currentTheme(THEME_DAY), currentRotation(0.0) {
    centerX = AMOLED_WIDTH / 2;
    centerY = AMOLED_HEIGHT / 2;
    radius = AMOLED_RADIUS;
}

UIManager::~UIManager() {
}

bool UIManager::init(AmoledDriver* disp) {
    if (!disp || !disp->isInitialized()) {
        Serial.println("UI Manager: Invalid display driver");
        return false;
    }
    
    display = disp;
    updateThemeColors();
    
    Serial.println("UI Manager initialized successfully");
    return true;
}

void UIManager::updateThemeColors() {
    switch (currentTheme) {
        case THEME_DAY:
            bgColor = COLOR_WHITE;
            textColor = COLOR_BLACK;
            accentColor = COLOR_BLUE;
            warningColor = COLOR_RED;
            break;
        case THEME_NIGHT:
            bgColor = COLOR_BLACK;
            textColor = COLOR_WHITE;
            accentColor = COLOR_GREEN;
            warningColor = COLOR_ORANGE;
            break;
    }
}

void UIManager::setTheme(UITheme theme) {
    currentTheme = theme;
    updateThemeColors();
    
    // Redraw current screen with new colors
    switch (currentState) {
        case UI_STARTUP: showStartupScreen(); break;
        case UI_CONNECTING: showConnectingScreen(); break;
        case UI_NAVIGATION: updateNavigation(lastNavData); break;
        case UI_NO_DATA: showNoDataScreen(); break;
        case UI_ERROR: showErrorScreen("Theme changed"); break;
    }
}

void UIManager::toggleTheme() {
    setTheme(currentTheme == THEME_DAY ? THEME_NIGHT : THEME_DAY);
}

void UIManager::setState(UIState state) {
    currentState = state;
}

void UIManager::drawBackground() {
    display->fillScreen(bgColor);
    
    // Draw circular border
    display->drawCircle(centerX, centerY, radius - 2, accentColor);
    display->drawCircle(centerX, centerY, radius - 3, accentColor);
}

void UIManager::showStartupScreen() {
    setState(UI_STARTUP);
    drawBackground();
    
    drawCenteredText("ESP32-S3", centerY - 40, 3, textColor);
    drawCenteredText("HUD Navigation", centerY, 2, textColor);
    drawCenteredText("Starting...", centerY + 40, 1, accentColor);
}

void UIManager::showConnectingScreen() {
    setState(UI_CONNECTING);
    drawBackground();
    
    drawCenteredText("HUD Ready", centerY - 30, 2, textColor);
    drawCenteredText("Waiting for", centerY, 1, textColor);
    drawCenteredText("Sygic Connection", centerY + 20, 1, accentColor);
    
    // Draw BLE indicator
    display->fillCircle(centerX, centerY + 60, 8, accentColor);
}

void UIManager::updateNavigation(const NavigationData& navData) {
    setState(UI_NAVIGATION);
    lastNavData = navData;
    
    if (!navData.isValid) {
        showNoDataScreen();
        return;
    }
    
    drawBackground();
    
    // Draw speed limit (top)
    if (navData.speedLimit > 0) {
        drawSpeedLimit(navData.speedLimit);
    }
    
    // Draw instruction (center)
    drawInstruction(navData.instruction);
    
    // Draw distance (bottom)
    drawDistance(navData.distance);
    
    // Draw turn direction indicator
    drawTurnDirection(navData.turnDirection);
}

void UIManager::showNoDataScreen() {
    setState(UI_NO_DATA);
    drawBackground();
    
    drawCenteredText("Connected", centerY - 30, 2, accentColor);
    drawCenteredText("No Navigation", centerY, 1, textColor);
    drawCenteredText("Data", centerY + 20, 1, textColor);
}

void UIManager::showErrorScreen(const String& error) {
    setState(UI_ERROR);
    drawBackground();
    
    drawCenteredText("ERROR", centerY - 30, 2, warningColor);
    drawCenteredText(error, centerY + 10, 1, textColor);
}

void UIManager::drawSpeedLimit(int speedLimit) {
    // Draw speed limit in top area
    int16_t x = centerX;
    int16_t y = centerY - 120;
    
    // Speed limit circle
    display->fillCircle(x, y, 35, COLOR_RED);
    display->fillCircle(x, y, 30, COLOR_WHITE);
    display->drawCircle(x, y, 30, COLOR_RED);
    
    // Speed limit text
    String speedText = String(speedLimit);
    int16_t textX = x - (speedText.length() * 6);
    int16_t textY = y - 8;
    
    display->setCursor(textX, textY);
    display->setTextColor(COLOR_BLACK);
    display->setTextSize(2);
    display->print(speedText);
}

void UIManager::drawDistance(int distance) {
    // Draw distance in bottom area
    String distText;
    if (distance >= 1000) {
        distText = String(distance / 1000.0, 1) + "km";
    } else {
        distText = String(distance) + "m";
    }
    
    drawCenteredText(distText, centerY + 100, 2, textColor);
}

void UIManager::drawInstruction(const String& instruction) {
    // Draw instruction in center area
    drawCenteredText(instruction, centerY, 1, textColor);
}

void UIManager::drawTurnDirection(int direction) {
    // Draw turn arrow based on direction
    int16_t arrowX = centerX;
    int16_t arrowY = centerY + 50;
    int16_t arrowSize = 20;
    
    switch (direction) {
        case 0x01: // Left turn
            // Draw left arrow
            for (int i = 0; i < arrowSize; i++) {
                display->drawFastHLine(arrowX - arrowSize + i, arrowY - i/2, arrowSize - i, accentColor);
                display->drawFastHLine(arrowX - arrowSize + i, arrowY + i/2, arrowSize - i, accentColor);
            }
            break;
            
        case 0x02: // Right turn
            // Draw right arrow
            for (int i = 0; i < arrowSize; i++) {
                display->drawFastHLine(arrowX + i, arrowY - i/2, arrowSize - i, accentColor);
                display->drawFastHLine(arrowX + i, arrowY + i/2, arrowSize - i, accentColor);
            }
            break;
            
        case 0x03: // Straight
            // Draw up arrow
            for (int i = 0; i < arrowSize; i++) {
                display->drawFastVLine(arrowX - i/2, arrowY - arrowSize + i, arrowSize - i, accentColor);
                display->drawFastVLine(arrowX + i/2, arrowY - arrowSize + i, arrowSize - i, accentColor);
            }
            break;
            
        case 0x04: // U-turn
            // Draw U-turn indicator
            display->drawCircle(arrowX, arrowY, arrowSize/2, accentColor);
            display->fillRect(arrowX - arrowSize/4, arrowY, arrowSize/2, arrowSize/2, bgColor);
            break;
    }
}

void UIManager::drawCenteredText(const String& text, int16_t y, uint8_t size, uint16_t color) {
    int16_t textWidth = text.length() * 6 * size;
    int16_t x = centerX - textWidth / 2;
    
    display->setCursor(x, y);
    display->setTextColor(color);
    display->setTextSize(size);
    display->print(text);
}

void UIManager::setRotation(float rotation) {
    currentRotation = rotation;
    
    // Apply rotation to display if significantly changed
    int newRotation = (int)((rotation + 45) / 90) % 4;
    display->setRotation(newRotation);
}

void UIManager::update() {
    // UI update logic - can be used for animations or periodic updates
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();
    
    if (now - lastUpdate > 1000) { // Update every second
        // Update connection indicator or other periodic elements
        lastUpdate = now;
    }
}