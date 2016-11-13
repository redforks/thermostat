#include <dht.h>
#include "thermostat.h"
#include "read_temp_hum.h"

using namespace core;

dht DHT;

idType idTempe, idHumi;

void setTempe(int16_t tempe) {
  store::setAnalog(idTempe, (uint16_t)tempe);
}

void updateHumidity(uint16_t hum) {
  store::setAnalog(idHumi, hum);
}

void updateTemperature(int16_t temp) {
  // To ensure temperature really changed, wait for last three readings are
  // identical.
  static int16_t lastTemperature = TEMPORATURE_TARGET;
  static int16_t lastLastTemperature = TEMPORATURE_TARGET;

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

void setupThemeHumi(void) {
  idHumi = store::defineAnalog();
  idTempe = store::defineAnalog();

  setTempe(TEMPORATURE_TARGET);

  clock::interval(DHT22_SAMPLE_RATE, &readDHT22);
  readDHT22(); 
}
