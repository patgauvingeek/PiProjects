#pragma once

#include <string>
#include "GpIoElement.h"
#include "SensorBehavior.h"

namespace PiAlarm
{
  class MotionSensorBehavior
    : public virtual SensorBehavior
  {
    public:
      MotionSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const &sensor, std::string const &gpio, std::string const &gpioPower);
      
      virtual void arm() override;
      virtual void unarm() override;

    protected:
      virtual void updateLow() override;
      virtual void updateHigh() override;

    private:
      bool mEventNotified;
      SimOn::GpIoElement mGpIoPower;
  };
}
