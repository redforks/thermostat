#include <core.h>
#include "display_mode.h"
#include "tempe_control.h"
#include "thermostat.h"

void setupNormalModeOnBlink() {
  static_cast<SetupSetpointMode*>(setupSetpointMode)->onBlink();
}

core::callback SetupSetpointMode::blinkCallback() {
  return setupNormalModeOnBlink;
}

void SetupSetpointMode::doBlink(bool showOrHide) {
  lcd.setCursor(2, 1);
  if (showOrHide) {
    printNumber00n0(setpoint);
    return;
  }

  lcd.print(F("    "));
}

void SetupSetpointMode::enterState() {
  lcd.print(F("SET TARGET TEMPE"));
  lcd.setCursor(6, 1);
  lcd.print(F("\337C"));

  setpoint = getTempeSetpoint();
  doBlink(true);

  SetupModeBase::enterState();
}

void SetupSetpointMode::onModeKey() {
  SetupModeBase::onModeKey();

  setTempeSetpoint(setpoint);
  switchMode(normalMode);
}

void SetupSetpointMode::onUpKey() {
  setpoint ++;
  if (setpoint > TEMPE_SETPOINT_MAX) {
    setpoint = TEMPE_SETPOINT_MIN;
  }

  updateForAdjust();
}

void SetupSetpointMode::onDownKey() {
  setpoint --;
  if (setpoint < TEMPE_SETPOINT_MIN) {
    setpoint = TEMPE_SETPOINT_MAX;
  }

  updateForAdjust();
}
