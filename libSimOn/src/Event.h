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

      void operator+=(std::function<void (TSender, TArgs...)> eventHandler) const
      {
        mEventHandlers.push_back(eventHandler);
      }

      void raise(TSender sender, TArgs... args)
      {
        for(auto eventHandler : mEventHandlers)
        {
          eventHandler(sender, args...);
        }
      }

    private:
      mutable std::vector<std::function<void (TSender, TArgs...)>> mEventHandlers;

  };
}