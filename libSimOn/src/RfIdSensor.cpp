#include "RfIdSensor.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace SimOn
{
  class RfIdSensorImpl
  {
    private:
    
      // RMD6300 device is "/dev/ttyAMA0"
      // RMD6300 Baud Rate is 9600
#ifdef __arm__
      const std::string DEVICE = "/dev/ttyAMA0";
#else
      const std::string DEVICE = "./dev/ttyAMA0";
#endif
      const int DEVICE_BAUD_RATE = 9600;

    public:
      RfIdSensorImpl()
      {
        std::stringstream wCommandStream;
        wCommandStream << "stty -F " << DEVICE << " sane raw pass8 -echo -hupcl clocal " << DEVICE_BAUD_RATE;
        system(wCommandStream.str().c_str());
      }

      static uint8_t getNibble(char iAsciiChar)
      {
        if (iAsciiChar >= '0' && iAsciiChar <= '9')
        {
          return iAsciiChar - '0';
        }
        if (iAsciiChar >= 'A' && iAsciiChar <= 'F')
        {
          return iAsciiChar - 'A' + 0xA;
        }
        if (iAsciiChar >= 'a' && iAsciiChar <= 'f')
        {
          return iAsciiChar - 'a' + 0xA;
        }
        return -1;
      }

      static uint8_t convertToByte(char a, char b)
      {
        auto wByte = getNibble(a) << 4 | getNibble(b);
        return wByte;
      }

      bool read(std::string & data) const
      {
        std::ifstream wFileStream(DEVICE);
        if (wFileStream.is_open() == false)
        {
          std::stringstream msg;
          msg << "OPERATION FAILED: Unable to open device.";
          throw std::runtime_error(msg.str());
        }

        // 0x02 | RF ID (10) | check sum (2) | 0x03
        char wChar;
        if (!wFileStream.get(wChar) || wChar != 0x02)
        {
          return false;
        }
        int wChecksum = 0;
        for(int i = 0; i < 5; i++)
        {
          char wA, wB;
          if (!wFileStream.get(wA) || !wFileStream.get(wB))
          {
            return false;
          }
          wChecksum ^= convertToByte(wA, wB);
          data.push_back(wA);
          data.push_back(wB);
        }

        char wA, wB;
        if (!wFileStream.get(wA) || !wFileStream.get(wB))
        {
          return false;
        }
        
        if (!wFileStream.get(wChar) || wChar != 0x03)
        {
          return false;
        }

        if (wChecksum != convertToByte(wA, wB))
        {
          return false;
        }

        return true;        
      }
  };

  RfIdSensor::RfIdSensor()
    : mImplementation(new RfIdSensorImpl())
  {
  }

  RfIdSensor::~RfIdSensor()
  {
    delete mImplementation;
  }

  bool RfIdSensor::read(std::string & data) const
  {
    return mImplementation->read(data);
  }

}
