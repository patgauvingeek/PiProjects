#include <iostream>
#include <vector>
#include <thread>

#include "RealTimeApplication.h"
#include "PwmElement.h"

int main(int argc, char *argv[])
{
  //
  // create strings in a vector for every arguments
  //
  std::vector<std::string> wArgs;
  for(auto i = 0; i < argc; i++)
  {
    wArgs.emplace_back(argv[i]);
  }
  
  //
  // Parse command line arguments
  //
  auto wTempo = 800;
  for (auto i = wArgs.begin(); i < wArgs.end(); i++)
  {
    if (*i == "--tempo")
    {
      i++;
      wTempo = atoi((*i).c_str());
    }
  }
  auto wSleep = wTempo / 8; // to be able to control fast note.

  //
  // Start playing music !
  //
  SimOn::RealTimeApplication::initialize();

  SimOn::PwmElement wPwm("0");
  wPwm.setFrequency(440);
  wPwm.start();
  while(!SimOn::RealTimeApplication::isTerminated())
  {
    
    std::this_thread::sleep_for(std::chrono::milliseconds(wSleep));
  }
  wPwm.stop();
}