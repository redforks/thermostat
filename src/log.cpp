#include <core.h>
#include <RCSwitch.h>
#include "log.h"
#include "thermostat.h"
#include "heater.h"
#include "read_temp_hum.h"

using namespace core;

RCSwitch mySwitch = RCSwitch();

void sendTempe() {
  // a value composed by 3 bit value type, 5 bit value id, 16 bit value.
  // so 7 types(type 0 reserved), 32 max value for each type.
  uint16_t tempe = store::analogs[idTempe];
  // 0x21, type is 01, signed int transfered as unsigned int, value id is 01.
  uint32_t token = 0x210000 + tempe;
  // send as 32bit, receiver will drop first 8bit, for lower transfer error.
  mySwitch.send(token, 32);
}

void sendHumi() {
  // a value composed by 3 bit value type, 5 bit value id, 16 bit value.
  // so 7 types(type 0 reserved), 32 max value for each type.
  uint16_t humi = store::analogs[idHumi];
  // 0x41, type is 02, unsigned int, value id is 01.
  uint32_t token = 0x410000 + humi;
  // send as 32bit, receiver will drop first 8bit, for lower transfer error.
  mySwitch.send(token, 32);
}

void sendHeater() {
  uint8_t isOpen = store::digitals[idHeaterAct];
  // 0x61, type is 03, bool, id is 01.
  uint32_t token = 0x610000 + (isOpen ? 1 : 0);
  // send as 32bit, receiver will drop first 8bit, for lower transfer error.
  mySwitch.send(token, 32);
}

void sendTempeHumi() {
  sendTempe();
  sendHumi();
}

void setupLog() {
  mySwitch.enableTransmit(TRANSMIT_PIN);
  mySwitch.setRepeatTransmit(10);

  store::monitorDigitals(&sendHeater, 1, idHeaterAct);
  // wait 1 seconds, to send temperature and humidity for the first time,
  // read_temp_hum module delays 1 millisecond before doing first read.
  clock::delay(1 * 1000, &sendTempeHumi);
  clock::interval(TEMPE_HUMI_LOG_INTERVAL, &sendTempeHumi);
}
