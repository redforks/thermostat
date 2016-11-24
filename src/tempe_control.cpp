#include <core.h>
#include <EEPROM.h>
#include "tempe_control.h"
#include "thermostat.h"
#include "read_temp_hum.h"
#include "thermostat.h"

using namespace core;

idType idTempeSetpoint;

void setTempeSetpoint(int16_t val) {
  val = constrain(val, TEMPE_SETPOINT_MIN, TEMPE_SETPOINT_MAX);

  if (val != getTempeSetpoint()) {
    EEPROM.put(TEMPE_SETPOINT_EEPROM_ADDRESS, val);
    store::setAnalog(idTempeSetpoint, val);
  }
}

int16_t getTempeSetpoint() {
  return int16_t(store::analogs[idTempeSetpoint]);
}

void initTempeSetpoint() {
  int16_t val;
  EEPROM.get(TEMPE_SETPOINT_EEPROM_ADDRESS, val);
  if (val < 100 || val > 350) {
    setTempeSetpoint(DEFAULT_TEMPE_SETPOINT);
    return;
  }

  store::setAnalog(idTempeSetpoint, val);
}

void temperatureLoop() {
  int16_t tempe = readTempe();
  if (tempe >= (getTempeSetpoint() + 2)) {
    store::setDigital(idHeaterReq, LOW);
  } else if (tempe <= (getTempeSetpoint() - 3)) {
    store::setDigital(idHeaterReq, HIGH);
  }
}

void setupTempeControl(void) {
  idTempeSetpoint = store::defineAnalog();
  initTempeSetpoint();

  store::monitorAnalogs(temperatureLoop, 1, idTempe);
}
