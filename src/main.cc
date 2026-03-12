#include "Server.h"

#include <iostream>

int main() {
    Server Server(9060);

    Server.start();

    std::cout << "Server setup successful. Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}