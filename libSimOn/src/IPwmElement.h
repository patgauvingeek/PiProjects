#pragma once

#include <string>

namespace SimOn
{
  class IPwmElement
  {
    public:
      virtual ~IPwmElement() {}

      virtual void setFrequency(double frequency) = 0;
      virtual void setPeriod(int period) = 0;
      virtual void setDutyCycle(int dutyCycle) = 0;
      virtual void start() = 0;
      virtual void stop() = 0;
  };

}
