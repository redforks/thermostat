#include <Time.h>
#include "display_mode.h"
#include "rtc.h"

void TimeMode::enterState() {
  onClock();
}

void TimeMode::onModeKey() {
  switchMode(normalMode);
}

void TimeMode::onSetupKey() {
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
