#pragma once

#include <string>
#include "EntranceState.h"
#include "SensorBehavior.h"

namespace PiAlarm
{
  class DoorSensorBehavior
    : public virtual SensorBehavior
  {
    public:
      DoorSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor, std::string const & gpio);
      
      virtual void update() override;

    protected:
      virtual void updateRising();
      virtual void updateHigh();
      virtual void updateFalling();
      virtual void updateLow();

    private:
      static const std::chrono::seconds cCountdownToUnarm;

      EntranceState::Type mEntranceState;
      int mExpectingUnarmedEventId;
      std::chrono::time_point<std::chrono::system_clock> mExpectingUnarmedTime;
      std::chrono::duration<long int, std::ratio<1l, 1000000000l>> mLastRemainingTimeToUnarm;
      int mCountdown;
      
  };
}
