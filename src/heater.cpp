#include <core.h>
#include "heater.h"
#include "thermostat.h"

using namespace core;

idType idHeaterReq, idHeaterAct;

// seconds since last change heater state
uint32_t lastHeaterActionSeconds = -(HEATER_ACTION_DELAY - 60); // ensure open heater in one minute after thermostat power up.  

void* currentSwitchHeaterDelay = NULL;

void doSwitchHeater() {
  currentSwitchHeaterDelay = NULL;
  lastHeaterActionSeconds = millis() / 1000;

  uint8_t req = store::digitals[idHeaterReq];
  digitalWrite(HEATER_PIN, req);
  store::setDigital(idHeaterAct, req);
}

void switchHeater() {
  if (currentSwitchHeaterDelay != NULL) {
    clock::removeDelay(currentSwitchHeaterDelay);
    currentSwitchHeaterDelay = NULL;
  }

  uint32_t current = millis() / 1000;
  uint32_t secondsSinceLastChange = current - lastHeaterActionSeconds;

  if (compareULong(HEATER_ACTION_DELAY, secondsSinceLastChange, HEATER_ACTION_DELAY) > 0) {
    currentSwitchHeaterDelay = clock::delay(
      (HEATER_ACTION_DELAY - secondsSinceLastChange) * 1000,
      &doSwitchHeater);

    Serial.print(F("Ignore heater action, not reaching minimal heater action delay. "));
    Serial.println(secondsSinceLastChange);
    return;
  }

  doSwitchHeater();
}

void setupHeater(void) {
  pinMode(HEATER_PIN, OUTPUT);

  idHeaterReq = store::defineDigital();
  idHeaterAct = store::defineDigital();
  store::monitorDigitals(switchHeater, 1, idHeaterReq);
}
