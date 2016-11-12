#include <core.h>
#include <dht.h>

dht DHT;

#define DHT22_PIN 5
#define HEATER_PIN 12

#define DHT22_SAMPLE_RATE 20000

#define TEMPORATURE_TARGET (204)

#define HEATER_ACTION_DELAY (15*60) // minimal seconds to open/close heater, prevent switch heater too frequently.

using namespace core;
using namespace core::store;

idType idTempe, idHumi;
idType idHeater;

// To ensure temperature really changed, wait for last three readings are identical.
short lastTemperature = TEMPORATURE_TARGET;
short lastLastTemperature = TEMPORATURE_TARGET;

void updateTemperature(short temp) {
  short last = lastTemperature;
  short lastLast = lastLastTemperature;
  lastLastTemperature = lastTemperature;
  lastTemperature = temp;
  // repeated reading the same temperature probable is the real value.
  if (temp == last && last == lastLast) {
    setAnalog(idTempe, (short)temp);
    return;
  }

  // Because dht22 actually has 0.2 precision, we only use the temperature only if 
  // dht reading has 0.2 changes.
  short delta = abs((short)analogs[idTempe] - temp);
  if (delta < 2) {
    return;
  }

  // it is unlikly temperature changes more than 2 degrees during DHT22_SAMPLE_RATE unless an error
  if (delta > 20) {
    return;
  }

  setAnalog(idTempe, (short)temp);
}

void updateHumidity(word hum) {
  setAnalog(idHumi, hum);
}

void readDHT22() {
  int chk = DHT.read22(DHT22_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
      updateTemperature(DHT.temperature);
      updateHumidity(DHT.humidity);
      Serial.print(analogs[idTempe]); 
      Serial.print(", "); 
      Serial.print(DHT.temperature); 
      Serial.print(F(", ")); 
      Serial.println(DHT.humidity); 
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.print("Checksum error,\n"); 
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.print("Time out error,\n"); 
      break;
    default: 
      Serial.print("Unknown error,\n"); 
      break;
  }
}

// seconds since last change heater state
unsigned long lastHeaterActionSeconds = -(HEATER_ACTION_DELAY - 60); // ensure open heater in one minute after thermostat power up.  

void* currentSwitchHeaterDelay = NULL;

void doSwitchHeater() {
  Serial.print(F("Set heater: "));
  Serial.println(digitals[idHeater] == HIGH);
  currentSwitchHeaterDelay = NULL;
  lastHeaterActionSeconds = millis() / 1000;
  digitalWrite(HEATER_PIN, digitals[idHeater]);
}

void switchHeater() {
  Serial.println("switch heater");
  if (currentSwitchHeaterDelay != NULL) {
    clock::removeDelay(currentSwitchHeaterDelay);
    currentSwitchHeaterDelay = NULL;
  }

  unsigned long current = millis() / 1000;
  unsigned long secondsSinceLastChange = current - lastHeaterActionSeconds;

  if (HEATER_ACTION_DELAY - secondsSinceLastChange > 0) {
    currentSwitchHeaterDelay = clock::delay(
      (HEATER_ACTION_DELAY - secondsSinceLastChange) * 1000,
      &doSwitchHeater);

    Serial.print("Ignore heater action, not reaching minimal heater action delay. ");
    Serial.println(secondsSinceLastChange);
    return;
  }

  doSwitchHeater();
}

void temperatureLoop() {
  Serial.println("temperatureLoop");
  short tempe = (short)analogs[idTempe];
  if (tempe > (TEMPORATURE_TARGET + 0.2)) {
    setDigital(idHeater, LOW);
  } else if (tempe < (TEMPORATURE_TARGET - 0.3)) {
    setDigital(idHeater, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("## Thermostat by Red Forks ##\n"));
  pinMode(HEATER_PIN, OUTPUT);

  idTempe = defineAnalog();
  idHumi = defineAnalog();

  idHeater = defineDigital();
  analogs[idHeater] = TEMPORATURE_TARGET;
  
  monitorAnalogs(temperatureLoop, 1, idTempe);
  monitorDigitals(switchHeater, 1, idHeater);
   
  clock::interval(DHT22_SAMPLE_RATE, &readDHT22);
  delay(2000);
  readDHT22(); 
}

void loop() {
  clock::check();
}
