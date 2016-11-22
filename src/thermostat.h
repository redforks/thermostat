#pragma once

#include <core.h>

#define DHT22_PIN 6
#define DHT22_SAMPLE_RATE 3000

#define TEMPORATURE_TARGET (204)

#define HEATER_PIN 10
#define HEATER_ACTION_DELAY (15*60) // minimal seconds to open/close heater, prevent switch heater too frequently.

// pin connect to 315/433 transmitter
#define TRANSMIT_PIN 8

// Log temperature and humidity every 5mins
#define TEMPE_HUMI_LOG_INTERVAL ((uint32_t)5 * 60 * 1000)

#define DISPLAY_PIN1 5
#define DISPLAY_PIN2 4
#define DISPLAY_PIN3 3
#define DISPLAY_PIN4 2
#define DISPLAY_PIN5 11
#define DISPLAY_PIN6 12

extern core::idType idTempe, idHumi;

// Set digital value idHeaterReq to turn on/off, heater module
// has a throttle inside to prevent turn on/off too frequently,
// idHeaterAct digtial value is the heater actual state.
extern core::idType idHeaterReq, idHeaterAct;

#define KEY_MODE_PIN 7
#define KEY_UP_PIN 9
#define KEY_DOWN_PIN 10
#define KEY_SETUP_PIN 13

extern core::idType idKeyMode, idKeyUp, idKeyDown, idKeySetup;
