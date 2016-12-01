#include <core.h>
#include "display_mode.h"
#include "shutdown.h"

void setupShutdownModeOnBlink() {
  setupShutdownMode->onBlink();
}

void SetupShutdownMode::enterState() {
  lcd.print(F("Shutdown Boiler?"));

  shutdown = isShutdown();
  SetupModeBase::enterState();
}

void SetupShutdownMode::onModeKey() {
  setShutdown(shutdown);
  SetupModeBase::onModeKey();
}

void SetupShutdownMode::onUpKey() {
  shutdown = !shutdown;
  updateForAdjust();
}

void SetupShutdownMode::onDownKey() {
  shutdown = !shutdown;
  updateForAdjust();
}

core::callback SetupShutdownMode::blinkCallback() {
  return setupShutdownModeOnBlink;
}

void SetupShutdownMode::doBlink(bool showOrHide) {
  lcd.setCursor(2, 1);
  if (showOrHide) {
    lcd.print(shutdown ? F("Yes") : F("No "));
  } else {
    lcd.print(F("   "));
  }
}
