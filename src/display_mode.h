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
    void onTempeHumiChanges() override;
    void onTempeSetpointChanges() override;
    void onHeaterChanges() override;
    void onClock() override;
};

class SetupModeBase : public DisplayMode {
    void* blinkDelayHandler;

    void reScheduleBlink();
    void onLeaveMode();
  protected:
    bool blinkOn;

    // return callback function register as core::clock::delay
    virtual core::callback blinkCallback() = 0;

    virtual void doBlink(bool showOrHide) = 0;

    // Call updateForAdjust() after change current setting value, to update
    // display and delays blink 
    void updateForAdjust();
  public:
    void enterState() override;
    void onModeKey() override;
    void onSetupKey() override;

    // onBlink is an internal method.
    void onBlink();
};

// SetupMode with multiple adjust items.
template <int8_t ItemCounts> class MultiItemSetupModeBase : public SetupModeBase {
  protected:
    int8_t curPart;
  public:
    void enterState() {
      // SetupMenuMode will call doBlink() on curPart == 0
      for (curPart = 1; curPart < ItemCounts; curPart++) {
        doBlink(true);
      }
      curPart = 0;
      SetupModeBase::enterState();
    }

    void onSetupKey() override {
      doBlink(true);
      curPart = (curPart + 1) % ItemCounts;

      // force newly focused value hidden, to make blink obviously.
      blinkOn = false;
      doBlink(false);
    }
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
class SetupSetpointMode : public SetupModeBase {
    int16_t setpoint;
  protected:
    core::callback blinkCallback() override;
    void doBlink(bool showOrHide) override;
  public:
    void enterState() override;
    void onModeKey() override;
    void onUpKey() override;
    void onDownKey() override;
};

// Display as
//
//  SET TEMPE HIGH/LOW
//   H: 0.2°C L: 0.3°C
//
// Config temperature setpoint high/low delta
class SetupTempeHysterMode : public SetupModeBase {
  uint8_t hysteresis;
  protected:
    core::callback blinkCallback() override;
    void doBlink(bool showOrHide) override;
  public:
    void enterState() override;
    void onModeKey() override;
    void onUpKey() override;
    void onDownKey() override;
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
class SetupTimeMode : public MultiItemSetupModeBase<6> {
    // 0: year, 1:month, 2:day, 3:Week day, 4:Hour, 5:Minute
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
};

class SetupMenuMode : public DisplayMode {
    int8_t curMenuItem;

    void updateMenuItem();
  public:
    void enterState() override;
    void onModeKey() override;
    void onSetupKey() override;
    void onUpKey() override;
    void onDownKey() override;
};

class SetupScheduleHighLowMode : public MultiItemSetupModeBase<2> {
    uint16_t high, low;
  protected:
    core::callback blinkCallback() override;
    void doBlink(bool showOrHide) override;
  public:
    void enterState() override;
    void onModeKey() override;
    void onUpKey() override;
    void onDownKey() override;
};

extern DisplayMode *const normalMode;
extern DisplayMode *const setupSetpointMode;
extern DisplayMode *const setupTempeHysterMode;
extern DisplayMode *const timeMode;
extern DisplayMode *const setupTimeMode;
extern DisplayMode *const dayScheduleMode;
extern DisplayMode *const setupMenuMode;
extern DisplayMode *const setupScheduleHighLowMode;

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
void setupScheduleHighLow();

// Inc val, if greater than max, round back to min.
template <typename T> T roundInc(T val, T min, T max) {
  val ++;
  if (val == max) {
    val = min;
  }
  return val;
}

// Dec val, if less than min, round back to max.
template <typename T> T roundDec(T val, T min, T max) {
  // if type T is unsigned, and min is 0, `val --` will cause value goto max value of type T,
  // compare first, before dec.
  if (val <= min) {
    val = max;
  }
  return val - 1;
}
