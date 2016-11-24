#include <core.h>
#include "display_mode.h"
#include "tempe_control.h"
#include "thermostat.h"

using namespace core;

void setupNormalModeOnBlink() {
  static_cast<SetupNormalMode*>(setupNormalMode)->onBlink();
}

callback SetupNormalMode::blinkCallback() {
  return setupNormalModeOnBlink;
}

void SetupNormalMode::doBlink(bool showOrHide) {
  lcd.setCursor(2, 1);
  if (showOrHide) {
    printNumber00n0(setpoint);
    return;
  }

  lcd.print("    ");
}

void SetupNormalMode::enterState() {
  lcd.print(F("SET TARGET TEMPE"));
  lcd.setCursor(6, 1);
  lcd.print(F("\337C"));

  setpoint = getTempeSetpoint();
  doBlink(true);

  SetupModeBase::enterState();
}

void SetupNormalMode::onModeKey() {
  SetupModeBase::onModeKey();

  setTempeSetpoint(setpoint);
  switchMode(normalMode);
}

void SetupNormalMode::onUpKey() {
  setpoint ++;
  if (setpoint > TEMPE_SETPOINT_MAX) {
    setpoint = TEMPE_SETPOINT_MIN;
  }

  updateForAdjust();
}

void SetupNormalMode::onDownKey() {
  setpoint --;
  if (setpoint < TEMPE_SETPOINT_MIN) {
    setpoint = TEMPE_SETPOINT_MAX;
  }

  updateForAdjust();
}
