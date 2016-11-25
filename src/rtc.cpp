#include <Wire.h>
#include <DS1307RTC.h>
#include <core.h>
#include "thermostat.h"
#include "rtc.h"

tmElements_t _rtcNow;

tmElementsPtr_t rtcNow() {
  if (!RTC.read(_rtcNow)) {
    Serial.println(F("Read RTC failed!"));
    Serial.println();
  }

  return &_rtcNow;
}

void setRtc(tmElements_t &now) {
  if (!RTC.write(now)) {
    Serial.println(F("Set DS1307 time failed"));
    Serial.println();
  }

  Serial.print(F("Set RTC to "));
  Serial.print(1970 + now.Year);
  Serial.print('-');
  Serial.print(now.Month);
  Serial.print('-');
  Serial.print(now.Day);
  Serial.print(' ');
  Serial.print(now.Hour);
  Serial.print(' ');
  Serial.print(now.Minute);
  Serial.print(' ');
  Serial.print(now.Second);
  Serial.println();
}

void setupRTC() {
  _rtcNow.Year = 2016 - 1970;
  _rtcNow.Month = 11;
  _rtcNow.Day = 22;
  _rtcNow.Hour = 15;
  _rtcNow.Minute = 50;
  _rtcNow.Second = 0;
  _rtcNow.Wday = 3;

  if (!RTC.read(_rtcNow)) {
    if (RTC.chipPresent()) {
      // RTC maybe not inited
      Serial.println(F("DS1307 not inited, set time to 2016-11-25 15:50:00"));
      setRtc(_rtcNow);

      return;
    }

    Serial.println(F("DS1307 read error!  Please check the circuitry."));
    Serial.println();
  }
}

struct Node {
  alarmFunc fn;
  tmElements_t when;
  Node *next;

  Node(tmElements_t aWhen, alarmFunc aFn) : fn(aFn), when(aWhen), next(NULL) {}
};

struct Queue {
  Node *head;

  void add(Node *);
  void remove(Node *);

  Queue() : head(NULL) {}
};

void Queue::add(Node *node) {
  if (head == NULL) {
    head = node;
    return;
  }

  for (Node *p = head; ; p = p->next) {
    if (p->next == NULL) {
      p->next = node;
      break;
    }
  }
}

void Queue::remove(Node *node) {
  for (Node *p = head, *prev = NULL; p != NULL; prev = p, p = p->next) {
    if (p == node) {
      if (prev == NULL) {
        head = p->next;
      } else {
        prev->next = p->next;
      }
      delete node;
      break;
    }
  }
}

Queue alarms;

void checkAlarm() {
  Serial.println("checkAlarm");
  tmElementsPtr_t now = rtcNow();
  for (Node *p = alarms.head; p != NULL; p = p->next) {
    if (p->when.Month != 255 && p->when.Month != now->Month) {
      continue;
    }

    if (p->when.Day != 255 && p->when.Day != now->Day) {
      continue;
    }

    if (p->when.Wday != 255 && p->when.Wday != now->Wday) {
      continue;
    }

    if (p->when.Hour != 255 && p->when.Hour != now->Hour) {
      continue;
    }

    if (p->when.Minute != 255 && p->when.Minute != now->Minute) {
      continue;
    }

    p->fn(now);
  }
}

void *defineRtcAlarm(tmElements_t when, alarmFunc fn) {
  static bool rtcHosted = false;
  if (!rtcHosted) {
    rtcHosted = true;
    Serial.println("Host RTC alarm");
    core::clock::interval(30 * 1000, &checkAlarm); // check alarm every 30 seconds.
  }

  Node *node = new Node(when, fn);
  alarms.add(node);
  return node;
}

void removeRtcAlarm(void *handle) {
  alarms.remove((Node*)handle);
}
