#pragma once

#include "SensorBehavior.h"
#include "EntranceState.h"

namespace PiAlarm
{
  class WindowSensorBehavior
    : public virtual SensorBehavior
  {
    public:
      WindowSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const &sensor, std::string const &gpio);
      
    protected:
      virtual void updateRising();
      virtual void updateHigh();
      virtual void updateFalling();
      virtual void updateLow();

    private:
      EntranceState::Type mEntranceState;
  };
}
