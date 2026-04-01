  ////////// WIFI TELEMETRY ////////// 
int ReCnctFlag;  // Reconnection Flag
int ReCnctCount = 0;  // Reconnection counter

BlynkTimer timer;

void setupWiFi(){
  if(enableWiFi==1){
    Serial.begin(baudRate);
    Serial.println("Setting up WiFi access point...");
    WiFiManager wm;
    wm.setConfigPortalTimeout(30);         // If no WiFi after 30s, give up and start charging
    bool res;
    res = wm.autoConnect("FUGU DIY MPPT"); // FUGU DIY MPPT Access point name
    if(!res) {
      Serial.println("WiFi Failed - Running in Offline Mode");
      WIFI = 0;
    }
    else {
        Serial.println("Connected to WiFi");
        WIFI = 1;
        lcd.setBacklight(HIGH);
        lcd.setCursor(0,0);lcd.print("Connected to    ");                              //Display LCD message
        lcd.setCursor(0,1);lcd.print("WiFi Network    ");                              //Display LCD message   
        delay(1500);                                                                   //For loop "loading... effect
        lcd.clear();
    }
    Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
    Blynk.connect();
    }
}

void Wireless_Telemetry(){
    if(enableWiFi==1){
      if(Blynk.connected()){
        Blynk.run();
      }
    static unsigned long lastBlynkUpdate = 0;
    if(millis() - lastBlynkUpdate > 1000){ // Only send data once per second
      lastBlynkUpdate = millis();
    if(!Blynk.connected()){ Blynk.connect(); }

    int LED1, LED2, LED3, LED4;                      //Declare LED brightness variable 
    if(buckEnable==1)      {LED1=200;}else{LED1=0;}  //BATTERY CHARGING STATUS
    if(batteryPercent>=99 ){LED2=200;}else{LED2=0;}  //FULL CHARGE STATUS
    if(batteryPercent<=10) {LED3=200;}else{LED3=0;}  //LOW BATTERY STATUS
    if(IUV==0)             {LED4=200;}else{LED4=0;}  //PV INPUT PRESENCE STATUS

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
    Blynk.virtualWrite(V12, LED3);               //LED - Low Battery Charge Status
    Blynk.virtualWrite(V13, LED4);               //LED - PV Harvesting
    Blynk.virtualWrite(V14, voltageBatteryMin);  //Minimum Battery Voltage (Read & Write)
    Blynk.virtualWrite(V15, voltageBatteryMax);  //Maximum Battery Voltage (Read & Write)
    Blynk.virtualWrite(V16, currentCharging);    //Charging Current  (Read & Write)
    Blynk.virtualWrite(V17, electricalPrice);    //Electrical Price  (Write)
    Blynk.virtualWrite(V18, daysRunning);        //Send number of days running to App
  }
  ////////// BLUETOOTH TELEMETRY ////////// 
  if(enableBluetooth==1){
    //ADD BLUETOOTH CODE
  }
 }
}
