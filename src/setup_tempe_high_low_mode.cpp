#include <core.h>
#include "display_mode.h"
#include "tempe_control.h"
#include "thermostat.h"

void setupTempeHighOnBlink() {
  static_cast<SetupTempeHighLowMode*>(setupTempeHighLowMode)->onBlink();
}

void SetupTempeHighLowMode::doBlink(bool showOrHide) {
  uint8_t val, x;
  if (curPart == 0) {
    val = high;
    x = 6;
  }  else {
    val = low;
    x = 14;
  }
  lcd.setCursor(x, 1);

  if (showOrHide) {
    lcd.print(val);
  } else {
    lcd.print(' ');
  }
}

core::callback SetupTempeHighLowMode::blinkCallback() {
  return setupTempeHighOnBlink;
}

void SetupTempeHighLowMode::enterState() {
  lcd.print(F("SET TEMP HIGH/LOW"));
  lcd.setCursor(1, 1);
  lcd.print(F("H: 0.   L: 0."));

  high = getTempeSetpointHigh();
  low = getTempeSetpointLow();

  curPart = 1;
  doBlink(true);

  curPart = 0;
  doBlink(true);

  SetupModeBase::enterState();
}

void SetupTempeHighLowMode::onModeKey() {
  SetupModeBase::onModeKey();

  setTempeSetpointHigh(high);
  setTempeSetpointLow(low);

  switchMode(normalMode);
}

void SetupTempeHighLowMode::onUpKey() {
  uint8_t *pVal = (curPart == 0) ? &high : &low;
  (*pVal) ++;
  if (*pVal > TEMPE_SETPOINT_HIGH_LOW_MAX) {
    *pVal = TEMPE_SETPOINT_HIGH_LOW_MIN;
  }

  updateForAdjust();
}

void SetupTempeHighLowMode::onDownKey() {
  uint8_t *pVal = (curPart == 0) ? &high : &low;
  (*pVal) --;
  if (*pVal < TEMPE_SETPOINT_HIGH_LOW_MIN) {
    *pVal = TEMPE_SETPOINT_HIGH_LOW_MAX;
  }

  updateForAdjust();
}

void SetupTempeHighLowMode::onSetupKey() {
  doBlink(true);
  curPart = (curPart + 1) % 2;
  doBlink(false);
}
