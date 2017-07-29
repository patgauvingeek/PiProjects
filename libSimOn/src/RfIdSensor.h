#pragma once

#include <string>

namespace SimOn
{
  class RfIdSensorImpl;

  class RfIdSensor
  {
    public:
      RfIdSensor(std::string const & device, int baudRate);
      virtual ~RfIdSensor();

    private:
      RfIdSensorImpl * const mImplementation;

  };
}
