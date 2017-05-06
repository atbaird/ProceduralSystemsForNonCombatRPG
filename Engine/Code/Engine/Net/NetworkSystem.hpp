#pragma once

#ifndef NETWORKSYSTEM_HPP
#define NETWORKSYSTEM_HPP
#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <ws2tcpip.h>
#include "Engine/Net/NetConstants.hpp"
#include <stdint.h>
typedef int int32_t;

class NetworkSystem
{
private:
protected:
public:
	//Startup and Shutdown
	static const bool NetSystemStartup();
	static void NetSystemShutdown();

	//Information
	static const char* GetLocalHostName();
	static void* GetInAddr(const sockaddr* sa);
	static const char* SockAddrToString(const sockaddr* addr);
	static sockaddr_in StringToSockAddrIPv4(char const *ip, const uint16_t port);
	static bool SocketErrorShouldDisconnect(const int32_t error);
	static bool DetermineIfSockAddrsAreTheSame(const sockaddr_in& addr1, const sockaddr_in& addr2);
	static const char* GetServiceFromIP(const char* IP);
	static const char* GetHostAddressFromIP(const char* IP, size_t& numPeriods);

	//Host specific
	static addrinfo* AllocAddressesForHost(const char* host, const char* service, int family, int socktype, int flags = 0);
	static SOCKET AcceptConnection(SOCKET host_socket, sockaddr_in* out_their_addr);
	
	//Sockets
	static SOCKET MakeSocket(int socketFamily, int socketType, int protocol);
	static int BindSocket(SOCKET& socket, sockaddr* addr, int addrLength);
	static int IOctlSocket(SOCKET& socket, u_long option, u_long active);
	static int SocketListen(SOCKET& socket, int backlog);
	static int SocketConnect(SOCKET& socket, const sockaddr* addr, int addrlen);
	static void FreeAddresses(addrinfo *addresses);
	static void CloseSocket(SOCKET sock);
	static size_t SocketSend(bool *out_should_disconnect, SOCKET my_socket, void const *data, size_t const data_size);
	static size_t SocketReceive(bool* out_should_disconnect, SOCKET my_socket, void* buffer, const size_t buffer_size);
};
#endif