#pragma once

#include <Time.h>
#include <LiquidCrystal.h>
#include <core.h>

extern LiquidCrystal lcd;

// Handles difference of different display mode, define a sub class for each
// display mode. In this base class, keyboard won't trigger any action.
class DisplayMode {
  public:
    virtual void enterState();
    virtual void onModeKey();
    virtual void onUpKey();
    virtual void onDownKey();
    virtual void onSetupKey();
    virtual void onTempeHumiChanges();
    virtual void onTempeSetpointChanges();
    virtual void onHeaterChanges();
    virtual void onClock();
};

// normal display mode:
//
// If heater on:
//
//   18.9°C   50.4%
//  =18.9°C 19:20 On
//
// If heater off:
//
//   18.9°C   50.4%
//  =18.9°C 19:20 
//
// If heater will on:
//
//   18.9°C   50.4%
//  =18.9°C Will On
//
// If heater will off:
//
//   18.9°C   50.4%
//  =18.9°C Will Off
//
// Setup key, enter SETUP_NORMAL_MODE,
// Mode key, enter TIME_MODE.
class NormalMode : public DisplayMode {
  public:
    void enterState() override;
    void onModeKey() override;
    void onSetupKey() override;
    void onTempeHumiChanges() override;
    void onTempeSetpointChanges() override;
    void onHeaterChanges() override;
    void onClock() override;
};

class SetupModeBase : public DisplayMode {
    bool blinkOn;
    void* blinkDelayHandler;

    void reScheduleBlink();
  protected:
    // return callback function register as core::clock::delay
    virtual core::callback blinkCallback() = 0;

    virtual void doBlink(bool showOrHide) = 0;

    // Call updateForAdjust() after change current setting value, to update
    // display and delays blink 
    void updateForAdjust();
  public:
    void enterState() override;
    void onModeKey() override;

    // onBlink is an internal method.
    void onBlink();
};

// Display as:
//
//  SET TARGET TEMP
//   19.1°C
//   ^^^^ blink
//
// Setup key, abort set return to NORMAL_MODE,
// Mode key, save and return to NORMAL_MODE,
// Up/Down key, inc/dec 0.1°C.
class SetupNormalMode : public SetupModeBase {
    int16_t setpoint;
  protected:
    core::callback blinkCallback() override;
    void doBlink(bool showOrHide) override;
  public:
    void enterState() override;
    void onModeKey() override;
    void onUpKey() override;
    void onDownKey() override;
    void onSetupKey() override;
};

// Display as
//
//  SET TEMPE HIGH/LOW
//   H: 0.2°C L: 0.3°C
//
// Config temperature setpoint high/low delta
class SetupTempeHighLowMode : public SetupModeBase {
  uint8_t high, low;
  uint8_t curPart; // 0: high, 1: low
  protected:
    core::callback blinkCallback() override;
    void doBlink(bool showOrHide) override;
  public:
    void enterState() override;
    void onModeKey() override;
    void onUpKey() override;
    void onDownKey() override;
    void onSetupKey() override;
};

// time display mode
//
//  2016-03-07 FRI
//    22:56:03
//
// Mode key, enter NORMAL_MODE,
// Setup key, enter SETUP_TIME_MODE.
class TimeMode : public DisplayMode {
  public:
    void enterState() override;
    void onModeKey() override;
    void onSetupKey() override;
    void onClock() override;
};

// time setup mode, display as TIME_MOD, but no second part. First blink
// year, push Setup key, cycle each part, until minute part, then blink year
// part again.
//
// Up/Down key inc/dec current blink part.
//
// Setup key, abort set return to NORMAL_MODE,
// Mode key, save and return to NORMAL_MODE,
class SetupTimeMode : public SetupModeBase {
    // 0: year, 1:month, 2:day, 3:Week day, 4:Hour, 5:Minute
    uint8_t currentPart;
    tmElements_t tm;

    uint8_t getCurrentPartMax();
    uint8_t* getCurrentPart();
  protected:
    core::callback blinkCallback() override;
    void doBlink(bool showOrHide) override;
  public:
    void enterState() override;
    void onModeKey() override;
    void onUpKey() override;
    void onDownKey() override;
    void onSetupKey() override;
};

// Display like Normal mode, but add "D" between temperature and humidity:
//
//   18.9°C   50.4%
//  =18.9°C 19:20 On
//
// Day schedule change setpoint by RTC alarm.
class DayScheduleMode : public NormalMode {
  public:
    void enterState() override;
    void onModeKey() override;
    void onSetupKey() override;
};

extern DisplayMode *const normalMode;
extern DisplayMode *const setupNormalMode;
extern DisplayMode *const setupTempeHighLowMode;
extern DisplayMode *const timeMode;
extern DisplayMode *const setupTimeMode;
extern DisplayMode *const dayScheduleMode;

void switchMode(DisplayMode *const mode);

// print number with 2 decimal digits, prefix with 0 if less than 10.
// Assume number >= 0, number < 100.
void print2DigitsZero(int number);

// print number like 12.5, store as 125.
void printNumber00n0(uint16_t number);

// callback registered 
void setupDayScheduleMode();

// return current display mode.
DisplayMode *currentMode();

// Return temperature high setpoint used in schedule mode.
int16_t getScheduleHighTempeSetpoint();

// Return temperature low setpoint used in schedule mode.
int16_t getScheduleLowTempeSetpoint();

// set temperature high setpoint used in schedule mode.
void setScheduleHighTempeSetpoint(int16_t tempe);

// set temperature low setpoint used in schedule mode.
void setScheduleLowTempeSetpoint(int16_t tempe);

// Setup low/high temperature setpoint.
void setupScheduleTempeSetpoint();

// Setup schedule temperature high low display mode.
void setupScheduleTempeHighLowMode();
