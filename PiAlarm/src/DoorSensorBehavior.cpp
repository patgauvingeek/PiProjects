#include "DoorSensorBehavior.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  DoorSensorBehavior::DoorSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor)
    : SensorBehavior(alarmSystem, sensor)
    , mEntranceState(EntranceState::Unknown)
    , mExpectingUnarmedEventId(-1)
    , mExpectingUnarmedTime()
  {}

  void DoorSensorBehavior::update()
  {
    // waiting 15 seconds before raising the alarm.
    if (alarmSystem().state() == AlarmSystemState::Armed)
    {
      if (mExpectingUnarmedEventId >= 0 &&
          std::chrono::system_clock::now() >= mExpectingUnarmedTime)
      {
        try
        {
          auto wEvent = alarmSystem().getEventById(mExpectingUnarmedEventId);
          alarmSystem().insertAlarm(wEvent);
          alarmSystem().raiseAlarm();
        }
        catch (litesql::NotFound e)
        {
          alarmSystem().log("DoorSensorBehavior::update()", "Invalid Event Id !", db::Log::Severity::Error);
          alarmSystem().insertAlarm();
          alarmSystem().raiseAlarm();
        }
        mExpectingUnarmedEventId = -1;
      }
    }
    else
    {
      mExpectingUnarmedEventId = -1;
    }
    // checking the sensor state
    SensorBehavior::update();
  }

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
      auto wEvent = alarmSystem().insertEvent(db::Event::Trigger::DoorOpened, sensor());
      mExpectingUnarmedEventId = wEvent.id;
      mExpectingUnarmedTime = std::chrono::system_clock::now() + std::chrono::seconds(15);
    }
  }
}
