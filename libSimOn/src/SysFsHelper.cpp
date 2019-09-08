#include "SysFsHelper.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace SimOn
{
  void SysFsHelper::writeData(std::string const &filename, std::string const &data, std::string const &action, std::string const &item)
  {
    std::ofstream fileStream(filename);
    if (fileStream.is_open() == false)
    {
      std::stringstream msg;
      msg << "OPERATION FAILED: Unable to " << action << " " << item << ".";
      throw std::runtime_error(msg.str());
    }
    fileStream << data;
  }

  void SysFsHelper::readData(std::string const &filename, std::string &data, std::string const &action, std::string const &item)
  {
    std::ifstream fileStream(filename);
    if (fileStream.is_open() == false)
    {
      std::stringstream msg;
      msg << "OPERATION FAILED: Unable to " << action << item << ".";
      throw std::runtime_error(msg.str());
    }
    fileStream >> data;
  }

}