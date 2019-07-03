#include "MotionSensorBehavior.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  MotionSensorBehavior::MotionSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const &sensor, std::string const &gpio, std::string const &gpioPower)
    : SensorBehavior(alarmSystem, sensor, gpio)
    , mEventNotified(false)
    , mGpIoPower(gpioPower)
  {
    mGpIoPower.setDirection(SimOn::Direction::Output);
  }

  void MotionSensorBehavior::arm()
  {
    mGpIoPower.setValue(SimOn::Value::High);
  }

  void MotionSensorBehavior::unarm()
  {
    mGpIoPower.setValue(SimOn::Value::Low);
  }

  void MotionSensorBehavior::updateLow()
  {
    if (gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      mEventNotified = false;
    }
  }

  void MotionSensorBehavior::updateHigh()
  {
    if (!mEventNotified && gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      auto wEvent = alarmSystem().insertEvent(db::Event::Trigger::MotionDetected, sensor());
      if (alarmSystem().state() == AlarmSystemState::Armed)
      {
        auto wAlarm = alarmSystem().insertAlarm(wEvent);
        alarmSystem().raiseAlarm(wAlarm);
      }
      mEventNotified = true;
    }
  }
}
