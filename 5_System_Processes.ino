void System_Processes(){
  ///////////////// FAN COOLING /////////////////
  if(enableFan==true){
    if(enableDynamicCooling==false){                                   //STATIC PWM COOLING MODE (2-PIN FAN - no need for hysteresis, temp data only refreshes after 'avgCountTS' or every 500 loop cycles)                       
      if(overrideFan==true){fanStatus=true;}                           //Force on fan
      else if(temperature>=temperatureFan){fanStatus=1;}               //Turn on fan when set fan temp reached
      else if(temperature<temperatureFan){fanStatus=0;}                //Turn off fan when set fan temp reached
      digitalWrite(FAN,fanStatus);                                     //Send a digital signal to the fan MOSFET
    }
    else{}                                                             //DYNAMIC PWM COOLING MODE (3-PIN FAN - coming soon)
  }
  else{digitalWrite(FAN,LOW);}                                         //Fan Disabled
  
  //////////// LOOP TIME STOPWATCH ////////////
  loopTimeStart = micros();                                            //Record Start Time
  loopTime = (loopTimeStart-loopTimeEnd)/1000.000;                     //Compute Loop Cycle Speed (mS)
  loopTimeEnd = micros();                                              //Record End Time

  ///////////// AUTO DATA RESET /////////////
  if(telemCounterReset==0){}                                           //Never Reset
  else if(telemCounterReset==1 && daysRunning>1)  {resetVariables();}  //Daily Reset
  else if(telemCounterReset==2 && daysRunning>7)  {resetVariables();}  //Weekly Reset
  else if(telemCounterReset==3 && daysRunning>30) {resetVariables();}  //Monthly Reset
  else if(telemCounterReset==4 && daysRunning>365){resetVariables();}  //Yearly Reset 

  ///////////// LOW POWER MODE /////////////
  if(lowPowerMode==1){}   
  else{}      
}

String getInternetDate() {
  // Synchronize via network pools using GMT+8 offset coordinates for the Philippines
  configTime(28800, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Syncing time via NTP...");
  
  struct tm timeinfo;
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 8) {
    delay(500);
    Serial.print(".");
    retry++;
  }
  
  if (retry >= 8) {
    Serial.println(" Failed!");
    return "2025-01-01"; // Fallback safe default value
  }
  
  Serial.println(" Success!");
  char dateBuffer[12];
  strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeinfo);
  return String(dateBuffer);
}

void setupWiFi(){
  wifiInitializing = true;
  if(enableWiFi==1){
    Serial.begin(baudRate);
    Serial.println("Setting up WiFi access point...");
    WiFiManager wm;
    wm.setConfigPortalTimeout(120);         // If no WiFi after 120s, give up and start charging
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
        lcd.clear();
        lcd.setCursor(0,0);lcd.print("WiFi Connected  ");
        lcd.setCursor(0,1);lcd.print("IP: ");
        lcd.print(WiFi.localIP());
        delay(3000);
        menuPage = 0;
    }
    Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
    Blynk.connect();
  }
  wifiInitializing = false;
}

