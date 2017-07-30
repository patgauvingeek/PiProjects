#pragma once

#include <string>

namespace SimOn
{
  class RfIdSensorImpl;

  class RfIdSensor
  {
    public:
      RfIdSensor();
      virtual ~RfIdSensor();

      bool read(std::string & data) const;

    private:
      RfIdSensorImpl * const mImplementation;

  };
}
