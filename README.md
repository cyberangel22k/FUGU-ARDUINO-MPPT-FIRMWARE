# FUGU-ARDUINO-MPPT-FIRMWARE
An open source Arduino ESP32 MPPT Solar Charge Controller firmware equipped with charging algorithms, WiFi, LCD menus &amp; more!

This is a clone from https://github.com/AngeloCasi/FUGU-ARDUINO-MPPT-FIRMWARE.git
I have modified the firmware and published here for anyone to use.

Required libraries:

* [Blynk](https://github.com/blynkkk/blynk-library/releases/latest)
* [LiquidCrystal-I2C-library](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
* [Adafruit_ADS1X15](https://github.com/adafruit/Adafruit_ADS1X15)
* [WiFiManager](https://github.com/tzapu/WiFiManager)

## [Release](https://github.com/cyberangel22k/FUGU-ARDUINO-MPPT-FIRMWARE/releases)

## Usage
Assuming you have already built the FUGU MPPT Solar Charge Controller using AngeloCasi's guide, here are the steps flashing this firmware version:
1. Edit user parameters in Arduino IDE such as Blynk authentication code, battery parameters (if you are using the non-lcd version), etc...
2. Flash the firmware to the device
3. Perform factory reset
4. If you use the LCD version, set your parameters in the setting.
5. Using a phone or Laptop, connect wifi to the "FUGU DIY MPPT" access point. There is no password needed. Once connected, you will be redirected to a configuration page to setup WiFi connection. Select your WiFi access point and input your WiFi password and connect. The device should now be connected to the internet. In case you change your WiFi SSID or password, perform a factory reset to reset the saved WiFi credentials.

## Changelog:

* set ADS1115 as default parameter (ADS1115 is actually cheaper now and easier to buy than ADS1015) NOTE: Change the values if you are using ADS1015
* added support for latest blynk (you need to create your own widgets in blynk console)
* added reconnect function for wifi and blynk when device gets offline
* added days running in Blynk Iot App
* Changed IUV protection code in charger mode that prevents small voltage panels (ex: 19v panel on 12v battery) where in IUV always triggers when the voltage input reaches above maximum battery voltage setting. This makes the MPPT inefficient as it restarts the buck repeatedly. By changing the code, the MPPT algorithm does its job more efficient while maintaining the input undervoltage protection.
* Add a wifi manager so hard coding wifi credentials into the sketch is no longer required. Wifi can be configured using a phone or computer using a web browser.
* Added BlynkTimer to prevent disconnecting from BLynk server frequently.
* Added buttons to adjust battery charging voltage settings in Blynk app.
