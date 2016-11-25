#pragma once

void setupTempeControl(void);

// Set temperature setpoint, value is temperature * 10, such as 215, means 21.5
void setTempeSetpoint(int16_t val);

// Return current temperature setpoint, value is temperature * 10, such as 215, means 21.5
int16_t getTempeSetpoint();

// Return temperature setpoint high limit, if setpoint is 200, setpoint high is 3, then
// when temperature reaches 203, close heater.
uint8_t getTempeSetpointHigh();

void setTempeSetpointHigh(uint8_t val);

// Return temperature setpoint low limit, if setpoint is 200, setpoint low is 3, then
// when temperature reaches 197, close heater.
uint8_t getTempeSetpointLow();

void setTempeSetpointLow(uint8_t val);
