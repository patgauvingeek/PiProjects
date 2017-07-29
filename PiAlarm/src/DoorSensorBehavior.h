#pragma once

#include "EntranceState.h"
#include "SensorBehavior.h"

namespace PiAlarm
{
  class DoorSensorBehavior
    : public virtual SensorBehavior
  {
    public:
      DoorSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor);
      
    protected:
      virtual void updateRising();
      virtual void updateHigh();
      virtual void updateFalling();
      virtual void updateLow();

    private:
      EntranceState::Type mEntranceState;
  };
}
