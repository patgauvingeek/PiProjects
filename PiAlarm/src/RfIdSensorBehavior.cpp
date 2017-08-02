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
        mAlarmSystem->arm(mSensor, wUser);
      }
      else
      {
        mAlarmSystem->unarm(mSensor, wUser);
      }
    }
    catch (litesql::NotFound e)
    {
      //event invalid RF ID
    }
  
    mLastCode = wRfId;
  }  

}
