#include <Time.h>
#include <EEPROM.h>
#include <core.h>
#include "display_mode.h"
#include "read_temp_hum.h"
#include "tempe_control.h"
#include "rtc.h"

using namespace core;

NormalMode _normalMode;
SetupSetpointMode _setupSetpointMode;
SetupTempeHysterMode _setupTempeHysterMode;
TimeMode _timeMode;
SetupTimeMode _setupTimeMode;
DayScheduleMode _dayScheduleMode;
SetupMenuMode _setupMenuMode;
SetupScheduleHighLowMode _setupScheduleHighLowMode;
SetupDayScheduleMode _setupDayScheduleMode;
ShutdownMode _shutdownMode;
SetupShutdownMode _setupShutdownMode;
SetupBacklightMode _setupBacklightMode;

NormalMode *const normalMode = &_normalMode;
SetupSetpointMode *const setupSetpointMode = &_setupSetpointMode;
SetupTempeHysterMode *const setupTempeHysterMode = &_setupTempeHysterMode;
TimeMode *const timeMode = &_timeMode;
SetupTimeMode *const setupTimeMode = &_setupTimeMode;
DayScheduleMode *const dayScheduleMode = &_dayScheduleMode;
SetupMenuMode *const setupMenuMode = &_setupMenuMode;
SetupScheduleHighLowMode *const setupScheduleHighLowMode = &_setupScheduleHighLowMode;
SetupDayScheduleMode *const setupDayScheduleMode = &_setupDayScheduleMode;
ShutdownMode *const shutdownMode = &_shutdownMode;
SetupShutdownMode *const setupShutdownMode = &_setupShutdownMode;
SetupBacklightMode *const setupBacklightMode = &_setupBacklightMode;

void DisplayMode::enterState() {
}

void DisplayMode::onModeKey() {
}

void DisplayMode::onUpKey() {
}

void DisplayMode::onDownKey() {
}

void DisplayMode::onSetupKey() {
  switchMode(setupMenuMode);
}

void DisplayMode::onTempeHumiChanges() {
}

void DisplayMode::onTempeSetpointChanges() {
}

void DisplayMode::onClock() {
}

void DisplayMode::onHeaterChanges() {
}

void NormalMode::enterState() {
  lcd.setCursor(5, 0);
  lcd.print(F("\337C       %"));
  lcd.setCursor(0, 1);
  lcd.print(F("=    \337C"));

  onTempeHumiChanges();
  onHeaterChanges();
  onClock();
  onTempeSetpointChanges();
  setTempeSetpoint(getTempeEEPOMSetpoint(), true);
}

void NormalMode::onModeKey() {
  switchMode(timeMode);
}

void print2Digits(uint16_t number, char prefix) {
  if (number < 10) {
    lcd.print(prefix);
  }
  lcd.print(number);
}

void print2DigitsSpace(uint16_t number) {
  print2Digits(number, ' ');
}

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

void NormalMode::onTempeSetpointChanges() {
  lcd.setCursor(1, 1);
  printNumber00n0(getTempeSetpoint());
}

void NormalMode::onHeaterChanges() {
  uint8_t req = store::digitals[idHeaterReq],
          act = store::digitals[idHeaterAct];

  if (req == act) {
    lcd.setCursor(13, 1);
    if (req) {
      // space before "On" to wipe "O" of "Will On "
      lcd.print(F(" On"));
    } else {
      lcd.print(F("   "));
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

    printHourMin(now->Hour, now->Minute);
  }
}

bool Schedule::get(uint8_t section) const {
  uint8_t arr_idx = section / 8;
  uint8_t bit_idx = section % 8;

  uint8_t byte = bits[arr_idx];
  return (byte & (0x1 << bit_idx)) != 0;
}

void Schedule::toggle(uint8_t section) {
  uint8_t arr_idx = section / 8;
  uint8_t bit_idx = section % 8;

  bits[arr_idx] ^= 1 << bit_idx;
}

void Schedule::load(uint16_t address) {
  EEPROM.get(address, bits);
}

void Schedule::save(uint16_t address) const {
  EEPROM.put(address, bits);
}

void Schedule::assignFrom(const Schedule& from) {
  memcpy(bits, from.bits, sizeof(bits));
}

void printHourMin(uint8_t hour, uint8_t min) {
  print2DigitsZero(hour);
  lcd.print(':');
  print2DigitsZero(min);
}
