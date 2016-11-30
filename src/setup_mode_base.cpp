#include <core.h>
#include "display_mode.h"

using namespace core;

void SetupModeBase::onLeaveMode() {
  clock::removeDelay(blinkDelayHandler);
}

void SetupModeBase::enterState() {
  blinkDelayHandler = clock::delay(500, blinkCallback());
}

void SetupModeBase::onModeKey() {
  onLeaveMode();
}

void SetupModeBase::reScheduleBlink() {
  clock::removeDelay(blinkDelayHandler);

  blinkDelayHandler = clock::delay(500, blinkCallback());
}

void SetupModeBase::onBlink() {
  blinkDelayHandler = clock::delay(500, blinkCallback());

  doBlink(blinkOn);
  blinkOn = !blinkOn;
}

void SetupModeBase::updateForAdjust() {
  doBlink(true);
  reScheduleBlink();
}

void SetupModeBase::onSetupKey() {
  onLeaveMode();

  DisplayMode::onSetupKey();
}
