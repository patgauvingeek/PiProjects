#pragma once

#include "SensorBehavior.h"

namespace PiAlarm
{
  class MotionSensorBehavior
    : public virtual SensorBehavior
  {
    public:
      MotionSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor);
      
    protected:
      virtual void updateRising();
      virtual void updateHigh();
      virtual void updateFalling();
      virtual void updateLow();

    private:
      bool mEventNotified;
  };
}
