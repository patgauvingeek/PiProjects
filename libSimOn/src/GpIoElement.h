#pragma once

#include "IGpIoElement.h"

namespace SimOn
{
  class GpIoElementImpl;

  class GpIoElement : IGpIoElement
  {
    public:
      GpIoElement(std::string const & gpio);
      virtual ~GpIoElement();

      virtual void setDirection(Direction::Type const direction);
      virtual void setEdgeDetection(EdgeDetection::Type const edgeDetection);
      virtual void setValue(Value::Type const value);
      virtual Value::Type getValue() const;
      virtual std::string const & getNumber() const;

    private:
      GpIoElementImpl * const mImplementation;
  };

}

