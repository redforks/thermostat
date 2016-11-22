#pragma once

#include <core.h>
#include "thermostat.h"

void setupThemeHumi(void);

inline int16_t readTempe() {
  return (int16_t)core::store::analogs[idTempe];
}

