#pragma once

#ifndef TCPLISTENER_HPP
#define TCPLISTENER_HPP
#include "Engine/Net/TCP/TCPConnection.hpp"

//*!*----------------------------------------------------------------------------
class TCPListener
{
private:
	SOCKET my_socket;
	sockaddr_in my_addr;
	std::string my_nickname;
	bool m_disconnectMe;
public:
	//Constructors
	TCPListener(const char* host, const char* port, int queue_count = 2);
	TCPListener(const char* port, int queue_count = 2); //

	void Stop(); // Close Socket.
	bool Is_Listening(); // socket is connected <-

	TCPConnection* Accept();

	//Setters
	const bool SetMyNickname(const std::string& nickname);

	//Getters
	SOCKET* GetSocketAsPtr();
	sockaddr_in* GetSocketAddrAsPtr();
	SOCKET GetSocket();
	sockaddr_in GetSocketAddr();
	const std::string GetMyNickName() const;
	const bool GetDisconnectMe() const;
private:
	void SocketTCPHost(int queue_count, const char* port, const char* host = NetConstants::s_localHost);
};
//*!*----------------------------------------------------------------------------

#endif