#include "GpIoSensor.h"
#include "GpIoElement.h"

namespace SimOn
{
  class GpIoSensorImpl
  {
    public:
      GpIoSensorImpl(std::string const & gpio)
        : mGpIo(gpio)
        , mState(GpIoSensorState::None)
        , mLastChangeTime()
      {
        mGpIo.setDirection(Direction::Input);
      }

      GpIoSensorState::Type update()
      {
        if (mGpIo.getValue() == Value::High)
        {
          if (mState == GpIoSensorState::Rising)
          {
            mState = GpIoSensorState::High;
          }
          else if (mState != GpIoSensorState::High)
          {
            mState = GpIoSensorState::Rising;
            mLastChangeTime = std::chrono::system_clock::now();
          }
        }
        else
        {
          if (mState == GpIoSensorState::Falling)
          {
            mState = GpIoSensorState::Low;
          }
          else if (mState != GpIoSensorState::Low)
          {
            mState = GpIoSensorState::Falling;
            mLastChangeTime = std::chrono::system_clock::now();
          }
        }
        return mState;
      }

      GpIoSensorState::Type state() const
      {
        return mState;
      }

      std::chrono::duration<double> elasped() const
      {
        auto wNow = std::chrono::system_clock::now();
        auto wElapsed = wNow - mLastChangeTime;
        return wElapsed;
      }

      std::string const & gpio() const
      {
        return mGpIo.getNumber();
      }

    private:
      GpIoElement mGpIo;
      GpIoSensorState::Type mState;
      std::chrono::time_point<std::chrono::system_clock> mLastChangeTime;

  };

  GpIoSensor::GpIoSensor(std::string const & gpio)
    : mImplementation(new GpIoSensorImpl(gpio))
  {
  }  

  GpIoSensor::~GpIoSensor()
  {
    delete mImplementation;
  }
  
  GpIoSensorState::Type GpIoSensor::update()
  {
    return mImplementation->update();
  }

  GpIoSensorState::Type GpIoSensor::state() const
  {
    return mImplementation->state();
  }  

  std::chrono::duration<double> GpIoSensor::elasped() const
  {
    return mImplementation->elasped();
  }

  std::string const & GpIoSensor::gpio() const
  {
    return mImplementation->gpio();
  }

}
