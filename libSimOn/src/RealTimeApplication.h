#pragma once

namespace SimOn
{
  class RealTimeApplicationImpl;

  class RealTimeApplication
  {
    public:
      static void initialize();
      static bool isTerminated();

    private:
      RealTimeApplication();
  };  
}
