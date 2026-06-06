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
  if(voltageInput <= 0){ PPWM = 0; }                      
  else{ PPWM = (PPWM_margin * pwmMax * voltageOutput) / (100.00 * voltageInput); }              
  PPWM = constrain(PPWM, 0, pwmMaxLimited);
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
  if(ERR > 0 || chargingPause == 1){ buck_Disable(); }               // ERROR PRESENT - Turn off MPPT buck 
  else{
    if(REC == 1){                                                    // IUV RECOVERY 
      REC = 0;
      chargingState = 0;                                             // Reset charging state back to BULK/MPPT upon recovery
      buck_Disable();
      
      Serial.println("> Solar Panel Detected");
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
        // ABSORPTION -> FLOAT Transition 
        if ((millis() - absStartMillis >= absWindow) || (currentOutput < tailCurrentThresh && currentOutput > 0.05)) {
          chargingState = 2;
        }
        // Safety Dropback to Bulk
        if (voltageOutput < (voltageBatteryMax - 1.0000)) {
          chargingState = 0;
        }
      }
      else if (chargingState == 2) {
        // FLOAT -> RE-BULK Transition 
        if (voltageOutput < (voltageBatteryFloat - 0.5000)) {
          chargingState = 0;
        }
      }

      // --- 2. STATE EXECUTION LOGIC ---
      if (chargingState == 0) {                                                    // STATE 0: BULK STAGE (MPPT TRACKING)
        if(currentOutput > currentCharging)         {PWM--;}                           
        else if(voltageOutput > voltageBatteryMax)  {PWM--;}                           
        else{                                                                      // MPPT P&O ALGORITHM
          if(powerInput > powerInputPrev && voltageInput > voltageInputPrev)        {PWM--;}  
          else if(powerInput > powerInputPrev && voltageInput < voltageInputPrev)   {PWM++;}  
          else if(powerInput < powerInputPrev && voltageInput > voltageInputPrev)   {PWM++;}  
          else if(powerInput < powerInputPrev && voltageInput < voltageInputPrev)   {PWM--;}  
          else if(voltageOutput < voltageBatteryMax)                                {PWM++;}  
          
          powerInputPrev   = powerInput;
          voltageInputPrev = voltageInput;
        }   
      }
      else if (chargingState == 1) {                                               // STATE 1: ABSORPTION STAGE 
        if(currentOutput > currentCharging)          {PWM--;}                           
        else if(voltageOutput > voltageBatteryMax)   {PWM--;}                           
        else if(voltageOutput < voltageBatteryMax)   {PWM++;}                           
        else{}
      }
      else if (chargingState == 2) {                                               // STATE 2: FLOAT STAGE 
        if(currentOutput > currentCharging)          {PWM--;}                         
        else if(voltageOutput > voltageBatteryFloat) {PWM--;}                   
        else if(voltageOutput < voltageBatteryFloat) {PWM++;}                         
        else{}
      }
      PWM_Modulation();                                                            // Execute PWM                                                                      
    }  
  }
}