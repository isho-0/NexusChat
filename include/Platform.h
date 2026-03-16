#ifndef NEXUSCHAT_PLATFORM_H_
#define NEXUSCHAT_PLATFORM_H_

#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif

using SocketHandle = SOCKET;
constexpr SocketHandle INVALID_SOCKET_HANDLE = INVALID_SOCKET;
constexpr int SOCKET_ERROR_CODE = SOCKET_ERROR;

inline int CloseSocket(SocketHandle sock) { return closesocket(sock); }

#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using SocketHandle = int;
constexpr SocketHandle INVALID_SOCKET_HANDLE = -1;
constexpr int SOCKET_ERROR_CODE = -1;

inline int CloseSocket(SocketHandle sock) { return close(sock); }

#endif

#endif  // NEXUSCHAT_PLATFORM_H_
