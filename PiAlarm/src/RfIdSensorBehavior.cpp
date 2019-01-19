#include "RfIdSensorBehavior.h"

#include "AlarmSystem.h"
#include "RfIdSensor.h"

namespace PiAlarm
{
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
      if (mLastRemainingTimeBeforeArming - wRemainingTimeBeforeArming > std::chrono::seconds(1) )
      {
        mAlarmSystem->notifyCountdown(std::chrono::duration_cast<std::chrono::seconds>(wRemainingTimeBeforeArming));
        mLastRemainingTimeBeforeArming = wRemainingTimeBeforeArming;
      }

      // waiting 15 second before arming the system
      if (std::chrono::system_clock::now() >= mArmTime)
      {
        mArming = false;
        mAlarmSystem->insertEvent(db::Event::Trigger::SystemArmed);
        mAlarmSystem->arm();
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
        mArmTime = std::chrono::system_clock::now() + std::chrono::seconds(15);
        mLastRemainingTimeBeforeArming = std::chrono::seconds(20);
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
