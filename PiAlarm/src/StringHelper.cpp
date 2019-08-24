#include "StringHelper.h"

#include <sstream>

namespace PiAlarm
{
  const std::map<int, std::string> SENSOR_KIND_TO_TEXT =
  {
    { db::Sensor::Kind::Door, "Door" },
    { db::Sensor::Kind::Window, "Window" },
    { db::Sensor::Kind::Motion, "Motion" },
    { db::Sensor::Kind::Button, "Button" },
    { db::Sensor::Kind::RfId, "RfId" },
  };

  const std::map<int, std::string> NOTIFIER_KIND_TO_TEXT =
  {
    { db::Notifier::Kind::Bell, "Bell" },
    { db::Notifier::Kind::Message, "Message" }
  };
  
  const std::map<int, std::string> EVENT_TRIGGER_TO_TEXT =
  {
    { db::Event::Trigger::SystemStarted, "Started" },
    { db::Event::Trigger::SystemStopped, "Stopped" },
    { db::Event::Trigger::SystemUnarmed, "System Unarmed" },
    { db::Event::Trigger::SystemArming, "System Arming" },
    { db::Event::Trigger::SystemArmed, "Armed" },
    { db::Event::Trigger::DoorOpened, "Opened" },
    { db::Event::Trigger::DoorClosed, "Closed" },
    { db::Event::Trigger::WindowOpened, "Opened" },
    { db::Event::Trigger::WindowClosed, "Closed" },
    { db::Event::Trigger::MotionDetected, "Motion Detected" },
  };
  
  const std::map<int, std::string> LOG_SEVERITY_TO_TEXT =
  {
    { db::Log::Severity::Debug, "Debug" },
    { db::Log::Severity::Info, "Info" },
    { db::Log::Severity::Warning, "Warning" },
    { db::Log::Severity::Error, "Error" }
  };

  std::string findInMap(std::map<int, std::string> map, int key)
  {
    auto wIterator = map.find(key);
    if (wIterator != map.end())
    {
      return wIterator->second;
    }
    else
    {
      std::stringstream wStream;
      wStream << "<" << key << ">";
      return wStream.str();
    }
  }
  
  std::string StringHelper::toString(litesql::DateTime date)
  {
    return date.asString("%m/%d/%y %h:%M:%s");
  }

  std::string StringHelper::toString(db::User &user, bool showEvents)
  {
    std::stringstream wStream;
    wStream << user.name << ":" << user.rfid;
    if (showEvents)
    {
      auto wEvents = user.events().get().all();
      for(auto &wEvent : wEvents)
      {
        wStream << std::endl << "  " << toString(wEvent);
      }
    }
    return wStream.str();
  }
  
  std::string StringHelper::toString(db::Sensor &sensor, bool showEvents)
  {
    std::string wKind = findInMap(SENSOR_KIND_TO_TEXT, sensor.kind);
    std::stringstream wStream;
    wStream << "[" << wKind << "] " << sensor.name << " (" << sensor.parameters << ")";
    if (showEvents)
    {
      auto wEvents = sensor.events().get().all();
      for(auto &wEvent : wEvents)
      {
        wStream << std::endl << "  " << toString(wEvent);
      }
    }
    return wStream.str();
  }

  std::string StringHelper::toString(db::Notifier &notifier)
  {
    std::string wKind = findInMap(NOTIFIER_KIND_TO_TEXT, notifier.kind);
    std::stringstream wStream;
    wStream << "[" << wKind << "] " << notifier.name << " (" << notifier.parameters << ")";
    return wStream.str();
  }

  std::string StringHelper::toString(db::Event &event)
  {
    auto wEventDate = toString(event.date.value());
    
    std::string wSensorName("System");
    try 
    {
      wSensorName = event.sensor().get().one().name;
    } 
    catch (litesql::NotFound &e) { }
  
    std::string wUserName("");
    try 
    {
      wUserName = event.user().get().one().name;
    } 
    catch (litesql::NotFound &e) { }
  
    std::string wTrigger = findInMap(EVENT_TRIGGER_TO_TEXT, event.trigger);
    std::stringstream wStream;
    wStream << wEventDate << " - " << wSensorName << " " << wTrigger;
    if (wUserName != "")
    {
      wStream << " by " << wUserName;
    }
    return wStream.str();
  }

  std::string StringHelper::toString(db::Alarm &alarm)
  {
    std::stringstream wStream;
    auto wAlarmDate = toString(alarm.date.value());
    wStream << wAlarmDate << " - ALARM !";
    try
    {
      auto wEvent = alarm.event().get().one();
      wStream << std::endl << "  " << toString(wEvent);
    } 
    catch (litesql::NotFound &e)
    { }
    return wStream.str();
  }

  std::string StringHelper::toString(db::Log &log)
  {
    std::stringstream wStream;
    auto wDate = toString(log.date.value());
    wStream <<  wDate << " - " << log.method << " - ";
    wStream << "(" << findInMap(LOG_SEVERITY_TO_TEXT, log.severity) << ") " << log.what;
    return wStream.str();
  }

}
