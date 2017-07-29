#include "RealTimeApplication.h"

#include <signal.h>

#include <iostream>

namespace SimOn
{
  bool mIsTerminated = false;

  void sigHandler(int signal)
  { 
    mIsTerminated = true;
  }

  void RealTimeApplication::initialize()
  { 
    signal(SIGTERM, sigHandler);
    signal(SIGINT, sigHandler);
    signal(SIGQUIT, sigHandler);
  }

  bool RealTimeApplication::isTerminated()
  {
    return mIsTerminated;
  }
}
