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
*Note: Ensure you have the latest ESP32 Board Manager installed (Version 3.0.0 or higher recommended).*
Starting May 9, 2026 update, this firmware now requires LCD and buttons to setup the device for the first time. I may revise this soon and have an option to set everything in the captive portal so it can work again without LCD and buttons.

## Usage
Assuming you have already built the FUGU MPPT Solar Charge Controller using AngeloCasi's guide, here are the steps flashing this firmware version:
1. Make sure you have the latest ESP32 Board Manager installed together with other libraries listed above in your Arduino IDE.
2. Edit Blynk template ID and template name according to your Blynk console. Go and select Tools>Partition Scheme>Minimal SPIFFS (1.9MB APP with OTA). Flash the firmware to the device.
3. Connect the device to your batteries and solar panels.
4. On first boot, select the module you are using (ADS1015 or ADS1115). Then select battery chemistry and voltage, otherwise, use custom and input the correct values. Then select connect to WiFI.
5. Using a phone or Laptop, connect WiFi access point named "FUGU DIY MPPT". Once connected, you will be redirected to a captive portal page to setup WiFi connection and Blynk credentials. Select your WiFi access point and input your WiFi password and Blynk Auth and save. You will be disconnected automatically once it connects. The LCD should also prompt that WiFi is connected and will reboot automatically. In case you change the WiFi SSID or password of your router, you can reset your credentials in the settings menu or perform a factory reset. You will be required to set everything back again if you chose to perform factory reset.
6. If you have updates to your firmware, you can just use Blynk.Air so you don't have to connect it via USB.
## Changelog:

Initial release Oct. 22, 2022

* set ADS1115 as default parameter (ADS1115 is actually cheaper now and easier to buy than ADS1015)

  NOTE: Change the values if you are using ADS1015

  Additional Note: During the testing of this project for years, I observed that using ADS1015 is more efficient as the temps are much cooler compared when using ADS1115. You may test this too to confirm my observations.

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

Update April 1, 2026

* Fixed the sleeptimer bug that kills the backlight during active charging when set to Daytime ON (LCD backlight timeout)

* Added a setting menu item to set USB serial monitoring mode.

* Optimized WiFi connection and reconnection function. Removed unnecessary code.

* Added lifetime harvest monitoring that saves the kWh data to the ESP32's flash memory so it survives reboots. Added a displayConfig page for lifetime harvest.
 
Update May 9, 2026

* Added an option to select the correct ADS module at first boot. (This means this firmware will not work anymore for devices without buttons and LCD).

* Potentially fixed LCD bugs when reseting Wi-Fi Credentials and when device is connected a network. 

Update May, 21, 2026

* Updated setup wizard. Added an option to set date as reference for the lifetime harvest. This is automatically set if connected to Wi-Fi but you need to manually input the date in offline mode.

* Rearranged boot sequence to avoid calling setupWiFI() at the twice on first boot.

* added a vTaskDelay in coreTwo  to prevent watchdog from getting triggered.

Update May 29, 2026

* Added 3-Stage Charging Algorithm Upgrade: Fully integrated the Float Stage into the charging algorithm (Bulk -> Absorption -> Float).

* Automated Battery Presets: Integrated a new Presets.h library to handle battery chemistries. The initial Setup Wizard now includes dedicated stages to select the Battery Type (LiFePO4, Li-Ion, AGM, Flooded, Custom) and System Voltage (12V, 24V, 48V), automatically calculating and saving the proper Max, Min, Float, LVD, and LVR parameters. You can still change these settings in the LCD settings menu manually.

* Advanced Load Management (LVD/LVR): Added a new 9_Load.ino process to control a physical relay for house loads based on Low Voltage Disconnect and Reconnect thresholds. This includes a 30-second hardware debounce timer (lvdDelay) to prevent heavy inverter loads from triggering false disconnects during voltage sags. This is applicable to those who made custom PCVs.

