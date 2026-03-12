#include "Server.h"

#include <iostream>

int main() {
  Server server(9060);

  server.Start();

  std::cout << "Server setup successful. Press Enter to exit..." << std::endl;
  std::cin.get();

  return 0;
}
