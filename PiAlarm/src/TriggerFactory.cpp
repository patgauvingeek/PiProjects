#include "TriggerFactory.h"

#include <stdexcept>

#include "BellTrigger.h"
#include "MessageTrigger.h"

namespace PiAlarm
{
  std::shared_ptr<ITrigger> TriggerFactory::create(AlarmSystem *alarmSystem, db::Notifier const &notifier)
  {        
    if (notifier.kind == db::Notifier::Kind::Bell)
    {
      return std::make_shared<PiAlarm::BellTrigger>(alarmSystem, notifier);     
    }
    else if (notifier.kind == db::Notifier::Kind::Message)
    {
      return std::make_shared<PiAlarm::MessageTrigger>(alarmSystem, notifier);     
    }
    else
    {
      throw std::runtime_error("Not supported notifier type.");
    }
  }
}
