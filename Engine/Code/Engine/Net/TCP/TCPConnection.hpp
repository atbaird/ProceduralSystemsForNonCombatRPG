#pragma once

#ifndef TCPCONNECTION_HPP
#define TCPCONNECTION_HPP
#include "Engine/Net/NetworkSystem.hpp"
#include <stdint.h>
#include <string>


//*!*----------------------------------------------------------------------------
enum TCPCodes
{
	InvalidTCPode,
	ConnectionTest,
	RegularMessage,
	ConsoleCommand,
	NUMBER_OF_TCPCODES
};

class TCPConnection
{
private:
	SOCKET my_socket;
	sockaddr_in my_addr;
	std::string my_nickname;
	size_t m_framesSinceLastDetectedConnected;
	bool m_isToAHost;
	bool m_disconnectMe;
public:
	//Constructors
	TCPConnection(SOCKET s, sockaddr_in& addr, bool isToAHost = false); //Just set
	TCPConnection(const char* host, const char* port, bool isToAHost = false); // Socket Connect

	//Connection
	void Disconnect(); // close socket
	bool Is_Connected(); // socket != INVALID_SOCKET

	//Send/Receive
	size_t Send(const void* data, size_t size); // Send on socket
	size_t Receive(void* buffer, size_t buffer_size); //Recv On Socket

	//Setters
	const bool SetMyNickname(const std::string& nickname);
	void IncrementFramesSinceLastConnected();
	void ResetFramesSinceLastConnected();

	//Getters
	SOCKET* GetSocketAsPtr();
	sockaddr_in* GetSocketAddrAsPtr();
	SOCKET GetSocket() const;
	sockaddr_in GetSocketAddr() const;
	const std::string GetMyNickname() const;
	const size_t GetFramesSinceLastConnected() const;
	const bool GetDisconnectMe() const;

private:
	void SocketTCPJoin(const char* addr, const char* service);

};
//*!*----------------------------------------------------------------------------


#endif