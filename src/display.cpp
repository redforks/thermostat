#include <EEPROM.h>
#include <core.h>
#include <LiquidCrystal.h>
#include "display.h"
#include "thermostat.h"
#include "display_mode.h"

using namespace core;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

DisplayMode _initialMode;

DisplayMode *mode = &_initialMode;

DisplayMode *currentMode() {
  return mode;
}

// Last display mode used in:
//
//  1. on power up, switch to last power down display mode.
//  2. close setup mode, return to last display mode.
// 0: normal mode, 1: time mode, 2: day schedule mode
ModeKind getLastDisplayMode() {
  uint8_t last = EEPROM.read(LAST_DISPLAY_MODE_ADDRESS);
  if (last < ModeKindMin || last > ModeKindMax || last == Initial) {
    return Normal;
  }
  return (ModeKind)last;
}

void setLastDisplayMode(ModeKind mode) {
  EEPROM.write(LAST_DISPLAY_MODE_ADDRESS, (uint8_t)mode);
}

void updateLastDisplayMode(DisplayMode *newMode) {
  ModeKind mode;
  switch (newMode->kind()) {
    case Normal:
    case Time:
    case DaySchedule:
      mode = newMode->kind();
      break;
    default:
      mode = (ModeKind)(ModeKindMax + 1);
      break;
  }

  if (mode != ModeKindMax + 1) {
    setLastDisplayMode(mode);
  }
}

void restoreLastDisplayMode() {
  ModeKind mode = getLastDisplayMode();
  switch (mode) {
    case Time:
      switchMode(timeMode);
      break;
    case DaySchedule:
      switchMode(dayScheduleMode);
      break;
    default:
      switchMode(normalMode);
  }
}

void switchMode(DisplayMode *newMode) {
  if (newMode == mode) {
    return;
  }

  updateLastDisplayMode(newMode);

  mode = newMode;
  lcd.clear();
  newMode->enterState();
}

void onModeKey() {
  if (store::digitals[idKeyMode] == HIGH) {
    mode->onModeKey();
  }
}

void onUpKey() {
  if (store::digitals[idKeyUp] == HIGH) {
    mode->onUpKey();
  }
}

void onDownKey() {
  if (store::digitals[idKeyDown] == HIGH) {
    mode->onDownKey();
  }
}

void onSetupKey() {
  if (store::digitals[idKeySetup] == HIGH) {
    mode->onSetupKey();
  }
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

void onTempeSetpointChanges() {
  mode->onTempeSetpointChanges();
}

void delayStart() {
  restoreLastDisplayMode();
  clock::interval(1000, &onClock);
}

void setupDisplay(void) {
  lcd.begin(16, 2);

  lcd.clear();
  lcd.print(F("Thermostat v2.0"));
  lcd.setCursor(2, 1);
  lcd.print(F("by Red Forks"));

  store::monitorAnalogs(onTempeHumiChanges, 2, idTempe, idHumi);
  store::monitorDigitals(onHeaterChanges, 2, idHeaterReq, idHeaterAct);
  store::monitorDigitals(onModeKey, 1, idKeyMode);
  store::monitorDigitals(onUpKey, 1, idKeyUp);
  store::monitorDigitals(onDownKey, 1, idKeyDown);
  store::monitorDigitals(onSetupKey, 1, idKeySetup);
  store::monitorAnalogs(onTempeSetpointChanges, 1, idTempeSetpoint);

  setupDaySchedule();
  setupScheduleHighLow();

  clock::delay(3000, delayStart);
}
