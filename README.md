# FUGU-ARDUINO-MPPT-FIRMWARE
An open source Arduino ESP32 MPPT Solar Charge Controller firmware equipped with charging algorithms, WiFi, LCD menus &amp; more!

This is a clone from https://github.com/AngeloCasi/FUGU-ARDUINO-MPPT-FIRMWARE.git
I have modified the firmware and published here for anyone to use.

## Required libraries:
[Blynk ESP32 Library](https://github.com/blynkkk/blynk-library)

[Arduino-LiquidCrystal-I2C-library](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)

[Adafruit_ADS1X15](https://github.com/adafruit/Adafruit_ADS1X15)

[WiFiManager](https://github.com/tzapu/WiFiManager)

## Additional requirement:
This sketch currently work on ESP32 library by Espressif Systems version 2.0.17 and below only. If you use updated board library, you will get errors.

## Usage
Assuming you have already built the FUGU MPPT Solar Charge Controller using AngeloCasi's guide, here are the steps flashing this firmware version:
1. Edit user parameters in Arduino IDE such as Blynk authentication code, battery and other parameters (if you are using the non-lcd version), etc...
2. Flash the firmware to the device.
3. Connect the device to your batteries and solar panels.
4. Perform factory reset if you have LCD and buttons and set your parameters in the settings menu. Skip to next step you don't have LCD.
5. Using a phone or Laptop, connect WiFi to the "FUGU DIY MPPT" access point. There is no password needed. Once connected, you will be redirected to a configuration page to setup WiFi connection. Select your WiFi access point and input your WiFi password and connect. You will be disconnected automatically once it connects. Ther LCD should also prompt that WiFi is connected. In case you change the WiFi SSID or password of your router, you can reset your credentials in the settings menu or perform a factory reset. You will be required to set everything back again if you chose to perform factory reset. 
## Changelog:

Initial release Oct. 22, 2022

* set ADS1115 as default parameter (ADS1115 is actually cheaper now and easier to buy than ADS1015) NOTE: Change the values if you are using ADS1015

* can still work with the latest Blynk. Blynk Legacy is dead. (you need to create your own widgets in blynk console)

* added reconnect function for wifi and blynk when device gets offline

* added days running in Blynk Iot App

* Changed IUV protection code in charger mode that prevents small voltage panels (ex: 19v panel on 12v battery) where in IUV always triggers when the voltage input reaches above maximum battery voltage setting. This makes the MPPT inefficient as it restarts the buck repeatedly. By changing the code, the MPPT algorithm does its job more efficient while maintaining the input undervoltage protection.

* Add a wifi manager so hard coding wifi credentials into the sketch is no longer required. Wifi can be configured using a phone or computer using a web browser. To reset WiFi configuration, you need to perform a factory reset. In the future, I will add an additional setting to reset WiFi credentials using the buttons and Display.

* Added BlynkTimer to prevent disconnecting from BLynk server frequently.

* Added buttons to adjust battery charging voltage settings in Blynk app.

Update Oct. 17, 2024

* Removed buttons to adjust battery charging voltage settings in Blynk app. This feature seems dangerous and can accidentally set the charging voltage too high because of the terrible input lag. It is better to set proper voltages using the sketch or via LCD and buttons.

* Added the option for LCD daytime on (LCD on when sunlight is detected and charging starts. LCD turns off when the sun is down.) I also removed some LCD sleep timer setting.

* Reduced Temperature Sensor Average Sampling Count for faster temperature sensor

Update Oct. 18, 2024

* Fixed bug where connected to WiFi notification does not appear on LCD

* Changed displayConfig1 to display temperature status instead of number of days running.

Update Nov. 7, 2024

* Adjust fan temp trigger to 45 degrees for better efficiency

* set LCD sleep timer default setting to Daytime on

Update Nov. 15, 2024

* Added an option to reset WiFi credentials in the settings menu
