#pragma once

#include <string>
#include "GpIoSensor.h"
#include "pialarm.hpp"
#include "ISensorBehavior.h"

namespace PiAlarm
{
  class AlarmSystem;

  class SensorBehavior
    : public virtual ISensorBehavior
  {
    public:
      SensorBehavior(AlarmSystem *alarmSystem, db::Sensor const &sensor, std::string const &gpio);
      
      virtual void arm() {};
      virtual void unarm() {};
      virtual void update();

    protected:
      virtual void updateFalling() {};
      virtual void updateLow() {};
      virtual void updateRising() {};
      virtual void updateHigh() {};

      AlarmSystem & alarmSystem() { return *mAlarmSystem; }
      db::Sensor & sensor() { return mSensor; }
      SimOn::GpIoSensor & gpIoSensor() { return mGpIoSensor; }
    
    private:
      AlarmSystem * const mAlarmSystem;
      db::Sensor mSensor;
      SimOn::GpIoSensor mGpIoSensor;      
  };
}
