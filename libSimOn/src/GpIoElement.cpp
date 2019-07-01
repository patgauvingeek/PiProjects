#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "GpIoElement.h"

namespace SimOn
{
  class GpIoElementImpl
  {
    private:

#ifdef __arm__
      const std::string GPIO_FOLDER = "/sys/class/gpio/";
      // do the following commands to install "gpio"
      //   git clone git://git.drogon.net/wiringPi
      //   cd wiringPi
      //   ./build
      const std::string GPIO_PROGRAM = "/usr/local/bin/gpio"
#else
      const std::string GPIO_FOLDER = "gpio/";
      const std::string GPIO_PROGRAM = "./gpio_mock.sh";
#endif

      void writeData(std::string filename, std::string const & data, std::string action)
      {
        std::ofstream fileStream(filename);
        if (fileStream.is_open() == false)
        {
          std::stringstream msg;
          msg << "OPERATION FAILED: Unable to " << action << " GPIO" << mGpIo << ".";
          throw std::runtime_error(msg.str());
        }
        fileStream << data;
      }

      void readData(std::string filename, std::string & data, std::string action) const
      {
        std::ifstream fileStream(filename);
        if (fileStream.is_open() == false)
        {
          std::stringstream msg;
          msg << "OPERATION FAILED: Unable to " << action << " GPIO" << mGpIo << ".";
          throw std::runtime_error(msg.str());
        }
        fileStream >> data;
      }

    public:
      GpIoElementImpl(std::string const & gpio)
        : mGpIo(gpio)
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "export";
        writeData(filename.str(), gpio, "export");
      }

      virtual ~GpIoElementImpl()
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "unexport";
        writeData(filename.str(), mGpIo, "unexport");
      }

      virtual void setDirection(Direction::Type const direction)
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "gpio" << mGpIo << "/direction";
        if (direction == Direction::Input)
        {
          writeData(filename.str(), "in", "set direction to 'in' on");
        }
        else
        {
          writeData(filename.str(), "out", "set direction to 'out' on");
        }
      }

      virtual void setEdgeDetection(EdgeDetection::Type const edgeDetection)
      {
        std::stringstream filename;
        filename << GPIO_FOLDER << "gpio" << mGpIo << "/edge";

        switch(edgeDetection)
        {
          case EdgeDetection::Falling: writeData(filename.str(), "falling", "set edge to 'falling' on"); break;
          case EdgeDetection::Raising: writeData(filename.str(), "raising", "set edge to 'raising' on"); break;
          case EdgeDetection::Both:    writeData(filename.str(), "both", "set edge to 'both' on"); break;
          default:                     writeData(filename.str(), "none", "set edge to 'none' on"); break;
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

        writeData(filename.str(), stringValue, "write");
      }

      virtual Value::Type getValue() const
      {
        std::stringstream filename;
        std::string value;
        filename << GPIO_FOLDER << "gpio" << mGpIo << "/value";
        readData(filename.str(), value, "read");
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
