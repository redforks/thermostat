#pragma once

#define DHT22_PIN 6
#define DHT22_SAMPLE_RATE 3000

#define TEMPORATURE_TARGET (204)

#define HEATER_PIN 10
#define HEATER_ACTION_DELAY (15*60) // minimal seconds to open/close heater, prevent switch heater too frequently.

// pin connect to 315/433 transmitter
#define TRANSMIT_PIN 8

// Log temperature and humidity every 5mins
#define TEMPE_HUMI_LOG_INTERVAL ((uint32_t)5 * 60 * 1000)
