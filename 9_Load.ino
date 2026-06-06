void load() {
  // 1. HARDWARE PROTECTIONS (Always overrides manual states)
  if (BNC == 1 || FLV == 1) {
    loadStatus = 0; 
    lvdTimerStart = 0;
  } 
  // 2. MANUAL OVERRIDE MODE
  else if (loadMode == 1) {
    loadStatus = manualLoadState;
  }
  // 3. AUTO MODE (LVD/LVR Logic)
  else {
    if (voltageOutput <= voltageLVD) {
      if (lvdTimerStart == 0) lvdTimerStart = millis();
      else if (millis() - lvdTimerStart >= lvdDelay) loadStatus = 0;
    } 
    else if (voltageOutput >= voltageLVR) {
      loadStatus = 1;
      lvdTimerStart = 0;
    } 
    else if (voltageOutput > voltageLVD) {
      lvdTimerStart = 0;
    }
  }
  // Execute the physical relay switch
  digitalWrite(Load, loadStatus);
}