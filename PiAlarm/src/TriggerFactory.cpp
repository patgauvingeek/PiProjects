#include "TriggerFactory.h"

#include <stdexcept>

#include "BellTrigger.h"

namespace PiAlarm
{
  std::shared_ptr<ITrigger> TriggerFactory::create(AlarmSystem *alarmSystem, db::Notifier const &notifier)
  {        
    if (notifier.kind == db::Notifier::Kind::Bell)
    {
      return std::make_shared<PiAlarm::BellTrigger>(alarmSystem, notifier);     
    }
    else
    {
      throw std::runtime_error("Not supported notifier type.");
    }
  }
}
