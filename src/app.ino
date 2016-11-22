#include "thermostat.h"
#include "display.h"
#include "read_temp_hum.h"
#include "heater.h"
#include "tempe_control.h"
#include "log.h"

using namespace core;
using namespace core::store;

void setup() {
  Serial.begin(115200);
  Serial.println(F("## Thermostat by Red Forks ##\n"));

  setupThemeHumi();
  setupHeater();
  setupTempeControl();
  setupDisplay();
  setupLog();
}

void loop() {
  clock::check();
}
