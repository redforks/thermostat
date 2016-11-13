#pragma once

#include <core.h>

// Set digital value idHeaterReq to turn on/off, heater module
// has a throttle inside to prevent turn on/off too frequently,
// idHeaterAct digtial value is the heater actual state.
extern core::idType idHeaterReq, idHeaterAct;

void setupHeater(void);
