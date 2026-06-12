//#include "BluetoothSerial.h"

//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif

//BluetoothSerial SerialBT;
//bool btInitialized = false;

  ////////// WIFI TELEMETRY ////////// 
extern bool otaUpdating;

void pushBlynkData() {
  // Only push data if WiFi is enabled AND we are actively connected to the server
  if(enableWiFi == 1 && Blynk.connected()) {
    
    int LED1, LED2, LED3, LED4;                      
    if(buckEnable==1)      {LED1=200;}else{LED1=0;}  
    if(batteryPercent>=99 ){LED2=200;}else{LED2=0;}  
    if(loadStatus==1)       {LED3=200;}else{LED3=0;} 
    if(fanStatus==1)        {LED4=200;}else{LED4=0;} 

    Blynk.virtualWrite(V1, powerInput); 
    Blynk.virtualWrite(V2, batteryPercent);
    Blynk.virtualWrite(V3, voltageInput);    
    Blynk.virtualWrite(V4, currentInput);   
    Blynk.virtualWrite(V5, voltageOutput); 
    Blynk.virtualWrite(V6, currentOutput); 
    Blynk.virtualWrite(V7, temperature); 
    Blynk.virtualWrite(V8, Wh/1000); 
    Blynk.virtualWrite(V9, energySavings);       
    Blynk.virtualWrite(V10, LED1);               
    Blynk.virtualWrite(V11, LED2);               
    Blynk.virtualWrite(V12, LED3);               
    Blynk.virtualWrite(V13, LED4);               
    Blynk.virtualWrite(V14, voltageBatteryMin);  
    Blynk.virtualWrite(V15, voltageBatteryMax);  
    Blynk.virtualWrite(V16, currentCharging);    
    Blynk.virtualWrite(V17, electricalPrice);    
    Blynk.virtualWrite(V18, daysRunning); 
  }
}

void Wireless_Telemetry(){
  if(enableWiFi==1){
    Blynk.run();
    blynkTimer.run();
    if(Blynk.connected()){
      static bool isFirstConnect = true;
      if(isFirstConnect) {
         terminal.println(F("System Online."));
         terminal.println(F("Firmware Update Successful."));
         terminal.println("====================");
         terminal.println("DEVICE BOOTED");
         terminal.printf("Firmware: %s\n", FIRMWARE_VERSION);
         terminal.printf("Sketch Size: %u\n", ESP.getSketchSize());
         terminal.printf("Free OTA Space: %u\n", ESP.getFreeSketchSpace());
         terminal.println("====================");
         terminal.flush();
         isFirstConnect = false;
      }
    }
  }
  ////////// BLUETOOTH TELEMETRY ////////// 
  if(enableBluetooth==1){
    
    // Initialize the Bluetooth radio once
    //if(!btInitialized){
    //    SerialBT.begin("FUGU_MPPT"); // This is the name you will see on your phone
    //    btInitialized = true;
    //}

    // We use a timer so we don't spam the phone with thousands of messages a second
    //static unsigned long lastBtUpdate = 0;
    //if(millis() - lastBtUpdate > 1500){ 
    //    lastBtUpdate = millis();

        // Format a clean, easy-to-read list for your mobile terminal
    //    SerialBT.print("PV Power:       "); SerialBT.print(powerInput, 0); SerialBT.println(" W");
    //    SerialBT.print("PV Voltage:     "); SerialBT.print(voltageInput, 1); SerialBT.println(" V");
    //    SerialBT.print("Batt Voltage:   "); SerialBT.print(voltageOutput, 1); SerialBT.println(" V");
    //    SerialBT.print("Charge Current: "); SerialBT.print(currentOutput, 2); SerialBT.println(" A");
    //    SerialBT.print("Battery Level:  "); SerialBT.print(batteryPercent); SerialBT.println(" %");
    //    SerialBT.print("System Temp:    "); SerialBT.print(temperature, 1); SerialBT.println(" C");
    //    SerialBT.println("-----------------------------");
    //}
  }
}

BLYNK_WRITE(InternalPinOTA) {
    String otaUrl = param.asString();

    // 1. FREEZE THE SYSTEM
    otaUpdating = true;       // Stop the main loop immediately
    buck_Disable();           // Turn off the solar charger for safety

    // 2. DRAW FINAL LOADING SCREEN
    lcd.setBacklight(HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("UPDATING FIRMWARE");
    lcd.setCursor(0, 1);
    lcd.print("PLEASE WAIT...   ");
    delay(500);               // Give the LCD a half-second to draw the text

    // 3. START THE FLASH WRITE
    performOTAUpdate(otaUrl);
}