#pragma once

namespace PiAlarm
{
  class ISensorBehavior
  {
    public:
      virtual ~ISensorBehavior() {}
      virtual void arm() = 0;
      virtual void unarm() = 0;
      virtual void update() = 0;

  };
}
