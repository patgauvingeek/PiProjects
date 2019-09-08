#include "GpIoElement.h"
#include "SysFsHelper.h"

#include <sstream>
#include <stdexcept>

namespace SimOn
{
  class GpIoElementImpl
  {
    private:

#ifdef __arm__
      const std::string GPIO_FOLDER = "/sys/class/gpio/";
      const std::string GPIO_PROGRAM = "/usr/local/bin/gpio";
#else
      const std::string GPIO_FOLDER = "gpio/";
      const std::string GPIO_PROGRAM = "./gpio_mock.sh";
#endif

    public:
      GpIoElementImpl(std::string const & gpio)
        : mGpIo(gpio)
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "export";
        SysFsHelper::writeData(filename.str(), gpio, "export GPIO", mGpIo);
      }

      virtual ~GpIoElementImpl()
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "unexport";
        SysFsHelper::writeData(filename.str(), mGpIo, "unexport GPIO", mGpIo);
      }

      virtual void setDirection(Direction::Type const direction)
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "gpio" << mGpIo << "/direction";
        if (direction == Direction::Input)
        {
          SysFsHelper::writeData(filename.str(), "in", "set direction to 'in' on GPIO", mGpIo);
        }
        else
        {
          SysFsHelper::writeData(filename.str(), "out", "set direction to 'out' on GPIO", mGpIo);
        }
      }

      virtual void setEdgeDetection(EdgeDetection::Type const edgeDetection)
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "gpio" << mGpIo << "/edge";

        switch(edgeDetection)
        {
          case EdgeDetection::Falling: SysFsHelper::writeData(filename.str(), "falling", "set edge to 'falling' on GPIO", mGpIo); break;
          case EdgeDetection::Raising: SysFsHelper::writeData(filename.str(), "raising", "set edge to 'raising' on GPIO", mGpIo); break;
          case EdgeDetection::Both:    SysFsHelper::writeData(filename.str(), "both", "set edge to 'both' on GPIO", mGpIo); break;
          default:                     SysFsHelper::writeData(filename.str(), "none", "set edge to 'none' on GPIO", mGpIo); break;
        }
      }

      // ground sensing is up
      // 3.3v sensing is down
      virtual void setPullResistor(PullResistor::Type const pullResistor)
      {
        std::string type = pullResistor == PullResistor::Up
          ? "down"
          : "up";
        std::stringstream command;
        command << GPIO_PROGRAM << " -g mode " << mGpIo << " " << type;
        auto error = system(command.str().c_str());
        if (error != 0)
        {
          std::stringstream message;
          message << "Error #" << error << " while executing \"" << GPIO_PROGRAM << "\" on GPIO " << mGpIo;
          throw std::runtime_error(message.str());
        }
      }

      virtual void setValue(Value::Type const value)
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "gpio" << mGpIo << "/value";

        std::string stringValue = value == Value::High ? "1" : "0";

        SysFsHelper::writeData(filename.str(), stringValue, "write GPIO", mGpIo);
      }

      virtual Value::Type getValue() const
      {
        std::stringstream filename;
        std::string value;
        filename << GPIO_FOLDER << "gpio" << mGpIo << "/value";
        SysFsHelper::readData(filename.str(), value, "read GPIO", mGpIo);
        if (value != "0")
        {
          return Value::High;
        }
        return Value::Low;
      }

      virtual std::string const & getNumber() const
      {
        return mGpIo;
      }

    private:
      std::string mGpIo;

  };

  GpIoElement::GpIoElement(std::string const & gpio)
    : mImplementation(new GpIoElementImpl(gpio))
  {
  }  

  GpIoElement::~GpIoElement()
  {
    delete mImplementation;
  }

  void GpIoElement::setDirection(Direction::Type const direction)
  {
    mImplementation->setDirection(direction);
  }

  void GpIoElement::setEdgeDetection(EdgeDetection::Type const edgeDetection)
  {
    mImplementation->setEdgeDetection(edgeDetection);
  }
  
  void GpIoElement::setPullResistor(PullResistor::Type const pullResistor)
  {
    mImplementation->setPullResistor(pullResistor);
  }

  void GpIoElement::setValue(Value::Type const value)
  {
    mImplementation->setValue(value);
  }

  Value::Type GpIoElement::getValue() const
  {
    return mImplementation->getValue();
  }

  std::string const & GpIoElement::getNumber() const
  {
    return mImplementation->getNumber();
  }

}
