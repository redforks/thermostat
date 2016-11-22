#include <core.h>
#include "thermostat.h"
#include "keyboard.h"

#define BUTTONS 4
uint8_t keyPins[BUTTONS] = {KEY_MODE_PIN, KEY_UP_PIN, KEY_DOWN_PIN, KEY_SETUP_PIN};
uint32_t lastKeyChangeMillis[BUTTONS] = {0, 0, 0, 0};

using namespace core;
idType keyIds[BUTTONS];
idType idKeyMode, idKeyUp, idKeyDown, idKeySetup;

void setupKeyboard() {
  for (int i = 0; i < BUTTONS; i++) {
    pinMode(keyPins[i], INPUT);
    keyIds[i] = store::defineDigital();
  }

  idKeyMode = keyPins[0];
  idKeyUp = keyPins[1];
  idKeyDown = keyPins[2];
  idKeySetup = keyPins[3];
}

void checkKeys() {
  for (int i = 0; i < BUTTONS; i++) {
    uint8_t state = digitalRead(keyPins[i]);
    if (state == store::digitals[keyIds[i]]) {
      continue;
    }

    uint32_t now = millis();
    if (now - lastKeyChangeMillis[i] < 300) {
      continue;
    }

    lastKeyChangeMillis[i] = now;
    store::setDigital(keyIds[i], state);

    Serial.print("Key ");
    Serial.print(i);
    Serial.println(state == HIGH ? " down" : " up");
  }
}
