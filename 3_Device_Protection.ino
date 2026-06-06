void backflowControl(){                                                 
  // PV BACKFLOW CONTROL (CHARGER MODE ONLY) 
   if((voltageInput > voltageOutput + voltageDropout) && (IUV == 0)){ 
    bypassEnable = 1;  // Normal Condition - Turn on Backflow MOSFET
  }  
  else{ 
    bypassEnable = 0;  // Input Undervoltage OR Night Timeout - Prevent PV Backflow
  }
  digitalWrite(backflow_MOSFET, bypassEnable);                            // Signal backflow MOSFET GPIO pin   
}

void Device_Protection(){
  // ERROR COUNTER RESET
  currentRoutineMillis = millis();
  if(currentErrorMillis - prevErrorMillis >= errorTimeLimit){                                          
    prevErrorMillis = currentErrorMillis;
    if(errorCount < errorCountLimit){ errorCount = 0; }                                                  
  } 
  
  // FAULT DETECTION     
  ERR = 0;                                                                                           // Reset local error counter
  
  if(temperature > temperatureMax)                             {OTE=1;ERR++;errorCount++;} else{OTE=0;}  // OTE - OVERTEMPERATURE
  if(currentInput > currentInAbsolute)                         {IOC=1;ERR++;errorCount++;} else{IOC=0;}  // IOC - INPUT  OVERCURRENT
  if(currentOutput > currentOutAbsolute)                       {OOC=1;ERR++;errorCount++;} else{OOC=0;}  // OOC - OUTPUT OVERCURRENT 
  if(voltageOutput > voltageBatteryMax + voltageBatteryThresh) {OOV=1;ERR++;errorCount++;} else{OOV=0;}  // OOV - OUTPUT OVERVOLTAGE                     
  if(voltageInput < vInSystemMin && voltageOutput < vInSystemMin){FLV=1;ERR++;errorCount++;} else{FLV=0;}  // FLV - Fatally low system voltage 

  // CHARGER MODE SPECIFIC PROTECTION 
  if(voltageOutput < vInSystemMin)                             {BNC=1;ERR++;}              else{BNC=0;}  // BNC - BATTERY NOT CONNECTED 
  
  // --- SUNSET CHATTER TIMER FIX ---
  static unsigned long lastIuvTrigger = 0;
  static bool iuvActive = false;

  if(voltageInput < voltageOutput + voltageDropout) {
    IUV=1;                     // IUV - INPUT UNDERVOLTAGE
    ERR++;
    REC=1;
    lastIuvTrigger = millis(); // Record the exact moment the panel collapsed
    iuvActive = true;          // Lock the system in sleep mode
  }
  else{
    if (iuvActive == true) {
      if (millis() - lastIuvTrigger > 60000) {
          IUV=0; 
          iuvActive = false;   // 60s passed, allow wake up!
      } else {
          IUV=1;               // Still in the 60-second penalty box.
          ERR++;
      }
    } else {
      IUV = 0;                 // Normal daylight operation
    }
  }

  // RUN BACKFLOW PROTECTION AT THE VERY END
  // Moving this here ensures it uses the completely fresh, up-to-date IUV status calculated above
  backflowControl();                                                                                 
}