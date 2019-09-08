#pragma once

#include "IPwmElement.h"

namespace SimOn
{
  class PwmElementImpl;

  class PwmElement
    : public virtual IPwmElement
  {
    public:
      PwmElement(std::string const & pwm);
      virtual ~PwmElement();

      virtual void setFrequency(double frequency);
      virtual void setPeriod(int period);
      virtual void setDutyCycle(int dutyCycle);
      virtual void start();
      virtual void stop();

    private:
      PwmElementImpl * const mImplementation;
  };

}

