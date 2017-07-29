#pragma once

#include <memory>
#include "GpIoSensor.h"
#include "pialarm.hpp"

namespace PiAlarm
{
  class AlarmSystem;

  class SensorBehavior
  {
    public:
      SensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor);
      
      void update();

    protected:
      virtual void updateRising() = 0;
      virtual void updateHigh() = 0;
      virtual void updateFalling() = 0;
      virtual void updateLow() = 0;

      AlarmSystem & alarmSystem() { return *mAlarmSystem; }
      db::Sensor & sensor() { return mSensor; }
      SimOn::GpIoSensor & gpIoSensor() { return mGpIoSensor; }
    
    private:
      AlarmSystem * const mAlarmSystem;
      db::Sensor mSensor;
      SimOn::GpIoSensor mGpIoSensor;      
  };
}
