#include <EEPROM.h>
#include "shutdown.h"
#include "thermostat.h"
#include "eeprom_helpers.h"
#include "tempe_control.h"

bool shutdownState;
bool isShutdown() {
  return shutdownState;
}

void setShutdown(bool newMode) {
  if (isShutdown() == newMode) {
    return;
  }

  shutdownState = newMode;
  EEPROM.write(SHUTDOWN_STATE_ADDRESS, newMode ? 1 : 0);
  if (newMode) {
    setTempeSetpoint(50, true);
  } else {
    setTempeSetpoint(50, getTempeEEPOMSetpoint());
  }
}

void setupShutdown() {
  int8_t val = initEEPROMValue<int8_t>(SHUTDOWN_STATE_ADDRESS, 0, 1, 0);
  shutdownState = val != 0;
  if (shutdownState) {
    setTempeSetpoint(50, true);
  }
}
