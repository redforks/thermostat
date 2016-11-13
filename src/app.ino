#include "thermostat.h"
#include "display.h"
#include "read_temp_hum.h"

#define HEATER_PIN 13

#define HEATER_ACTION_DELAY (15*60) // minimal seconds to open/close heater, prevent switch heater too frequently.

using namespace core;
using namespace core::store;

idType idHeater;

// seconds since last change heater state
uint32_t lastHeaterActionSeconds = -(HEATER_ACTION_DELAY - 60); // ensure open heater in one minute after thermostat power up.  

void* currentSwitchHeaterDelay = NULL;

void doSwitchHeater() {
  Serial.print(F("Set heater: "));
  Serial.println(digitals[idHeater] == HIGH);
  currentSwitchHeaterDelay = NULL;
  lastHeaterActionSeconds = millis() / 1000;
  digitalWrite(HEATER_PIN, digitals[idHeater]);
}

void switchHeater() {
  if (currentSwitchHeaterDelay != NULL) {
    clock::removeDelay(currentSwitchHeaterDelay);
    currentSwitchHeaterDelay = NULL;
  }

  uint32_t current = millis() / 1000;
  uint32_t secondsSinceLastChange = current - lastHeaterActionSeconds;

  if (HEATER_ACTION_DELAY - secondsSinceLastChange > 0) {
    currentSwitchHeaterDelay = clock::delay(
      (HEATER_ACTION_DELAY - secondsSinceLastChange) * 1000,
      &doSwitchHeater);

    Serial.print(F("Ignore heater action, not reaching minimal heater action delay. "));
    Serial.println(secondsSinceLastChange);
    return;
  }

  doSwitchHeater();
}

void temperatureLoop() {
  int16_t tempe = readTempe();
  if (tempe > (TEMPORATURE_TARGET + 2)) {
    setDigital(idHeater, LOW);
  } else if (tempe < (TEMPORATURE_TARGET - 3)) {
    setDigital(idHeater, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("## Thermostat by Red Forks ##\n"));
  setupDisplay();
  setupThemeHumi();

  pinMode(HEATER_PIN, OUTPUT);

  idHeater = defineDigital();
  
  monitorAnalogs(temperatureLoop, 1, idTempe);
  monitorDigitals(switchHeater, 1, idHeater);
}

void loop() {
  clock::check();
}
