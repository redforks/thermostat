#include <EEPROM.h>
#include "display_mode.h"
#include "thermostat.h"
#include "eeprom_helpers.h"

int16_t scheduleHighTemperature, scheduleLowTemperature;

void setupScheduleTempeSetpoint() {
  scheduleHighTemperature = initEEPROMValue<int16_t>(
      TEMPE_SCHEDULE_HIGH_TEMPE_ADDRESS,
      TEMPE_SETPOINT_MIN,
      TEMPE_SETPOINT_MAX,
      204);

  scheduleLowTemperature = initEEPROMValue<int16_t>(
      TEMPE_SCHEDULE_LOW_TEMPE_ADDRESS,
      TEMPE_SETPOINT_MIN,
      TEMPE_SETPOINT_MAX,
      195);
}

int16_t getScheduleHighTempeSetpoint() {
  return scheduleHighTemperature;
}

int16_t getScheduleLowTempeSetpoint() {
  return scheduleLowTemperature;
}

void setScheduleHighTempeSetpoint(int16_t val) {
  val = constrain(val, TEMPE_SETPOINT_MIN, TEMPE_SETPOINT_MAX);

  if (val != getScheduleHighTempeSetpoint()) {
    EEPROM.put(TEMPE_SCHEDULE_HIGH_TEMPE_ADDRESS, val);
    scheduleHighTemperature = val;
  }
}

void setScheduleLowTempeSetpoint(int16_t val) {
  val = constrain(val, TEMPE_SETPOINT_MIN, TEMPE_SETPOINT_MAX);

  if (val != getScheduleLowTempeSetpoint()) {
    EEPROM.put(TEMPE_SCHEDULE_LOW_TEMPE_ADDRESS, val);
    scheduleLowTemperature = val;
  }
}

void setupScheduleTempeHighLowMode() {
  setupScheduleTempeSetpoint();
}
