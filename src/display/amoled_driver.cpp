#include "amoled_driver.h"

AmoledDriver::AmoledDriver() : spi(nullptr), initialized(false), rotation(0) {
}

AmoledDriver::~AmoledDriver() {
    if (spi) {
        spi->end();
    }
}

bool AmoledDriver::init() {
    Serial.println("Initializing AMOLED display...");
    
    // Configure QSPI pins (Official Waveshare ESP32-S3 Touch AMOLED 1.43)
    pinMode(AMOLED_RST, OUTPUT);
    pinMode(AMOLED_EN, OUTPUT);
    
    digitalWrite(AMOLED_RST, HIGH);
    digitalWrite(AMOLED_EN, HIGH); // Enable AMOLED power
    
    // Initialize QSPI interface
    // Note: QSPI setup requires special ESP32-S3 QSPI library or custom implementation
    // For now, using standard SPI on D0 line as fallback
    spi = new SPIClass(HSPI);
    spi->begin(AMOLED_CLK, -1, AMOLED_D0, -1); // CLK, MISO(unused), MOSI(D0), CS(unused)
    spi->setFrequency(40000000); // 40MHz
    spi->setDataMode(SPI_MODE0);
    
    // Reset display
    reset();
    
    // Initialize display controller
    initDisplay();
    
    // Clear screen
    fillScreen(COLOR_BLACK);
    
    initialized = true;
    Serial.println("AMOLED display initialized successfully");
    return true;
}

void AmoledDriver::reset() {
    digitalWrite(AMOLED_RST, LOW);
    delay(10);
    digitalWrite(AMOLED_RST, HIGH);
    delay(10);
}

void AmoledDriver::initDisplay() {
    // Basic initialization sequence for AMOLED controller
    // This is a simplified version - actual sequence depends on specific controller
    
    writeCommand(0x11); // Sleep out
    delay(120);
    
    writeCommand(0x36); // Memory access control
    writeData(0x00);    // Normal orientation
    
    writeCommand(0x3A); // Pixel format
    writeData(0x55);    // 16-bit RGB565
    
    writeCommand(0x29); // Display on
    delay(10);
}

void AmoledDriver::writeCommand(uint8_t cmd) {
    // QSPI command write - simplified for single data line
    spi->transfer(cmd);
}

void AmoledDriver::writeData(uint8_t data) {
    // QSPI data write - simplified for single data line
    spi->transfer(data);
}

void AmoledDriver::writeData16(uint16_t data) {
    // QSPI 16-bit data write - simplified for single data line
    spi->transfer16(data);
}

void AmoledDriver::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    writeCommand(0x2A); // Column address set
    writeData16(x0);
    writeData16(x1);
    
    writeCommand(0x2B); // Row address set
    writeData16(y0);
    writeData16(y1);
    
    writeCommand(0x2C); // Memory write
}

void AmoledDriver::setRotation(uint8_t rot) {
    rotation = rot % 4;
    writeCommand(0x36); // Memory access control
    
    switch (rotation) {
        case 0: writeData(0x00); break; // Normal
        case 1: writeData(0x60); break; // 90 degrees
        case 2: writeData(0xC0); break; // 180 degrees
        case 3: writeData(0xA0); break; // 270 degrees
    }
}

void AmoledDriver::fillScreen(uint16_t color) {
    fillRect(0, 0, AMOLED_WIDTH, AMOLED_HEIGHT, color);
}

void AmoledDriver::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || y < 0 || x >= AMOLED_WIDTH || y >= AMOLED_HEIGHT) return;
    if (!isInCircle(x, y)) return; // Only draw within circle for round display
    
    setAddrWindow(x, y, x, y);
    writeData16(color);
}

void AmoledDriver::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if (y < 0 || y >= AMOLED_HEIGHT) return;
    if (x < 0) { w += x; x = 0; }
    if ((x + w - 1) >= AMOLED_WIDTH) w = AMOLED_WIDTH - x;
    if (w <= 0) return;
    
    setAddrWindow(x, y, x + w - 1, y);
    for (int16_t i = 0; i < w; i++) {
        if (isInCircle(x + i, y)) {
            writeData16(color);
        } else {
            writeData16(COLOR_BLACK); // Background for areas outside circle
        }
    }
}

void AmoledDriver::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if (x < 0 || x >= AMOLED_WIDTH) return;
    if (y < 0) { h += y; y = 0; }
    if ((y + h - 1) >= AMOLED_HEIGHT) h = AMOLED_HEIGHT - y;
    if (h <= 0) return;
    
    setAddrWindow(x, y, x, y + h - 1);
    for (int16_t i = 0; i < h; i++) {
        if (isInCircle(x, y + i)) {
            writeData16(color);
        } else {
            writeData16(COLOR_BLACK);
        }
    }
}

void AmoledDriver::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (x >= AMOLED_WIDTH || y >= AMOLED_HEIGHT) return;
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if ((x + w - 1) >= AMOLED_WIDTH) w = AMOLED_WIDTH - x;
    if ((y + h - 1) >= AMOLED_HEIGHT) h = AMOLED_HEIGHT - y;
    if (w <= 0 || h <= 0) return;
    
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    for (int16_t i = 0; i < h; i++) {
        for (int16_t j = 0; j < w; j++) {
            if (isInCircle(x + j, y + i)) {
                writeData16(color);
            } else {
                writeData16(COLOR_BLACK);
            }
        }
    }
}

bool AmoledDriver::isInCircle(int16_t x, int16_t y) {
    int16_t centerX = AMOLED_WIDTH / 2;
    int16_t centerY = AMOLED_HEIGHT / 2;
    int16_t dx = x - centerX;
    int16_t dy = y - centerY;
    return (dx * dx + dy * dy) <= (AMOLED_RADIUS * AMOLED_RADIUS);
}

void AmoledDriver::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void AmoledDriver::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        drawFastVLine(x0 + x, y0 - y, 2 * y + 1, color);
        drawFastVLine(x0 - x, y0 - y, 2 * y + 1, color);
        drawFastVLine(x0 + y, y0 - x, 2 * x + 1, color);
        drawFastVLine(x0 - y, y0 - x, 2 * x + 1, color);
    }
}

void AmoledDriver::sleep() {
    writeCommand(0x10); // Sleep in
    delay(5);
}

void AmoledDriver::wakeup() {
    writeCommand(0x11); // Sleep out
    delay(120);
}