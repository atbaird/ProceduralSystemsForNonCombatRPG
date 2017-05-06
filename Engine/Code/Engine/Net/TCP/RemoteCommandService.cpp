#include "Engine/Net/TCP/RemoteCommandService.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/StringUtils.hpp"

size_t RemoteCommandService::m_framesBeforeConsiderConnectionDisconnected = 500;
const char* RemoteCommandService::s_DisconnectedMessage = " disconnected.";
const char* RemoteCommandService::s_ConnectedMessage = " has connected.";
RemoteCommandService RemoteCommandService::g_globalCommandService;

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Constructors
RemoteCommandService::RemoteCommandService()
	: m_host(nullptr),
	m_client(nullptr),
	m_currentlyAvailableClientID(0),
	m_echoCommands(false)
{
	m_on_connection_join.RegisterMethod(this, &RemoteCommandService::Add_Connection);
	m_on_connection_leave.RegisterMethod(this, &RemoteCommandService::Remove_Connection);
	m_on_message.RegisterMethod(this, &RemoteCommandService::OnConnectionMSG);
	m_on_registered_message_try_send.RegisterMethod(this, &RemoteCommandService::RegisteredMessageTryToSend);
	m_on_register_message_to_send.RegisterMethod(this, &RemoteCommandService::RegisterMessageToSend);
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Hosting and Joining
const bool RemoteCommandService::Host(const char* ip_add, const char* host_name)
{
	if (m_host != nullptr && m_host->Is_Listening() == true)
	{
		return false;
	}
	if (m_client != nullptr)
	{
		m_client->Disconnect();
		delete m_client;
	}
	m_currentlyAvailableClientID = 0;
	m_host = new TCPListener(host_name, ip_add);
	return m_host->Is_Listening();
}

const bool RemoteCommandService::Join(const char* ip_add, const char* host_name)
{
	if (m_client != nullptr)
	{
		m_client->Disconnect();
		delete m_client;
	}
	m_currentlyAvailableClientID = 0;
	m_client = new TCPConnection(host_name, ip_add, true);

	return true;
	//return client->Is_Connected();
}

const char* RemoteCommandService::GetLocalHostName()
{
	return NetworkSystem::GetLocalHostName();
}

void RemoteCommandService::SendCommand(byte msg_id, const char* msg)
{
	for (auto con : g_globalCommandService.m_connections)
	{
		//TCPConnection* c = con;
		RSC* c = con;
		c->Send(&msg_id, 1);
		c->Send(msg, (byte)strlen(msg));
		char nil = NULL;
		c->Send(&nil, 1);
	}
}

void RemoteCommandService::Step()
{
	{
		CheckForDisconnection();
		CheckForConnection();
		CheckForMessages();
		SendMessages();
	}
}

void RemoteCommandService::StopHosting()
{
	if (m_host != nullptr && m_host->Is_Listening())
	{
		for (size_t i = 0; i < m_connections.size(); i++)
		{
			m_on_connection_leave.Trigger(m_connections.at(i));
			i--;
		}
		m_host->Stop();
		delete m_host;
		m_host = nullptr;
		m_toSend.clear();
	}
}

void RemoteCommandService::LeaveServer()
{
	if (m_client != nullptr && m_client->Is_Connected())
	{
		m_client->Disconnect();
		delete m_client;
		m_client = nullptr;
		m_toSend.clear();
	}
}

void RemoteCommandService::Shutdown()
{
	for (size_t i = 0; i < m_connections.size(); i++)
	{
		m_on_connection_leave.Trigger(m_connections.at(i));
	}
	if (m_client != nullptr)
	{
		m_client->Disconnect();
		delete m_client;
		m_client = nullptr;
	}
	if (m_host != nullptr)
	{
		m_host->Stop();
		delete m_host;
		m_host = nullptr;
	}
}

void RemoteCommandService::Info()
{
	if (m_host != nullptr)
	{
		sockaddr_in my_addr = RemoteCommandService::g_globalCommandService.GetHostAddr();
		const char* my_adr = NetworkSystem::SockAddrToString((const sockaddr*)&my_addr);
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "Hosting IP: " + std::string(my_adr));
		}
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "Connected Clients:");
		}
		if (m_connections.size() == 0)
		{
			if (g_Console != nullptr)
			{
				g_Console->ConsolePrintf(UNIMPORTANT, "NONE.");
			}
		}
		else
		{
			for (size_t i = 0; i < m_connections.size(); i++)
			{
				CONN* conn = m_connections.at(i);
				if (conn != nullptr)
				{
					sockaddr_in clientAddr = conn->GetSocketAddr();
					const char* clientAdr = NetworkSystem::SockAddrToString((const sockaddr*)&clientAddr);
					if (g_Console != nullptr)
					{
						g_Console->ConsolePrintf(UNIMPORTANT, (conn->GetMyNickname() + " = " + std::string(clientAdr)));
					}
				}
			}
		}
	}
	else if (m_client != nullptr)
	{
		sockaddr_in my_addr = m_client->GetSocketAddr();
		const char* my_adr = NetworkSystem::SockAddrToString((const sockaddr*)&my_addr);
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "Connected to: " + std::string(my_adr));
		}
	}
	else
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "NONE.");
		}
	}
}

