#ifndef NEXUSCHAT_SERVER_H_
#define NEXUSCHAT_SERVER_H_

#include <WS2tcpip.h>
#include <winsock2.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

class Server {
 public:
  Server(int port);
  ~Server();

  void Start();
  void Stop();

 private:
  void HandleClient(SOCKET client_socket);

  int port_;
  SOCKET listen_socket_;
  bool running_;
  std::vector<SOCKET> client_sockets_;
  std::mutex clients_mutex_;
};

#endif  // NEXUSCHAT_SERVER_H_
