#ifndef BLE
#define BLE
#include <BLEDevice.h>

class Ble
{
public:
    // properties

    // methods
    static bool connect(BLEAddress pAddress);
    static void setup();
    static void loop();
};

#endif // BLE