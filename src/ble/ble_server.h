#ifndef BLE_SERVER_H
#define BLE_SERVER_H

#include <Arduino.h>
#include <NimBLEDevice.h>

struct NavigationData {
    String instruction;
    int distance;
    int speedLimit;
    int turnDirection;
    bool isValid;
    
    NavigationData() : distance(0), speedLimit(0), turnDirection(0), isValid(false) {}
};

class BLEServer {
private:
    NimBLEServer* pServer;
    NimBLEService* pService;
    NimBLECharacteristic* pCharacteristic;
    NimBLEAdvertising* pAdvertising;
    
    NavigationData currentNavData;
    bool deviceConnected;
    bool hasNewNavData;
    
    // Sygic BLE Service UUID (from original project)
    static const char* SERVICE_UUID;
    static const char* CHARACTERISTIC_UUID;
    
    void parseNavigationData(uint8_t* data, size_t length);
    
public:
    BLEServer();
    ~BLEServer();
    
    bool init();
    void startAdvertising();
    void stopAdvertising();
    
    bool isConnected() const { return deviceConnected; }
    bool hasNewData() const { return hasNewNavData; }
    NavigationData getNavigationData();
    
    // Callback classes
    class ServerCallbacks;
    class CharacteristicCallbacks;
};

#endif // BLE_SERVER_H