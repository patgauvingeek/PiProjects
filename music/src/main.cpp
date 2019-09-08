#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
  std::vector<std::string> wArgs;

  for(auto i = 0; i < argc; i++)
  {
    wArgs.emplace_back(argv[i]);
  }
  std::cout << "music..." << std::endl;
}