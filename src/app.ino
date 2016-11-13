#include "thermostat.h"
#include "display.h"
#include "read_temp_hum.h"
#include "heater.h"

using namespace core;
using namespace core::store;
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
  setupHeater();

  monitorAnalogs(temperatureLoop, 1, idTempe);
}

void loop() {
  clock::check();
}
