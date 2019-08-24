#include "DoorSensorBehavior.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  const std::chrono::seconds DoorSensorBehavior::cCountdownToUnarm(15);

  DoorSensorBehavior::DoorSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor, std::string const & gpio)
    : SensorBehavior(alarmSystem, sensor, gpio)
    , mEntranceState(EntranceState::Unknown)
    , mExpectingUnarmedEventId(-1)
    , mExpectingUnarmedTime()
    , mLastRemainingTimeToUnarm()
  {}

  void DoorSensorBehavior::update()
  {
    // waiting 15 seconds before raising the alarm.
    if (alarmSystem().state() == AlarmSystemState::Armed && mExpectingUnarmedEventId >= 0)
    {
      // Updating the WebSockets with remaining time before the system is armed.
      auto wRemainingTimeToUnarm = mExpectingUnarmedTime - std::chrono::system_clock::now();
      if (mLastRemainingTimeToUnarm - wRemainingTimeToUnarm >= std::chrono::seconds(1) )
      {
        mCountdown--;
        if (mCountdown == 0)
        {
          try
          {
            auto wEvent = alarmSystem().getEventById(mExpectingUnarmedEventId);
            auto wAlarm = alarmSystem().insertAlarm(wEvent);
            alarmSystem().raiseAlarm(wAlarm);
          }
          catch (litesql::NotFound &e)
          {
            alarmSystem().log("DoorSensorBehavior::update()", "Invalid Event Id !", db::Log::Severity::Error);
            auto wAlarm = alarmSystem().insertAlarm();
            alarmSystem().raiseAlarm(wAlarm);
          }
          mExpectingUnarmedEventId = -1;
        }
        else
        {
          alarmSystem().notifyCountdown(std::chrono::seconds(mCountdown));
        }
        mLastRemainingTimeToUnarm = wRemainingTimeToUnarm;
      }
    }
    else
    {
      mExpectingUnarmedEventId = -1;
    }
    // checking the sensor state
    SensorBehavior::update();
  }

  void DoorSensorBehavior::updateLow()
  {    
    if (mEntranceState != EntranceState::Closed && gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      mEntranceState = EntranceState::Closed;
      alarmSystem().insertEvent(db::Event::Trigger::DoorClosed, sensor());
    }
  }
  
  void DoorSensorBehavior::updateHigh()
  {
    if (mEntranceState != EntranceState::Opened && gpIoSensor().elasped() > std::chrono::milliseconds(500))
    {
      mEntranceState = EntranceState::Opened;
      auto wEvent = alarmSystem().insertEvent(db::Event::Trigger::DoorOpened, sensor());
      mExpectingUnarmedEventId = wEvent.id;
      mExpectingUnarmedTime = std::chrono::system_clock::now() + std::chrono::seconds(15);
      // cCountdownToUnarm + 1 sec to make sure notifyCountdown won't be skipped once.
      mLastRemainingTimeToUnarm = cCountdownToUnarm + std::chrono::seconds(1);
      mCountdown = cCountdownToUnarm.count();
    }
  }
}
