#include <core.h>
#include <LiquidCrystal.h>
#include "display.h"
#include "thermostat.h"
#include "display_mode.h"

using namespace core;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

DisplayMode _initialMode;

DisplayMode *mode = &_initialMode;

void switchMode(DisplayMode *newMode) {
  if (newMode == mode) {
    return;
  }

  mode = newMode;
  lcd.clear();
  newMode->enterState();
}

void onModeKey() {
  mode->onModeKey();
}

void onUpKey() {
  mode->onUpKey();
}

void onDownKey() {
  mode->onDownKey();
}

void onSetupKey() {
  mode->onSetupKey();
}

void onTempeHumiChanges() {
  mode->onTempeHumiChanges();
}

void onHeaterChanges() {
  mode->onHeaterChanges();
}

void onClock() {
  mode->onClock();
}

void switchToNormalMode() {
  switchMode(normalMode);
}

void setupDisplay(void) {
  lcd.begin(16, 2);

  lcd.clear();
  lcd.print(F("Thermostat v2.0"));
  lcd.setCursor(0, 1);
  lcd.print(F("Handwork of Forks"));

  store::monitorAnalogs(&onTempeHumiChanges, 2, idTempe, idHumi);
  store::monitorDigitals(&onHeaterChanges, 2, idHeaterReq, idHeaterAct);
  store::monitorDigitals(&onModeKey, 1, idKeyMode);
  store::monitorDigitals(&onUpKey, 1, idKeyUp);
  store::monitorDigitals(&onDownKey, 1, idKeyDown);
  store::monitorDigitals(&onSetupKey, 1, idKeySetup);

  clock::interval(1000, &onClock);
  clock::delay(1000, &switchToNormalMode);
}
