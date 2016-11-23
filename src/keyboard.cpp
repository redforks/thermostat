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

  idKeyMode = keyIds[0];
  idKeyUp = keyIds[1];
  idKeyDown = keyIds[2];
  idKeySetup = keyIds[3];
}

void *keyRepeatIntervalHandler = NULL;

void repeatUpDownKeys() {
  if (digitalRead(KEY_DOWN_PIN) == HIGH) {
    store::setDigital(idKeyDown, LOW);
    store::setDigital(idKeyDown, HIGH);
  }

  if (digitalRead(KEY_UP_PIN) == HIGH) {
    store::setDigital(idKeyUp, LOW);
    store::setDigital(idKeyUp, HIGH);
  }
}

void checkKeys() {
  uint32_t now = millis();
  for (int i = 0; i < BUTTONS; i++) {
    uint8_t state = digitalRead(keyPins[i]);

    uint32_t delta = now - lastKeyChangeMillis[i];

    if (state == store::digitals[keyIds[i]]) {
      if (state == HIGH && (keyPins[i] == KEY_UP_PIN || keyPins[i] == KEY_DOWN_PIN) && delta > 800) {
        if (keyRepeatIntervalHandler == NULL) {
          keyRepeatIntervalHandler = clock::interval(200, repeatUpDownKeys);
        }
      }

      continue;
    }

    if (keyRepeatIntervalHandler != NULL) {
      clock::removeInterval(keyRepeatIntervalHandler);
      keyRepeatIntervalHandler = NULL;
    }

    if (delta < 300) {
      continue;
    }

    lastKeyChangeMillis[i] = now;
    store::setDigital(keyIds[i], state);

    Serial.print(F("Key "));
    Serial.print(i);
    Serial.println(state == HIGH ? F(" down") : F(" up"));
  }
}
