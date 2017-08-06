#pragma once

#include <chrono>
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

    private:
      AlarmSystem * const mAlarmSystem;
      db::Sensor mSensor;
      bool mArming;
      std::chrono::time_point<std::chrono::system_clock> mArmTime;

      SimOn::RfIdSensor mRfIdSensor;
      std::string mLastCode;      

  };
}
