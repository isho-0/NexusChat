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

void Server::start() {
    // socket
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        return;
    }


    // add struct sockaddr_in for bind
    sockaddr_in serverAddr;                     
    serverAddr.sin_family = AF_INET;            // IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;    // IP 접근 허용
    serverAddr.sin_port = htons(port);          // 포트 번호 설정 - hton: Host To Network Short 변환
    
    // bind
    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(listenSocket);
        return;
    }

    // listen
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(listenSocket);
        return;
    }

    std::cout << "Server started on port " << port << ". Waiting for connections..." << std::endl;
    running = true;
    
    while (running) {
        // accept
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        SOCKET clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed!" << std::endl;
            continue;
        }

        std::cout << "New client connected!" << std::endl;

        std::thread clientThread(&Server::handleClient, this, clientSocket);

        clientThread.detach();
    }
}

void Server::handleClient(SOCKET clientSocket) {
    char buffer[1024];

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
        std::cout << "Client disconnected." << std::endl;
        closesocket(clientSocket);
        break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    
        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        send(clientSocket, buffer, bytesReceived, 0);
    }

}