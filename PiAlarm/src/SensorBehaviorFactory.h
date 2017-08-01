#include <memory>

#include "pialarm.hpp"
#include "ISensorBehavior.h"
#include "AlarmSystem.h"

namespace PiAlarm
{
  class SensorBehaviorFactory
  {
    public: 
      static std::shared_ptr<ISensorBehavior> create(AlarmSystem *alarmSystem, db::Sensor const &sensor);
  };
}
