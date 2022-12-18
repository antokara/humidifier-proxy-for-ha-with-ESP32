# humidifier-proxy-for-ha-with-ESP32

a humidifier proxy for Home Assistant with an ESP32 DEVKIT V1

## prerequisites

### Mosquitto MQTT broker

#### Mosquitto Installation

[install the Mosquitto MQTT broker on HA](https://github.com/home-assistant/addons/blob/master/mosquitto/DOCS.md)

#### Mosquitto Options

```yaml
logins: []
require_certificate: false
certfile: fullchain.pem
keyfile: privkey.pem
customize:
active: false
folder: mosquitto
```

\*Note that without a proper certificate, we can't enable the secure ports and we will have to enable and use the insecure 1883, 1884 ports.

#### Mosquitto User

go to HA user management and create a dedicated user, that will be used by our Mosquitto client.

Warning:

- changing the password, because it caches the credentials, requires:
  - reload the Mosquitto broker integration first
  - restart of the _Mosquitto broker_ addon second
  - otherwise you will get `Connection error: Connection Refused: not authorised. Error: The connection was refused.`
  - if you get only `Error: Connection refused` it means the port you try to use is not open (probably due to certificate issues), it does not mean the credentials are not correct
- certain special characters are not allowed (ie. the underscore is safe to use) and perhaps even very long passwords as well
- the user should not be an admin
- the user should only have access from local network

### IDE


### VS Code

1. install VS Code
1. make sure the _Arduino_ and _z-uno_ extensions are not installed or at least, _disabled_ if installed
1. [install the PlatformIO extension](https://platformio.org/platformio-ide)
   1. on Chromebook
      1. `sudo apt-get install python3-venv`
      1. restart VSCode
      1. warning: it may take up to 10 minutes to finish the installation and all the pio commands to become available
1. open Libraries of PIO from side panel, search for `home-assistant-integration` and install it
1. install [99-platformio-udev.rules](https://docs.platformio.org/en/latest/core/installation/udev-rules.html)
   1. `curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules`
   1. on Fedora
      1. `sudo udevadm control --reload-rules && sudo udevadm trigger`
   1. on Chromebook
      1. `sudo service udev restart`
   1. on Fedora/Chromebook
      1. `sudo usermod -a -G dialout $USER`
      1. `sudo usermod -a -G plugdev $USER`
   1. logout / login to O/S
   1. physically unplug and reconnect your board
      1. on Chromebook
         1. select connect to Linux on chromebook
         1. it should appear as PicoArduino in the Manage USB devices
         1. then, serial monitor can connect
1. to initialize the project
   1. `pio project init`
   1. should give `Project has been successfully updated!`
1. select the active project environment
    1. `>PlatformIO: Switch Project Environment` or from the bottom left corner of the IDE
    1. select USB (for first upload) or OTA (for subsequent ones but change the `auth` inside `platformio.ini` file)
1. to build
   1. `>PlatformIO: Build` or `pio run` or from the bottom left corner of the IDE
   1. should result in `[SUCCESS]`
1. to monitor the serial port for debugging
   1. `>PlatformIO: Serial Monitor` or from the bottom left corner of the IDE
   1. should open up a new Terminal with the serial monitor
1. to upload
   1. `>PlatformIO: Upload` or from the bottom left corner of the IDE
   1. should show progress `Loading into Flash: [====] 100%` and `[SUCCESS]`
   1. if not, make sure you have installed the udev rules properly...
   1. warning: Upload over USB is [not possible currently on Chromebooks](https://bugs.chromium.org/p/chromium/issues/detail?id=980456). Therefore, the first upload MUST take place from another O/S (ie. Fedora) and subsequent uploads can happen OTA from Chromebook
