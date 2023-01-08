#include <Arduino.h>
#include <BLEDevice.h>

#include "ble.h"
// #include "device.h"

// H400
// E0:33:05:3D:0B:74
#define BLE_SERVER_NAME "H400"
#define BLE_SERVER_ADDRESS "e0:33:05:3d:0b:74"
// static BLEAddress bleServerAddress("e0:33:05:3d:0b:74");

static BLEUUID bmeServiceUUID("fdce1234-1013-4120-b919-1dbb32a2d132");
static BLEUUID temperatureCharacteristicUUID("fdce2346-1013-4120-b919-1dbb32a2d132");

// Flags stating if should begin connecting and if the connection is up
static bool doConnect = false;
static bool connected = false;

// Address of the peripheral device. Address will be found during scanning...
static BLEAddress *pServerAddress;
static BLEAdvertisedDevice *pAdvertisedDevice;

// Characteristicd that we want to read
// static BLERemoteCharacteristic *temperatureCharacteristic;

// Callback function that gets called, when another device's advertisement has been received

BLEClient *pClient;
BLEScan *pBLEScan;

bool Ble::connect(BLEAddress address)
{
    pBLEScan->stop();
    Serial.println("Connecting to BLE Server...");
    bool connected = pClient->connect(address);
    // bool connected = pClient->connect(address, BLE_ADDR_TYPE_RANDOM);
    if (connected)
    {
        Serial.println("Connected to BLE server");
        if (pClient->isConnected())
        {
            Serial.println("Connection Successfull");
        }
    }
    else
    {
        Serial.println("Failed to connect BLE server");
    }

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(bmeServiceUUID);
    if (pRemoteService == nullptr)
    {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(bmeServiceUUID.toString().c_str());
        return (false);
    }

    // Obtain a reference to the characteristics in the service of the remote BLE server.
    //   temperatureCharacteristic = pRemoteService->getCharacteristic(temperatureCharacteristicUUID);
    //   humidityCharacteristic = pRemoteService->getCharacteristic(humidityCharacteristicUUID);

    //   if (temperatureCharacteristic == nullptr || humidityCharacteristic == nullptr) {
    //     Serial.print("Failed to find our characteristic UUID");
    //     return false;
    //   }
    //   Serial.println(" - Found our characteristics");

    //   //Assign callback functions for the Characteristics
    //   temperatureCharacteristic->registerForNotify(temperatureNotifyCallback);
    //   humidityCharacteristic->registerForNotify(humidityNotifyCallback);
    return true;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        String serverName = advertisedDevice.getName().c_str();
        Serial.println(serverName);
        if (advertisedDevice.getName() == BLE_SERVER_NAME)
        {
            Serial.println("BLE Server with Name was found!");
            pServerAddress = new BLEAddress(advertisedDevice.getAddress()); // Address of advertiser is the one we need
            String serverAddress = (*pServerAddress).toString().c_str();
            if (serverAddress == BLE_SERVER_ADDRESS)
            {
                pAdvertisedDevice = &advertisedDevice;
                Serial.println("BLE Server with Address found!");
                advertisedDevice.getScan()->stop();
                // Ble::connect(*pServerAddress);
                // pBLEScan->stop();
                doConnect = true;
            }
            else
            {
                Serial.print("BLE Server mismatch: ");
                Serial.println(serverAddress);
            }
        }
    }
};

class MySecurity : public BLESecurityCallbacks
{

    uint32_t onPassKeyRequest()
    {
        return 123456;
    }
    void onPassKeyNotify(uint32_t pass_key)
    {
        Serial.println("The passkey YES/NO number:");
        Serial.println(pass_key);
    }
    bool onConfirmPIN(uint32_t pass_key)
    {
        Serial.println("The passkey YES/NO number:");
        Serial.println(pass_key);
        delay(5000);
        return true;
    }
    bool onSecurityRequest()
    {
        Serial.println("Security Request");
        return true;
    }
    void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl)
    {
        if (auth_cmpl.success)
        {
            Serial.println("remote BD_ADDR:");
            // Serial.println(auth_cmpl.bd_addr);
            // Serial.println("address type = %d", auth_cmpl.addr_type);
        }
        if (auth_cmpl.success)
        {
            Serial.println("pair success");
        }
        else
        {
            Serial.println("pair fail");
        }
    }
};

/**
 * @brief should be called once, from the main setup() function
 *
 */
void Ble::setup()
{
    // Init BLE device
    BLEDevice::init("testClient");

    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
    BLEDevice::setSecurityCallbacks(new MySecurity());
    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setKeySize();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);
    pSecurity->setCapability(ESP_IO_CAP_IO);
    pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    pClient = BLEDevice::createClient();
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->start(30);
}

/**
 * @brief should be called on every iteration of the main loop() function
 *
 */
void Ble::loop()
{
    if (doConnect)
    {
        Serial.println("Connecting to BLE Server...");
        // Ble::connect(*pServerAddress);
        // bool connected = pClient->connect(&advertisedDevice);
        pClient->connect(pAdvertisedDevice);
        // bool connected = pClient->connect(address, BLE_ADDR_TYPE_RANDOM);
        if (pClient->isConnected() || connected)
        {
            Serial.println("Connected to BLE server");
            if (pClient->isConnected())
            {
                Serial.println("Connection Successfull");
            }
        }
        else
        {
            Serial.println("Failed to connect BLE server");
        }
    }
}