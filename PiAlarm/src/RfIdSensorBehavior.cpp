#include "RfIdSensorBehavior.h"

#include "AlarmSystem.h"
#include "RfIdSensor.h"

namespace PiAlarm
{
  const std::chrono::seconds RfIdSensorBehavior::cCountdownBeforeArming(15);

  RfIdSensorBehavior::RfIdSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor,
                                         std::string const &device, std::string const &baudRate)
    : mAlarmSystem(alarmSystem)
    , mSensor(sensor)
    , mArming(false)
    , mArmTime()
    , mRfIdSensor(device, baudRate)
  {}

  void RfIdSensorBehavior::update()
  {
    if (mArming)
    {
      // Updating the WebSockets with remaining time before the system is armed.
      auto wRemainingTimeBeforeArming = mArmTime - std::chrono::system_clock::now();
      if (mLastRemainingTimeBeforeArming - wRemainingTimeBeforeArming >= std::chrono::seconds(1) )
      {
        mCountdown--;
        if (mCountdown == 0)
        {
          mArming = false;
          mAlarmSystem->insertEvent(db::Event::Trigger::SystemArmed);
          mAlarmSystem->arm();
        }
        else
        {
          mAlarmSystem->notifyCountdown(std::chrono::seconds(mCountdown));
        }
        mLastRemainingTimeBeforeArming = wRemainingTimeBeforeArming;
      }
    }

    // reading the sensor
    std::string wRfId;
    if (mRfIdSensor.read(wRfId) == false)
    {
      mLastCode = "";
      return;
    }

    if (mLastCode == wRfId)
    {
      return;
    }
      
    try
    {
      auto wUser = mAlarmSystem->getUserByRfId(wRfId);

      if (mAlarmSystem->state() == AlarmSystemState::Unarmed && mArming == false)
      {
        mArming = true;
        mAlarmSystem->insertEvent(db::Event::Trigger::SystemArming, mSensor, wUser);
        mArmTime = std::chrono::system_clock::now() + cCountdownBeforeArming;
         // cCountdownBeforeArming + 1 sec to make sure notifyCountdown won't be skipped once.
        mLastRemainingTimeBeforeArming = cCountdownBeforeArming + std::chrono::seconds(1);
        mCountdown = cCountdownBeforeArming.count();
      }
      else
      {
        mArming = false;
        mAlarmSystem->insertEvent(db::Event::Trigger::SystemUnarmed, mSensor, wUser);
        mAlarmSystem->unarm();
      }
    }
    catch (litesql::NotFound e)
    {
      mAlarmSystem->log("RfIdSensorBehavior::update()", "Invalid RF ID", db::Log::Severity::Error);
    }
  
    mLastCode = wRfId;
  }  

}
