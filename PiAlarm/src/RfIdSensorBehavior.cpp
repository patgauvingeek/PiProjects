#include "RfIdSensorBehavior.h"

#include "AlarmSystem.h"
#include "RfIdSensor.h"

namespace PiAlarm
{
  RfIdSensorBehavior::RfIdSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor)
    : mAlarmSystem(alarmSystem)
    , mSensor(sensor)
    , mRfIdSensor()
  {}

  void RfIdSensorBehavior::update()
  {
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

      if (mAlarmSystem->state() == AlarmSystemState::Unarmed)
      {
        mAlarmSystem->insertEvent(db::Event::Trigger::SystemArming, mSensor, wUser);
        mAlarmSystem->arm();
      }
      else
      {
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
