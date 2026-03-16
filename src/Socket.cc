#include "Socket.h"

#include <cstring>
#include <iostream>

#ifdef _WIN32
Socket::Socket() : socket_handle_(INVALID_SOCKET_HANDLE), initialized_(false) {
  WSADATA wsa_data;
  int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (result != 0) {
    std::cerr << "WSAStartup failed: " << result << std::endl;
    return;
  }
  initialized_ = true;
}

Socket::Socket(SocketHandle handle)
    : socket_handle_(handle), initialized_(true) {}
#else
Socket::Socket() : socket_handle_(INVALID_SOCKET_HANDLE), initialized_(true) {}

Socket::Socket(SocketHandle handle)
    : socket_handle_(handle), initialized_(true) {}
#endif

Socket::~Socket() {
  Close();
#ifdef _WIN32
  if (initialized_) {
    WSACleanup();
  }
#endif
}

bool Socket::Create(int domain, int type, int protocol) {
  socket_handle_ = socket(domain, type, protocol);
  return socket_handle_ != INVALID_SOCKET_HANDLE;
}

bool Socket::Bind(const std::string& address, int port) {
  sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (address.empty() || address == "0.0.0.0") {
    addr.sin_addr.s_addr = INADDR_ANY;
  } else {
    inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
  }

  return bind(socket_handle_, reinterpret_cast<struct sockaddr*>(&addr),
              sizeof(addr)) != SOCKET_ERROR_CODE;
}

bool Socket::Listen(int backlog) {
  return listen(socket_handle_, backlog) != SOCKET_ERROR_CODE;
}

Socket* Socket::Accept() {
  sockaddr_in client_addr;
#ifdef _WIN32
  int client_addr_size = sizeof(client_addr);
#else
  socklen_t client_addr_size = sizeof(client_addr);
#endif

  SocketHandle client_handle = accept(
      socket_handle_, reinterpret_cast<struct sockaddr*>(&client_addr),
      &client_addr_size);

  if (client_handle == INVALID_SOCKET_HANDLE) {
    return nullptr;
  }

  return new Socket(client_handle);
}

int Socket::Send(const void* buffer, size_t length) {
  return send(socket_handle_, static_cast<const char*>(buffer),
              static_cast<int>(length), 0);
}

int Socket::Recv(void* buffer, size_t length) {
  return recv(socket_handle_, static_cast<char*>(buffer),
              static_cast<int>(length), 0);
}

void Socket::Close() {
  if (socket_handle_ != INVALID_SOCKET_HANDLE) {
    CloseSocket(socket_handle_);
    socket_handle_ = INVALID_SOCKET_HANDLE;
  }
}

bool Socket::IsValid() const {
  return socket_handle_ != INVALID_SOCKET_HANDLE;
}
