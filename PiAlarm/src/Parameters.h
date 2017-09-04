#pragma once

#include <string>
#include <vector>

namespace PiAlarm
{
  class Parameters
  {
    public:
      static std::vector<std::string> toVector(std::string const &parameters);
      static std::string toParameters(std::vector<std::string> const &vector, size_t startIndex = 0);

  };
}
