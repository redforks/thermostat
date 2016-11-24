#include "display_mode.h"
#include "tempe_control.h"
#include "thermostat.h"

void SetupNormalMode::updateSetpoint() {
  lcd.setCursor(2, 1);
  printNumber00n0(setpoint);
}

void SetupNormalMode::enterState() {
  lcd.setCursor(1, 0);
  lcd.print(F("SET TARGET TEMPE"));
  lcd.setCursor(6, 1);
  lcd.print(F("\337C"));

  setpoint = getTempeSetpoint();
  updateSetpoint();
}

void SetupNormalMode::onModeKey() {
  setTempeSetpoint(setpoint);
  switchMode(normalMode);
}

void SetupNormalMode::onUpKey() {
  setpoint ++;
  if (setpoint > TEMPE_SETPOINT_MAX) {
    setpoint = TEMPE_SETPOINT_MIN;
  }

  updateSetpoint();
}

void SetupNormalMode::onDownKey() {
  setpoint --;
  if (setpoint < TEMPE_SETPOINT_MIN) {
    setpoint = TEMPE_SETPOINT_MAX;
  }

  updateSetpoint();
}

void SetupNormalMode::onSetupKey() {
}
