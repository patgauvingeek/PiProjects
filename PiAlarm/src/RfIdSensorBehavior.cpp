#include "RfIdSensorBehavior.h"

#include "RfIdSensor.h"

namespace PiAlarm
{
  RfIdSensorBehavior::RfIdSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor)
    : mAlarmSystem(alarmSystem)
    , mSensor(sensor)
    , mRfIdSensor()
  {}

  void RfIdSensorBehavior::update()
  {
   
  }  

}
