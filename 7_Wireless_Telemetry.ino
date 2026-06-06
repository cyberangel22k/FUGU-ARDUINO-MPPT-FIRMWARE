//#include "BluetoothSerial.h"

//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif

//BluetoothSerial SerialBT;
//bool btInitialized = false;

  ////////// WIFI TELEMETRY ////////// 
BlynkTimer timer;
extern bool otaUpdating;

void Wireless_Telemetry(){
  if(enableWiFi==1){
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
      Blynk.run();
    }
    
    static unsigned long lastBlynkUpdate = 0;
    if(millis() - lastBlynkUpdate > 1000){ // Only send data once per second
      lastBlynkUpdate = millis();
      
      if(!Blynk.connected()){ Blynk.connect(); }

      int LED1, LED2, LED3, LED4;                      //Declare LED brightness variable 
      if(buckEnable==1)      {LED1=200;}else{LED1=0;}  //BATTERY CHARGING STATUS
      if(batteryPercent>=99 ){LED2=200;}else{LED2=0;}  //FULL CHARGE STATUS
      if(loadStatus==1)       {LED3=200;}else{LED3=0;}  //LOAD/LDV STATUS
      if(fanStatus==1)        {LED4=200;}else{LED4=0;}  //FAN STATUS

      Blynk.virtualWrite(V1, powerInput); 
      Blynk.virtualWrite(V2, batteryPercent);
      Blynk.virtualWrite(V3, voltageInput);    
      Blynk.virtualWrite(V4, currentInput);   
      Blynk.virtualWrite(V5, voltageOutput); 
      Blynk.virtualWrite(V6, currentOutput); 
      Blynk.virtualWrite(V7, temperature); 
      Blynk.virtualWrite(V8, Wh/1000); 
      Blynk.virtualWrite(V9, energySavings);       
      Blynk.virtualWrite(V10, LED1);               //LED - Battery Charging Status
      Blynk.virtualWrite(V11, LED2);               //LED - Full Battery Charge Status
      Blynk.virtualWrite(V12, LED3);               //LED - LOAD/LDV STATUS
      Blynk.virtualWrite(V13, LED4);               //LED - FAN STATUS
      Blynk.virtualWrite(V14, voltageBatteryMin);  //Minimum Battery Voltage (Read & Write)
      Blynk.virtualWrite(V15, voltageBatteryMax);  //Maximum Battery Voltage (Read & Write)
      Blynk.virtualWrite(V16, currentCharging);    //Charging Current  (Read & Write)
      Blynk.virtualWrite(V17, electricalPrice);    //Electrical Price  (Write)
      Blynk.virtualWrite(V18, daysRunning);        //Send number of days running to App
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
