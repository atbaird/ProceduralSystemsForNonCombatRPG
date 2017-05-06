#pragma once

#ifndef UDPSOCKET_HPP
#define UDPSOCKET_HPP
#include "Engine/Net/NetworkSystem.hpp"

class UDPSocket
{
private:
protected:
	SOCKET my_socket;
	sockaddr_in my_local_address;
public:
	//Constructors
	UDPSocket(const char* port = NetConstants::s_defaultPort, const char* minPort = NetConstants::s_minPortDefault, 
		const char* maxPort = NetConstants::s_maxPortDefault, const char* addr = NetConstants::s_localHost);
	virtual ~UDPSocket();

	//Binding
	bool Bind(const char* addr, const char* port, const char* minPort = NetConstants::s_minPortDefault, const char* maxPort = NetConstants::s_maxPortDefault);
	void Unbind();
	bool IsBound();

	//Send Recv
	size_t SendTo(sockaddr_in to_addr, void* data, size_t datalength);
	size_t RecvFrom(sockaddr_in *from_addr, void *buffer, const size_t buffer_size);

	//Getters
	SOCKET GetSocket() const;
	sockaddr_in GetAddress() const;
private:
	bool GoThroughSocketUDP(const char* addr, const char* port, const char* minPort, const char* maxPort);
	bool SocketUDP(const char* addr, const char* port);

};
#endif