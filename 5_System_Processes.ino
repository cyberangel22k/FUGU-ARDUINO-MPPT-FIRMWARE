void System_Processes(){
  ///////////////// FAN COOLING /////////////////
  if (enableFan == 1) {
    // 1. FORCED ON MODE
    if (overrideFan == 1) {
      ledcWrite(FAN, 255); // Full power
      fanStatus = 1;
    } 
    // 2. DYNAMIC PWM MODE
    else if (enableDynamicCooling == 1) {
      if (temperature < temperatureFan) {
        ledcWrite(FAN, 0); // Off
        fanStatus = 0;
      } 
      // SAFETY FALLBACK: If temp is critically high, ignore PWM and run full speed
      else if (temperature >= temperatureMax) {
        ledcWrite(FAN, 255); 
        fanStatus = 1;
      }
      else {
        // Map speed: 100 is min (prevents stalling), 255 is max
        int pwmSpeed = map(temperature, temperatureFan, temperatureMax, 100, 255);
        pwmSpeed = constrain(pwmSpeed, 100, 255);
        ledcWrite(FAN, pwmSpeed);
        fanStatus = 1;
      }
    }
    // 3. STATIC ON/OFF MODE (Fallback if neither override nor PWM flags are set)
    else {
      if (temperature >= temperatureFan) {
        ledcWrite(FAN, 255); // Full power
        fanStatus = 1;
      } else {
        ledcWrite(FAN, 0);   // Off
        fanStatus = 0;
      }
    }
  } else {
    // Fan disabled globally in settings
    ledcWrite(FAN, 0);
    fanStatus = 0;
  }
  
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

void saveConfigCallback () {
  shouldSaveConfig = true;
}

void setupWiFi(bool showLCD){
  if(enableWiFi==1){
    Serial.begin(baudRate);
    Serial.println("Setting up WiFi access point...");
    WiFiManager wm;
    wm.setConfigPortalTimeout(120); // If no WiFi after 120s, give up and start charging

    // Set config save notify callback
    wm.setSaveConfigCallback(saveConfigCallback);

    // Create custom parameters for the captive portal
    WiFiManagerParameter custom_blynk_auth("auth", "Auth Token", blynk_auth, 40);

    // Add parameters to WiFiManager
    wm.addParameter(&custom_blynk_auth);

    bool res;
    res = wm.autoConnect("FUGU DIY MPPT"); // FUGU DIY MPPT Access point name
    if(!res) {
      Serial.println("WiFi Failed - Running in Offline Mode");
      WIFI = 0;
    }
    else {
      Serial.println("Connected to WiFi");
      WIFI = 1;

      // Read updated parameters from the portal
      strcpy(blynk_auth, custom_blynk_auth.getValue());

      // Save the custom parameters to Preferences ONLY if they were changed in the portal
      if (shouldSaveConfig) {
        stats.begin("fugu-stats", false); // Open in read/write mode
        stats.putString("bAuth", String(blynk_auth));
        stats.end();
        Serial.println("> SYSTEM: Saved new Blynk credentials to flash");
        shouldSaveConfig = false;
      }

      if (showLCD == true){
        lcd.setCursor(0,0);lcd.print("WiFi Connected  ");
        lcd.setCursor(0,1);lcd.print("IP: ");
        lcd.print(WiFi.localIP());
        delay(1500);
        lcd.clear();
      }
    }
    
    // Inject the dynamic Auth Token into Blynk
    Blynk.config(blynk_auth, "blynk.cloud", 80);
    Blynk.connect();
  }
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
        delay(3000);
        setupWiFi(true);
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
void resetWiFi(){
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
void resetBlynk() {
  Serial.println("Resetting Blynk Credentials...");
  stats.begin("fugu-stats", false);
  stats.remove("bAuth");
  stats.end();
  
  Serial.println("Blynk cleared. Rebooting...");
  delay(1000);
  ESP.restart(); 
}
void factoryReset(){
  // 1. Wipe the core configuration namespace
  stats.begin("fugu-cfg", false);
  stats.clear(); 
  stats.end();

  // 2. Reset all RAM variables to safe base defaults
  currentCharging = 30.00;
  enableFan = true;
  temperatureFan = 50;
  temperatureMax = 80;
  enableWiFi = true;
  flashMemLoad = true;
  backlightSleepMode = 3;
  serialTelemMode = 1;
  enableBluetooth = true;
  battPreset = 4; // Custom
  sysVoltage = 12;
  loadMode = 0;
  manualLoadState = 0;
  lvdDelay = 30000;
  CC_Mode = 0;
  overrideFan = false;
  enableDynamicCooling = false;

  // 3. Inject Battery Architecture Defaults to RAM
  applyBatteryPreset(TYPE_AGM_SEALED, 12); 
  
  // 4. Save these perfectly clean RAM variables straight to NVS
  saveSettings(); 

  // 5. Wipe Network/Lifetime Data & Force Setup Wizard
  stats.begin("fugu-stats", false);
  stats.clear();
  stats.putBool("isFirstBoot", true); 
  stats.end(); 

  resetWiFi();
  resetBlynk();

  // 6. System Reboot UI
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
  stats.begin("fugu-cfg", true); 
  voltageBatteryMax   = stats.getFloat("vBatMax", 14.40);
  voltageBatteryMin   = stats.getFloat("vBatMin", 10.00);
  voltageBatteryFloat = stats.getFloat("vBatFlt", 13.50);
  voltageLVD          = stats.getFloat("vLVD", 11.50);
  voltageLVR          = stats.getFloat("vLVR", 12.50);
  currentCharging     = stats.getFloat("iChg", 30.00);
  enableFan           = stats.getBool("enFan", true);
  temperatureFan      = stats.getInt("tFan", 50);
  temperatureMax      = stats.getInt("tMax", 80);
  enableWiFi          = stats.getBool("enWiFi", true);
  flashMemLoad        = stats.getBool("enMem", true);
  backlightSleepMode  = stats.getInt("lcdSlp", 3);
  serialTelemMode     = stats.getInt("serMode", 1);
  enableBluetooth     = stats.getBool("enBT", true);
  battPreset          = stats.getInt("batPre", 4);
  sysVoltage          = stats.getInt("sysV", 12);
  loadMode            = stats.getInt("ldMode", 0);
  manualLoadState     = stats.getInt("manLd", 0);
  lvdDelay            = stats.getUInt("lvdDly", 30000); 
  CC_Mode             = stats.getBool("ccMode", false);
  overrideFan         = stats.getBool("ovrFan", false);
  enableDynamicCooling= stats.getBool("dynFan", false);
  stats.end();

  // --- SAFETY CHECKS ---
  if (battPreset > 4) battPreset = 4;
  if (sysVoltage != 12 && sysVoltage != 24 && sysVoltage != 48) sysVoltage = 12;
  if (lvdDelay > 254000) lvdDelay = 30000; 
  if (CC_Mode > 1) CC_Mode = 1;
}

void saveSettings(){
  stats.begin("fugu-cfg", false); 
  stats.putFloat("vBatMax", voltageBatteryMax);
  stats.putFloat("vBatMin", voltageBatteryMin);
  stats.putFloat("vBatFlt", voltageBatteryFloat);
  stats.putFloat("vLVD", voltageLVD);
  stats.putFloat("vLVR", voltageLVR);
  stats.putFloat("iChg", currentCharging);
  stats.putBool("enFan", enableFan);
  stats.putInt("tFan", temperatureFan);
  stats.putInt("tMax", temperatureMax);
  stats.putBool("enWiFi", enableWiFi);
  stats.putInt("lcdSlp", backlightSleepMode);
  stats.putInt("serMode", serialTelemMode);
  stats.putBool("enBT", enableBluetooth);
  stats.putInt("batPre", battPreset);
  stats.putInt("sysV", sysVoltage);
  stats.putInt("ldMode", loadMode);
  stats.putInt("manLd", manualLoadState);
  stats.putUInt("lvdDly", lvdDelay); 
  stats.putBool("ccMode", CC_Mode);
  stats.putBool("ovrFan", overrideFan);
  stats.putBool("dynFan", enableDynamicCooling);
  stats.end();
}
void saveAutoloadSettings(){
  stats.begin("fugu-cfg", false);
  stats.putBool("enMem", flashMemLoad);
  stats.end();
}
void initializeFlashAutoload(){
  if(disableFlashAutoLoad == 0){
    stats.begin("fugu-cfg", true);
    flashMemLoad = stats.getBool("enMem", true);
    stats.end();
    
    if(flashMemLoad == 1){ loadSettings(); } 
  } 
}
void performOTAUpdate(String otaUrl)
{
    terminal.println("=================================");
    terminal.println("Starting OTA Update via Blynk.Air");
    terminal.println("=================================");
    terminal.flush();

    HTTPClient http;
    bool updateSuccessful = false;

    http.begin(otaUrl);

    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK)
    {
        terminal.printf("HTTP Error: %d\n", httpCode);
        terminal.flush();
        http.end();
        return;
    }

    int contentLength = http.getSize();

    terminal.printf("Content Length: %d bytes\n", contentLength);
    terminal.printf("Current Sketch Size: %u bytes\n", ESP.getSketchSize());
    terminal.printf("Free OTA Space: %u bytes\n", ESP.getFreeSketchSpace());
    terminal.flush();

    if (contentLength <= 0)
    {
        terminal.println("ERROR: Invalid content length!");
        terminal.flush();
        http.end();
        return;
    }

    if (!Update.begin(contentLength))
    {
        terminal.println("ERROR: Update.begin() failed!");
        terminal.printf("Reason: %s\n", Update.errorString());
        terminal.flush();
        http.end();
        return;
    }

    terminal.println("Writing firmware...");
    terminal.flush();

    size_t written = Update.writeStream(http.getStream());

    terminal.printf("Bytes Written: %u\n", written);
    terminal.printf("Bytes Expected: %d\n", contentLength);
    terminal.flush();

    if (written != (size_t)contentLength)
    {
        terminal.println("ERROR: Incomplete firmware download!");
        terminal.printf("Update Error: %s\n", Update.errorString());
        terminal.flush();

        Update.abort();
        http.end();
        return;
    }

    terminal.println("Finalizing update...");
    terminal.flush();

    if (!Update.end(true))
    {
        terminal.println("ERROR: Firmware validation failed!");
        terminal.printf("Reason: %s\n", Update.errorString());
        terminal.flush();

        http.end();
        return;
    }

    terminal.println("Firmware written successfully.");
    terminal.println("OTA verification PASSED.");
    terminal.flush();

    updateSuccessful = true;

    http.end();

    if (updateSuccessful)
    {
        terminal.println("Disconnecting Blynk...");
        terminal.flush();

        Blynk.disconnect();

        delay(1000);

        terminal.println("Rebooting...");
        terminal.flush();

        delay(500);

        ESP.restart();
    }
}