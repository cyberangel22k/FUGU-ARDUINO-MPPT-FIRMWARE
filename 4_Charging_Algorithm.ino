extern unsigned long prevLCDMillis;

void buck_Enable(){                                                                  
  buckEnable = 1;
  digitalWrite(buck_EN,HIGH);
  digitalWrite(LED,HIGH);
}

void buck_Disable(){                                                                 
  buckEnable = 0;
  digitalWrite(buck_EN,LOW);
  digitalWrite(LED,LOW);
  PWM = 0;
}   

void predictivePWM(){                                                                
  // PATCH: Restored the enablePPWM bypass for high-resistance batteries
  if (enablePPWM == 1) {
    if(voltageInput <= 0){ PPWM = 0; }                      
    else{ PPWM = (PPWM_margin * pwmMax * voltageOutput) / (100.00 * voltageInput); }              
    PPWM = constrain(PPWM, 0, pwmMaxLimited);
  } else {
    PPWM = 0; // Forces a slow, gentle soft-start from zero
  }
} 

void PWM_Modulation(){
  predictivePWM();                                                   
  if(chargingState == 0 && currentOutput < currentCharging && voltageOutput < voltageBatteryMax) {
    // ACTIVE MPPT MODE: Safe to use PPWM floor for fast tracking
    PWM = constrain(PWM, PPWM, pwmMaxLimited); 
  } else {
    // CONSTANT VOLTAGE/CURRENT MODE: Allow PWM to drop to 0 to prevent spikes
    PWM = constrain(PWM, 0, pwmMaxLimited);    
  }
  // --- IR2104 GATE DRIVER SHOOT-THROUGH PROTECTION ---
    if (PWM > 0 && PWM < 160) {
    if (voltageOutput > voltageBatteryMax || (chargingState == 2 && voltageOutput > voltageBatteryFloat)) {
       PWM = 0; 
    } else {
       PWM = 160; 
    }
  }
  ledcWrite(buck_IN, PWM);
  // Hardware-level safety: Cleanly disable the gate driver chip entirely if duty cycle is 0
  if (PWM > 0) {
    buck_Enable(); 
  } else {
    buck_Disable();
  }
}
     
void Charging_Algorithm(){
static bool wasPaused = false; // Memory flag to track menu entry/exit

  if(ERR > 0 || chargingPause == 1){ 
    buck_Disable();
    if(chargingPause == 1) { wasPaused = true; } // Flag that we are in the menu
  }               
  else{
    // Combine IUV Recovery (REC) and Menu Resume (wasPaused) into one safe restart block
    if(REC == 1 || wasPaused == true){                                                     
      REC = 0;
      wasPaused = false; // Clear the flag
      chargingState = 0;                                             
      buck_Disable();
      
      lcd.setBacklight(HIGH);
      lcd.setCursor(0,0);
      lcd.print("POWER SOURCE    ");
      lcd.setCursor(0,1); lcd.print("DETECTED        ");
      Serial.println("> Resuming Tracking Algorithm...");
      Serial.print("> Computing For Predictive PWM ");
      for(int i = 0; i < 40; i++){ Serial.print("."); delay(30); }                        
      Serial.println("");
      
      Read_Sensors();
      predictivePWM();
      PWM = PPWM; 
      lcd.clear();
      prevLCDMillis = 0;
    }  
    else{           
      // ================= MPPT & MULTI-STAGE CHARGING ALGORITHM ================= //
      
      // --- 1. STATE TRANSITION LOGIC ---
      if (chargingState == 0) { 
        // BULK -> ABSORPTION Transition
        if (voltageOutput >= voltageBatteryMax) {
          chargingState = 1;
          absStartMillis = millis();
        }
      }
      else if (chargingState == 1) { 
        // ABSORPTION -> NEXT STAGE Transition 
        if ((millis() - absStartMillis >= absWindow) || (currentOutput < tailCurrentThresh && currentOutput > 0.05)) {
          
          // *** CHEMISTRY BRANCHING LOGIC ***
          if (battPreset == 0 || battPreset == 1) { 
            chargingState = 3; // LITHIUM: Terminate charge completely
          } else {
            chargingState = 2; // LEAD-ACID / CUSTOM: Enter Float Mode
          }
          
        }
        // Safety Dropback to Bulk
        if (voltageOutput < (voltageBatteryMax - 1.0000)) {
          chargingState = 0;
        }
      }
      else if (chargingState == 2) {
        // FLOAT -> RE-BULK Transition (Lead-Acid)
        if (voltageOutput < (voltageBatteryFloat - 0.5000)) {
          chargingState = 0;
        }
      }
      else if (chargingState == 3) {
        // TERMINATED -> RE-BULK Transition (Lithium)
        // Repurposes the "Float" variable to act as the Restart threshold
        if (voltageOutput < voltageBatteryFloat) {
          chargingState = 0;
        }
      }

      // --- 2. STATE EXECUTION LOGIC ---
      if (chargingState == 0) {                                                    
        // STATE 0: BULK STAGE (MPPT TRACKING)
        if(currentOutput > currentCharging)                         {PWM--;}                           
        else if(voltageOutput > voltageBatteryMax)                  {PWM--;}                           
        else{                    
          // MPPT P&O ALGORITHM
          if(powerInput > powerInputPrev && voltageInput > voltageInputPrev)        {PWM--;}  
          else if(powerInput > powerInputPrev && voltageInput < voltageInputPrev)   {PWM++;} 
          else if(powerInput < powerInputPrev && voltageInput > voltageInputPrev)   {PWM++;}  
          else if(powerInput < powerInputPrev && voltageInput < voltageInputPrev)   {PWM--;}  
          else if(voltageOutput < voltageBatteryMax)                                {PWM++;}  
        
          powerInputPrev   = powerInput;
          voltageInputPrev = voltageInput;
        }   
      }
      else if (chargingState == 1) {                                               
        // STATE 1: ABSORPTION STAGE 
        if(currentOutput > currentCharging)          {PWM--;}        
        else if(voltageOutput > voltageBatteryMax)   {PWM--;}                           
        else if(voltageOutput < voltageBatteryMax)   {PWM++;}                          
        else{}
      }
      else if (chargingState == 2) {                                               
        // STATE 2: FLOAT STAGE (LEAD-ACID ONLY)
        if(currentOutput > currentCharging)          {PWM--;}  
        else if(voltageOutput > voltageBatteryFloat) {PWM--;}                   
        else if(voltageOutput < voltageBatteryFloat) {PWM++;}                         
        else{}
      }
      else if (chargingState == 3) {
        // STATE 3: CHARGE TERMINATION (LITHIUM ONLY)
        // Set PWM to 0. The PWM_Modulation() function will automatically execute buck_Disable()
        PWM = 0;
      }
      
      PWM_Modulation(); // Execute PWM                                                                      
    }  
  }
}
