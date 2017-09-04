#include "MessageTrigger.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  MessageTrigger::MessageTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier)
    : AlarmTrigger(alarmSystem, notifier)
  { 
  }
  
  void MessageTrigger::activate()
  {
    alarmSystem().log("MessageTrigger::activate", "Message has been sent.", db::Log::Severity::Debug);
  }
  
  void MessageTrigger::deactivate()
  {
  }

}
