#include "thermostat.h"
#include <dht.h>
#include "display.h"

dht DHT;

#define DHT22_PIN 6
#define HEATER_PIN 13

#define DHT22_SAMPLE_RATE 3000

#define HEATER_ACTION_DELAY (15*60) // minimal seconds to open/close heater, prevent switch heater too frequently.

using namespace core;
using namespace core::store;

idType idTempe, idHumi;
idType idHeater;

// To ensure temperature really changed, wait for last three readings are identical.
int16_t lastTemperature = TEMPORATURE_TARGET;
int16_t lastLastTemperature = TEMPORATURE_TARGET;

void setTempe(int16_t tempe) {
  setAnalog(idTempe, (uint16_t)tempe);
}

int16_t readTempe() {
  return (int16_t)analogs[idTempe];
}

void updateTemperature(int16_t temp) {
  int16_t last = lastTemperature;
  int16_t lastLast = lastLastTemperature;
  lastLastTemperature = lastTemperature;
  lastTemperature = temp;
  // repeated reading the same temperature probable is the real value.
  if (temp == last && last == lastLast) {
    setTempe(temp);
    return;
  }

  // Because dht22 actually has 0.2 precision, we only use the temperature only if 
  // dht reading has 0.2 changes.
  int16_t cur = readTempe();
  int16_t delta = abs(cur - temp);
  if (delta < 2) {
    return;
  }

  // it is unlikly temperature changes more than 2 degrees during DHT22_SAMPLE_RATE unless an error
  if (delta > 20) {
    return;
  }

  setTempe(temp);
}

void updateHumidity(uint16_t hum) {
  setAnalog(idHumi, hum);
}

void readDHT22() {
  delay(2); // TODO: it seems DHT not stable without this delay
  int chk = DHT.read22(DHT22_PIN);

  switch (chk)
  {
    case DHTLIB_OK:  
      updateTemperature(DHT.temperature);
      updateHumidity(DHT.humidity);
      Serial.print(readTempe()); 
      Serial.print(F(", ")); 
      Serial.print(DHT.temperature); 
      Serial.print(F(", ")); 
      Serial.println(DHT.humidity); 
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.print(F("Checksum error,\n")); 
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.print(F("Time out error,\n")); 
      break;
    default: 
      Serial.print(F("Unknown error,\n")); 
      break;
  }
}

// seconds since last change heater state
uint32_t lastHeaterActionSeconds = -(HEATER_ACTION_DELAY - 60); // ensure open heater in one minute after thermostat power up.  

void* currentSwitchHeaterDelay = NULL;

void doSwitchHeater() {
  Serial.print(F("Set heater: "));
  Serial.println(digitals[idHeater] == HIGH);
  currentSwitchHeaterDelay = NULL;
  lastHeaterActionSeconds = millis() / 1000;
  digitalWrite(HEATER_PIN, digitals[idHeater]);
}

void switchHeater() {
  if (currentSwitchHeaterDelay != NULL) {
    clock::removeDelay(currentSwitchHeaterDelay);
    currentSwitchHeaterDelay = NULL;
  }

  uint32_t current = millis() / 1000;
  uint32_t secondsSinceLastChange = current - lastHeaterActionSeconds;

  if (HEATER_ACTION_DELAY - secondsSinceLastChange > 0) {
    currentSwitchHeaterDelay = clock::delay(
      (HEATER_ACTION_DELAY - secondsSinceLastChange) * 1000,
      &doSwitchHeater);

    Serial.print(F("Ignore heater action, not reaching minimal heater action delay. "));
    Serial.println(secondsSinceLastChange);
    return;
  }

  doSwitchHeater();
}

void temperatureLoop() {
  int16_t tempe = readTempe();
  if (tempe > (TEMPORATURE_TARGET + 2)) {
    setDigital(idHeater, LOW);
  } else if (tempe < (TEMPORATURE_TARGET - 3)) {
    setDigital(idHeater, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("## Thermostat by Red Forks ##\n"));
  setupDisplay();

  pinMode(HEATER_PIN, OUTPUT);

  idHumi = defineAnalog();
  idTempe = defineAnalog();

  idHeater = defineDigital();
  
  setTempe(TEMPORATURE_TARGET);

  monitorAnalogs(temperatureLoop, 1, idTempe);
  monitorDigitals(switchHeater, 1, idHeater);
   
  clock::interval(DHT22_SAMPLE_RATE, &readDHT22);
  readDHT22(); 
}

void loop() {
  clock::check();
}
