#include <core.h>
#include <EEPROM.h>
#include "tempe_control.h"
#include "thermostat.h"
#include "read_temp_hum.h"
#include "thermostat.h"
#include "eeprom_helpers.h"

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
  int16_t val = initEEPROMValue(
      TEMPE_SETPOINT_EEPROM_ADDRESS,
      TEMPE_SETPOINT_MIN,
      TEMPE_SETPOINT_MAX,
      DEFAULT_TEMPE_SETPOINT);

  store::setAnalog(idTempeSetpoint, val);
}

void temperatureLoop() {
  int16_t tempe = readTempe();
  if (tempe >= (getTempeSetpoint() + getTempeSetpointHigh())) {
    store::setDigital(idHeaterReq, LOW);
  } else if (tempe <= (getTempeSetpoint() - getTempeSetpointLow())) {
    store::setDigital(idHeaterReq, HIGH);
  }
}

uint8_t tempSetpointHigh, tempSetpointLow;

uint8_t getTempeSetpointHigh() {
  return tempSetpointHigh;
}

uint8_t getTempeSetpointLow() {
  return tempSetpointLow;
}

void setTempeSetpointHigh(uint8_t val) {
  val = constrain(val, TEMPE_SETPOINT_HIGH_LOW_MIN, TEMPE_SETPOINT_HIGH_LOW_MAX);
  if (tempSetpointHigh != val) {
    tempSetpointHigh = val;
    EEPROM.write(TEMPE_SETPOINT_HIGH_EEPROM_ADDRESS, val);
  }
} 

void setTempeSetpointLow(uint8_t val) {
  val = min(val, TEMPE_SETPOINT_HIGH_LOW_MAX);
  if (tempSetpointLow != val) {
    tempSetpointLow = val;
    EEPROM.write(TEMPE_SETPOINT_LOW_EEPROM_ADDRESS, val);
  }
}

void initTempSetpointHighLow() {
  tempSetpointHigh = initEEPROMValue<uint8_t>(
      TEMPE_SETPOINT_HIGH_EEPROM_ADDRESS,
      TEMPE_SETPOINT_HIGH_LOW_MIN,
      TEMPE_SETPOINT_HIGH_LOW_MAX,
      2);

  tempSetpointLow = initEEPROMValue<uint8_t>(
      TEMPE_SETPOINT_LOW_EEPROM_ADDRESS,
      TEMPE_SETPOINT_HIGH_LOW_MIN,
      TEMPE_SETPOINT_HIGH_LOW_MAX,
      3);
}

void setupTempeControl(void) {
  idTempeSetpoint = store::defineAnalog();
  initTempeSetpoint();
  initTempSetpointHighLow();

  store::monitorAnalogs(temperatureLoop, 1, idTempe);
}
