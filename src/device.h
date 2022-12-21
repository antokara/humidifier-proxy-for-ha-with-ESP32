#ifndef DEVICE
#define DEVICE

#include <ArduinoHA.h>
#include <WiFi.h>

#define DEVICE_ID "humidifierProxyTest"
#define DEVICE_NAME "Humidifier Proxy"
#define FIRMWARE_VERSION "1.0.0"

// uncomment to enable serial.print debug messages
#define SERIAL_DEBUG

/**
 * @brief time in milliseconds to wait for the WiFi to connect
 *
 */
#define WAIT_FOR_WIFI 10000

/**
 * @brief frequence in milliseconds,
 * to check for the Wifi connection status
 */
#define WIFI_CHECK_FREQUENCY 10000

/**
 * @brief frequence in milliseconds,
 * to send a heartbit status update to the controller
 */
#define HEARTBIT_FREQUENCY 15000

/**
 * @brief the status we send when we first connect to the controller
 * at the very first loop iteration
 */
#define STATUS_CONNECTED "connected"

/**
 * @brief the status we send to to controller, after the first loop iteration
 * to signify we are ready and also as a heartbit
 */
#define STATUS_READY "ready"

class Device
{
public:
    // properties
    static int wifiStatus;
    static WiFiClient client;
    static HADevice device;
    static HAMqtt mqtt;
    static HASensor statusSensor;
    static bool firstLoop;
    static unsigned long lastWifiCheck;
    static unsigned long lastHeartbit;

    // methods
    static void connectToMQTT();
    static void connectToWifi();
    static void wifiLoop();
    static void heartbitLoop();
    static void setupOTA();
    static void setup();
    static void loop();
};

#endif // DEVICE