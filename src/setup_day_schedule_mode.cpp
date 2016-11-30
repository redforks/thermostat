#include "display_mode.h"

void setupDayScheduleOnBlink() {
  setupDayScheduleMode->onBlink();
}

core::callback SetupDayScheduleMode::blinkCallback() {
  return setupDayScheduleOnBlink;
}

void SetupDayScheduleMode::doBlink(bool showOrHide) {
  lcd.setCursor(7, chooseTempe ? 1 : 0);
  if (!showOrHide) {
    if (chooseTempe) {
      lcd.print(F("    "));
    } else {
      lcd.print(F("  :  "));
    }
    return;
  }

  if (chooseTempe) {
    if (schedule.get(curSection)) {
      lcd.print(F("HIGH"));
    } else {
      lcd.print(F("LOW "));
    }

    return;
  }

  printHourMin(curSection / 2, curSection % 2 == 0 ? 0 : 30);
}

void SetupDayScheduleMode::enterState() {
  lcd.print(F(" Time: 00:00"));
  lcd.setCursor(0, 1);
  lcd.print(F("Tempe: "));

  schedule.assignFrom(dayScheduleMode->getSchedule());

  chooseTempe = true;
  doBlink(true);
  chooseTempe = false;

  SetupModeBase::enterState();
}

void SetupDayScheduleMode::onModeKey() {
  dayScheduleMode->setSchedule(schedule);

  SetupModeBase::onModeKey();
}

void SetupDayScheduleMode::onSetupKey() {
  doBlink(true);

  chooseTempe = !chooseTempe;

  blinkOn = false;
  doBlink(false);
}

void SetupDayScheduleMode::onUpKey() {
  if (chooseTempe) {
    schedule.toggle(curSection);
  } else {
    curSection = roundInc<uint8_t>(curSection, 0, 47);
  }

  updateForAdjust();
}

void SetupDayScheduleMode::onDownKey() {
  if (chooseTempe) {
    schedule.toggle(curSection);
  } else {
    curSection = roundDec<uint8_t>(curSection, 0, 47);
  }

  updateForAdjust();
}
