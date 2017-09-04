#pragma once

#include <memory>

#include "ITrigger.h"
#include "AlarmSystem.h"
#include "pialarm.hpp"

namespace PiAlarm
{
  class TriggerFactory
  {
    public: 
      static std::shared_ptr<ITrigger> create(AlarmSystem *alarmSystem, db::Notifier const &notifier);
  };
}
