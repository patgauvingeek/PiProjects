#include "RotaryEncoder.h"
#include "GpIoElement.h"

namespace SimOn
{
  class RotaryEncoderImpl
  {
  public:
    RotaryEncoderImpl(std::string const & gpioA, std::string const & gpioB)  
      : mGpioA(gpioA)
      , mGpioB(gpioB)
      , mPreviousSequence(0)
      , mDirection(EncoderDirection::NoChange)
    {
      mGpioA.setDirection(Direction::Input);
      mGpioA.setEdgeDetection(EdgeDetection::Both);
      mGpioB.setDirection(Direction::Input);
      mGpioB.setEdgeDetection(EdgeDetection::Both);
    }

    EncoderDirection::Type update()
    {
      int wValueA = 0;
      if (mGpioA.getValue() == Value::High)
      {
        wValueA = 1;
      }

      int wValueB = 0;
      if (mGpioB.getValue() == Value::High)
      {
        wValueB = 1;
      }

      int wSequence = (wValueA ^ wValueB) | wValueB << 1;
      int wResult = (wSequence - mPreviousSequence) % 4;
      mPreviousSequence = wSequence;
      
      switch(wResult)
      {
        case 1: mDirection = EncoderDirection::Clockwise; break;
        case 3: mDirection = EncoderDirection::CounterClockwise; break;
      }
      
      mDirection = EncoderDirection::NoChange;

      return mDirection;
    }
  
    EncoderDirection::Type getDirection() const
    {
      return mDirection;
    }

  private:
    GpIoElement mGpioA;
    GpIoElement mGpioB;
    int mPreviousSequence;
    EncoderDirection::Type mDirection;
    
  };
  
  RotaryEncoder::RotaryEncoder(std::string const & gpioA, std::string const & gpioB)
   : mImplementation(new RotaryEncoderImpl(gpioA, gpioB))
  {
  }

  RotaryEncoder::~RotaryEncoder()
  {
    delete mImplementation;
  }

  EncoderDirection::Type RotaryEncoder::update()
  {
    return mImplementation->update();
  }

  EncoderDirection::Type RotaryEncoder::getDirection() const
  {
    return mImplementation->getDirection();
  }
}
