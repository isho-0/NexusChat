#include "Server.h"

#include <iostream>

Server::Server(int port)
    : port_(port), listen_socket_(std::make_unique<Socket>()), running_(false) {}

Server::~Server() {}

void Server::Start() {
  if (!listen_socket_->Create()) {
    std::cerr << "Socket creation failed!" << std::endl;
    return;
  }

  if (!listen_socket_->Bind("", port_)) {
    std::cerr << "Bind failed!" << std::endl;
    listen_socket_->Close();
    return;
  }

  if (!listen_socket_->Listen()) {
    std::cerr << "Listen failed!" << std::endl;
    listen_socket_->Close();
    return;
  }

  std::cout << "Server started on port " << port_
            << ". Waiting for connections..." << std::endl;
  running_ = true;

  while (running_) {
    Socket* client_socket = listen_socket_->Accept();

    if (client_socket == nullptr) {
      std::cerr << "Accept failed!" << std::endl;
      continue;
    }

    std::cout << "New client connected!" << std::endl;

    std::lock_guard<std::mutex> lock(clients_mutex_);
    client_sockets_.push_back(std::unique_ptr<Socket>(client_socket));

    std::thread client_thread(&Server::HandleClient, this, client_socket);
    client_thread.detach();
  }
}

void Server::HandleClient(Socket* client_socket) {
  char buffer[1024];

  while (true) {
    int bytes_received = client_socket->Recv(buffer, sizeof(buffer) - 1);

    if (bytes_received <= 0) {
      std::cout << "Client disconnected." << std::endl;
      client_socket->Close();
      break;
    }

    buffer[bytes_received] = '\0';
    std::cout << "Received: " << buffer << std::endl;

    client_socket->Send(buffer, bytes_received);
  }

  std::lock_guard<std::mutex> lock(clients_mutex_);
  client_sockets_.erase(
      std::remove_if(client_sockets_.begin(), client_sockets_.end(),
                     [client_socket](const std::unique_ptr<Socket>& sock) {
                       return sock.get() == client_socket;
                     }),
      client_sockets_.end());
}
