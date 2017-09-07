#include "BellTrigger.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  BellTrigger::BellTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier, std::string const &gpio)
    : AlarmTrigger(alarmSystem, notifier)
    , mGpIo(gpio)
  {
    mGpIo.setDirection(SimOn::Direction::Output); 
  }
  
  void BellTrigger::activate(db::Alarm &alarm)
  {
    alarmSystem().log("BellTrigger::activate", "Bell has been activated.", db::Log::Severity::Debug);
    mGpIo.setValue(SimOn::Value::High);
  }
  
  void BellTrigger::deactivate()
  {
    alarmSystem().log("BellTrigger::deactivate", "Bell has been deactivated.", db::Log::Severity::Debug);
    mGpIo.setValue(SimOn::Value::Low);
  }

}
