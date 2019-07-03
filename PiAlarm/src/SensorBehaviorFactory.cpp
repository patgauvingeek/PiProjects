#include "SensorBehaviorFactory.h"

#include <stdexcept>

#include "Parameters.h"
#include "DoorSensorBehavior.h"
#include "WindowSensorBehavior.h"
#include "MotionSensorBehavior.h"
#include "ButtonSensorBehavior.h"
#include "RfIdSensorBehavior.h"

#include "StringHelper.h"

namespace PiAlarm
{
  void AssertParametersCount(AlarmSystem *alarmSystem, db::Sensor &sensor, int expected, int size)
  {
    if (expected == size)
    {
      return;
    }
    auto sensor_str = StringHelper::toString(sensor, false);
    std::stringstream message;
    message << sensor_str << " requires " << expected << " arguments, but has " << size << " arguments.";
    alarmSystem->log("AssertParametersCount", message.str(), db::Log::Severity::Error);
    throw std::out_of_range(message.str());
  }
  
  std::shared_ptr<ISensorBehavior> SensorBehaviorFactory::create(AlarmSystem *alarmSystem, db::Sensor &sensor)
  {        
    if (sensor.kind == db::Sensor::Kind::Door)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      AssertParametersCount(alarmSystem, sensor, 1, wParameters.size());
      auto wGpio = wParameters[0];
      return std::make_shared<PiAlarm::DoorSensorBehavior>(alarmSystem, sensor, wGpio);     
    } 
    else if (sensor.kind == db::Sensor::Kind::Window)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      AssertParametersCount(alarmSystem, sensor, 1, wParameters.size());
      auto wGpio = wParameters[0];
      return std::make_shared<PiAlarm::WindowSensorBehavior>(alarmSystem, sensor, wGpio);
    }
    else if (sensor.kind == db::Sensor::Kind::Motion)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      AssertParametersCount(alarmSystem, sensor, 2, wParameters.size());
      auto wGpio = wParameters[0];
      auto wGpioPower = wParameters[1];
      return std::make_shared<PiAlarm::MotionSensorBehavior>(alarmSystem, sensor, wGpio, wGpioPower);
    }
    else if (sensor.kind == db::Sensor::Kind::Button)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      AssertParametersCount(alarmSystem, sensor, 1, wParameters.size());
      auto wGpio = wParameters[0];
      return std::make_shared<PiAlarm::ButtonSensorBehavior>(alarmSystem, sensor, wGpio);
    }
    else if (sensor.kind == db::Sensor::Kind::RfId)
    {
      auto wParameters = Parameters::toVector(sensor.parameters);
      AssertParametersCount(alarmSystem, sensor, 2, wParameters.size());
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
