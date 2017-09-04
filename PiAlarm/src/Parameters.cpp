#include "Parameters.h"

#include <regex>
#include <sstream>

namespace PiAlarm
{
  std::vector<std::string> Parameters::toVector(std::string const &parameters)
  {
    std::regex wRegex("[\\s,]+");
    std::sregex_token_iterator wIterator(parameters.begin(), parameters.end(), wRegex, -1);
    std::sregex_token_iterator wEnd;
    std::vector<std::string> wVector;
    for (; wIterator != wEnd; ++wIterator)
    {
      wVector.push_back(*wIterator);
    }
    return wVector;
  }

  std::string Parameters::toParameters(std::vector<std::string> const &vector, size_t startIndex)
  {
    std::stringstream wStream;
    for(size_t i = startIndex; i < vector.size(); ++i)
    {
      if(i > startIndex)
      {
        wStream << ", ";
      }
      wStream << vector[i];
    }
    return wStream.str();
  }

}
