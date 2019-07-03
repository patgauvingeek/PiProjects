#pragma once

#include <memory>
#include "ISensorBehavior.h"
#include "AlarmSystem.h"
#include "pialarm.hpp"

namespace PiAlarm
{
  class SensorBehaviorFactory
  {
    public: 
      static std::shared_ptr<ISensorBehavior> create(AlarmSystem *alarmSystem, db::Sensor &sensor);
  };
}
