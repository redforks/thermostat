#include <core.h>
#include <LiquidCrystal.h>
#include "display.h"
#include "thermostat.h"
#include "read_temp_hum.h"

using namespace core;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void displayLoop() {
  int16_t tempe = readTempe();
  lcd.setCursor(0, 0);
  if (tempe < 0) {
    lcd.setCursor(0, 0);
    lcd.print('-');
    tempe = -tempe;
  } else {
    lcd.print(' '); // clear the possible negative '-'
  }
  lcd.print(tempe / 10);
  lcd.print('.');
  lcd.print(tempe % 10);
  lcd.print((char)223);
  lcd.print('C');

  int16_t humi = store::analogs[idHumi];
  lcd.setCursor(10, 0);
  lcd.print(humi / 10);
  lcd.print('.');
  lcd.print(humi % 10);
  lcd.print('%');

  lcd.setCursor(1, 1);
  lcd.print(TEMPORATURE_TARGET / 10);
  lcd.print('.');
  lcd.print(TEMPORATURE_TARGET % 10);
  lcd.print((char)223);
  lcd.print('C');
}

void updateHeaterStatus() {
  uint8_t req = store::digitals[idHeaterReq],
          act = store::digitals[idHeaterAct];

  lcd.setCursor(8, 1);
  if (req == act) {
    if (req) {
      lcd.print(F("     On "));
    } else {
      lcd.print(F("     Off"));
    }
  } else {
    if (req) {
      lcd.print(F("Will On "));
    } else {
      lcd.print(F("Will Off"));
    }
  }
}

void setupDisplay(void) {
  lcd.begin(16, 2);

  lcd.setCursor(0, 1);
  lcd.write('=');

  displayLoop();
  updateHeaterStatus();

  store::monitorAnalogs(&displayLoop, 2, idTempe, idHumi);
  store::monitorDigitals(&updateHeaterStatus, 2, idHeaterReq, idHeaterAct);
}

