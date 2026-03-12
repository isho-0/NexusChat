#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class Server {
public:
    Server(int port);
    ~Server();

    void start();
    void stop();
    

private:
    int port;
    SOCKET listenSocket;
    bool running;

    std::vector<SOCKET> clientSockets;
    std::mutex clientsMutex;

    void handleClient(SOCKET clientSocket);
};

#endif