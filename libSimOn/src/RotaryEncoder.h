#pragma once

#include "IRotaryEncoder.h"
#include <string>

namespace SimOn
{
  class RotaryEncoderImpl;

  class RotaryEncoder : public IRotaryEncoder
  {
    public:
      RotaryEncoder(std::string const & gpioA, std::string const & gpioB);
      virtual ~RotaryEncoder();

      virtual EncoderDirection::Type update();
      virtual EncoderDirection::Type getDirection() const;

    private:
      RotaryEncoderImpl * const mImplementation;
  };

}
