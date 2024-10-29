  ////////// WIFI TELEMETRY ////////// 
int ReCnctFlag;  // Reconnection Flag
int ReCnctCount = 0;  // Reconnection counter

BlynkTimer timer;

void setupWiFi(){
  if(enableWiFi==1){
    Serial.begin(baudRate);
    Serial.println("Setting up WiFi access point...");
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("FUGU DIY MPPT"); // FUGU DIY MPPT Access point name
    if(!res) {
        Serial.println("Failed to connect");
    } 
    else {
        Serial.println("Connected to WiFi");
        lcd.setBacklight(HIGH);
        lcd.setCursor(0,0);lcd.print("Connected to    ");                              //Display LCD message
        lcd.setCursor(0,1);lcd.print("WiFi Network    ");                              //Display LCD message   
        delay(1500);                                                                   //For loop "loading... effect
        lcd.clear();
    }
    Blynk.config(auth, "blynk.cloud", 80);
    Blynk.connect();
    timer.setInterval(1000L, UpTime);
    }
}

BLYNK_CONNECTED() {
  Serial.println("Connected");
  ReCnctCount = 0;
  WIFI = 1;
}

void UpTime() {
  Blynk.virtualWrite(V19, millis() / 1000);  // Send UpTime seconds to App (you can create a new widget in blynk for uptime)
  Serial.print("UpTime: ");
  Serial.println(millis() / 1000);  // Send UpTime seconds to Serial
}

void Wireless_Telemetry(){
  timer.run();
  if (Blynk.connected()) {  // If connected run as normal
    Blynk.run();
  } else if (ReCnctFlag == 0) {  // If NOT connected and not already trying to reconnect, set timer to try to reconnect in 30 seconds
    WIFI = 0;
    ReCnctFlag = 1;  // Set reconnection Flag
    Serial.println("Starting reconnection timer in 30 seconds...");
    timer.setTimeout(30000L, []() {  // Lambda Reconnection Timer Function
      ReCnctFlag = 0;  // Reset reconnection Flag
      ReCnctCount++;  // Increment reconnection Counter
      Serial.print("Attempting reconnection #");
      Serial.println(ReCnctCount);
      WiFi.reconnect();
      Blynk.connect();  // Try to reconnect to the server
    });  // END Timer Function
  }

  if(WIFI==1){
    int LED1, LED2, LED3, LED4;                      //Declare LED brightness variable 
    if(buckEnable==1)      {LED1=200;}else{LED1=0;}  //BATTERY CHARGING STATUS
    if(batteryPercent>=99 ){LED2=200;}else{LED2=0;}  //FULL CHARGE STATUS
    if(batteryPercent<=10) {LED3=200;}else{LED3=0;}  //LOW BATTERY STATUS
    if(IUV==0)             {LED4=200;}else{LED4=0;}  //PV INPUT PRESENCE STATUS

    Blynk.run();  
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
