#include "RfIdSensorBehavior.h"

#include "AlarmSystem.h"
#include "RfIdSensor.h"

namespace PiAlarm
{
  RfIdSensorBehavior::RfIdSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor)
    : mAlarmSystem(alarmSystem)
    , mSensor(sensor)
    , mArming(false)
    , mArmTime()
    , mRfIdSensor()
  {}

  void RfIdSensorBehavior::update()
  {
    // waiting 15 second before arming the system
    if (mArming && std::chrono::system_clock::now() >= mArmTime)
    {
      mArming = false;
      mAlarmSystem->insertEvent(db::Event::Trigger::SystemArmed);
      mAlarmSystem->arm();
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
