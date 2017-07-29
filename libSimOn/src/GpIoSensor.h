#pragma once

#include <chrono>
#include <string>

namespace SimOn
{
  class GpIoSensorImpl;

  namespace GpIoSensorState
  {
    enum Type
    {
      None,
      Rising,
      High,
      Falling,
      Low
    };
  }

  class GpIoSensor
  {
    public:
      GpIoSensor(std::string const & gpio);
      virtual ~GpIoSensor();

      virtual GpIoSensorState::Type update();
      virtual GpIoSensorState::Type state() const;
      virtual std::chrono::duration<double> elasped() const;

      virtual std::string const & gpio() const;

    private:
      GpIoSensorImpl * const mImplementation;
  };

}
