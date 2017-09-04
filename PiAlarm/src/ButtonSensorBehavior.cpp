#include "ButtonSensorBehavior.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  ButtonSensorBehavior::ButtonSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor, std::string const & gpio)
    : SensorBehavior(alarmSystem, sensor, gpio)
    , mPressed(false)
  {}

  void ButtonSensorBehavior::updateRising()
  {
    if (mPressed)
    {
      mPressed = false;
    }
    alarmSystem().log("updateRising", "button up", db::Log::Severity::Debug);
  }

  void ButtonSensorBehavior::updateHigh()
  {
  }

  void ButtonSensorBehavior::updateFalling()
  {
    mPressed = true;
    alarmSystem().log("updateFalling", "button down", db::Log::Severity::Debug);
  }

  void ButtonSensorBehavior::updateLow()
  {
    alarmSystem().log("updateLow", "button press", db::Log::Severity::Debug);
  }
}
