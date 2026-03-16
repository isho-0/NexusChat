#ifndef NEXUSCHAT_SOCKET_H_
#define NEXUSCHAT_SOCKET_H_

#include "Platform.h"

#include <string>

class Socket {
 public:
  Socket();
  ~Socket();

  bool Create(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
  bool Bind(const std::string& address, int port);
  bool Listen(int backlog = SOMAXCONN);
  Socket* Accept();
  int Send(const void* buffer, size_t length);
  int Recv(void* buffer, size_t length);
  void Close();
  bool IsValid() const;

  SocketHandle GetHandle() const { return socket_handle_; }

 private:
  Socket(SocketHandle handle);

  SocketHandle socket_handle_;
  bool initialized_;
};

#endif  // NEXUSCHAT_SOCKET_H_
