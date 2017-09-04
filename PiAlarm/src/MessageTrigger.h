#pragma once

#include "GpIoElement.h"
#include "pialarm.hpp"
#include "AlarmTrigger.h"

namespace PiAlarm
{
  class AlarmSystem;

  class MessageTrigger
    : public virtual AlarmTrigger
  {
    public:
      MessageTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier);

      virtual void activate();
      virtual void deactivate();

  };
}