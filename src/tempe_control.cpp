#include <core.h>
#include <EEPROM.h>
#include "tempe_control.h"
#include "thermostat.h"
#include "read_temp_hum.h"
#include "thermostat.h"
#include "eeprom_helpers.h"
#include "shutdown.h"

using namespace core;

idType idTempeSetpoint;

void setTempeSetpoint(int16_t val, bool notUpdateEEPOM) {
  val = constrain(val, TEMPE_SETPOINT_MIN, TEMPE_SETPOINT_MAX);

  if (val != getTempeSetpoint()) {
    if (!notUpdateEEPOM) {
      EEPROM.put(TEMPE_SETPOINT_EEPROM_ADDRESS, val);
    }
    store::setAnalog(idTempeSetpoint, val);
  }
}

int16_t getTempeSetpoint() {
  return int16_t(store::analogs[idTempeSetpoint]);
}

int16_t getTempeEEPOMSetpoint() {
  return initEEPROMValue(
      TEMPE_SETPOINT_EEPROM_ADDRESS,
      TEMPE_SETPOINT_MIN,
      TEMPE_SETPOINT_MAX,
      DEFAULT_TEMPE_SETPOINT);
}

void initTempeSetpoint() {
  store::setAnalog(idTempeSetpoint, getTempeEEPOMSetpoint());
}

void temperatureLoop() {
  int16_t tempe = readTempe();
  if (tempe >= (getTempeSetpoint() + getTempeHysteresis())) {
    store::setDigital(idHeaterReq, LOW);
  } else if (tempe <= (getTempeSetpoint() - getTempeHysteresis())) {
    store::setDigital(idHeaterReq, HIGH);
  }
}

uint8_t tempeHysteresis;

uint8_t getTempeHysteresis() {
  return tempeHysteresis;
}

void setTempeHysteresis(uint8_t val) {
  val = constrain(val, TEMPE_SETPOINT_HIGH_LOW_MIN, TEMPE_SETPOINT_HIGH_LOW_MAX);
  if (tempeHysteresis != val) {
    tempeHysteresis = val;
    EEPROM.write(TEMPE_SETPOINT_HYSTERESIS_EEPROM_ADDRESS, val);
  }
} 

void initTempSetpointHyster() {
  tempeHysteresis = initEEPROMValue<uint8_t>(
      TEMPE_SETPOINT_HYSTERESIS_EEPROM_ADDRESS,
      TEMPE_SETPOINT_HIGH_LOW_MIN,
      TEMPE_SETPOINT_HIGH_LOW_MAX,
      2);
}

void setupTempeControl(void) {
  idTempeSetpoint = store::defineAnalog();
  initTempeSetpoint();
  initTempSetpointHyster();

  store::monitorAnalogs(temperatureLoop, 2, idTempe, idTempeSetpoint);
}