void runSetupWizard() {
  bool selectionMade = false;
  int wizardStep = 1; // 1: ADC, 2: Battery, 3: Voltage, 4: WiFi, 5: Date, 6: Finalize

  // ================= STAGE 1: HARDWARE SELECTION =================
  while (wizardStep == 1) {
    lcd.setCursor(0, 0); lcd.print("SELECT ADC TYPE ");
    if(setMenuPage == 1) { lcd.setCursor(0, 1); lcd.print(" >"); }
    else                 { lcd.setCursor(0, 1); lcd.print("= "); }
    
    if(ADS1015_Mode == 0) { lcd.print("ADS1115 (16bit)"); }
    else                  { lcd.print("ADS1015 (12bit)"); }

    if(digitalRead(buttonRight) == 1 || digitalRead(buttonLeft) == 1) {
      while(digitalRead(buttonRight) == 1 || digitalRead(buttonLeft) == 1) {}
      if(ADS1015_Mode == 1) { ADS1015_Mode = 0; } else { ADS1015_Mode = 1; }
      lcd.clear();
    }
    
    if(digitalRead(buttonBack) == 1) {
      while(digitalRead(buttonBack) == 1) {}
      lcd.clear(); lcd.print("REQUIRED SETTING"); delay(1000); lcd.clear();
    }

    if(digitalRead(buttonSelect) == 1) {
      while(digitalRead(buttonSelect) == 1) {}
      // Lock in ADC profile straight away
      stats.begin("fugu-stats", false);
      stats.putBool("is1015", ADS1015_Mode);
      stats.end();
      
      savedMessageLCD();
      wizardStep = 2; // Transition to Battery Type
      lcd.clear();
    }
  }

  // ================= STAGE 2: BATTERY TYPE (NEW) =================
  int battType = 0; // 0=LiFePO4, 1=Li-Ion, 2=AGM, 3=Flooded, 4=Custom
  while (wizardStep == 2) {
    lcd.setCursor(0, 0); lcd.print("BATTERY TYPE:   ");
    lcd.setCursor(0, 1); lcd.print("> "); 
    lcd.setCursor(2, 1);
    
    if (battType == 0)      lcd.print("LiFePO4       ");
    else if (battType == 1) lcd.print("Li-Ion 3S     ");
    else if (battType == 2) lcd.print("AGM / Sealed  ");
    else if (battType == 3) lcd.print("Flooded Lead  ");
    else if (battType == 4) lcd.print("Custom / Skip ");

    if(digitalRead(buttonRight) == 1) {
      while(digitalRead(buttonRight) == 1) {}
      battType++;
      if(battType > 4) battType = 0;
      lcd.clear();
    }
    if(digitalRead(buttonLeft) == 1) {
      while(digitalRead(buttonLeft) == 1) {}
      battType--;
      if(battType < 0) battType = 4;
      lcd.clear();
    }
    if(digitalRead(buttonSelect) == 1) {
      while(digitalRead(buttonSelect) == 1) {}
      wizardStep = 3; // Transition to System Voltage
      lcd.clear();
    }
  }

  // ================= STAGE 3: SYSTEM VOLTAGE (NEW) =================
  int sysVolts = 12; // Valid options: 12, 24, 48
  while (wizardStep == 3) {
    lcd.setCursor(0, 0); lcd.print("SYSTEM VOLTAGE: ");
    lcd.setCursor(0, 1); lcd.print("> ");
    lcd.setCursor(2, 1);
    lcd.print(sysVolts); lcd.print("V           ");

    if(digitalRead(buttonRight) == 1 || digitalRead(buttonLeft) == 1) {
      while(digitalRead(buttonRight) == 1 || digitalRead(buttonLeft) == 1) {}
      if (sysVolts == 12) sysVolts = 24;
      else if (sysVolts == 24) sysVolts = 48;
      else if (sysVolts == 48) sysVolts = 12;
      lcd.clear();
    }
    if(digitalRead(buttonSelect) == 1) {
      while(digitalRead(buttonSelect) == 1) {}
      
      // Compute thresholds and save them to EEPROM immediately!
      applyBatteryPreset(battType, sysVolts);
      saveSettings(); 
      
      savedMessageLCD();
      wizardStep = 4; // Transition to Wi-Fi Setup
      lcd.clear();
    }
  }

  // ================= STAGE 4: USE WI-FI PROMPT =================
  bool useWiFiSelection = true; // Default cursor on YES
  while (wizardStep == 4) {
    lcd.setCursor(0, 0); lcd.print("CONNECT TO WIFI?");
    lcd.setCursor(0, 1);
    if(useWiFiSelection) { lcd.print("   > YES    NO "); }
    else                 { lcd.print("     YES  > NO "); }

    if(digitalRead(buttonRight) == 1 || digitalRead(buttonLeft) == 1) {
      while(digitalRead(buttonRight) == 1 || digitalRead(buttonLeft) == 1) {}
      useWiFiSelection = !useWiFiSelection;
      lcd.clear();
    }

    if(digitalRead(buttonSelect) == 1) {
      while(digitalRead(buttonSelect) == 1) {}
      lcd.clear();
      
      if(useWiFiSelection && enableWiFi == 1) {
        lcd.setCursor(0, 0); lcd.print("CONNECT TO WIFI ");
        lcd.setCursor(0, 1); lcd.print("USE PHONE APP...");
        delay(10000);
        setupWiFi();
        if (WIFI == 1) {
          String netDate = getInternetDate();
          stats.begin("fugu-stats", false);
          stats.putString("startDate", netDate);
          stats.end();
          
          lcd.clear();
          lcd.setCursor(0, 0); lcd.print("DATE SYNCED:   ");
          lcd.setCursor(0, 1); lcd.print(netDate);
          delay(2000);
          wizardStep = 6; // Skip manual adjustments, jump straight to completion
        } else {
          lcd.setCursor(0,0); lcd.print("WIFI FAILED!    ");
          lcd.setCursor(0,1); lcd.print("GOING TO MANUAL ");
          delay(2000); lcd.clear();
          wizardStep = 5; // Connection failed, route to manual setup
        }
      } else {
        WIFI = 0; // Explicitly enforce offline status
        wizardStep = 5; // Route directly to manual date entry
      }
    }
  }

  // ================= STAGE 5: MANUAL DATE PICKER =================
  int subStep = 0; // 0: Year, 1: Month, 2: Day
  while (wizardStep == 5) {
    lcd.setCursor(0, 0); lcd.print("SET START DATE: ");
    lcd.setCursor(0, 1);
    
    if(subStep == 0) lcd.print(">"); else lcd.print(" ");
    lcd.print(wizardYear); lcd.print("-");
    
    if(subStep == 1) lcd.print(">"); else lcd.print("");
    if(wizardMonth < 10) lcd.print("0"); lcd.print(wizardMonth); lcd.print("-");
    
    if(subStep == 2) lcd.print(">"); else lcd.print("");
    if(wizardDay < 10) lcd.print("0"); lcd.print(wizardDay);
    
    if(digitalRead(buttonRight) == 1) {
      while(digitalRead(buttonRight) == 1) {}
      if(subStep == 0) { wizardYear++;  if(wizardYear > 2040) wizardYear = 2040; } 
      if(subStep == 1) { wizardMonth++; if(wizardMonth > 12) wizardMonth = 1; }
      if(subStep == 2) { wizardDay++;   if(wizardDay > 31) wizardDay = 1; }
      lcd.clear();
    }
    if(digitalRead(buttonLeft) == 1) {
      while(digitalRead(buttonLeft) == 1) {}
      if(subStep == 0) { wizardYear--;  if(wizardYear < 2020) wizardYear = 2020; } 
      if(subStep == 1) { wizardMonth--; if(wizardMonth < 1) wizardMonth = 12; }
      if(subStep == 2) { wizardDay--;   if(wizardDay < 1) wizardDay = 31; }
      lcd.clear();
    }
    if(digitalRead(buttonSelect) == 1) {
      while(digitalRead(buttonSelect) == 1) {}
      subStep++;
      lcd.clear();
      
      if(subStep > 2) {
        char manualDateBuf[12];
        sprintf(manualDateBuf, "%04d-%02d-%02d", wizardYear, wizardMonth, wizardDay);
        
        stats.begin("fugu-stats", false);
        stats.putString("startDate", String(manualDateBuf));
        stats.end();
        
        savedMessageLCD();
        wizardStep = 6; // Progress to finalization block
      }
    }
  }

  // ================= STAGE 6: FINALIZE SYSTEM =================
  stats.begin("fugu-stats", false);
  stats.putBool("isFirstBoot", false); // Close down the wizard state permanently
  stats.end();

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("SETUP COMPLETE! ");
  lcd.setCursor(0, 1); lcd.print("REBOOTING...    ");
  delay(1500);
  
  ESP.restart();
}

