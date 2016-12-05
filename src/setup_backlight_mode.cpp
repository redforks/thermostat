#include <core.h>
#include "display.h"
#include "display_mode.h"

void setupBacklightModeOnBlink() {
  setupBacklightMode->onBlink();
}

core::callback SetupBacklightMode::blinkCallback() {
  return setupBacklightModeOnBlink;
} 

void SetupBacklightMode::doBlink(bool showOrHide) {
  lcd.setCursor(12, 0);
  if (!showOrHide) {
    lcd.print(F("   "));
    return;
  }

  lcd.print(onOrOff ? F("On ") : F("Off"));
}

void SetupBacklightMode::enterState() {
  lcd.print(F("Backlight: "));

  onOrOff = getBacklight();
  SetupModeBase::enterState();
}

void SetupBacklightMode::onUpKey() {
  onOrOff = !onOrOff;
  setBacklight(onOrOff);

  updateForAdjust();
}

void SetupBacklightMode::onDownKey() {
  onUpKey();
}

