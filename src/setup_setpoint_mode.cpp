#include <core.h>
#include "display_mode.h"
#include "tempe_control.h"
#include "thermostat.h"

void setupNormalModeOnBlink() {
  setupSetpointMode->onBlink();
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

  SetupModeBase::enterState();
}

void SetupSetpointMode::onModeKey() {
  SetupModeBase::onModeKey();

  setTempeSetpoint(setpoint);
}

void SetupSetpointMode::onUpKey() {
  setpoint = roundInc(setpoint, TEMPE_SETPOINT_MIN, TEMPE_SETPOINT_MAX);

  updateForAdjust();
}

void SetupSetpointMode::onDownKey() {
  setpoint = roundDec(setpoint, TEMPE_SETPOINT_MIN, TEMPE_SETPOINT_MAX);

  updateForAdjust();
}
