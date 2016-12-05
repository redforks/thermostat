#include <core.h>
#include <SPI.h>
#include <RF24.h>
#include "log.h"
#include "thermostat.h"
#include "heater.h"
#include "read_temp_hum.h"

using namespace core;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);

void sendTempe() {
  // a value composed by 3 bit value type, 5 bit value id, 16 bit value.
  // so 7 types(type 0 reserved), 32 max value for each type.
  uint16_t tempe = store::analogs[idTempe];
  // 0x21, type is 01, signed int transfered as unsigned int, value id is 01.
  uint32_t token = 0x210000 + tempe;
  // send as 32bit, receiver will drop first 8bit, for lower transfer error.
  Serial.println(F("Sending tempe"));
  delay(1);
  if (!radio.write(&token, sizeof(token))) {
    Serial.println(F("Send tempe failed"));
  }
}

void sendHumi() {
  // a value composed by 3 bit value type, 5 bit value id, 16 bit value.
  // so 7 types(type 0 reserved), 32 max value for each type.
  uint16_t humi = store::analogs[idHumi];
  // 0x41, type is 02, unsigned int, value id is 01.
  uint32_t token = 0x410000 + humi;

  // send as 32bit, receiver will drop first 8bit, for lower transfer error.
  Serial.println(F("Sending humi"));
  if (!radio.write(&token, sizeof(token))) {
    Serial.println(F("Send humi failed"));
  }
}

void sendHeater() {
  uint8_t isOpen = store::digitals[idHeaterAct];
  // 0x61, type is 03, bool, id is 01.
  uint32_t token = 0x610000 + (isOpen ? 1 : 0);

  // send as 32bit, receiver will drop first 8bit, for lower transfer error.
  Serial.println(F("Sending heater switch"));
  if (!radio.write(&token, sizeof(token))) {
    Serial.println(F("Send heater failed"));
  }
}

void sendTempeHumi() {
  sendTempe();
  sendHumi();
}

unsigned char rf24WriteAddr[6] = "THER1";

void setupLog() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(4);
  radio.setChannel(46);

  Serial.println(F("before openWritingPipe"));
  radio.openWritingPipe(rf24WriteAddr);
  Serial.println(F("RF24 inited"));

  store::monitorDigitals(sendHeater, 1, idHeaterAct);
  // wait 1 seconds, to send temperature and humidity for the first time,
  // read_temp_hum module delays 1 millisecond before doing first read.
  clock::delay(1 * 1000, sendTempeHumi);
  clock::interval(TEMPE_HUMI_LOG_INTERVAL, sendTempeHumi);
}
