void lcdBacklight_Wake(){
    lcd.setBacklight(HIGH);
    prevLCDBackLMillis = millis();
}

void lcdBacklight(){
  unsigned long backLightInterval = 10000;
  if(backlightSleepMode==0){prevLCDBackLMillis = millis();}                 
  else if(backlightSleepMode==1){backLightInterval=10000;}                  
  else if(backlightSleepMode==2){backLightInterval=300000;}                 
  else if(backlightSleepMode==3){
    if(buckEnable==1){ lcdBacklight_Wake(); return; } 
    else { backLightInterval = 10000; }
  }
  if(backlightSleepMode>0 && settingMode==0){
    currentLCDBackLMillis = millis();
    if(currentLCDBackLMillis-prevLCDBackLMillis>=backLightInterval){        
      prevLCDBackLMillis = currentLCDBackLMillis;
      lcd.setBacklight(LOW);
    } 
  }  
}

void padding100(int padVar){ if(padVar<10){lcd.print("  ");} else if(padVar<100){lcd.print(" ");} }
void padding10(int padVar){ if(padVar<10){lcd.print(" ");} }

// ================= TELEMETRY SCREENS ================= //
void displayConfig1(){
  lcd.setCursor(0,0);lcd.print(powerInput,0);lcd.print("W");padding100(powerInput);
  lcd.setCursor(5,0);
  if(Wh<10){lcd.print(Wh,3);lcd.print("Wh ");}                 
  else if(Wh<100){lcd.print(Wh,2);lcd.print("Wh ");}           
  else if(Wh<1000){lcd.print(Wh,1);lcd.print("Wh ");}          
  else if(Wh<10000){lcd.print(kWh,2);lcd.print("kWh ");}       
  else if(Wh<100000){lcd.print(kWh,1);lcd.print("kWh ");}      
  else if(Wh<1000000){lcd.print(kWh,0);lcd.print("kWh  ");}    
  else if(Wh<10000000){lcd.print(MWh,2);lcd.print("MWh ");}    
  else if(Wh<100000000){lcd.print(MWh,1);lcd.print("MWh ");}   
  else if(Wh<1000000000){lcd.print(MWh,0);lcd.print("MWh  ");} 
 
  lcd.setCursor(13,0);lcd.print(temperature);lcd.print((char)223);padding100(temperature);
  lcd.setCursor(0,1);
  if(buckEnable == 0)          { lcd.print("OFF "); } 
  else if(chargingState == 0)  { lcd.print("MPPT"); } 
  else if(chargingState == 1)  { lcd.print("ABS "); } 
  else if(chargingState == 2)  { lcd.print("FLT "); } 
  else if(chargingState == 3)  { lcd.print("FULL"); }
  if(BNC==0){lcd.setCursor(5,1); lcd.print(voltageOutput,1);lcd.print("V");padding10(voltageOutput);}
  else{lcd.setCursor(5,1);lcd.print("NOBAT ");}          
  lcd.setCursor(11,1);lcd.print(currentOutput,1);lcd.print("A");padding10(currentOutput);     
}
void displayConfig2(){
  lcd.setCursor(0,0); lcd.print(powerInput,0);  lcd.print("W");padding100(powerInput);    
  lcd.setCursor(5,0); lcd.print(voltageInput,1);lcd.print("V");padding10(voltageInput);            
  lcd.setCursor(11,0);lcd.print(currentInput,1);lcd.print("A");padding10(currentInput);    
  lcd.setCursor(0,1); lcd.print(batteryPercent);lcd.print("%");padding100(batteryPercent); 
  if(BNC==0){lcd.setCursor(5,1); lcd.print(voltageOutput,1);lcd.print("V");padding10(voltageOutput);}
  else{lcd.setCursor(5,1);lcd.print("NOBAT");}
  lcd.setCursor(11,1);lcd.print(currentOutput,1);lcd.print("A");padding10(currentOutput);    
}
void displayConfig3(){
  lcd.setCursor(0,0);  lcd.print(powerInput,0);lcd.print("W");padding100(powerInput); 
  lcd.setCursor(5,0);
  if(Wh<10){lcd.print(Wh,2);lcd.print("Wh ");}                 
  else if(Wh<100){lcd.print(Wh,1);lcd.print("Wh ");}           
  else if(Wh<1000){lcd.print(Wh,0);lcd.print("Wh  ");}         
  else if(Wh<10000){lcd.print(kWh,1);lcd.print("kWh ");}       
  else if(Wh<100000){lcd.print(kWh,0);lcd.print("kWh  ");}     
  else if(Wh<1000000){lcd.print(kWh,0);lcd.print("kWh ");}     
  else if(Wh<10000000){lcd.print(MWh,1);lcd.print("MWh ");}    
  else if(Wh<100000000){lcd.print(MWh,0);lcd.print("MWh  ");}  
  else if(Wh<1000000000){lcd.print(MWh,0);lcd.print("MWh ");}  
  
  lcd.setCursor(12,0);lcd.print(batteryPercent);lcd.print("%");padding100(batteryPercent);
  int batteryPercentBars = batteryPercent/6.18; 
  lcd.setCursor(0,1);
  for(int i=0;i<batteryPercentBars;i++){lcd.print((char)255);}     
  for(int i=0;i<16-batteryPercentBars;i++){lcd.print(" ");}    
}
void displayConfig4(){
  lcd.setCursor(0,0);lcd.print("TEMPERATURE STAT");
  lcd.setCursor(0,1);lcd.print(temperature);lcd.print((char)223);lcd.print("C");padding100(temperature);
  lcd.setCursor(8,1);lcd.print("FAN");
  lcd.setCursor(12,1);
  if(fanStatus==1){lcd.print("ON ");}
  else{lcd.print("OFF");}
}
void displayConfig5(){
  lcd.setCursor(0,0);lcd.print("Harvest: "); lcd.print(lifetimeKwh, 1); lcd.print(" kWh   ");
  lcd.setCursor(0,1);lcd.print("DATE: "); lcd.print(startDate);
}
void displayConfig6(){
  lcd.setCursor(0,0);lcd.print(" SETTINGS MENU  ");
  lcd.setCursor(0,1);lcd.print("--PRESS SELECT--");
}

