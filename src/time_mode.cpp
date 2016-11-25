#include <avr/pgmspace.h>
#include <Time.h>
#include <core.h>
#include "display_mode.h"
#include "rtc.h"

using namespace core;

void TimeMode::enterState() {
  onClock();
}

void TimeMode::onModeKey() {
  switchMode(dayScheduleMode);
}

void TimeMode::onSetupKey() {
  switchMode(setupTimeMode);
}

const PROGMEM char weekDayNames[8][4] = {
  "   ", "SUN", "MON", "TUE", "WES", "THU", "FRI", "SAT"
};

const char* getWeekDayName(uint8_t wday) {
  if (wday < 1 || wday > 7) {
    wday = 0;
  }
  return weekDayNames[wday];
}

void TimeMode::onClock() {
  tmElementsPtr_t now = rtcNow();

  lcd.setCursor(1, 0);
  lcd.print(1970 + now->Year);
  lcd.print('-');
  print2DigitsZero(now->Month);
  lcd.print('-');
  print2DigitsZero(now->Day);
  lcd.print(' ');
  lcd.print(getWeekDayName(now->Wday));

  lcd.setCursor(4, 1);
  print2DigitsZero(now->Hour);
  lcd.print(':');
  print2DigitsZero(now->Minute);
  lcd.print(':');
  print2DigitsZero(now->Second);
}


#define TIME_PARTS 6

const PROGMEM uint8_t timePartMins[TIME_PARTS] = {2010-1970, 1, 1, 1, 0, 0};
const PROGMEM uint8_t timePartMaxs[TIME_PARTS] = {2050-1970, 12, 0, 7, 23, 59};

uint8_t* SetupTimeMode::getCurrentPart() {
  switch (currentPart) {
    case 0:
      return &tm.Year;
    case 1:
      return &tm.Month;
    case 2:
      return &tm.Day;
    case 3:
      return &tm.Wday;
    case 4:
      return &tm.Hour;
    case 5:
    default:
      return &tm.Minute;
  }
}

void setupTimeModeOnBlink() {
  static_cast<SetupTimeMode*>(setupTimeMode)->onBlink();
}

callback SetupTimeMode::blinkCallback() {
  return setupTimeModeOnBlink;
}

const PROGMEM uint8_t cursorPosByPart[TIME_PARTS][2] = {
  {1, 0},
  {6, 0},
  {9, 0},
  {12, 0},
  {5, 1},
  {8, 1}
};

void SetupTimeMode::doBlink(bool showOrHide) {
  int16_t val = *getCurrentPart();
  switch (currentPart) {
    case 0:
      val += 1970;
      break;
    case 3:
      val = 1000;
      break;
  }

  lcd.setCursor(cursorPosByPart[currentPart][0], cursorPosByPart[currentPart][1]);

  if (!showOrHide) {
    lcd.print(val > 100 ? F("    ") : F("  "));
    return;
  }

  if (currentPart == 3) {
    lcd.print(getWeekDayName(tm.Wday));
    return;
  }

  if (val > 100) {
    lcd.print(val);
  } else {
    print2DigitsZero(val);
  }
}

void SetupTimeMode::enterState() {
  tm = *rtcNow();

  lcd.print(F("     -  -"));
  lcd.setCursor(7, 1);
  lcd.print(':');
  for (currentPart = 0; currentPart < TIME_PARTS; currentPart++) {
    doBlink(true);
  }
  currentPart = 0;

  SetupModeBase::enterState();
}

void SetupTimeMode::onModeKey() {
  SetupModeBase::onModeKey();

  tm.Second = 0;
  setRtc(tm);

  switchMode(timeMode);
}

const PROGMEM uint8_t monthDays[13] = {
  // 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12 
  0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

uint8_t SetupTimeMode::getCurrentPartMax() {
  if (currentPart != 2) {
    return timePartMaxs[currentPart];
  }

  uint8_t days = monthDays[tm.Month];
  if (tm.Month == 2 && (tm.Year + 1970) % 4 == 0) {
    days++;
  }
  return days;
}

void SetupTimeMode::onUpKey() {
  uint8_t *pVal = getCurrentPart();
  (*pVal) ++;
  if (*pVal > getCurrentPartMax()) {
    *pVal = timePartMins[currentPart];
  }

  updateForAdjust();
}

void SetupTimeMode::onDownKey() {
  uint8_t *pVal = getCurrentPart();
  // value can be zero, compare before (*pVal)-- 
  if (*pVal == timePartMins[currentPart]) {
    *pVal = getCurrentPartMax();
  } else {
    (*pVal)--;
  }

  updateForAdjust();
}

void SetupTimeMode::onSetupKey() {
  doBlink(true);
  currentPart = (currentPart + 1) % 6;
}

