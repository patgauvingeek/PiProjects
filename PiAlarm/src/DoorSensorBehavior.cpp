#include "DoorSensorBehavior.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  DoorSensorBehavior::DoorSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor)
    : SensorBehavior(alarmSystem, sensor)
    , mEntranceState(EntranceState::Unknown)
  {}

  void DoorSensorBehavior::updateRising()
  {
  }

  void DoorSensorBehavior::updateHigh()
  {    
    if (mEntranceState != EntranceState::Closed && gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      mEntranceState = EntranceState::Closed;
      alarmSystem().insertEvent(db::Event::Trigger::DoorClosed, sensor());
    }
  }

  void DoorSensorBehavior::updateFalling()
  {    
  }

  void DoorSensorBehavior::updateLow()
  {
    if (mEntranceState != EntranceState::Opened && gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      mEntranceState = EntranceState::Opened;
      alarmSystem().doorOpened(sensor());
    }
  }
}
