#pragma once

#include <functional>
#include <vector>

namespace SimOn
{
  template <class TSender, class TArg>
  class Event
  {
    public:
      Event()
        : mEventHandlers()
      {}

      Event& operator+=(std::function<void (TSender, TArg)> eventHandler)
      {
        mEventHandlers.push_back(eventHandler);
        return *this;
      }

      void raise(TSender sender, TArg arg)
      {
        for(auto eventHandler : mEventHandlers)
        {
          eventHandler(sender, arg);
        }
      }

    private:
      std::vector<std::function<void (TSender, TArg)>> mEventHandlers;

  };
}