#pragma once

#ifndef RSC_HPP
#define RSC_HPP
#include "Engine/Net/TCP/TCPConnection.hpp"
#include "Engine/EventSystem/Event.hpp"
#include <vector>

//RemoteServiceConnection
class RSC
{
private:
protected:
	TCPConnection* m_TCPConn;
	std::vector<char> m_next_message;
public:
	Event<RSC*, byte, const char*> m_on_message;
	//Constructors
	RSC(TCPConnection* TCPConn = nullptr);
	virtual ~RSC();

	//Send/Receive
	void Send(const void* msg, byte msg_id);
	void Receive();

	//Tests
	void Disconnect(); // close socket
	bool Is_Connected(); // socket != INVALID_SOCKET

	//Setters
	TCPConnection* SetTCPConnection(TCPConnection* conn);
	void IncrementFramesSinceLastConnected();
	void ResetFramesSinceLastConnected();

	//Getters
	const TCPConnection* GetTCPConn() const;
	TCPConnection* GetEditableTCPConn() const;
	SOCKET* GetSocketAsPtr();
	sockaddr_in* GetSocketAddrAsPtr();
	SOCKET GetSocket();
	sockaddr_in GetSocketAddr();
	const std::string GetMyNickname() const;
	const size_t GetFramesSinceLastConnected() const;

	//Static Methods;
	static void OnMessage(RSC* rsc, byte messageID, const char* message);

};
#endif