#include <Time.h>
#include <core.h>
#include "display_mode.h"
#include "rtc.h"

using namespace core;

void TimeMode::enterState() {
  onClock();
}

void TimeMode::onModeKey() {
  switchMode(normalMode);
}

void TimeMode::onSetupKey() {
  switchMode(setupTimeMode);
}

const __FlashStringHelper* getWeekDayName(uint8_t wday) {
  switch (wday) {
    case 1:
      return F("SUN");
    case 2:
      return F("MON");
    case 3:
      return F("TUE");
    case 4:
      return F("WES");
    case 5:
      return F("THU");
    case 6:
      return F("FRI");
    case 7:
      return F("SAT");
  }
  return F("  ");
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

uint8_t timePartMins[TIME_PARTS] = {2010-1970, 1, 1, 1, 0, 0};
uint8_t timePartMaxs[TIME_PARTS] = {2050-1970, 12, 0, 7, 23, 59};

uint8_t SetupTimeMode::getCurrentPartValue() {
  switch (currentPart) {
    case 0:
      return tm.Year;
    case 1:
      return tm.Month;
    case 2:
      return tm.Day;
    case 3:
      return tm.Wday;
    case 4:
      return tm.Hour;
    case 5:
      return tm.Minute;
  }
  return 0;
}

void SetupTimeMode::setCurrentPartValue(uint8_t val) {
  switch (currentPart) {
    case 0:
      tm.Year = val;
      break;
    case 1:
      tm.Month = val;
      break;
    case 2:
      tm.Day = val;
      break;
    case 3:
      tm.Wday = val;
      break;
    case 4:
      tm.Hour = val;
      break;
    case 5:
      tm.Minute = val;
      break;
  }
}

void setupTimeModeOnBlink() {
  static_cast<SetupTimeMode*>(setupTimeMode)->onBlink();
}

callback SetupTimeMode::blinkCallback() {
  return setupTimeModeOnBlink;
}

void SetupTimeMode::doBlink(bool showOrHide) {
  int8_t x, y;
  int16_t val;
  switch (currentPart) {
    case 0:
      val = 1970 + tm.Year, x = 1, y = 0;
      break;
    case 1:
      val = tm.Month, x = 6, y = 0;
      break;
    case 2:
      val = tm.Day, x = 9, y = 0;
      break;
    case 3:
      x = 12, y = 0;
      val = 1000;
      break;
    case 4:
      val = tm.Hour, x = 5, y = 1;
      break;
    case 5:
      val = tm.Minute, x = 8, y = 1;
      break;
  }

  lcd.setCursor(x, y);

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

uint8_t SetupTimeMode::getCurrentPartMax() {
  if (currentPart != 2) {
    return timePartMaxs[currentPart];
  }

  switch (tm.Month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      return 31;
    case 2:
      return ((tm.Year + 1970) % 4 == 0) ? 29 : 28;
    default:
      return 30;
  }
}

void SetupTimeMode::onUpKey() {
  uint8_t val = getCurrentPartValue() + 1;
  if (val > getCurrentPartMax()) {
    val = timePartMins[currentPart];
  }

  setCurrentPartValue(val);
  updateForAdjust();
}

void SetupTimeMode::onDownKey() {
  uint8_t val = getCurrentPartValue();
  if (val == timePartMins[currentPart]) {
    val = getCurrentPartMax();
  } else {
    val--;
  }

  setCurrentPartValue(val);
  updateForAdjust();
}

void SetupTimeMode::onSetupKey() {
  doBlink(true);
  currentPart = (currentPart + 1) % 6;
}

