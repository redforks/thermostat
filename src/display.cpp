#include <EEPROM.h>
#include <core.h>
#include <LiquidCrystal_I2C.h>
#include "display.h"
#include "thermostat.h"
#include "display_mode.h"
#include "shutdown.h"

using namespace core;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set LCD I2C address 0x27, 16 chars wide, 2 lines

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
uint8_t getLastDisplayMode() {
  uint8_t last = EEPROM.read(LAST_DISPLAY_MODE_ADDRESS);
  if (last < 0 || last > 2) {
    return 0;
  }
  return last;
}

void setLastDisplayMode(uint8_t mode) {
  EEPROM.write(LAST_DISPLAY_MODE_ADDRESS, mode);
}

#define NOT_DISPLAY_MODE 255

uint8_t resolveDisplayKind(DisplayMode *mode) {
  if (mode == normalMode) {
    return 0;
  } else if (mode == timeMode) {
    return 1;
  } else if (mode == dayScheduleMode) {
    return 2;
  }
  return NOT_DISPLAY_MODE;
}

void updateLastDisplayMode(DisplayMode *newMode) {
  uint8_t mode = resolveDisplayKind(newMode);

  if (mode != NOT_DISPLAY_MODE) {
    setLastDisplayMode(mode);
  }
}

void restoreLastDisplayMode() {
  uint8_t mode = getLastDisplayMode();
  switch (mode) {
    case 1:
      switchMode(timeMode);
      break;
    case 2:
      switchMode(dayScheduleMode);
      break;
    default:
      switchMode(normalMode);
  }
}

void switchMode(DisplayMode *newMode) {
  if (isShutdown()) {
    uint8_t kind = resolveDisplayKind(newMode);
    if (kind != NOT_DISPLAY_MODE) {
      // If shutdown, replace all non setup display to shutdownMode.
      newMode = shutdownMode;
    }
  }

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

void setBacklight(bool onOrOff) {
  if (onOrOff) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }

  EEPROM.put(BRIGHTNESS_ADDRESS, onOrOff ? 1 : 0);
}

bool getBacklight() {
  return EEPROM.read(BRIGHTNESS_ADDRESS) != 0;
}

void restoreBrightness() {
  setBacklight(getBacklight());
}

void setupDisplay(void) {
  lcd.begin();
  restoreBrightness();

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
