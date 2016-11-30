#include <Time.h>
#include "rtc.h"
#include "display_mode.h"
#include "tempe_control.h"
#include "read_temp_hum.h"

void dayScheduleOnRTCAlarm(tmElementsPtr_t tm) {
  if (currentMode() != dayScheduleMode) {
    return;
  }

  if (tm->Hour == 8) {
    setTempeSetpoint(getScheduleLowTempeSetpoint());
  }

  if (tm->Hour == 16) {
    setTempeSetpoint(getScheduleHighTempeSetpoint());
  }
}

void setupDayScheduleMode() {
  tmElements_t when;
  when.Month = 255;
  when.Day = 255;
  when.Wday = 255;
  when.Hour = 8;
  when.Minute = 0;
  defineRtcAlarm(when, dayScheduleOnRTCAlarm);

  when.Hour = 16;
  defineRtcAlarm(when, dayScheduleOnRTCAlarm);
}

void DayScheduleMode::enterState() {
  NormalMode::enterState();

  lcd.setCursor(8, 0);
  lcd.print('D');
}

void DayScheduleMode::onModeKey() {
  switchMode(normalMode);
}
