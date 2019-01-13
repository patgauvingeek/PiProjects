#pragma once

#include <functional>
#include <vector>

namespace SimOn
{
  template <class TSender, class ... TArgs>
  class Event
  {
    public:
      Event()
        : mEventHandlers()
      {}

      Event& operator+=(std::function<void (TSender, TArgs...)> eventHandler)
      {
        mEventHandlers.push_back(eventHandler);
        return *this;
      }

      void raise(TSender sender, TArgs... args)
      {
        for(auto eventHandler : mEventHandlers)
        {
          eventHandler(sender, args...);
        }
      }

    private:
      std::vector<std::function<void (TSender, TArgs...)>> mEventHandlers;

  };
}