* Blynk.Air OTA Updates: Replaced the legacy manual HTTP update button with Blynk's native InternalPinOTA push architecture. The ESP32 now intercepts the cloud signal, safely halts the buck converter (buck_Disable()), freezes the main loop, and flashes the new firmware completely over-the-air from the Blynk Web Console. This feature is currently in testing phase. You may test it and report anything you encounter.

* Offline Bluetooth Telemetry: Integrated native BluetoothSerial to broadcast real-time solar harvest data (Power, Voltage, Current, Temp) directly to mobile terminal apps for off-grid scenarios when Wi-Fi is unavailable. A dedicated toggle was added to the LCD settings menu to enable/disable the Bluetooth radio, with state memory saved to EEPROM slot 15.

* UI/UX improvements.

* Note: The PSU Mode was intentionaly stripped in this update for safety. To avoid getting compilation storage errors, go to Tools>Partition Scheme>Minimal SPIFFS (1.9MB APP with OTA).

Update May 30, 2026

* Fixed LCD bugs

* Fixed a bug in Float mode.

* Fixed Blynk.OTA bug, device not rebooting or getting stuck during update.

Update June 6, 2026

* Migrated to ESP32 Arduino Core 3.0.0+ API: Refactored all PWM hardware control to use the new pin-centric ledc API. *Note: Ensure you have the latest ESP32 Board Manager installed (Version 3.0.0 or higher recommended).*

* Added Sunset Chatter Timer: Added a 60s timer during sunrise and sunset where the buck turns on and off rapidly because there is not enough sunlight.
  
* Implemented 4-Mode Cooling Logic. Added a PWM-based cooling system. (3PIN Fan setup required: PWM, 12v, GND. For 2PIN fan setup, use STATIC MODE.)
 1. Mode 0 (OFF): Disables the cooling fan.
 2. Mode 1 (FORCED ON): Sets the fan to 100% duty cycle.
 3. Mode 2 (DYNAMIC PWM): Modulates fan speed based on system temperature using map() and constrain() functions for quiet, efficient cooling.
 4. Mode 3 (STATIC ON/OFF): Fallback mode using simple temperature thresholds.
  
    Safety Fallback: Added a critical temperature check that forces the fan to 100% speed regardless of the selected mode if the system temperature hits the defined maximum.

* UI/UX & Menu Updates. Reorganized the settings menu. Integrated new features to the menu such as: battery and voltage selection, charging control, load control, new fan modes, etc.

* Fixed remaining UI bugs encountered in the previous builds.

Update June 12, 2026 (Happy Indepence Day Philippines!!!)

* Lithium Float Logic: Modified the charging state machine to automatically skip the Float charging stage when Lithium battery chemistries (e.g., LiFePO4, Li-ion) are selected. This prevents chronic micro-cycling at high voltages and protects the cells from degradation.

* Non-Volatile Storage Migration: Completely transitioned the memory architecture from legacy EEPROM byte addressing to the native ESP32 Preferences.h library.

* NVS Namespace Implementation: Established dedicated namespaces (fugu-cfg for system settings and fugu-stats for network/lifetime data) to ensure safer and more structured flash memory management.

* Factory Reset Overhaul: Patched a critical logic bug where volatile, fragmented RAM variables would overwrite clean flash memory during a reset. The system now properly enforces safe baseline defaults to RAM prior to executing the NVS save state.

* Menu Execution Cleanup: Removed redundant execution strings and dead code from the LCD factory reset confirmation submenu to prevent system hang-ups and overlapping reboot commands.

* Action Required: A factory reset is strictly required immediately after flashing this build to wipe out legacy EEPROM fragmentation and initialize the new key-value structure. Make sure to turn off Solar Input during this process as some values may read 255 and may damage your hardware.

June 15, 2026

* Changed default CC_mode to Maximum Charging Current.

June 23, 2026

* Patched a bug that prevents charging in Lead Acid/Flooded batteries.
