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
  setupDayScheduleMode
};

void SetupMenuMode::enterState() {
  lcd.print(F("SETUP MENU"));

  updateMenuItem();
}

void SetupMenuMode::onModeKey() {
  restoreLastDisplayMode();
}

void SetupMenuMode::onSetupKey() {
  DisplayMode *mode = setupModes[curMenuItem];
  switchMode(mode);
}

void SetupMenuMode::onUpKey() {
  curMenuItem = roundInc<int8_t>(curMenuItem, 0, SETUP_MENU_ITEMS - 1);
  updateMenuItem();
}

void SetupMenuMode::onDownKey() {
  curMenuItem = roundDec<int8_t>(curMenuItem, 0, SETUP_MENU_ITEMS - 1);
  updateMenuItem();
}

void SetupMenuMode::updateMenuItem() {
  lcd.setCursor(0, 1);
  lcd.print(curMenuItem + 1);
  lcd.print(F(". "));
  lcd.print(getSetupMenuText(curMenuItem));
}
