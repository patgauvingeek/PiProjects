#include "BellTrigger.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  BellTrigger::BellTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier)
    : AlarmTrigger(alarmSystem, notifier)
    , mGpIo(notifier.gpio)
  {
    mGpIo.setDirection(SimOn::Direction::Output); 
  }
  
  void BellTrigger::activate()
  {
    alarmSystem().log("BellTrigger::activate", "Bell trigger has been activated.", db::Log::Severity::Debug);
    mGpIo.setValue(SimOn::Value::High);
  }
  
  void BellTrigger::deactivate()
  {
    alarmSystem().log("BellTrigger::deactivate", "Bell trigger has been deactivated.", db::Log::Severity::Debug);
    mGpIo.setValue(SimOn::Value::Low);
  }

}
