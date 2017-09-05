#include "TriggerFactory.h"

#include <stdexcept>

#include "Parameters.h"
#include "BellTrigger.h"
#include "MessageTrigger.h"

namespace PiAlarm
{
  std::shared_ptr<ITrigger> TriggerFactory::create(AlarmSystem *alarmSystem, db::Notifier const &notifier)
  {        
    if (notifier.kind == db::Notifier::Kind::Bell)
    {
      auto wParameters = Parameters::toVector(notifier.parameters);
      auto wGpio = wParameters[0];
      return std::make_shared<PiAlarm::BellTrigger>(alarmSystem, notifier, wGpio);     
    }
    else if (notifier.kind == db::Notifier::Kind::Message)
    {
      auto wParameters = Parameters::toVector(notifier.parameters);
      auto wAddress = wParameters[0];
      return std::make_shared<PiAlarm::MessageTrigger>(alarmSystem, notifier, wAddress);     
    }
    else
    {
      throw std::runtime_error("Not supported notifier type.");
    }
  }
}
