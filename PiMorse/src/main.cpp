#include <iostream>
#include <thread>
#include <stdlib.h>
#include "GpIoElement.h"
#include "Message.h"

void processingWord(std::string const & word)
{
  std::cout << word << "..." << std::endl;
}

void processingLetter(char letter)
{
  std::cout << letter << ": ";
  std::cout.flush();
}

void processingUnit(char unit)
{
  std::cout << (unit == '.' ? "DOT" : "DASH");
  std::cout.flush();
}

void unitProcessed(char unit)
{
  std::cout << "-";
  std::cout.flush();
}

void letterProcessed(char letter)
{
  std::cout << std::endl;
  std::cout.flush();
}

void wordProcessed(std::string const & word)
{
  std::cout << std::endl;
  std::cout.flush();
}

int main(int argc, char *argv[])
{
  PiMorse::Message m("HELLO WORLD");

  m.setProcessingWord(&processingWord);
  m.setWordProcessed(&wordProcessed);
  m.setProcessingLetter(&processingLetter);
  m.setLetterProcessed(&letterProcessed);
  m.setProcessingUnit(&processingUnit);
  m.setUnitProcessed(&unitProcessed);

  m.send("25");

  return 0;
}
