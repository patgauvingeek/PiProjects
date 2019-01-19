#pragma once

#include <chrono>
#include <string>
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
      RfIdSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const &sensor,
                         std::string const &device, std::string const &baudRate);
      
      virtual void update();

    private:
      static const std::chrono::seconds cCountdownBeforeArming;

      AlarmSystem * const mAlarmSystem;
      db::Sensor mSensor;
      bool mArming;
      std::chrono::time_point<std::chrono::system_clock> mArmTime;
      std::chrono::duration<long int, std::ratio<1l, 1000000000l>> mLastRemainingTimeBeforeArming;
      int mCountdown;

      SimOn::RfIdSensor mRfIdSensor;
      std::string mLastCode;      

  };
}
