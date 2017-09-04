#include "SensorBehavior.h"

namespace PiAlarm
{
  SensorBehavior::SensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor)
    : mAlarmSystem(alarmSystem)
    , mSensor(sensor)
    , mGpIoSensor(sensor.parameters)
  {}

  void SensorBehavior::update()
  {
    switch (mGpIoSensor.update())
    {
      case SimOn::GpIoSensorState::Rising:
      {
        updateRising();
      } break;

      case SimOn::GpIoSensorState::High:
      {
        updateHigh();
      } break;

      case SimOn::GpIoSensorState::Falling:
      {          
        updateFalling();
      } break;

      case SimOn::GpIoSensorState::Low:
      {
        updateLow();
      } break;

      default: break;
    }
  }  

}
