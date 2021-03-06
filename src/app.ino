#include "thermostat.h"
#include "display.h"
#include "read_temp_hum.h"
#include "heater.h"
#include "tempe_control.h"
#include "log.h"
#include "keyboard.h"
#include "rtc.h"
#include "shutdown.h"

using namespace core;
using namespace core::store;

void setup() {
  Serial.begin(115200);
  Serial.println(F("## Thermostat by Red Forks ##\n"));

  setupRTC();
  setupKeyboard();
  setupThemeHumi();
  setupHeater();
  setupTempeControl();
  setupDisplay();
  setupLog();
  setupShutdown();
}

void loop() {
  clock::check();
  checkKeys();
}
