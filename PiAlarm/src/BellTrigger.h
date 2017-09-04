#pragma once

#include "GpIoElement.h"
#include "pialarm.hpp"
#include "AlarmTrigger.h"

namespace PiAlarm
{
  class AlarmSystem;

  class BellTrigger
    : public virtual AlarmTrigger
  {
    public:
      BellTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier);

      virtual void activate();
      virtual void deactivate();
      
    protected:
      SimOn::GpIoElement & gpIo() { return mGpIo; }
    
    private:
      SimOn::GpIoElement mGpIo;   

  };
}
