#include <core.h>
#include "display_mode.h"
#include "tempe_control.h"
#include "thermostat.h"

void setupTempeHighOnBlink() {
  setupTempeHysterMode->onBlink();
}

void SetupTempeHysterMode::doBlink(bool showOrHide) {
  lcd.setCursor(13, 1);

  if (showOrHide) {
    lcd.print(hysteresis);
  } else {
    lcd.print(' ');
  }
}

core::callback SetupTempeHysterMode::blinkCallback() {
  return setupTempeHighOnBlink;
}

void SetupTempeHysterMode::enterState() {
  lcd.print(F("SET TEMPERATURE"));
  lcd.setCursor(0, 1);
  lcd.print(F("HYSTERESIS 0."));

  hysteresis = getTempeHysteresis();

  SetupModeBase::enterState();
}

void SetupTempeHysterMode::onModeKey() {
  SetupModeBase::onModeKey();

  setTempeHysteresis(hysteresis);
}

void SetupTempeHysterMode::onUpKey() {
  hysteresis = roundInc<uint8_t>(hysteresis, TEMPE_SETPOINT_HIGH_LOW_MIN, TEMPE_SETPOINT_HIGH_LOW_MAX);

  updateForAdjust();
}

void SetupTempeHysterMode::onDownKey() {
  hysteresis = roundDec<uint8_t>(hysteresis, TEMPE_SETPOINT_HIGH_LOW_MIN, TEMPE_SETPOINT_HIGH_LOW_MAX);

  updateForAdjust();
}
