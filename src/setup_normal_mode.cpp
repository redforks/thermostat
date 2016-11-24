#include <core.h>
#include "display_mode.h"
#include "tempe_control.h"
#include "thermostat.h"

using namespace core;

void setupNormalModeOnBlink() {
  static_cast<SetupNormalMode*>(setupNormalMode)->onBlink();
}

void SetupNormalMode::updateSetpointForAdjust() {
  blinkOn = true;
  updateSetpoint();

  if (setupNormalModeOnBlink != NULL) {
    clock::removeDelay(blinkDelayHandler);
    blinkDelayHandler = clock::delay(500, setupNormalModeOnBlink);
  }
}

void SetupNormalMode::onBlink() {
  blinkDelayHandler = clock::delay(500, setupNormalModeOnBlink);

  blinkOn = !blinkOn;
  updateSetpoint();
}

void SetupNormalMode::updateSetpoint() {
  lcd.setCursor(2, 1);
  if (blinkOn) {
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
  updateSetpoint();

  blinkDelayHandler = clock::delay(500, setupNormalModeOnBlink);
}

void SetupNormalMode::onModeKey() {
  clock::removeDelay(blinkDelayHandler);

  setTempeSetpoint(setpoint);
  switchMode(normalMode);
}

void SetupNormalMode::onUpKey() {
  setpoint ++;
  if (setpoint > TEMPE_SETPOINT_MAX) {
    setpoint = TEMPE_SETPOINT_MIN;
  }

  updateSetpointForAdjust();
}

void SetupNormalMode::onDownKey() {
  setpoint --;
  if (setpoint < TEMPE_SETPOINT_MIN) {
    setpoint = TEMPE_SETPOINT_MAX;
  }

  updateSetpointForAdjust();
}

void SetupNormalMode::onSetupKey() {
}
