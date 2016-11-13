#include "thermostat.h"
#include "display.h"
#include "read_temp_hum.h"
#include "heater.h"
#include "tempe_control.h"

using namespace core;
using namespace core::store;

void setup() {
  Serial.begin(115200);
  Serial.println(F("## Thermostat by Red Forks ##\n"));

  setupDisplay();
  setupThemeHumi();
  setupHeater();
  setupTempeControl();
}

void loop() {
  clock::check();
}
