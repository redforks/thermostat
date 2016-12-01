#include <Time.h>
#include <core.h>
#include "display_mode.h"
#include "rtc.h"
#include "read_temp_hum.h"

using namespace core;

void ShutdownMode::enterState() {
  lcd.print(F("     \337C       %"));
  lcd.setCursor(1, 1);
  lcd.print(F("Shutdown"));

  onTempeHumiChanges();
  onClock();
}

void ShutdownMode::onClock() {
  tmElementsPtr_t now = rtcNow();
  lcd.setCursor(10, 1);
  printHourMin(now->Hour, now->Minute);
}

void ShutdownMode::onTempeHumiChanges() {
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
