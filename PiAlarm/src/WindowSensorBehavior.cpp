#include "WindowSensorBehavior.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  WindowSensorBehavior::WindowSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const &sensor, std::string const &gpio)
    : SensorBehavior(alarmSystem, sensor, gpio)
    , mEntranceState(EntranceState::Unknown)
  {}

  void WindowSensorBehavior::updateRising()
  {
  }

  void WindowSensorBehavior::updateHigh()
  {
    if (mEntranceState != EntranceState::Closed && gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      mEntranceState = EntranceState::Closed;
      alarmSystem().insertEvent(db::Event::Trigger::WindowClosed, sensor());
    }
  }

  void WindowSensorBehavior::updateFalling()
  {
  }

  void WindowSensorBehavior::updateLow()
  {    
    if (mEntranceState != EntranceState::Opened && gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      mEntranceState = EntranceState::Opened;
      auto wEvent = alarmSystem().insertEvent(db::Event::Trigger::WindowOpened, sensor());
      if (alarmSystem().state() == AlarmSystemState::Armed)
      {
        alarmSystem().insertAlarm(wEvent);
        alarmSystem().raiseAlarm();
      }
    }
  }
}
