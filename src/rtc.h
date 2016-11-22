#pragma once

#include <Time.h>

// return current rtc time.
tmElementsPtr_t rtcNow();

// set current time to now.
void setRtc(tmElements_t &now);

// setup RTC module, call setupRTC() in main setup() function.
void setupRTC();

// alarmFunc get called at specific RTC time.
typedef void (*alarmFunc)(tmElementsPtr_t);

// call alarmFunc at specific RTC time, it is a repeated alarm, if one
// tmElements_t field is 255, means any, such as:
//
//  tmElements_t tm;
//  tm.Hour = 1;
//  tm.Wday = 2;
//  tm.Day = 255;
//  tm.Year = 255;
//  tm.Month = 255;
//
// call fn on 1:00 every Monday.
//
// rtcAlarm has a resolution about one minute, don't care year.
//
// Returns handel of the alarm, use the handler to remove the alarm.
//
// For one shot alarm, plz remove it after alarm fired, no auto remove logic
// inside rtc alarm.
void *defineRtcAlarm(tmElements_t when, alarmFunc fn);

// remove RTC alarm, ignored if alarm not exist
void removeRtcAlarm(void * handle);
