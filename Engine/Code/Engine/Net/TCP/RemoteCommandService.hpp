#pragma once

#ifndef REMOTECOMMANDSERVICE_HPP
#define REMOTECOMMANDSERVICE_HPP
#include "Engine/EventSystem/Event.hpp"
#include "Engine/Net/TCP/TCPConnection.hpp"
#include "Engine/Net/TCP/TCPListener.hpp"
#include "Engine/Net/TCP/RSC.hpp"



class RemoteCommandService
{
private:
protected:

	//typedef TCPConnection CONN;
	typedef RSC CONN;
	struct TCPMessage
	{
		TCPConnection* m_connectionToSendTo;
		byte m_messageType; //0 - invalid, 1 - regular message, 2 - connection test, 3 - console command
		std::string m_message;
		bool m_echo;
	};
	static size_t m_framesBeforeConsiderConnectionDisconnected;
	size_t m_currentlyAvailableClientID;
	TCPListener* m_host; //hosting
	TCPConnection* m_client; //joining
	bool m_echoCommands;
	std::vector<CONN*> m_connections;
	Event<CONN*> m_on_connection_join;
	Event<CONN*> m_on_connection_leave;
	Event<CONN*, const byte, const char*> m_on_message;
	Event<TCPMessage, bool*> m_on_registered_message_try_send;
	Event<TCPConnection*, byte, const std::string&, bool> m_on_register_message_to_send;

	std::vector<TCPMessage> m_toSend;
public:
	static const char* s_DisconnectedMessage;
	static const char* s_ConnectedMessage;
	static RemoteCommandService g_globalCommandService;

	RemoteCommandService();

	//Hosting and Joining
	const bool Host(const char* ip_add, const char* host_name = NetConstants::s_localHost);
	const bool Join(const char* ip_add, const char* host_name = NetConstants::s_localHost);
	static const char* GetLocalHostName();
	void SendCommand(byte msg_id, const char* msg);
	void Step();
	void StopHosting();
	void LeaveServer();
	void Shutdown();
	void Info();
	void ToggleEchoCommands();
	void MessageAll(const char* message, byte messageType, bool echo = false);
	void RCSSendMessage(const char* hostAddr, const char* port, const char* message, byte messageType, bool echo = false);
	void RCSSendMessage(const char* ipAddr, const char* message, byte messageType, bool echo = false);
	void RCSSendMessage(TCPConnection* conn, const char* message, byte messageType, bool echo = false);

	//Tests
	void CheckForConnection();
	void CheckForMessages();
	void CheckForDisconnection();
	void SendMessages();
	const bool CurrentlyHosting();
	const bool CurrentlyJoined();
	void CheckForMessageTCPConn(TCPConnection* conn);

	//Getters
	sockaddr_in GetHostAddr() const;
	bool GetEchoCommands() const;

	//static methods
	static void BeginNetworking();
	static void EndNetworking();

	//Events
	void Add_Connection(CONN* conn);
	void OnConnectionMSG(CONN* conn, const byte id, const char* msg);
	void Remove_Connection(CONN* conn);
	void RegisteredMessageTryToSend(TCPMessage message, bool* succeeded);
	void RegisterMessageToSend(TCPConnection* conn, byte code, const std::string& message, bool echo);

	//RCS::add_Connection(RSC* conn)
	//{
	//	conn->on_message register_method(this, &(RCS::on_conn_message));
	//}
	//RCS::on_conn_msg(RSC* cp, id, msg)
	//{
	//	ON_MESSAGE.trigger(cp, id, msg);
	//}

	/*
	static void SendCommand(byte_t msg_id, const char* msg)
	{
		foreach(con in Connections)
		{
			TCPConnection* c = com;
			c->send(&msg_id, 1);
			c->Send(msg, strlen(msg)); 
			char nil = NULL;
			c->Send(&nil, 1);
		}
	}

	*/
};
#endif