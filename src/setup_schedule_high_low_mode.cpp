#include "display_mode.h"
#include "thermostat.h"

void setupScheduleHighLowModeOnBlink() {
  static_cast<SetupScheduleHighLowMode*>(setupScheduleHighLowMode)->onBlink();
}

core::callback SetupScheduleHighLowMode::blinkCallback() {
  return setupScheduleHighLowModeOnBlink;
}

void SetupScheduleHighLowMode::enterState() {
  lcd.print(F(" High:     \337C"));
  lcd.setCursor(2, 1);
  lcd.print(F("Low:     \337C"));

  high = getScheduleHighTempeSetpoint();
  low = getScheduleLowTempeSetpoint();

  MultiItemSetupModeBase::enterState();
}

void SetupScheduleHighLowMode::onModeKey() {
  setScheduleHighTempeSetpoint(high);
  setScheduleLowTempeSetpoint(low);

  MultiItemSetupModeBase::onModeKey();
}

void SetupScheduleHighLowMode::onUpKey() {
  uint16_t *pVal = curPart == 0 ? &high : &low;
  (*pVal) ++;
  if (*pVal > TEMPE_SETPOINT_MAX) {
    *pVal = TEMPE_SETPOINT_MIN;
  }

  updateForAdjust();
}

void SetupScheduleHighLowMode::onDownKey() {
  uint16_t *pVal = curPart == 0 ? &high : &low;
  (*pVal) --;
  if (*pVal < TEMPE_SETPOINT_MIN) {
    *pVal = TEMPE_SETPOINT_MAX;
  }

  updateForAdjust();
}

void SetupScheduleHighLowMode::doBlink(bool showOrHide) {
  lcd.setCursor(7, curPart);

  if (showOrHide) {
    printNumber00n0(curPart == 0 ? high : low);
  } else {
    lcd.print(F("    "));
  }
}
