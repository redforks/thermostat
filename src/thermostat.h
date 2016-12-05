#pragma once

#include <core.h>

#define DHT22_PIN 10
#define DHT22_SAMPLE_RATE 3000

#define DEFAULT_TEMPE_SETPOINT (204)
#define TEMPE_SETPOINT_HIGH_LOW_MAX 9
#define TEMPE_SETPOINT_HIGH_LOW_MIN 1
#define TEMPE_SETPOINT_MAX 350 // min temperature setpoint
#define TEMPE_SETPOINT_MIN 100 // max temperature setpoint
#define TEMPE_SHUTDOWN 50 // temperature setpoint if isShutdown()

#define TEMPE_SETPOINT_EEPROM_ADDRESS 0 // address of EEPROM to store temperature setpoint
#define TEMPE_SETPOINT_HYSTERESIS_EEPROM_ADDRESS 2 // address of EEPROM to store temperature setpoint hysteresis
#define LAST_DISPLAY_MODE_ADDRESS 3 // address of display mode, on startup, restore display mode of power off
// Address 3 not used by historic reason, 
#define TEMPE_SCHEDULE_HIGH_TEMPE_ADDRESS 4 // address of EEPROM to store schedule high setpoint
#define TEMPE_SCHEDULE_LOW_TEMPE_ADDRESS 6 // address of EEPROM to store schedule low setpoint
#define DAY_SCHEDULE_ADDRESS 8 // day schedule occupy 6 bytes
#define SHUTDOWN_STATE_ADDRESS 14
#define BRIGHTNESS_ADDRESS 15

#define HEATER_PIN 9
#define HEATER_ACTION_DELAY (15*60) // minimal seconds to open/close heater, prevent switch heater too frequently.

// pin connect to 315/433 transmitter
#define TRANSMIT_PIN 8

// Log temperature and humidity every 5mins
#define TEMPE_HUMI_LOG_INTERVAL ((uint32_t)5 * 60 * 1000)

extern core::idType idTempe, idHumi, idTempeSetpoint;

// Set digital value idHeaterReq to turn on/off, heater module
// has a throttle inside to prevent turn on/off too frequently,
// idHeaterAct digtial value is the heater actual state.
extern core::idType idHeaterReq, idHeaterAct;

#define KEY_MODE_PIN 5
#define KEY_UP_PIN 4
#define KEY_DOWN_PIN 3
#define KEY_SETUP_PIN 2

extern core::idType idKeyMode, idKeyUp, idKeyDown, idKeySetup;