// ================= UI MESSAGES ================= //
void factoryResetMessageLCD(){
  lcd.setCursor(0,0);lcd.print("  FACTORY RESET ");
  lcd.setCursor(0,1);lcd.print("   SUCCESSFUL   "); delay(1000);
}
void savedMessageLCD(){
  lcd.setCursor(0,0);lcd.print(" SETTINGS SAVED ");
  lcd.setCursor(0,1);lcd.print(" SUCCESSFULLY   "); delay(500); lcd.clear();
}
void cancelledMessageLCD(){
  lcd.setCursor(0,0);lcd.print(" SETTINGS       ");
  lcd.setCursor(0,1);lcd.print(" CANCELLED      "); delay(500); lcd.clear();
}

// ================= THE NESTED MENU SYSTEM ================= //
void LCD_Menu(){
  int longPressTime = 3000, longPressInterval = 500, shortPressInterval = 100;
  static int subItemSelect = 0; 
  static int menuLevel = 1;      // 1: Categories, 2: Items, 3: Edit
  static int editState = 0;      // Tracks deep jumps (like Battery Type -> Voltage)
  
  // ================= SETTINGS MODE ================= //
  if(settingMode == 1) { 
    chargingPause = 1;
    lcdBacklight_Wake();

    // --- NAVIGATION HANDLER ---
    if(digitalRead(buttonBack) == 1) {
      while(digitalRead(buttonBack)==1){}
      if(menuLevel == 1) { settingMode = 0; menuPage = 5; } // Exit to Home (Page 5 is Settings)
      else if(menuLevel == 2) { menuLevel = 1; }           // Go back to Category List
      else if(menuLevel == 3) { 
        loadSettings();                                    // Cancel edits and restore EEPROM values
        menuLevel = 2; editState = 0; subItemSelect = 0;
        cancelledMessageLCD(); 
      }
      lcd.clear();
      return; 
    }

    // --- LEVEL 1: CATEGORIES ---
    if(menuLevel == 1) {
      if(digitalRead(buttonRight) == 1) { menuPage++; if(menuPage>5) menuPage=0; while(digitalRead(buttonRight)==1){} lcd.clear(); return; }
      if(digitalRead(buttonLeft) == 1)  { menuPage--; if(menuPage<0) menuPage=5; while(digitalRead(buttonLeft)==1){} lcd.clear(); return; }
      if(digitalRead(buttonSelect) == 1){ menuLevel = 2; subMenuPage = 0; subItemSelect = 0; while(digitalRead(buttonSelect)==1){} lcd.clear(); return; }

      lcd.setCursor(0,0); lcd.print(" SETTINGS MENU  ");
      lcd.setCursor(0,1);
      if(menuPage == 0)      lcd.print("> BATTERY       ");
      else if(menuPage == 1) lcd.print("> LOAD CONTROL  ");
      else if(menuPage == 2) lcd.print("> CHARGING      ");
      else if(menuPage == 3) lcd.print("> COOLING       ");
      else if(menuPage == 4) lcd.print("> DISPLAY       ");
      else if(menuPage == 5) lcd.print("> SYSTEM        ");
    }

    // --- LEVEL 2 & 3: ITEMS & EDITING ---
    else if (menuLevel == 2 || menuLevel == 3) {
      
      // ---------- CATEGORY 0: BATTERY SETTINGS ---------- //
      if(menuPage == 0) {
        int maxItems = (battPreset == 4) ? 3 : 0; 
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>maxItems) subMenuPage=0; while(digitalRead(buttonRight)==1){} lcd.clear(); return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=maxItems; while(digitalRead(buttonLeft)==1){} lcd.clear(); return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("BATTERY TYPE    ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(menuLevel == 2 || (menuLevel==3 && editState==0)) {
            if(battPreset==0) lcd.print("LiFePO4       ");
            else if(battPreset==1) lcd.print("Li-Ion        ");
            else if(battPreset==2) lcd.print("AGM           ");
            else if(battPreset==3) lcd.print("Flooded       ");
            else if(battPreset==4) lcd.print("Custom        ");
          } else if(menuLevel == 3 && editState == 1) {
            lcd.print(sysVoltage); lcd.print("V SYSTEM      ");
          }

          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; editState=0; while(digitalRead(buttonSelect)==1){} return; }
          else if (menuLevel == 3) {
            if(editState == 0) {
              if(digitalRead(buttonRight)==1){ battPreset++; if(battPreset>4) battPreset=0; while(digitalRead(buttonRight)==1){} return;}
              if(digitalRead(buttonLeft)==1) { battPreset--; if(battPreset<0) battPreset=4; while(digitalRead(buttonLeft)==1){} return;}
              if(digitalRead(buttonSelect)==1){ 
                while(digitalRead(buttonSelect)==1){}
                if(battPreset==4) { saveSettings(); menuLevel=2; savedMessageLCD(); } 
                else { editState = 1; lcd.clear(); } 
                return;
              }
            } else if (editState == 1) {
              if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ 
                if(sysVoltage==12) sysVoltage=24;
                else if(sysVoltage==24) sysVoltage=48; else sysVoltage=12; 
                while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){}
                return;
              }
              if(digitalRead(buttonSelect)==1){ 
                while(digitalRead(buttonSelect)==1){} 
                applyBatteryPreset(battPreset, sysVoltage);
                saveSettings(); menuLevel=2; editState=0; savedMessageLCD(); 
                return;
              }
            }
          }
        }
        else if(subMenuPage == 1) {
          lcd.setCursor(0,0); lcd.print("BATTERY MAX     ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(voltageBatteryMax,2); lcd.print("V   ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { voltageBatteryMax+=1.00; delay(longPressInterval); } else { voltageBatteryMax+=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageBatteryMax,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { voltageBatteryMax-=1.00; delay(longPressInterval); } else { voltageBatteryMax-=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageBatteryMax,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 2) {
          lcd.setCursor(0,0); lcd.print("BATTERY FLOAT   ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(voltageBatteryFloat,2); lcd.print("V   ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { voltageBatteryFloat+=1.00; delay(longPressInterval); } else { voltageBatteryFloat+=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageBatteryFloat,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { voltageBatteryFloat-=1.00; delay(longPressInterval); } else { voltageBatteryFloat-=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageBatteryFloat,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 3) {
          lcd.setCursor(0,0); lcd.print("BATTERY MIN     ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(voltageBatteryMin,2); lcd.print("V   ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { voltageBatteryMin+=1.00; delay(longPressInterval); } else { voltageBatteryMin+=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageBatteryMin,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { voltageBatteryMin-=1.00; delay(longPressInterval); } else { voltageBatteryMin-=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageBatteryMin,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 1: LOAD CONTROL ---------- //
      else if(menuPage == 1) {
        int maxItems = (loadMode == 1) ? 1 : 3; 
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>maxItems) subMenuPage=0; while(digitalRead(buttonRight)==1){} lcd.clear(); return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=maxItems; while(digitalRead(buttonLeft)==1){} lcd.clear(); return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("LOAD CONTROL MODE");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(loadMode==0) lcd.print("AUTOMATIC     "); else lcd.print("MANUAL OVERRD ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ loadMode = !loadMode; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 1) {
          if (loadMode == 1) { // Manual Switch
            lcd.setCursor(0,0); lcd.print("MANUAL STATE    ");
            lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
            if(manualLoadState==0) lcd.print("FORCE OFF     "); else lcd.print("FORCE ON      ");
            if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
            else if (menuLevel == 3) {
              if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ manualLoadState = !manualLoadState; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
              if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
            }
          } else { // Auto LVD
            lcd.setCursor(0,0); lcd.print("LOAD LVD VOLT   ");
            lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(voltageLVD,2); lcd.print("V         ");
            if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
            else if(menuLevel == 3) {
              if(digitalRead(buttonRight)==1) { voltageLVD+=0.1; while(digitalRead(buttonRight)==1){} return; }
              if(digitalRead(buttonLeft)==1)  { voltageLVD-=0.1; while(digitalRead(buttonLeft)==1){} return; }
              if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
            }
          }
        }
        else if(subMenuPage == 2) {
          lcd.setCursor(0,0); lcd.print("LOAD LVR VOLT   ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(voltageLVR,2); lcd.print("V         ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1) { voltageLVR+=0.1; while(digitalRead(buttonRight)==1){} return; }
            if(digitalRead(buttonLeft)==1)  { voltageLVR-=0.1; while(digitalRead(buttonLeft)==1){} return; }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 3) {
          lcd.setCursor(0,0); lcd.print("LOAD LVD DELAY  ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(lvdDelay/1000); lcd.print(" SEC         ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1) { lvdDelay+=1000; while(digitalRead(buttonRight)==1){} return; }
            if(digitalRead(buttonLeft)==1)  { if(lvdDelay>0) lvdDelay-=1000; while(digitalRead(buttonLeft)==1){} return; }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 2: CHARGING ---------- //
      else if(menuPage == 2) {
        int maxItems = 1; 
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>maxItems) subMenuPage=0; while(digitalRead(buttonRight)==1){} lcd.clear(); return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=maxItems; while(digitalRead(buttonLeft)==1){} lcd.clear(); return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("CHARGING CC MODE");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(CC_Mode==1) lcd.print("LIMIT CURRENT "); else lcd.print("MAX HARVEST   ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ CC_Mode = !CC_Mode; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 1) {
          lcd.setCursor(0,0); lcd.print("CHARGE CURRENT  ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(currentCharging,2); lcd.print("A    ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { currentCharging+=1.00; delay(longPressInterval); } else { currentCharging+=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(currentCharging,2); lcd.print("A    ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { currentCharging-=1.00; delay(longPressInterval); } else { currentCharging-=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(currentCharging,2); lcd.print("A    ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 3: COOLING ---------- //
      else if(menuPage == 3) {
        int maxItems = 3; 
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>maxItems) subMenuPage=0; while(digitalRead(buttonRight)==1){} lcd.clear(); return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=maxItems; while(digitalRead(buttonLeft)==1){} lcd.clear(); return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("FAN OVERRIDE    ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(overrideFan==1) lcd.print("FORCE ON      "); else lcd.print("AUTOMATIC     ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ overrideFan = !overrideFan; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 1) {
          lcd.setCursor(0,0); lcd.print("TRIGGER TEMP    ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(temperatureFan); lcd.print((char)223); lcd.print("C    ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { temperatureFan++; delay(longPressInterval); } else { temperatureFan++; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(temperatureFan); lcd.print((char)223); lcd.print("C    ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { temperatureFan--; delay(longPressInterval); } else { temperatureFan--; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(temperatureFan); lcd.print((char)223); lcd.print("C    ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 2) {
          lcd.setCursor(0,0); lcd.print("DYNAMIC COOLING ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(enableDynamicCooling==1) lcd.print("ENABLED       "); else lcd.print("DISABLED      ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ enableDynamicCooling = !enableDynamicCooling; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 3) {
          lcd.setCursor(0,0); lcd.print("SHUTDOWN TEMP   ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(temperatureMax); lcd.print((char)223); lcd.print("C    ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { temperatureMax++; delay(longPressInterval); } else { temperatureMax++; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(temperatureMax); lcd.print((char)223); lcd.print("C    ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { temperatureMax--; delay(longPressInterval); } else { temperatureMax--; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(temperatureMax); lcd.print((char)223); lcd.print("C    ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 4: DISPLAY ---------- //
      else if(menuPage == 4) {
        int maxItems = 1; 
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>maxItems) subMenuPage=0; while(digitalRead(buttonRight)==1){} lcd.clear(); return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=maxItems; while(digitalRead(buttonLeft)==1){} lcd.clear(); return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("BACKLIGHT SLEEP ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(backlightSleepMode==0)      lcd.print("ALWAYS ON     ");
          else if(backlightSleepMode==1) lcd.print("10 SECONDS    ");
          else if(backlightSleepMode==2) lcd.print("5 MINUTES     ");
          else if(backlightSleepMode==3) lcd.print("ON WHEN CHRGNG");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1){ backlightSleepMode++; if(backlightSleepMode>3) backlightSleepMode=0; while(digitalRead(buttonRight)==1){} return;}
            if(digitalRead(buttonLeft)==1) { backlightSleepMode--; if(backlightSleepMode<0) backlightSleepMode=3; while(digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 1) {
          lcd.setCursor(0,0); lcd.print("SERIAL TELEMETRY");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(serialTelemMode==0)      lcd.print("DISABLED      ");
          else if(serialTelemMode==1) lcd.print("DISPLAY ALL   ");
          else if(serialTelemMode==2) lcd.print("ESSENTIAL DATA");
          else if(serialTelemMode==3) lcd.print("NUMBERS ONLY  ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1){ serialTelemMode++; if(serialTelemMode>3) serialTelemMode=0; while(digitalRead(buttonRight)==1){} return;}
            if(digitalRead(buttonLeft)==1) { serialTelemMode--; if(serialTelemMode<0) serialTelemMode=3; while(digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 5: SYSTEM ---------- //
      else if(menuPage == 5) {
        int maxItems = 4; 
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>maxItems) subMenuPage=0; while(digitalRead(buttonRight)==1){} lcd.clear(); return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=maxItems; while(digitalRead(buttonLeft)==1){} lcd.clear(); return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("WIFI RADIO      ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(menuLevel == 2) {
            if(enableWiFi==1) lcd.print("ENABLED       "); else lcd.print("DISABLED      ");
          } else if (menuLevel==3) {
            if(subItemSelect == 0) lcd.print("DISABLE       "); else if(subItemSelect == 1) lcd.print("ENABLE        ");
          }
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; subItemSelect = enableWiFi; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ subItemSelect = !subItemSelect; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ 
              while(digitalRead(buttonSelect)==1){} 
              enableWiFi = subItemSelect; saveSettings(); menuLevel=2; savedMessageLCD(); 
              delay(500); ESP.restart(); 
              return; 
            }
          }
        }
        else if(subMenuPage == 1) { // RESET WIFI CREDENTIALS
          lcd.setCursor(0,0); lcd.print("RESET WIFI CRED?");
          lcd.setCursor(0,1); 
          if(menuLevel == 2) {
            lcd.print("= PRESS SELECT  ");
          } else if (menuLevel == 3) {
            if(subItemSelect == 0) lcd.print("> NO    YES     ");
            else                   lcd.print("  NO   >YES     ");
          }

          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; subItemSelect = 0; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ subItemSelect = !subItemSelect; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ 
              while(digitalRead(buttonSelect)==1){} 
              if(subItemSelect == 1) {
                lcd.clear(); lcd.setCursor(0,0); lcd.print("Clearing WiFi...");
                WiFi.disconnect(true, true); 
                delay(1000); ESP.restart();
              } else {
                menuLevel = 2; subItemSelect = 0; lcd.clear();
              }
              return; 
            }
          }
        }
        else if(subMenuPage == 2) { // FACTORY RESET
          lcd.setCursor(0,0); lcd.print("FACTORY RESET?  ");
          lcd.setCursor(0,1); 
          if(menuLevel == 2) {
            lcd.print("= PRESS SELECT  ");
          } else if (menuLevel == 3) {
            if(subItemSelect == 0) lcd.print("> NO    YES     ");
            else                   lcd.print("  NO   >YES     ");
          }

          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; subItemSelect = 0; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ subItemSelect = !subItemSelect; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ 
              while(digitalRead(buttonSelect)==1){} 
              if(subItemSelect == 1) {
                lcd.clear();
                factoryReset();
                delay(500); ESP.restart();
              } else {
                menuLevel = 2; subItemSelect = 0; lcd.clear();
              }
              return; 
            }
          }
        }
        else if(subMenuPage == 3) { // REBOOT DEVICE
          lcd.setCursor(0,0); lcd.print("REBOOT DEVICE?  ");
          lcd.setCursor(0,1); 
          if(menuLevel == 2) {
            lcd.print("= PRESS SELECT  ");
          } else if (menuLevel == 3) {
            if(subItemSelect == 0) lcd.print("> NO    YES     ");
            else                   lcd.print("  NO   >YES     ");
          }

          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; subItemSelect = 0; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){ subItemSelect = !subItemSelect; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ 
              while(digitalRead(buttonSelect)==1){} 
              if(subItemSelect == 1) {
                lcd.clear(); lcd.setCursor(0,0); lcd.print("Rebooting Corp..");
                delay(1000); ESP.restart();
              } else {
                menuLevel = 2; subItemSelect = 0; lcd.clear();
              }
              return; 
            }
          }
        }
        else if(subMenuPage == 4) {
          lcd.setCursor(0,0); lcd.print("FIRMWARE VER    ");
          lcd.setCursor(0,1); lcd.print("= "); lcd.print(FIRMWARE_VERSION);
        }
      }
    }
  }

  // ================= TELEMETRY MODE ================= //
  else { 
    chargingPause = 0;
    lcdBacklight();

    // 1. INSTANT NAVIGATION: Completely bypasses the 1-second interval execution lock for snappy responsiveness
    if(digitalRead(buttonRight)==1) {
        while(digitalRead(buttonRight)==1){} lcdBacklight_Wake();
        menuPage++; if(menuPage>5) menuPage=0;
        lcd.clear(); prevLCDMillis = 0; // Force immediate frame draw
    }
    else if(digitalRead(buttonLeft)==1) {
        while(digitalRead(buttonLeft)==1){} lcdBacklight_Wake();
        menuPage--; if(menuPage<0) menuPage=5;
        lcd.clear(); prevLCDMillis = 0; 
    }
    else if(digitalRead(buttonBack)==1) {
        while(digitalRead(buttonBack)==1){} lcdBacklight_Wake();
        menuPage=0;
        lcd.clear(); prevLCDMillis = 0; 
    }
    
    // 2. INSTANT MENU ENTRY
    else if(digitalRead(buttonSelect)==1) {
        while(digitalRead(buttonSelect)==1){} lcdBacklight_Wake();
        if(menuPage == 5) {
            settingMode = 1;
            menuLevel   = 1; // Reset deep menu state to Root Category level
            menuPage    = 0; // Point natively to BATTERY category item
            lcd.clear(); 
            return; 
        }
    }
    
    // 3. INTERVAL RENDERING
    currentLCDMillis = millis();
    if(currentLCDMillis-prevLCDMillis>=millisLCDInterval&&enableLCD==1){   
      prevLCDMillis = currentLCDMillis; 
      if(menuPage==0)      {displayConfig1();}
      else if(menuPage==1) {displayConfig2();}
      else if(menuPage==2) {displayConfig3();}
      else if(menuPage==3) {displayConfig4();}
      else if(menuPage==4) {displayConfig5();}
      else if(menuPage==5) {displayConfig6();}
    }
  }
}