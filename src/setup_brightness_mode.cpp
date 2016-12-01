#include <core.h>
#include "display.h"
#include "display_mode.h"

void setupBrightnessModeOnBlink() {
  setupBrightnessMode->onBlink();
}

core::callback SetupBrightnessMode::blinkCallback() {
  return setupBrightnessModeOnBlink;
} 

void SetupBrightnessMode::doBlink(bool showOrHide) {
  lcd.setCursor(12, 0);
  if (!showOrHide) {
    lcd.print(F("  "));
    return;
  }

  print2DigitsSpace(brightness);
}

void SetupBrightnessMode::enterState() {
  lcd.print(F("Brightness: "));

  brightness = getBrightness() / 16;
  SetupModeBase::enterState();
}

void SetupBrightnessMode::onUpKey() {
  brightness = roundInc<uint8_t>(brightness, 0, 15);
  setBrightness(brightness * 16);

  updateForAdjust();
}

void SetupBrightnessMode::onDownKey() {
  brightness = roundDec<uint8_t>(brightness, 0, 15);
  setBrightness(brightness * 16);

  updateForAdjust();
}

