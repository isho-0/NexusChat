#ifndef NEXUSCHAT_SERVER_H_
#define NEXUSCHAT_SERVER_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "Socket.h"

class Server {
 public:
  Server(int port);
  ~Server();

  void Start();
  void Stop();

 private:
  void HandleClient(Socket* client_socket);
  void Broadcast(const std::string& message, Socket* sender);

  int port_;
  std::unique_ptr<Socket> listen_socket_;
  bool running_;
  std::vector<std::unique_ptr<Socket>> client_sockets_;
  std::mutex clients_mutex_;
};

#endif  // NEXUSCHAT_SERVER_H_
