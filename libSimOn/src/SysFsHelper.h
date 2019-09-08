#pragma once

#include <string>

namespace SimOn
{
  class SysFsHelper
  {
    public:
      static void writeData(std::string const &filename, std::string const &data, std::string const &action, std::string const &item);
      static void readData(std::string const &filename, std::string &data, std::string const &action, std::string const &item);

  };
}