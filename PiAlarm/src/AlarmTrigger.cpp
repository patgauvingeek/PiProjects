#include "AlarmTrigger.h"

namespace PiAlarm
{
  AlarmTrigger::AlarmTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier)
    : mAlarmSystem(alarmSystem)
    , mNotifier(notifier)
  {
  }    

}
