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
      
      virtual void update() override;

    protected:
      virtual void updateRising();
      virtual void updateHigh();
      virtual void updateFalling();
      virtual void updateLow();

    private:
      EntranceState::Type mEntranceState;
      int mExpectingUnarmedEventId;
      std::chrono::time_point<std::chrono::system_clock> mExpectingUnarmedTime;
      
  };
}
