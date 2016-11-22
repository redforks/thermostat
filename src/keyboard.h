#pragma once

void setupKeyboard();

// key touches must polling constantly, call checkKeys() in loop() as fast as possible. 
void checkKeys();
