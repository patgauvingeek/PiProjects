#pragma once

#include <string>
#include "pialarm.hpp"

namespace PiAlarm
{
  class ITrigger
  {
    public:
      virtual ~ITrigger() {}
      
      virtual void activate(db::Alarm &alarm) = 0;
      virtual void deactivate() = 0;

  };
}