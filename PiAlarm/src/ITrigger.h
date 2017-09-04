#pragma once

namespace PiAlarm
{
  class ITrigger
  {
    public:
      virtual ~ITrigger() {}
      
      virtual void activate() = 0;
      virtual void deactivate() = 0;

  };
}