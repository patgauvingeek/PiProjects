#include <memory>

#include "SensorBehavior.h"
#include "pialarm.hpp"

namespace PiAlarm
{
  class SensorBehaviorFactory
  {
    public: 
      static std::shared_ptr<SensorBehavior> create(AlarmSystem *alarmSystem, db::Sensor const &sensor);
  };
}
