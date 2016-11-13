#pragma once

#include <core.h>

extern core::idType idTempe, idHumi;

void setupThemeHumi(void);

inline int16_t readTempe() {
  return (int16_t)core::store::analogs[idTempe];
}

