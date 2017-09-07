#include "MessageTrigger.h"

#include <sstream>
#include "StringHelper.h"
#include "AlarmSystem.h"

namespace PiAlarm
{
  MessageTrigger::MessageTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier, std::string const &address)
    : AlarmTrigger(alarmSystem, notifier)
    , mAddress(address)
  { 
  }
  
  void MessageTrigger::activate(db::Alarm &alarm)
  {
    // http://www.raspberry-projects.com/pi/software_utilities/email/ssmtp-to-send-emails
    std::stringstream wCommandStream;
    wCommandStream << "echo \"" << StringHelper::toString(alarm) << "\" | mail -s \"INTRUSION ALERT!\" " << mAddress;
    system(wCommandStream.str().c_str());
    alarmSystem().log("MessageTrigger::activate", "Message has been sent.", db::Log::Severity::Debug);
  }
  
  void MessageTrigger::deactivate()
  {
  }

}
