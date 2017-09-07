#pragma once

#include <string>
#include "pialarm.hpp"

namespace PiAlarm
{
  class StringHelper
  {
    public:
      static std::string toString(litesql::DateTime date);
      static std::string toString(db::User &user, bool showEvents = false);
      static std::string toString(db::Sensor &sensor, bool showEvents = false);
      static std::string toString(db::Notifier &notifier);
      static std::string toString(db::Event &event);
      static std::string toString(db::Alarm &alarm);
      static std::string toString(db::Log &log);

  };
}
