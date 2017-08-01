#pragma once

namespace PiAlarm
{
  class ISensorBehavior
  {
    public:
      virtual ~ISensorBehavior() {}
      virtual void update() = 0;

  };
}