void RemoteCommandService::ToggleEchoCommands()
{
	m_echoCommands = !m_echoCommands;
}

void RemoteCommandService::MessageAll(const char* message, byte messageType, bool echo)
{
	if (m_host != nullptr)
	{
		for (size_t i = 0; i < m_connections.size(); i++)
		{
			RCSSendMessage(m_connections.at(i)->GetEditableTCPConn(), message, messageType, echo);
		}

	}
	if (m_client != nullptr)
	{
		RCSSendMessage(m_client, message, messageType, echo);
	}
}

void RemoteCommandService::RCSSendMessage(const char* hostAddr, const char* port, const char* message, byte messageType, bool echo)
{
	std::string ipAddr = std::string(hostAddr) + ":" + std::string(port);
	RCSSendMessage(ipAddr.c_str(), message, messageType, echo);
}

void RemoteCommandService::RCSSendMessage(const char* ipAddr, const char* message, byte messageType, bool echo)
{
	if (ipAddr == nullptr)
	{
		return;
	}
	if (m_host != nullptr)
	{
		for (size_t i = 0; i < m_connections.size(); i++)
		{
			TCPConnection* conn = m_connections.at(i)->GetEditableTCPConn();
			sockaddr_in addr = conn->GetSocketAddr();
			const char* adr = NetworkSystem::SockAddrToString((const sockaddr*)&addr);
			if (SimpleStrCmp(ipAddr, adr))
			{
				RCSSendMessage(conn, message, messageType, echo);
			}
		}
	}
	if (m_client != nullptr)
	{
		sockaddr_in addr = m_client->GetSocketAddr();
		const char* adr = NetworkSystem::SockAddrToString((const sockaddr*)&addr);
		if (strcmp(ipAddr, adr) == 0)
		{
			RCSSendMessage(m_client, message, messageType, echo);
		}
	}
}

