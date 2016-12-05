#pragma once

#include "thermostat.h"

// Module display display current status in setupDisplay(),
// make sure call setupDisplay() after all other modules.
void setupDisplay(void);

// get current backlight, true is on, false is off.
bool getBacklight();

void setBacklight(bool onOrOff);
