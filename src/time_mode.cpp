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

  lcd.setCursor(3, 1);
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

void SetupTimeMode::doBlink(int16_t val, int8_t x, int8_t y) {
  lcd.setCursor(x, y);
  if (blinkOn) {
    if (val > 100) {
      lcd.print(val);
    } else {
      print2DigitsZero(val);
    }
  } else {
    lcd.print(val > 100 ? F("    ") : F("  "));
  }
}

void SetupTimeMode::blinkWeekDay() {
  lcd.setCursor(12, 0);
  if (blinkOn) {
    lcd.print(getWeekDayName(tm.Wday));
  } else {
    lcd.print(F("   "));
  }
}

void SetupTimeMode::onBlink() {
  if (currentPart == 3) {
    blinkWeekDay();
  }

  switch (currentPart) {
    case 0:
      doBlink(1970 + tm.Year, 1, 0);
      break;
    case 1:
      doBlink(tm.Month, 6, 0);
      break;
    case 2:
      doBlink(tm.Day, 9, 0);
      break;
    case 4:
      doBlink(tm.Hour, 5, 1);
      break;
    case 5:
      doBlink(tm.Minute, 8, 1);
      break;
  }
  blinkOn = !blinkOn;
}

void setupTimeModeOnBlink() {
  static_cast<SetupTimeMode*>(setupTimeMode)->onBlink();
}

void SetupTimeMode::enterState() {
  intervalHandler = clock::interval(500, setupTimeModeOnBlink);
  currentPart = 0;

  tm = *rtcNow();

  lcd.setCursor(1, 0);
  lcd.print(1970 + tm.Year);
  lcd.print('-');
  print2DigitsZero(tm.Month);
  lcd.print('-');
  print2DigitsZero(tm.Day);
  lcd.print(' ');
  lcd.print(getWeekDayName(tm.Wday));

  lcd.setCursor(5, 1);
  print2DigitsZero(tm.Hour);
  lcd.print(':');
  print2DigitsZero(tm.Minute);
}

void SetupTimeMode::onModeKey() {
  tm.Second = 0;
  setRtc(tm);

  clock::removeInterval(intervalHandler);
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
}

void SetupTimeMode::onDownKey() {
  uint8_t val = getCurrentPartValue();
  if (val == timePartMins[currentPart]) {
    val = getCurrentPartMax();
  } else {
    val--;
  }
  setCurrentPartValue(val);
}

void SetupTimeMode::onSetupKey() {
  bool saved = blinkOn;
  blinkOn = true;
  onBlink();
  blinkOn = saved;
  currentPart = (currentPart + 1) % 6;
}

