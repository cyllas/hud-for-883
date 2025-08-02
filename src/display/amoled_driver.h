#ifndef AMOLED_DRIVER_H
#define AMOLED_DRIVER_H

#include <Arduino.h>
#include <SPI.h>

// ESP32-S3 Touch AMOLED 1.43 specifications
#define AMOLED_WIDTH  466
#define AMOLED_HEIGHT 466
#define AMOLED_RADIUS 233

// QSPI pins for ESP32-S3 Touch AMOLED 1.43 (Official Waveshare pinout)
#define AMOLED_CLK    10  // QSPI_CLK
#define AMOLED_D0     11  // QSPI_D0
#define AMOLED_D1     12  // QSPI_D1  
#define AMOLED_D2     13  // QSPI_D2
#define AMOLED_D3     14  // QSPI_D3
#define AMOLED_RST    38  // AMOLED_RST
#define AMOLED_EN     42  // AMOLED_EN

// Touch screen pins (Official Waveshare pinout)
#define TOUCH_SDA     47  // TP_SDA
#define TOUCH_SCL     48  // TP_SCL

// Color definitions (RGB565)
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_ORANGE  0xFC00
#define COLOR_GRAY    0x8410

class AmoledDriver {
private:
    SPIClass* spi;
    bool initialized;
    uint8_t rotation;
    
    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
    void writeData16(uint16_t data);
    void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void initDisplay();
    
public:
    AmoledDriver();
    ~AmoledDriver();
    
    bool init();
    void reset();
    void sleep();
    void wakeup();
    
    // Display control
    void setRotation(uint8_t rot);
    void fillScreen(uint16_t color);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    
    // Circle-specific functions (round display)
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    bool isInCircle(int16_t x, int16_t y);
    
    // Text functions
    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t color);
    void setTextSize(uint8_t size);
    void print(const String& text);
    
    // Getters
    uint16_t width() const { return AMOLED_WIDTH; }
    uint16_t height() const { return AMOLED_HEIGHT; }
    bool isInitialized() const { return initialized; }
};