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
  static int menuLevel = 1; // 1: Categories, 2: Items, 3: Edit
  static int editState = 0; // Tracks deep jumps (like Battery Type -> Voltage)
  
  // ================= SETTINGS MODE ================= //
  if(settingMode == 1) { 
    chargingPause = 1;
    lcdBacklight_Wake();

    // --- NAVIGATION HANDLER ---
    if(digitalRead(buttonBack) == 1) {
      while(digitalRead(buttonBack)==1){}
      if(menuLevel == 1) { settingMode = 0; menuPage = 5; } // Exit to Home (Page 5 is Settings)
      else if(menuLevel == 2) { menuLevel = 1; } // Go back to Category List
      else if(menuLevel == 3) { 
        loadSettings(); // Cancel edits and restore EEPROM values
        menuLevel = 2; editState = 0; cancelledMessageLCD(); 
      }
      lcd.clear();
      return; 
    }

    // --- LEVEL 1: CATEGORIES ---
    if(menuLevel == 1) {
      if(digitalRead(buttonRight) == 1) { menuPage++; if(menuPage>5) menuPage=0; while(digitalRead(buttonRight)==1){} lcd.clear(); return; }
      if(digitalRead(buttonLeft) == 1)  { menuPage--; if(menuPage<0) menuPage=5; while(digitalRead(buttonLeft)==1){} lcd.clear(); return; }
      if(digitalRead(buttonSelect) == 1){ menuLevel = 2; subMenuPage = 0; while(digitalRead(buttonSelect)==1){} lcd.clear(); return; }

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
                if(sysVoltage==12) sysVoltage=24; else if(sysVoltage==24) sysVoltage=48; else sysVoltage=12; 
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
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>3) subMenuPage=0; while(digitalRead(buttonRight)==1){} return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=3; while(digitalRead(buttonLeft)==1){} return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("LOAD MODE       ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(loadMode==0) lcd.print("AUTO (LVD/R)  "); else lcd.print("MANUAL        ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if (menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1) { loadMode = !loadMode; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 1) {
          lcd.setCursor(0,0); lcd.print("MANUAL STATE    ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(manualLoadState==0) lcd.print("FORCE OFF     "); else lcd.print("FORCE ON      ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if (menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1) { manualLoadState = !manualLoadState; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 2) {
          lcd.setCursor(0,0); lcd.print("DISCONNECT (LVD)");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(voltageLVD,2); lcd.print("V   ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
             if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { voltageLVD+=1.00; delay(longPressInterval); } else { voltageLVD+=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageLVD,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { voltageLVD-=1.00; delay(longPressInterval); } else { voltageLVD-=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageLVD,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 3) {
          lcd.setCursor(0,0); lcd.print("RECONNECT (LVR) ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(voltageLVR,2); lcd.print("V   ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
             if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { voltageLVR+=1.00; delay(longPressInterval); } else { voltageLVR+=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageLVR,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { voltageLVR-=1.00; delay(longPressInterval); } else { voltageLVR-=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(voltageLVR,2); lcd.print("V   ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 2: CHARGING SETTINGS ---------- //
      else if(menuPage == 2) {
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>1) subMenuPage=0; while(digitalRead(buttonRight)==1){} return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=1; while(digitalRead(buttonLeft)==1){} return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("MPPT ALGORITHM  ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(MPPT_Mode==1) lcd.print("ENABLED       "); else lcd.print("DISABLED      ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if (menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1) { MPPT_Mode = !MPPT_Mode; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 1) {
          lcd.setCursor(0,0); lcd.print("CHARGE CURRENT  ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(currentCharging,2); lcd.print("A   ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
             if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { currentCharging+=1.00; delay(longPressInterval); } else { currentCharging+=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(currentCharging,2); lcd.print("A   ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { currentCharging-=1.00; delay(longPressInterval); } else { currentCharging-=0.01; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(currentCharging,2); lcd.print("A   ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 3: COOLING SETTINGS ---------- //
      else if(menuPage == 3) {
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>2) subMenuPage=0; while(digitalRead(buttonRight)==1){} return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=2; while(digitalRead(buttonLeft)==1){} return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("FAN MODE        ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          
          // 1. Determine current active mode
          int tempFanMode = 0;
          if(enableFan == 0) tempFanMode = 0; 
          else if(overrideFan == 1) tempFanMode = 1;
          else if(enableDynamicCooling == 1) tempFanMode = 2;
          else tempFanMode = 3; // Static On/Off Mode
          
          // 2. Print selection to LCD
          if(menuLevel == 2) {
            if(tempFanMode==0) lcd.print("OFF           ");
            else if(tempFanMode==1) lcd.print("ON            ");
            else if(tempFanMode==2) lcd.print("AUTO PWM      ");
            else lcd.print("AUTO STATIC   ");
          } else if(menuLevel == 3) {
            if(subItemSelect==0) lcd.print("OFF           ");
            else if(subItemSelect==1) lcd.print("ON            ");
            else if(subItemSelect==2) lcd.print("AUTO PWM      ");
            else lcd.print("AUTO STATIC   ");
          }
          
          // 3. Handle Button Inputs
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { 
            menuLevel=3; subItemSelect = tempFanMode; 
            while(digitalRead(buttonSelect)==1){} return; 
          }
          else if (menuLevel == 3) {
            // Expand selection limit from 2 to 3
            if(digitalRead(buttonRight)==1){ 
              subItemSelect++; if(subItemSelect>3) subItemSelect=0; 
              while(digitalRead(buttonRight)==1){} return;
            }
            if(digitalRead(buttonLeft)==1) { 
              subItemSelect--; if(subItemSelect<0) subItemSelect=3;
              while(digitalRead(buttonLeft)==1){} return;
            }
            
            // 4. Save Logic Mapping
            if(digitalRead(buttonSelect)==1){ 
              while(digitalRead(buttonSelect)==1){} 
              
              if(subItemSelect == 0)      { enableFan = 0; overrideFan = 0; enableDynamicCooling = 0; }
              else if(subItemSelect == 1) { enableFan = 1; overrideFan = 1; enableDynamicCooling = 0; }
              else if(subItemSelect == 2) { enableFan = 1; overrideFan = 0; enableDynamicCooling = 1; }
              else if(subItemSelect == 3) { enableFan = 1; overrideFan = 0; enableDynamicCooling = 0; }
              
              saveSettings(); 
              menuLevel=2; 
              savedMessageLCD(); 
              return;
            }
          }
        }
        else if(subMenuPage == 1) {
          lcd.setCursor(0,0); lcd.print("TRIGGER TEMP    ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(temperatureFan); lcd.print((char)223); lcd.print("C  ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
             if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { temperatureFan++; delay(longPressInterval); } else { temperatureFan++; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(temperatureFan); lcd.print((char)223); lcd.print("C  ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { temperatureFan--; delay(longPressInterval); } else { temperatureFan--; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(temperatureFan); lcd.print((char)223); lcd.print("C  ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 2) {
          lcd.setCursor(0,0); lcd.print("SHUTDOWN TEMP   ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print(">"); else lcd.print("="); lcd.print(temperatureMax); lcd.print((char)223); lcd.print("C  ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if(menuLevel == 3) {
             if(digitalRead(buttonRight)==1) { 
              unsigned long pTime = millis();
              while(digitalRead(buttonRight)==1) {
                if(millis()-pTime > longPressTime) { temperatureMax++; delay(longPressInterval); } else { temperatureMax++; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(temperatureMax); lcd.print((char)223); lcd.print("C  ");
              }
            }
            if(digitalRead(buttonLeft)==1)  { 
              unsigned long pTime = millis();
              while(digitalRead(buttonLeft)==1) {
                if(millis()-pTime > longPressTime) { temperatureMax--; delay(longPressInterval); } else { temperatureMax--; delay(shortPressInterval); }
                lcd.setCursor(1,1); lcd.print(temperatureMax); lcd.print((char)223); lcd.print("C  ");
              }
            }
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 4: DISPLAY SETTINGS ---------- //
      else if(menuPage == 4) {
        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("TIMEOUT LIMIT   ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          
          if(menuLevel == 2) {
            if(backlightSleepMode==1) lcd.print("10 SECONDS  "); else if(backlightSleepMode==2) lcd.print("5 MINUTES   ");
            else if(backlightSleepMode==3) lcd.print("DAYTIME ON  "); else lcd.print("NEVER       ");
          } else if(menuLevel == 3) {
            if(subItemSelect==1) lcd.print("10 SECONDS  "); else if(subItemSelect==2) lcd.print("5 MINUTES   ");
            else if(subItemSelect==3) lcd.print("DAYTIME ON  "); else lcd.print("NEVER       ");
          }

          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; subItemSelect = backlightSleepMode; while(digitalRead(buttonSelect)==1){} return; }
          else if (menuLevel == 3) {
            if(digitalRead(buttonRight)==1){ subItemSelect++; if(subItemSelect>3) subItemSelect=0; while(digitalRead(buttonRight)==1){} return;}
            if(digitalRead(buttonLeft)==1) { subItemSelect--; if(subItemSelect<0) subItemSelect=3; while(digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} backlightSleepMode = subItemSelect; saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
      }

      // ---------- CATEGORY 5: SYSTEM CONFIG ---------- //
      else if(menuPage == 5) {
        if(digitalRead(buttonRight) == 1 && menuLevel == 2) { subMenuPage++; if(subMenuPage>4) subMenuPage=0; while(digitalRead(buttonRight)==1){} return; }
        if(digitalRead(buttonLeft) == 1 && menuLevel == 2)  { subMenuPage--; if(subMenuPage<0) subMenuPage=4; while(digitalRead(buttonLeft)==1){} return; }

        if(subMenuPage == 0) {
          lcd.setCursor(0,0); lcd.print("WIFI RADIO      ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          
          if(menuLevel == 2) {
            if(enableWiFi==1) lcd.print("ENABLED       "); else lcd.print("DISABLED      ");
          } else if (menuLevel==3) { 
            if(subItemSelect == 0) lcd.print("DISABLE       "); else if(subItemSelect == 1) lcd.print("ENABLE        "); else lcd.print("RESET CREDS   ");
          }

          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; subItemSelect = enableWiFi; while(digitalRead(buttonSelect)==1){} return; }
          else if (menuLevel == 3) {
            if(digitalRead(buttonRight)==1){ subItemSelect++; if(subItemSelect>2) subItemSelect=0; while(digitalRead(buttonRight)==1){} return;}
            if(digitalRead(buttonLeft)==1) { subItemSelect--; if(subItemSelect<0) subItemSelect=2; while(digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ 
              while(digitalRead(buttonSelect)==1){} 
              if(subItemSelect == 2) { resetWiFi(); resetBlynk(); menuLevel=2; } 
              else { enableWiFi = subItemSelect; saveSettings(); menuLevel=2; savedMessageLCD(); }
              return;
            }
          }
        }
        else if(subMenuPage == 1) {
          lcd.setCursor(0,0); lcd.print("BLUETOOTH RADIO ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          if(enableBluetooth==1) lcd.print("ENABLED       "); else lcd.print("DISABLED      ");
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; while(digitalRead(buttonSelect)==1){} return; }
          else if (menuLevel == 3) {
            if(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1) { enableBluetooth = !enableBluetooth; while(digitalRead(buttonRight)==1 || digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 2) {
          lcd.setCursor(0,0); lcd.print("USB TELEMETRY   ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> "); else lcd.print("= ");
          
          if(menuLevel == 2) {
            if(serialTelemMode==1) lcd.print("DISPLAY ALL "); else if(serialTelemMode==2) lcd.print("ESSENTIAL   "); 
            else if(serialTelemMode==3) lcd.print("NUMBERS ONLY"); else lcd.print("DISABLED    ");
          } else if(menuLevel == 3) {
            if(subItemSelect==1) lcd.print("DISPLAY ALL "); else if(subItemSelect==2) lcd.print("ESSENTIAL   "); 
            else if(subItemSelect==3) lcd.print("NUMBERS ONLY"); else lcd.print("DISABLED    ");
          }

          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { menuLevel=3; subItemSelect = serialTelemMode; while(digitalRead(buttonSelect)==1){} return; }
          else if (menuLevel == 3) {
            if(digitalRead(buttonRight)==1){ subItemSelect++; if(subItemSelect>3) subItemSelect=0; while(digitalRead(buttonRight)==1){} return;}
            if(digitalRead(buttonLeft)==1) { subItemSelect--; if(subItemSelect<0) subItemSelect=3; while(digitalRead(buttonLeft)==1){} return;}
            if(digitalRead(buttonSelect)==1){ while(digitalRead(buttonSelect)==1){} serialTelemMode = subItemSelect; saveSettings(); menuLevel=2; savedMessageLCD(); return; }
          }
        }
        else if(subMenuPage == 3) {
          lcd.setCursor(0,0); lcd.print("FACTORY RESET   ");
          lcd.setCursor(0,1); if(menuLevel==3) lcd.print("> HOLD SELECT "); else lcd.print("  PRESS SELECT");
          
          if(menuLevel == 2 && digitalRead(buttonSelect)==1) { 
              menuLevel=3; currentButtonMillis=0; while(digitalRead(buttonSelect)==1){} return; 
          }
          else if (menuLevel == 3) {
            if(digitalRead(buttonSelect) == 1) {
                if (currentButtonMillis == 0) currentButtonMillis = millis(); 
                if (millis() - currentButtonMillis > 3000) { factoryReset(); } 
            } else {
                if (currentButtonMillis > 0) { 
                    menuLevel=2; currentButtonMillis=0; lcd.clear(); return;
                }
            }
          }
        }
        else if(subMenuPage == 4) {
          lcd.setCursor(0,0); lcd.print("FIRMWARE VERSION");
          lcd.setCursor(0,1); lcd.print(firmwareInfo); lcd.setCursor(8,1); lcd.print(firmwareDate); 
        }
      }
    }
  }
  
  // ================= MAIN UI (VIEW MODE) ================= //
  else if(settingMode==0) {
    chargingPause = 0;
    lcdBacklight();

    // 1. INSTANT NAVIGATION: Completely bypasses the 1-second refresh timer
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
            menuLevel   = 1; // Reset deep menu state to Root
            menuPage    = 0; // Point to BATTERY category
            lcd.clear(); 
            return; 
        }
    }
    
    // 3. INTERVAL RENDERING
    currentLCDMillis = millis();
    if(currentLCDMillis-prevLCDMillis>=millisLCDInterval&&enableLCD==1){   
      prevLCDMillis = currentLCDMillis;

      if(menuPage==0)     {displayConfig1();}
      else if(menuPage==1){displayConfig2();}
      else if(menuPage==2){displayConfig3();}
      else if(menuPage==3){displayConfig4();}
      else if(menuPage==4){displayConfig5();} 
      else if(menuPage==5){displayConfig6();} 
    }    
  }
}