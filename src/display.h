#pragma once

#include "thermostat.h"

// Module display display current status in setupDisplay(),
// make sure call setupDisplay() after all other modules.
void setupDisplay(void);

// get current brightness, 0 is darkest, 255 is brightest.
uint8_t getBrightness();

void setBrightness(uint8_t val);
