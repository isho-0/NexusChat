#include "Server.h"

#include <iostream>

Server::Server(int port)
    : port_(port), listen_socket_(INVALID_SOCKET), running_(false) {
  WSADATA wsa_data;

  int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (result != 0) {
    std::cerr << "WSAStartup failed: " << result << std::endl;
  }
}

Server::~Server() { WSACleanup(); }

void Server::Start() {
  listen_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_socket_ == INVALID_SOCKET) {
    std::cerr << "Socket creation failed!" << std::endl;
    return;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port_);

  if (bind(listen_socket_, reinterpret_cast<struct sockaddr*>(&server_addr),
           sizeof(server_addr)) == SOCKET_ERROR) {
    std::cerr << "Bind failed!" << std::endl;
    closesocket(listen_socket_);
    return;
  }

  if (listen(listen_socket_, SOMAXCONN) == SOCKET_ERROR) {
    std::cerr << "Listen failed!" << std::endl;
    closesocket(listen_socket_);
    return;
  }

  std::cout << "Server started on port " << port_
            << ". Waiting for connections..." << std::endl;
  running_ = true;

  while (running_) {
    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    SOCKET client_socket =
        accept(listen_socket_, reinterpret_cast<struct sockaddr*>(&client_addr),
               &client_addr_size);

    if (client_socket == INVALID_SOCKET) {
      std::cerr << "Accept failed!" << std::endl;
      continue;
    }

    std::cout << "New client connected!" << std::endl;

    std::thread client_thread(&Server::HandleClient, this, client_socket);
    client_thread.detach();
  }
}

void Server::HandleClient(SOCKET client_socket) {
  char buffer[1024];

  while (true) {
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (bytes_received <= 0) {
      std::cout << "Client disconnected." << std::endl;
      closesocket(client_socket);
      break;
    }

    buffer[bytes_received] = '\0';
    std::cout << "Received: " << buffer << std::endl;

    send(client_socket, buffer, bytes_received, 0);
  }
}
