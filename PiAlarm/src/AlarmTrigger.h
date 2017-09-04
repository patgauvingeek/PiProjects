#pragma once

#include "pialarm.hpp"
#include "ITrigger.h"

namespace PiAlarm
{
  class AlarmSystem;

  class AlarmTrigger
    : public virtual ITrigger
  {
    public:
      AlarmTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier);
      
    protected:
      AlarmSystem & alarmSystem() { return *mAlarmSystem; }
      db::Notifier & notifier() { return mNotifier; }
    
    private:
      AlarmSystem * const mAlarmSystem;
      db::Notifier mNotifier;

  };
}
