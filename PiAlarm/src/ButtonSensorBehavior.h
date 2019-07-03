#pragma once

#include <string>
#include "SensorBehavior.h"
#include "EntranceState.h"

namespace PiAlarm
{
  class ButtonSensorBehavior
    : public virtual SensorBehavior
  {
    public:
      ButtonSensorBehavior(AlarmSystem *alarmSystem, db::Sensor const & sensor, std::string const & gpio);
      
    protected:
      virtual void updateFalling() override;
      virtual void updateLow() override;
      virtual void updateRising() override;

    private:
      bool mPressed;
  };
}
