#include <Time.h>
#include "rtc.h"
#include "display_mode.h"
#include "tempe_control.h"
#include "read_temp_hum.h"

void dayScheduleOnRTCAlarm(tmElementsPtr_t tm) {
  const Schedule &schedule = static_cast<DayScheduleMode*>(dayScheduleMode)->getSchedule();

  if (currentMode() != dayScheduleMode) {
    return;
  }

  uint8_t section = tm->Hour * 2;
  if (tm->Minute != 0) {
    section ++;
  }

  if (schedule.get(section)) {
    setTempeSetpoint(getScheduleHighTempeSetpoint());
  } else {
    setTempeSetpoint(getScheduleLowTempeSetpoint());
  }
}

void setupDaySchedule() {
  static_cast<DayScheduleMode*>(dayScheduleMode)->init();

  tmElements_t when;
  when.Month = 255;
  when.Day = 255;
  when.Wday = 255;
  when.Hour = 255;
  when.Minute = 0;
  defineRtcAlarm(when, dayScheduleOnRTCAlarm);

  when.Minute = 30;
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

void DayScheduleMode::init() {
  schedule.load(DAY_SCHEDULE_ADDRESS);
}

const Schedule& DayScheduleMode::getSchedule() {
  return schedule;
}

void DayScheduleMode::setSchedule(const Schedule& newSchedule) {
  schedule.assignFrom(newSchedule);
  schedule.save(DAY_SCHEDULE_ADDRESS);
}
