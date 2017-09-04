#pragma once

#include <string>

namespace SimOn
{
  class RfIdSensorImpl;

  class RfIdSensor
  {
    public:
      RfIdSensor(std::string const &device, std::string const & baudRate);
      virtual ~RfIdSensor();

      bool read(std::string & data) const;

    private:
      RfIdSensorImpl * const mImplementation;

  };
}