void WifiReset(){
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Resetting WiFi...");
  lcd.setCursor(0,1); lcd.print("Please wait...");
  delay(1000);
  WiFiManager wm;
  wm.resetSettings();
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Rebooting...");
  delay(1000);
  ESP.restart();
}

void factoryReset(){
  // 1. Core Hardware & Feature Defaults (Preserving Angelo's original map)
  EEPROM.write(0, 1);   // STORE: Charging Algorithm (1 = MPPT Mode)
  // EEPROM 1, 2, 3, and 4 are deliberately left out here; they will be populated by applyBatteryPreset()
  EEPROM.write(5, 30);  // STORE: Charging Current (whole)
  EEPROM.write(6, 0);   // STORE: Charging Current (decimal)
  EEPROM.write(7, 1);   // STORE: Fan Enable (Bool)
  EEPROM.write(8, 50);  // STORE: Fan Temp (Integer)
  EEPROM.write(9, 80);  // STORE: Shutdown Temp (Integer)
  EEPROM.write(10, 1);  // STORE: Enable WiFi (Boolean)
  EEPROM.write(11, 1);  // STORE: Enable autoload (on by default)
  EEPROM.write(12, 1);  // STORE: Charger/PSU Mode Selection (1 = Charger Mode)
  EEPROM.write(13, 3);  // STORE: LCD backlight sleep timer (default: 3 = Daytime On)
  EEPROM.write(14, 1);  // STORE: Onboard Telemetry
  EEPROM.write(15, 1);  // STORE: Enable Bluetooth
  
  // 2. Inject New Battery Architecture Defaults
  // This function will automatically calculate 12V AGM limits and save them to 
  // EEPROM addresses 1, 2, 3, 4, 21, and 22 via your updated saveSettings() function.
  applyBatteryPreset(TYPE_AGM_SEALED, 12); 

  // Commit all EEPROM changes to flash memory
  EEPROM.commit();

  // 3. Clear Network and Lifetime Statistic Data
  stats.begin("fugu-stats", false);
  stats.clear(); 
  
  // 4. Force Setup Wizard on next boot
  stats.putBool("isFirstBoot", true); 
  stats.end(); // Always close Preferences when done

  WifiReset();

  // 5. System Reboot UI
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" FACTORY RESET  ");
  lcd.setCursor(0,1);
  lcd.print(" REBOOTING...   ");
  Serial.println("> SYSTEM: FACTORY RESET COMPLETE. REBOOTING...");
  delay(3000);
  ESP.restart();
}

