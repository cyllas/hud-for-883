#include "ble_server.h"

// Sygic BLE Service UUID from original project
const char* BLEServer::SERVICE_UUID = "DD3F0AD1-6239-4E1F-81F1-91F6C9F01D86";
const char* BLEServer::CHARACTERISTIC_UUID = "5D0360B2-2D3B-4BDC-B688-E1EC92394B8C";

class BLEServer::ServerCallbacks: public NimBLEServerCallbacks {
    BLEServer* server;
public:
    ServerCallbacks(BLEServer* srv) : server(srv) {}
    
    void onConnect(NimBLEServer* pServer) {
        server->deviceConnected = true;
        Serial.println("BLE Client connected");
    }
    
    void onDisconnect(NimBLEServer* pServer) {
        server->deviceConnected = false;
        Serial.println("BLE Client disconnected");
        server->startAdvertising();
    }
};

class BLEServer::CharacteristicCallbacks: public NimBLECharacteristicCallbacks {
    BLEServer* server;
public:
    CharacteristicCallbacks(BLEServer* srv) : server(srv) {}
    
    void onWrite(NimBLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
            server->parseNavigationData((uint8_t*)value.data(), value.length());
        }
    }
};

BLEServer::BLEServer() : pServer(nullptr), pService(nullptr), 
                         pCharacteristic(nullptr), pAdvertising(nullptr),
                         deviceConnected(false), hasNewNavData(false) {
}

BLEServer::~BLEServer() {
    if (pServer) {
        NimBLEDevice::deinit(true);
    }
}

bool BLEServer::init() {
    Serial.println("Initializing BLE server...");
    
    // Initialize NimBLE
    NimBLEDevice::init("ESP32-S3-HUD");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    
    // Create BLE Server
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks(this));
    
    // Create BLE Service
    pService = pServer->createService(SERVICE_UUID);
    
    // Create BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
    );
    
    pCharacteristic->setCallbacks(new CharacteristicCallbacks(this));
    
    // Start the service
    pService->start();
    
    // Start advertising
    pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    
    Serial.println("BLE server initialized successfully");
    return true;
}

void BLEServer::startAdvertising() {
    if (pAdvertising && !deviceConnected) {
        pAdvertising->start();
        Serial.println("BLE advertising started");
    }
}

void BLEServer::stopAdvertising() {
    if (pAdvertising) {
        pAdvertising->stop();
        Serial.println("BLE advertising stopped");
    }
}

void BLEServer::parseNavigationData(uint8_t* data, size_t length) {
    if (length < 7) {
        Serial.println("Invalid navigation data length");
        return;
    }
    
    // Parse based on original project format
    // Example data: 0x01320A3335306D
    // [0] = basic data flag
    // [1][2] = speed limit (hex to decimal)
    // [3] = turn direction
    // [4][5][6][7] = distance in meters
    
    currentNavData.isValid = true;
    
    // Speed limit (km/h)
    currentNavData.speedLimit = (data[1] << 8) | data[2];
    
    // Turn direction
    currentNavData.turnDirection = data[3];
    
    // Distance (meters) - convert from hex string
    if (length >= 7) {
        String distStr = "";
        for (int i = 4; i < min((int)length, 7); i++) {
            distStr += String(data[i], HEX);
        }
        currentNavData.distance = distStr.toInt();
    }
    
    // Set instruction based on turn direction
    switch (currentNavData.turnDirection) {
        case 0x01: currentNavData.instruction = "Turn Left"; break;
        case 0x02: currentNavData.instruction = "Turn Right"; break;
        case 0x03: currentNavData.instruction = "Go Straight"; break;
        case 0x04: currentNavData.instruction = "U-Turn"; break;
        default: currentNavData.instruction = "Continue"; break;
    }
    
    hasNewNavData = true;
    
    Serial.printf("Navigation Update: %s, %dm, %dkm/h\n", 
                  currentNavData.instruction.c_str(),
                  currentNavData.distance,
                  currentNavData.speedLimit);
}

NavigationData BLEServer::getNavigationData() {
    hasNewNavData = false;
    return currentNavData;
}