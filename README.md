# FUGU-ARDUINO-MPPT-FIRMWARE
An open source Arduino ESP32 MPPT Solar Charge Controller firmware equipped with charging algorithms, WiFi, LCD menus &amp; more!

This is a clone from https://github.com/AngeloCasi/FUGU-ARDUINO-MPPT-FIRMWARE.git
I have modified the firmware and published here for anyone to use.

Changelog:
Initial release Oct. 22, 2022
-set ADS1115 as default parameter (ADS1115 is actually cheaper now and easier to buy than ADS1015) NOTE: Change the values if you are using ADS1015

-added support for latest blynk (you need to create your own widgets in blynk console)

-added reconnect function for wifi and blynk when device gets offline

-added days running in Blynk Iot App

-Changed IUV protection code in charger mode that prevents small voltage panels (ex: 19v panel on 12v battery) where in IUV always triggers when the voltage input reaches above maximum battery voltage setting. This makes the MPPT inefficient as it restarts the buck repeatedly. By changing the code, the MPPT algorithm does its job more efficient while maintaining the input undervoltage protection.

-Add a wifi manager so hard coding wifi credentials into the sketch is no longer required. Wifi can be configured using a phone or computer using a web browser.

-Added BlynkTimer to prevent disconnecting from BLynk server frequently.

-Added buttons to adjust battery charging voltage settings in Blynk app.

Update Oct. 17, 2024

-Removed buttons to adjust battery charging voltage settings in Blynk app. This feature seems dangerous and can accidentally set the charging voltage too high because of the terrible input lag. It is better to set proper voltages using the sketch or via LCD and buttons.

-Added the option for LCD daytime on (LCD on when sunlight is detected and charging starts. LCD turns off when the sun is down.) I also removed some LCD sleep timer setting.
