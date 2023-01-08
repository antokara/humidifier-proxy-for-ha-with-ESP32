#include <Arduino.h>
// #include "device.h"
#include "ble.h"

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(115200);
  delay(1000); // Give the serial terminal a chance to connect, if present
  Serial.println("Ble::Setup()");

  // Device::setup();
  Ble::setup();

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // after everything is setup...
  // Device::connectToMQTT();
}

// the loop function runs over and over again forever
void loop()
{
  // Device::loop();
  Ble::loop();
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  delay(500);                      // wait for a second
}
