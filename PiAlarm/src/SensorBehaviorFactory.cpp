#include "SensorBehaviorFactory.h"

#include <stdexcept>

#include "Parameters.h"
#include "DoorSensorBehavior.h"
#include "WindowSensorBehavior.h"
#include "MotionSensorBehavior.h"
#include "ButtonSensorBehavior.h"
#include "RfIdSensorBehavior.h"

namespace PiAlarm
{
  std::shared_ptr<ISensorBehavior> SensorBehaviorFactory::create(AlarmSystem *alarmSystem, db::Sensor const &sensor)
  {        
    if (sensor.kind == db::Sensor::Kind::Door)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      auto wGpio = wParameters[0];
      return std::make_shared<PiAlarm::DoorSensorBehavior>(alarmSystem, sensor, wGpio);     
    } 
    else if (sensor.kind == db::Sensor::Kind::Window)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      auto wGpio = wParameters[0];
      return std::make_shared<PiAlarm::WindowSensorBehavior>(alarmSystem, sensor, wGpio);
    }
    else if (sensor.kind == db::Sensor::Kind::Motion)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      auto wGpio = wParameters[0];
      return std::make_shared<PiAlarm::MotionSensorBehavior>(alarmSystem, sensor, wGpio);
    }
    else if (sensor.kind == db::Sensor::Kind::Button)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      auto wGpio = wParameters[0];
      return std::make_shared<PiAlarm::ButtonSensorBehavior>(alarmSystem, sensor, wGpio);
    }
    else if (sensor.kind == db::Sensor::Kind::RfId)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      auto wDevice = wParameters[0];
      auto wBaudRate = wParameters[1];
      return std::make_shared<PiAlarm::RfIdSensorBehavior>(alarmSystem, sensor, wDevice, wBaudRate);
    }
    else
    {
      throw std::runtime_error("Not supported sensor type.");
    }
  }
}
