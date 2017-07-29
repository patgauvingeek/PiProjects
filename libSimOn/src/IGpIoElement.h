#pragma once

#include <string>

namespace SimOn
{
  namespace Direction
  {
    enum Type
    {
      Input,
      Output
    };
  }

  namespace EdgeDetection
  {
    enum Type
    {
      None,
      Falling,
      Raising,
      Both
    };
  }

  namespace Value
  {
    enum Type
    {
      Low,
      High
    };
  }

  class IGpIoElement
  {
    public:
      virtual ~IGpIoElement() {}

      virtual void setDirection(Direction::Type const direction) = 0;
      virtual void setEdgeDetection(EdgeDetection::Type const edgeDetection) = 0;
      virtual void setValue(Value::Type const value) = 0;
      virtual Value::Type getValue() const = 0;
      virtual std::string const & getNumber() const = 0;
  };

}
