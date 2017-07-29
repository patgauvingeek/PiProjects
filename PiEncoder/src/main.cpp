#include <iostream>
#include "RotaryEncoder.h"

int main(int argc, char *argv[])
{
  SimOn::RotaryEncoder wEncoder("23", "24");
  int wEncoderValue = 0;

  while(true)
  {
    switch(wEncoder.update())
    {
      case SimOn::EncoderDirection::Clockwise:
      {
        wEncoderValue++;
        std::cout << wEncoderValue << std::endl;
      } break;
      case SimOn::EncoderDirection::CounterClockwise:
      {
        wEncoderValue--;
        std::cout << wEncoderValue << std::endl;
      } break;      
      case SimOn::EncoderDirection::NoChange:
      {
      } break;
    }
  }

  return 0;
}
