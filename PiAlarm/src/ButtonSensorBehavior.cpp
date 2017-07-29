#include "ButtonSensorBehavior.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  ButtonSensorBehavior::ButtonSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor)
    : SensorBehavior(alarmSystem, sensor)
    , mPressed(false)
  {}

  void ButtonSensorBehavior::updateRising()
  {
    if (mPressed)
    {
      mPressed = false;
      alarmSystem().toggleState(sensor());
    }
  }

  void ButtonSensorBehavior::updateHigh()
  {
  }

  void ButtonSensorBehavior::updateFalling()
  {
    mPressed = true;
  }

  void ButtonSensorBehavior::updateLow()
  {
  }
}
