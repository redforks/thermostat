#include <Time.h>
#include <core.h>
#include "display_mode.h"
#include "read_temp_hum.h"
#include "rtc.h"

using namespace core;

NormalMode _normalMode;
SetupNormalMode _setupNormalMode;
TimeMode _timeMode;
SetupTimeMode _setupTimeMode;

DisplayMode *const normalMode = &_normalMode;
DisplayMode *const setupNormalMode = &_setupNormalMode;
DisplayMode *const timeMode = &_timeMode;
DisplayMode *const setupTimeMode = &_setupTimeMode;

void DisplayMode::enterState() {
}

void DisplayMode::onModeKey() {
}

void DisplayMode::onUpKey() {
}

void DisplayMode::onDownKey() {
}

void DisplayMode::onSetupKey() {
}

void DisplayMode::onTempeHumiChanges() {
}

void DisplayMode::onClock() {
}

void DisplayMode::onHeaterChanges() {
}

void NormalMode::enterState() {
  lcd.setCursor(5, 0);
  lcd.print(F("\337C       %"));
  lcd.setCursor(0, 1);
  lcd.print(F("=20.4\337C"));

  onTempeHumiChanges();
  onHeaterChanges();
  onClock();
}

void NormalMode::onModeKey() {
  switchMode(timeMode);
}

void NormalMode::onSetupKey() {
}

void print2Digits(uint16_t number, char prefix) {
  if (number < 10) {
    lcd.print(prefix);
  }
  lcd.print(number);
}

// print number with 2 decimal digits, prefix with ' ' if less than 10.
// Assume number >= 0, number < 100.
void print2DigitsSpace(uint16_t number) {
  print2Digits(number, ' ');
}

// print number with 2 decimal digits, prefix with 0 if less than 10.
// Assume number >= 0, number < 100.
void print2DigitsZero(int number) {
  print2Digits(number, '0');
}

// print number like 12.5, store as 125.
void printNumber00n0(uint16_t number) {
  print2DigitsSpace(number / 10);
  lcd.print('.');
  lcd.print(number % 10);
}

void NormalMode::onTempeHumiChanges() {
  int16_t tempe = readTempe();
  lcd.setCursor(0, 0);
  if (tempe < 0) {
    lcd.setCursor(0, 0);
    lcd.print('-');
    tempe = -tempe;
  } else {
    lcd.print(' '); // clear the possible negative '-'
  }
  printNumber00n0(tempe);

  uint16_t humi = store::analogs[idHumi];
  lcd.setCursor(10, 0);
  printNumber00n0(humi);
}

void NormalMode::onHeaterChanges() {
  uint8_t req = store::digitals[idHeaterReq],
          act = store::digitals[idHeaterAct];

  if (req == act) {
    lcd.setCursor(14, 1);
    if (req) {
      lcd.print(F("On"));
    } else {
      lcd.print(F("  "));
    }
  } else {
    lcd.setCursor(8, 1);
    if (req) {
      lcd.print(F("Will On "));
    } else {
      lcd.print(F("Will Off"));
    }
  }
}

void NormalMode::onClock() {
  tmElementsPtr_t now = rtcNow();
  uint8_t req = store::digitals[idHeaterReq],
          act = store::digitals[idHeaterAct];

  if (req == act) {
    lcd.setCursor(8, 1);

    print2DigitsZero(now->Hour);
    lcd.print(':');
    print2DigitsZero(now->Minute);
  }
}
