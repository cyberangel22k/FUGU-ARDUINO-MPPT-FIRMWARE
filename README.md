# FUGU-ARDUINO-MPPT-FIRMWARE
An open source Arduino ESP32 MPPT Solar Charge Controller firmware equipped with charging algorithms, WiFi, LCD menus &amp; more!

This is a clone from https://github.com/AngeloCasi/FUGU-ARDUINO-MPPT-FIRMWARE.git
I have modified the firmware and published here for anyone to use.

Changelog:

-set ADS1115 as default parameter (ADS1115 is actually cheaper now and easier to buy than ADS1015) NOTE: Change the values if you are using ADS1015

-added support for latest blynk (you need to create your own widgets in blynk console)

-added reconnect function for wifi and blynk when device gets offline
