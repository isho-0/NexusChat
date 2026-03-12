#include "Server.h"

Server::Server(int port) : port(port), listenSocket(INVALID_SOCKET), running(false) {
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
    }
}

Server::~Server() {
    WSACleanup();
}