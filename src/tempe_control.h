#pragma once

void setupTempeControl(void);

// Set temperature setpoint, value is temperature * 10, such as 215, means 21.5
void setTempeSetpoint(int16_t val);

// Return current temperature setpoint, value is temperature * 10, such as 215, means 21.5
int16_t getTempeSetpoint();
