#include "MessageTrigger.h"

#include "AlarmSystem.h"

namespace PiAlarm
{
  MessageTrigger::MessageTrigger(AlarmSystem *alarmSystem, db::Notifier const & notifier, std::string const &address)
    : AlarmTrigger(alarmSystem, notifier)
    , mAddress(address)
  { 
  }
  
  void MessageTrigger::activate()
  {
    // http://www.raspberry-projects.com/pi/software_utilities/email/ssmtp-to-send-emails
    // https://easyengine.io/tutorials/linux/ubuntu-postfix-gmail-smtp/
    // https://www.linode.com/docs/email/postfix/configure-postfix-to-send-mail-using-gmail-and-google-apps-on-debian-or-ubuntu

    //https://web.archive.org/web/20130423104300/http://mattlong.posterous.com/enable-cron-emails-using-gmail-on-ubuntu
    // echo "Hello world email body" | mail -s "Test Subject" recipientname@domain.com
    alarmSystem().log("MessageTrigger::activate", "Message has been sent.", db::Log::Severity::Debug);
  }
  
  void MessageTrigger::deactivate()
  {
  }

}
