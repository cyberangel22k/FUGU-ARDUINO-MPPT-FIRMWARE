void backflowControl(){                                                
  // PV BACKFLOW CONTROL (CHARGER MODE ONLY) 
  if(voltageInput > voltageOutput + voltageDropout){ bypassEnable = 1; }  // Normal Condition - Turn on Backflow MOSFET
  else{ bypassEnable = 0; }                                               // Input Undervoltage - Prevent PV Backflow (SS)
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
  ERR = 0;                                                                                          // Reset local error counter
  backflowControl();                                                                                // Run backflow current protection protocol  
  
  if(temperature > temperatureMax)                             {OTE=1;ERR++;errorCount++;} else{OTE=0;}  // OTE - OVERTEMPERATURE: System overheat detected
  if(currentInput > currentInAbsolute)                         {IOC=1;ERR++;errorCount++;} else{IOC=0;}  // IOC - INPUT  OVERCURRENT: Input current has reached absolute limit
  if(currentOutput > currentOutAbsolute)                       {OOC=1;ERR++;errorCount++;} else{OOC=0;}  // OOC - OUTPUT OVERCURRENT: Output current has reached absolute limit 
  if(voltageOutput > voltageBatteryMax + voltageBatteryThresh) {OOV=1;ERR++;errorCount++;} else{OOV=0;}  // OOV - OUTPUT OVERVOLTAGE: Output voltage has reached absolute limit                     
  if(voltageInput < vInSystemMin && voltageOutput < vInSystemMin){FLV=1;ERR++;errorCount++;} else{FLV=0;}  // FLV - Fatally low system voltage 

  // CHARGER MODE SPECIFIC PROTECTION 
  if(voltageOutput < vInSystemMin)                             {BNC=1;ERR++;}              else{BNC=0;}  // BNC - BATTERY NOT CONNECTED 
  if(voltageInput < voltageOutput + voltageDropout)            {IUV=1;ERR++;REC=1;}        else{IUV=0;}  // IUV - INPUT UNDERVOLTAGE: Input below max battery charging voltage     
}