#include "RfIdSensor.h"

#include <sstream>

namespace SimOn
{
  class RfIdSensorImpl
  {
    public:
      // RMD6300 device is "/dev/ttyAMA0"
      // RMD6300 Baud Rate is 9600
      RfIdSensorImpl(std::string const & device, int baudRate)
        : mDevice(device)
        , mBaudRate(baudRate)
      {
        std::ostringstream wCommandStream;
        wCommandStream << "stty -F " << device << " sane raw pass8 -echo -hupcl clocal " << baudRate;
        system(wCommandStream.str().c_str());
      }

    private:
      std::string mDevice;
      int mBaudRate;
  };

  RfIdSensor::RfIdSensor(std::string const & device, int baudRate)
    : mImplementation(new RfIdSensorImpl(device, baudRate))
  {
  }  

  RfIdSensor::~RfIdSensor()
  {
    delete mImplementation;
  }

}