void loadSettings(){ 
  voltageBatteryMax   = EEPROM.read(1)  + (EEPROM.read(2)*.01);  // Load saved maximum battery voltage setting
  voltageBatteryMin   = EEPROM.read(3)  + (EEPROM.read(4)*.01);  // Load saved minimum battery voltage setting
  voltageBatteryFloat = EEPROM.read(21) + (EEPROM.read(22)*.01);
  voltageLVD          = EEPROM.read(23) + (EEPROM.read(24)*0.01);
  voltageLVR          = EEPROM.read(25) + (EEPROM.read(26)*0.01);
  currentCharging     = EEPROM.read(5)  + (EEPROM.read(6)*.01);  // Load saved charging current setting
  enableFan           = EEPROM.read(7);                          // Load saved fan enable settings
  temperatureFan      = EEPROM.read(8);                          // Load saved fan temperature settings
  temperatureMax      = EEPROM.read(9);                          // Load saved shutdown temperature settings
  enableWiFi          = EEPROM.read(10);                         // Load saved WiFi enable settings  
  flashMemLoad        = EEPROM.read(11);                         // Load saved flash memory autoload feature
  backlightSleepMode  = EEPROM.read(13);                         // Load saved lcd backlight sleep timer
  serialTelemMode     = EEPROM.read(14);                         // Load saved Onboard telemetry settings
  enableBluetooth     = EEPROM.read(15);                         // Load saved Bluetooth enable setting
}

void saveSettings(){
  conv1 = voltageBatteryMax*100;       //STORE: Maximum Battery Voltage (gets whole number)
  conv2 = conv1%100;                   //STORE: Maximum Battery Voltage (gets decimal number and converts to a whole number)
  EEPROM.write(1,voltageBatteryMax);
  EEPROM.write(2,conv2);
  conv1 = voltageBatteryMin*100;       //STORE: Minimum Battery Voltage (gets whole number)
  conv2 = conv1%100;                   //STORE: Minimum Battery Voltage (gets decimal number and converts to a whole number)
  EEPROM.write(3,voltageBatteryMin);
  EEPROM.write(4,conv2);
  conv1 = currentCharging*100;         //STORE: Charging Current
  conv2 = conv1%100;
  EEPROM.write(5,currentCharging);
  EEPROM.write(6,conv2);
  EEPROM.write(7,enableFan);           //STORE: Fan Enable
  EEPROM.write(8,temperatureFan);      //STORE: Fan Temp
  EEPROM.write(9,temperatureMax);      //STORE: Shutdown Temp
  EEPROM.write(10,enableWiFi);         //STORE: Enable WiFi
//EEPROM.write(11,flashMemLoad);       //STORE: Enable autoload (must be excluded from bulk save, uncomment under discretion)
  EEPROM.write(13,backlightSleepMode); //STORE: LCD backlight sleep timer 
  EEPROM.write(14,serialTelemMode);    //STORE: Onboard Telemetry setting 
  EEPROM.write(15,enableBluetooth);
  EEPROM.write(21, int(voltageBatteryFloat));
  EEPROM.write(22, (voltageBatteryFloat - int(voltageBatteryFloat)) * 100);
  EEPROM.write(23, int(voltageLVD));
  EEPROM.write(24, (voltageLVD - int(voltageLVD)) * 100);
  EEPROM.write(25, int(voltageLVR));
  EEPROM.write(26, (voltageLVR - int(voltageLVR)) * 100);
  EEPROM.commit();                     //Saves setting changes to flash memory
}
void saveAutoloadSettings(){
  EEPROM.write(11,flashMemLoad);       //STORE: Enable autoload
  EEPROM.commit();                     //Saves setting changes to flash memory
}
void initializeFlashAutoload(){
  if(disableFlashAutoLoad==0){
    flashMemLoad = EEPROM.read(11);       //Load saved autoload (must be excluded from bulk save, uncomment under discretion) 
    if(flashMemLoad==1){loadSettings();}  //Load stored settings from flash memory  
  } 
}

void performOTAUpdate(String otaUrl) {
    terminal.println("Starting OTA Update via Blynk.Air...");
    terminal.flush();

    WiFiClient client;
    HTTPClient http;
    
    // Pass the dynamic URL given to us by Blynk
    http.begin(otaUrl);
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        int contentLength = http.getSize();
        if (Update.begin(contentLength)) {
            Update.writeStream(http.getStream());
            if (Update.end()) {
                terminal.println("Update Success! Rebooting...");
                terminal.flush();
                delay(1000);
                ESP.restart();
            } else {
                terminal.println("Update Failed!");
                terminal.flush();
            }
        }
    } else {
        terminal.print("HTTP Error Code: ");
        terminal.println(httpCode);
        terminal.flush();
    }
    http.end();
}
