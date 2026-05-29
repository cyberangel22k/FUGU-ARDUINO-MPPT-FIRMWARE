void load() {
  // If the battery is completely disconnected or system is fatally low, force the load off immediately.
  if (BNC == 1 || FLV == 1) {
    loadStatus = 0; 
    lvdTimerStart = 0;
  } 
  else {
    // 1. LOW VOLTAGE DISCONNECT LOGIC (With 30-Second Debounce)
    if (voltageOutput <= voltageLVD) {
      if (lvdTimerStart == 0) {
        lvdTimerStart = millis(); // Start the timer the moment voltage dips below LVD
      } 
      else if (millis() - lvdTimerStart >= lvdDelay) { 
        // Voltage has stayed below LVD continuously for 30 seconds
        loadStatus = 0; 
      }
    } 
    // 2. LOW VOLTAGE RECONNECT LOGIC
    else if (voltageOutput >= voltageLVR) {
      loadStatus = 1;       // Battery is healthy enough to take the house load back
      lvdTimerStart = 0;    // Reset the timer
    } 
    // 3. HYSTERESIS ZONE (Between LVD and LVR)
    else {
      // If voltage sags but recovers before the 30 seconds is up, reset the timer.
      // This prevents the timer from accumulating micro-sags over time.
      if (voltageOutput > voltageLVD) {
        lvdTimerStart = 0;
      }
    }
  }

  // Execute the physical relay switch
  digitalWrite(Load, loadStatus);
}