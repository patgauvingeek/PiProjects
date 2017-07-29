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
#else
      const std::string GPIO_FOLDER = "gpio/";
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
        fileStream.close();
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
        fileStream.close();
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
