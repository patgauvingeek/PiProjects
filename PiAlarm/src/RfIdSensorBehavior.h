#pragma once

#include <memory>
#include "pialarm.hpp"
#include "ISensorBehavior.h"
#include "RfIdSensor.h"

namespace PiAlarm
{
  class AlarmSystem;

  class RfIdSensorBehavior
    : public virtual ISensorBehavior
  {
    public:
      RfIdSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor);
      
      virtual void update();

    protected:
      AlarmSystem & alarmSystem() { return *mAlarmSystem; }
      db::Sensor & sensor() { return mSensor; }
    
    private:
      AlarmSystem * const mAlarmSystem;
      db::Sensor mSensor;
      SimOn::RfIdSensor mRfIdSensor;      
  };
}
