#pragma once

#include <EEPROM.h>

// Init value stored in EEPROM.
// If value in EEPROM not in range [low, high], store defVal to EEPROM, return defVal.
// If value in range [low, high], returns.
template<typename T> T initEEPROMValue(int16_t address, T low, T high, T defVal) {
  T val;
  EEPROM.get(address, val);
  if (val < low || val > high) {
    EEPROM.put(address, defVal);
    return defVal;
  }
  return val;
}
