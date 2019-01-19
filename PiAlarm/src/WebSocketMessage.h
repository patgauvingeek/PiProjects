#pragma once

#include "AlarmSystem.h"

#include <chrono>
#include <sstream>
#include <string>

namespace PiAlarm
{
  template <typename T>
  void convert(std::stringstream &stringStream, T field)
  {
    stringStream << "|" << field;
  }

  // specific types conversion
  template <>
  void convert(std::stringstream &stringStream, AlarmSystemState::Type field)
  {
    switch(field)
    {
      case AlarmSystemState::Unarmed: stringStream << "|unarmed"; break;
      case AlarmSystemState::Armed: stringStream << "|armed"; break;
      case AlarmSystemState::AlarmTripped: stringStream << "|tripped"; break;
    }
  }

  template <>
  void convert(std::stringstream &stringStream, std::chrono::seconds field)
  {
    stringStream << "|" << field.count();
  }

  class WebSocketMessage
  {   
    public:
      template <typename... TFields>
      static std::string create(std::string type, TFields... fields)
      {
        std::stringstream wStringStream;
        wStringStream << type;
        addFields(wStringStream, fields...);
        return wStringStream.str();
      }

    private:
      template <typename TFirst, typename... TFields>
      static void addFields(std::stringstream &stringStream, TFirst field, TFields... fields)
      {
        convert(stringStream, field);
        addFields(stringStream, fields...);
      }

      // call that will end the recursivity
      static void addFields(std::stringstream &stringStream)
      {
      }


  }; 
}