#pragma once

namespace SimOn
{
  namespace EncoderDirection
  {
    enum Type
    {
      NoChange,
      Clockwise,
      CounterClockwise
    };
  }

  class IRotaryEncoder
  {
    public:
      virtual ~IRotaryEncoder() {}

      virtual EncoderDirection::Type update() = 0;
      virtual EncoderDirection::Type getDirection() const = 0;
  };

}
