#pragma once

#include <LiquidCrystal.h>

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
    void onHeaterChanges() override;
    void onClock() override;
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
class SetupNormalMode : public DisplayMode {
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
};

// time setup mode, display as TIME_MOD, but no second part. First blink
// year, push Setup key, cycle each part, until minute part, then blink year
// part again.
//
// Up/Down key inc/dec current blink part.
//
// Setup key, abort set return to NORMAL_MODE,
// Mode key, save and return to NORMAL_MODE,
class SetupTimeMode : public DisplayMode {
  public:
    void enterState() override;
    void onModeKey() override;
    void onUpKey() override;
    void onDownKey() override;
    void onSetupKey() override;
};

extern DisplayMode *const normalMode;
extern DisplayMode *const setupNormalMode;
extern DisplayMode *const timeMode;
extern DisplayMode *const setupTimeMode;

void switchMode(DisplayMode *const mode);
