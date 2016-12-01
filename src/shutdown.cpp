#include <EEPROM.h>
#include "shutdown.h"
#include "thermostat.h"

int8_t shutdownState = -1; // -1: not inited, 0: false, 1: true
bool isShutdown() {
  if (shutdownState == -1) {
    shutdownState = EEPROM.read(SHUTDOWN_STATE_ADDRESS);
  }

  if (shutdownState < 0 || shutdownState > 1) {
    return false;
  }
  return shutdownState != 0;
}

void setShutdown(bool newMode) {
  bool exist = isShutdown();
  if (newMode == exist) {
    return;
  }

  shutdownState = newMode;
  EEPROM.write(SHUTDOWN_STATE_ADDRESS, newMode ? 1 : 0);
}
