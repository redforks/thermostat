#include "display_mode.h"

#define SETUP_MENU_ITEMS 5

const __FlashStringHelper* getSetupMenuText(uint8_t idx) {
  switch (idx) {
    case 0:
      return F("Setpoint    ");
    case 1:
      return F("Time        ");
    case 2:
      return F("Hysteresis  ");
    case 3:
      return F("Schedule H/L");
    case 4:
      return F("Day Schedule ");
  }
  return F("  ");
}

DisplayMode *const setupModes[SETUP_MENU_ITEMS] = {
  setupSetpointMode,
  setupTimeMode,
  setupTempeHysterMode,
  setupScheduleHighLowMode,
  NULL
};

void SetupMenuMode::enterState() {
  lcd.print(F("SETUP MENU"));

  updateMenuItem();
}

void SetupMenuMode::onModeKey() {
  switchMode(normalMode);
}

void SetupMenuMode::onSetupKey() {
  DisplayMode *mode = setupModes[curMenuItem];
  if (mode != NULL) {
    // TODO: remove if if all setupModes items are not NULL.
    switchMode(mode);
  }
}

void SetupMenuMode::onUpKey() {
  curMenuItem ++;
  if (curMenuItem == SETUP_MENU_ITEMS) {
    curMenuItem = 0;
  }
  updateMenuItem();
}

void SetupMenuMode::onDownKey() {
  curMenuItem --;
  if (curMenuItem == -1) {
    curMenuItem = SETUP_MENU_ITEMS - 1;
  }
  updateMenuItem();
}

void SetupMenuMode::updateMenuItem() {
  lcd.setCursor(0, 1);
  lcd.print(curMenuItem + 1);
  lcd.print(F(". "));
  lcd.print(getSetupMenuText(curMenuItem));
}