void RemoteCommandService::RCSSendMessage(TCPConnection* conn, const char* message, byte messageType, bool echo)
{
	m_on_register_message_to_send.Trigger(conn, messageType, message, echo);
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Tests
void RemoteCommandService::CheckForConnection()
{
	//Test for new connections
	if (m_host != nullptr)
	{
		bool keepTesting = true;
		while (keepTesting)
		{
			TCPConnection* conn = m_host->Accept();
			if (conn == nullptr)
			{
				keepTesting = false;
				continue;
			}
			conn->SetMyNickname(std::string(NetConstants::s_defaultDebugClientName) + std::to_string(m_currentlyAvailableClientID));
			RSC* newRSC = new RSC(conn);
			m_currentlyAvailableClientID++;
			m_connections.push_back(newRSC);
			m_on_connection_join.Trigger(newRSC);
			//on_connection_join.RegisterMethod(this, &RemoteCommandService::Add_Connection);
		}
		return;
	}
	else if (m_client != nullptr)
	{

		return;
	}
}

void RemoteCommandService::CheckForMessages()
{
	if (m_host != nullptr)
	{
		for (size_t i = 0; i < m_connections.size(); i++)
		{
			CONN* conn = m_connections.at(i);
			TCPConnection* connTCP = conn->GetEditableTCPConn();
			CheckForMessageTCPConn(connTCP);
		}
	}
	else if (m_client != nullptr)
	{
		CheckForMessageTCPConn(m_client);
	}
}

void RemoteCommandService::CheckForDisconnection()
{
	if (m_host != nullptr)
	{
		for (size_t i = 0; i < m_connections.size(); i++)
		{
			if (m_connections.at(i)->Is_Connected() == false)
			{
				m_on_connection_leave.Trigger(m_connections.at(i));
				i--;
			}
		}
	}
	else if (m_client != nullptr)
	{
		//if (client->Is_Connected() == false)
		//{
		//	client->Disconnect();
		//	delete client;
		//	client = nullptr;
		//	if (g_Console != nullptr)
		//	{
		//		g_Console->ConsolePrintf(WARNING, "Lost connection to host!");
		//	}
		//}
	}
}

void RemoteCommandService::SendMessages()
{
	std::vector<TCPConnection*> previouslyEncounteredConnections;
	for (size_t i = 0; i < m_toSend.size(); i++)
	{
		//Verify Connection Exists and also this is the first message encountered for this connection
		if (m_toSend.at(i).m_connectionToSendTo == nullptr)
		{
			m_toSend.erase(m_toSend.begin() + i);
			i--;
			continue;
		}
		bool previouslyEncounteredConn = false;
		for (size_t j = 0; j < previouslyEncounteredConnections.size(); j++)
		{
			if (m_toSend.at(i).m_connectionToSendTo == previouslyEncounteredConnections.at(j))
			{
				previouslyEncounteredConn = true;
				break;
			}
		}
		if (previouslyEncounteredConn == true)
		{
			continue;
		}
		bool ConnectionExists = false;
		for (size_t j = 0; j < m_connections.size(); j++)
		{
			if (m_toSend.at(i).m_connectionToSendTo == m_connections.at(j)->GetEditableTCPConn())
			{
				ConnectionExists = true;
				break;
			}
		}
		if (m_toSend.at(i).m_connectionToSendTo == m_client)
		{
			ConnectionExists = true;
		}
		if (ConnectionExists == false)
		{
			m_toSend.erase(m_toSend.begin() + i);
			i--;
			continue;
		}
		previouslyEncounteredConnections.push_back(m_toSend.at(i).m_connectionToSendTo);

		bool succeededInSending;
		m_on_registered_message_try_send.Trigger(m_toSend.at(i), &succeededInSending);
		if (succeededInSending == true)
		{
			m_toSend.erase(m_toSend.begin() + i);
			i--;
			continue;
		}
	}
}

const bool RemoteCommandService::CurrentlyHosting()
{
	if (m_host != nullptr)
	{
		return true;
	}
	return false;
}

const bool RemoteCommandService::CurrentlyJoined()
{
	if (m_client != nullptr)
	{
		return true;
	}
	return false;
}

void RemoteCommandService::CheckForMessageTCPConn(TCPConnection* conn)
{
	if (conn == nullptr)
	{
		return;
	}
	byte code;
	size_t messageSize;
	size_t codelength = conn->Receive(&code, sizeof(byte));
	if (codelength == sizeof(byte))
	{
		bool echo = false;
		//conn->Receive(&echo, sizeof(bool));
		size_t messageSizeLength = conn->Receive(&messageSize, sizeof(size_t));
		if (messageSizeLength == sizeof(size_t) && messageSize > 0)
		{
			char* message = new char[messageSize + 1];
			conn->Receive((void*)message, messageSize);
			message[messageSize] = '\0';
			if (g_Console != nullptr)
			{
				size_t numOfMessagesCurrently = g_Console->GetNumberOfCommandPromptLines();
				if (code == 3)
				{
					//Console command
					g_Console->ConsolePrintf(UNIMPORTANT, "Host : CONSOLE COMMAND");
					g_Console->ConsolePrintf(UNIMPORTANT, std::string(message));
					numOfMessagesCurrently = g_Console->GetNumberOfCommandPromptLines();
					g_Console->RunCommand(std::string(message));
				}
				else
				{
					g_Console->ConsolePrintf(UNIMPORTANT, "Host: " + std::string(message));
				}
				size_t newNumOfMessages = g_Console->GetNumberOfCommandPromptLines();
				if (echo == true && g_Console != nullptr)
				{
					std::vector<std::string> messages = g_Console->GetMessages(numOfMessagesCurrently, newNumOfMessages);
					for (size_t i = 0; i < messages.size(); i++)
					{
						RCSSendMessage(conn, messages.at(i).c_str(), (byte)1, false);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Getters
sockaddr_in RemoteCommandService::GetHostAddr() const
{
	if (m_host == nullptr)
	{
		return sockaddr_in();
	}
	return m_host->GetSocketAddr();
}

bool RemoteCommandService::GetEchoCommands() const
{
	return m_echoCommands;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//static methods
void RemoteCommandService::BeginNetworking()
{
	NetworkSystem::NetSystemStartup();
}

void RemoteCommandService::EndNetworking()
{
	g_globalCommandService.Shutdown();
	NetworkSystem::NetSystemShutdown();
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Events
void RemoteCommandService::Add_Connection( CONN* conn)
{
	//g_globalCommandService.on_message.RegisterMethod(this, (&RemoteCommandService::OnConnectionMSG));
	m_on_message.Trigger(conn, 1, s_ConnectedMessage);
}

void RemoteCommandService::OnConnectionMSG(CONN* conn, const byte id, const char* msg)
{
	id;
	//on_message.Trigger(conn, id, msg);
	if (conn == nullptr)
	{
		return;
	}
	//SOCKET* socket = conn->GetSocketAsPtr();
	sockaddr_in addr = conn->GetSocketAddr();
	const char* adr = NetworkSystem::SockAddrToString((sockaddr*)&addr);
	if (g_Console != nullptr)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, std::string(adr) + " (" + std::string(conn->GetMyNickname()) + ")" + std::string(msg));
	}
}

void RemoteCommandService::Remove_Connection(CONN* conn)
{
	if (conn == nullptr)
	{
		return;
	}
	int error = WSAGetLastError();
	int idx = -1;
	for (size_t i = 0; i < m_connections.size(); i++)
	{
		if (m_connections.at(i) == conn)
		{
			idx = i;
			break;
		}
	}
	if (idx == -1)
	{
		return;
	}
	sockaddr_in* addr = conn->GetSocketAddrAsPtr();
	const char* adr = NetworkSystem::SockAddrToString((sockaddr*)addr);
	adr;
	m_on_message.Trigger(conn, 1, s_DisconnectedMessage);
	if (g_Console != nullptr)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, ("Error Code: ") + std::to_string(error));
	}
	conn->Disconnect();
	m_connections.erase(m_connections.begin() + idx);
	delete conn;
}

void RemoteCommandService::RegisteredMessageTryToSend(TCPMessage message, bool* succeeded)
{
	*succeeded = false;
	if (message.m_connectionToSendTo == nullptr)
	{
		*succeeded = true;
		return;
	}
	size_t messageSize = strlen(message.m_message.c_str());
	message.m_connectionToSendTo->Send(&message.m_messageType, sizeof(byte));
	//message.connectionToSendTo->Send(&message.echo, sizeof(bool));
	message.m_connectionToSendTo->Send(&messageSize, sizeof(size_t));
	message.m_connectionToSendTo->Send(message.m_message.c_str(), messageSize);
	char nil = NULL;
	message.m_connectionToSendTo->Send(&nil, 1);
	*succeeded = true;
}

void RemoteCommandService::RegisterMessageToSend(TCPConnection* conn, byte code, const std::string& message, bool echo)
{
	if (conn == nullptr)
	{
		return;
	}
	TCPMessage tcpmessage;
	tcpmessage.m_connectionToSendTo = conn;
	tcpmessage.m_message = message;
	tcpmessage.m_messageType = code;
	tcpmessage.m_echo = echo;
	m_toSend.push_back(tcpmessage);
}