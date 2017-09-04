#include "MotionSensorBehavior.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  MotionSensorBehavior::MotionSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const &sensor, std::string const &gpio)
    : SensorBehavior(alarmSystem, sensor, gpio)
    , mEventNotified(false)
  {}

  void MotionSensorBehavior::updateRising()
  {
  }

  void MotionSensorBehavior::updateHigh()
  {
    if (gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      mEventNotified = false;
    }
  }

  void MotionSensorBehavior::updateFalling()
  {
  }

  void MotionSensorBehavior::updateLow()
  {
    if (!mEventNotified && gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      auto wEvent = alarmSystem().insertEvent(db::Event::Trigger::MotionDetected, sensor());
      if (alarmSystem().state() == AlarmSystemState::Armed)
      {
        alarmSystem().insertAlarm(wEvent);
        alarmSystem().raiseAlarm();
      }
      mEventNotified = true;
    }
  }
}
