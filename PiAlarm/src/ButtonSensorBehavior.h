#pragma once

#include "SensorBehavior.h"
#include "EntranceState.h"

namespace PiAlarm
{
  class ButtonSensorBehavior
    : public virtual SensorBehavior
  {
    public:
      ButtonSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor);
      
    protected:
      virtual void updateRising();
      virtual void updateHigh();
      virtual void updateFalling();
      virtual void updateLow();

    private:
      bool mPressed;
  };
}
