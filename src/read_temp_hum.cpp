#include "read_temp_hum.h"
#include <dht.h>
#include "thermostat.h"
#include "tempe_control.h"

#define TEMPE_HISTORY 6  // How many temperature history value used for filter

using namespace core;

dht DHT;

// Return false if read failed.
bool doReadDHT22() {
  int chk = DHT.read22(DHT22_PIN);

  switch (chk) {
    case DHTLIB_OK:
      return true;
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

  return false;
}

idType idTempe, idHumi;

void setTempe(int16_t tempe) { store::setAnalog(idTempe, (uint16_t)tempe); }

void updateHumidity(uint16_t hum) { store::setAnalog(idHumi, hum); }

int16_t recentTempes[TEMPE_HISTORY];
uint8_t recentTempesIdx = 0;
int32_t tempeSum;
int16_t lastTempe;
int16_t lastLastTempe;

void updateTemperature(int16_t temp) {
  // if lastTempe equals current one, probably it is real value, skip delta
  // filter.
  if (temp != lastTempe || lastLastTempe != lastTempe) {
    lastLastTempe = lastTempe;
    lastTempe = temp;

    // Because dht22 actually has 0.2 precision, we only use the temperature
    // only if
    // dht reading has 0.2 changes.
    int16_t cur = readTempe();
    int16_t delta = abs(cur - temp);
    if (delta < 2) {
      return;
    }

    // it is unlikly temperature changes more than 2 degrees during
    // DHT22_SAMPLE_RATE unless an error
    if (delta > 20) {
      return;
    }
  }

  tempeSum -= recentTempes[recentTempesIdx];
  recentTempes[recentTempesIdx] = temp;
  tempeSum += temp;
  if (++recentTempesIdx == TEMPE_HISTORY) {
    recentTempesIdx = 0;
  }

  setTempe(tempeSum / TEMPE_HISTORY);
}

void readDHT22() {
  if (!doReadDHT22()) {
    return;
  }

  updateTemperature(DHT.temperature);
  updateHumidity(DHT.humidity);
}

void readTempeHumiFirstTime() {
  int16_t tempe = getTempeSetpoint();
  uint16_t humi = 500;
  if (doReadDHT22()) {
    tempe = DHT.temperature;
    humi = DHT.humidity;
  }

  for (int i = 0; i < TEMPE_HISTORY; i++) {
    recentTempes[i] = tempe;
  }
  lastTempe = lastLastTempe = tempe; 
  tempeSum = tempe * TEMPE_HISTORY;

  updateTemperature(tempe);
  updateHumidity(humi);

  clock::interval(DHT22_SAMPLE_RATE, readDHT22);
}

void setupThemeHumi(void) {
  idHumi = store::defineAnalog();
  idTempe = store::defineAnalog();

  setTempe(getTempeSetpoint());

  // At this time, no other modules hooks idTempe & idHumi,
  // delays inital read, to trigger interested modules.
  // DHT22 datasheet said, should not read DHT22 in first second after power up.
  clock::delay(1500, readTempeHumiFirstTime);
}
