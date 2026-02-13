#include "reset.h"

void reset() {
  clearEEPROM();
  // reset WiFiManager saved credentials
  wm.resetSettings();
  // also ensure WiFi credentials removed from WiFi stack
  WiFi.disconnect(true);
  delay(200);
  // restart device to apply changes
  ESP.restart();
}