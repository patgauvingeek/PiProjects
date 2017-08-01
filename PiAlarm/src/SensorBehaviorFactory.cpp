#include "SensorBehaviorFactory.h"
#include <memory>

#include "DoorSensorBehavior.h"
#include "WindowSensorBehavior.h"
#include "MotionSensorBehavior.h"
#include "ButtonSensorBehavior.h"

#include "pialarm.hpp"

namespace PiAlarm
{
  std::shared_ptr<ISensorBehavior> SensorBehaviorFactory::create(AlarmSystem *alarmSystem, db::Sensor const &sensor)
  {        
    if (sensor.kind == db::Sensor::Kind::Door)
    {
      return std::make_shared<PiAlarm::DoorSensorBehavior>(alarmSystem, sensor);     
    } 
    else if (sensor.kind == db::Sensor::Kind::Window)
    {
      return std::make_shared<PiAlarm::WindowSensorBehavior>(alarmSystem, sensor);
    }
    else if (sensor.kind == db::Sensor::Kind::Motion)
    {
      return std::make_shared<PiAlarm::MotionSensorBehavior>(alarmSystem, sensor);
    }
    else if (sensor.kind == db::Sensor::Kind::Button)
    {
      return std::make_shared<PiAlarm::ButtonSensorBehavior>(alarmSystem, sensor);
    }
    else
    {
      throw -1;
    }
  }
}
