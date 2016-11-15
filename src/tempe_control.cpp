#include <core.h>
#include "tempe_control.h"
#include "thermostat.h"
#include "read_temp_hum.h"
#include "heater.h"

using namespace core;

void temperatureLoop() {
  int16_t tempe = readTempe();
  if (tempe >= (TEMPORATURE_TARGET + 2)) {
    store::setDigital(idHeaterReq, LOW);
  } else if (tempe <= (TEMPORATURE_TARGET - 3)) {
    store::setDigital(idHeaterReq, HIGH);
  }
}

void setupTempeControl(void) {
  store::monitorAnalogs(temperatureLoop, 1, idTempe);
}
