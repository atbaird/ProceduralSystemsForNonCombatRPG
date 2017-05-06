#include "Engine/Net/TCP/TCPConnection.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


//*!*----------------------------------------------------------------------------
//*!*----------------------------------------------------------------------------
//TCPConnection

//-----------------------------------------------------------
//-----------------------------------------------------------
//Constructors
TCPConnection::TCPConnection(SOCKET s, sockaddr_in& addr, bool isToAHost) //Just set
	: my_socket(s),
	my_addr(addr),
	m_isToAHost(isToAHost),
	m_framesSinceLastDetectedConnected(0),
	m_disconnectMe(false)
{
}
TCPConnection::TCPConnection(const char* host, const char* port, bool isToAHost) // Socket Connect
	: m_isToAHost(isToAHost),
	m_framesSinceLastDetectedConnected(0),
	m_disconnectMe(false)
{
	SocketTCPJoin(host, port);
	//setsockopt(SO_BROADCAST);
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//Connection
void TCPConnection::Disconnect() // close socket
{
	NetworkSystem::CloseSocket(my_socket);
}
bool TCPConnection::Is_Connected() // socket != INVALID_SOCKET
{

	int result = WSAPoll((LPWSAPOLLFD)my_socket, 1, 300);
	if (result == 0)
	{
		//time out
		return false;
	}
	else if (result == -1)
	{
		//socket error
		int errorCode = WSAGetLastError();
		if (errorCode != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;

	//http://stackoverflow.com/questions/851654/how-can-i-check-is-a-socket-is-still-open
	//int flags = MSG_PEEK;
	//char buf;
	//
	//int err = recv(my_socket, &buf, 1, flags);
	//if (err == SOCKET_ERROR)
	//{
	//	int errorCode = WSAGetLastError();
	//	if (errorCode != WSAEWOULDBLOCK)
	//	{
	//		return false;
	//	}
	//}
	//return true;
	//int flags = MSG_PEEK;
	//char data;
	//int err;
	//int one = 1;
	//int zero = 0;
	//{
	//	err = recv(my_socket, &data, 1, flags);
	//}
	//if (err == SOCKET_ERROR)
	//{
	//	int errorCode = WSAGetLastError();
	//	if (NetworkSystem::SocketErrorShouldDisconnect(errorCode))
	//	{
	//		return false;
	//	}
	//}
	//return true;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//Send/Receive
size_t TCPConnection::Send(const void* data, size_t size)// Send on socket
{
	bool disconnect = false;
	size_t messageSize = NetworkSystem::SocketSend(&disconnect, my_socket, data, size);
	if (disconnect == true)
	{
		//Disconnect.
		NetworkSystem::CloseSocket(my_socket);
	}
	return messageSize;
}
size_t TCPConnection::Receive(void* buffer, size_t buffer_size) //Recv On Socket
{
	bool disconnect = false;
	size_t messageSize = NetworkSystem::SocketReceive(&disconnect, my_socket, buffer, buffer_size);
	if (disconnect == true)
	{
		//disconnect
		NetworkSystem::CloseSocket(my_socket);
	}
	return messageSize;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//Setters
const bool TCPConnection::SetMyNickname(const std::string& nickname)
{
	if (SimpleStrCmp(nickname, "") == true)
	{
		return false;
	}
	my_nickname = nickname;
	return true;
}

void TCPConnection::IncrementFramesSinceLastConnected()
{
	m_framesSinceLastDetectedConnected++;
}

void TCPConnection::ResetFramesSinceLastConnected()
{
	m_framesSinceLastDetectedConnected = 0;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//Getters
SOCKET* TCPConnection::GetSocketAsPtr()
{
	return &my_socket;
}

sockaddr_in* TCPConnection::GetSocketAddrAsPtr()
{
	return &my_addr;
}

SOCKET TCPConnection::GetSocket() const
{
	return my_socket;
}

sockaddr_in TCPConnection::GetSocketAddr() const
{
	return my_addr;
}

const std::string TCPConnection::GetMyNickname() const
{
	return my_nickname;
}

const size_t TCPConnection::GetFramesSinceLastConnected() const
{
	return m_framesSinceLastDetectedConnected;
}

const bool TCPConnection::GetDisconnectMe() const
{
	return m_disconnectMe;
}


void TCPConnection::SocketTCPJoin(const char* addr, const char* service)
{

	// First, we don't look for AI_PASSIVE connections - we don't bind this one
	// We'll be sending to it.
	addrinfo *info_list = NetworkSystem::AllocAddressesForHost(addr, service, AF_INET, SOCK_STREAM, 0);
	if (info_list == nullptr)
	{
		return;
	}

	my_socket = INVALID_SOCKET;
	addrinfo *iter = info_list;
	while ((iter != nullptr) && (my_socket == INVALID_SOCKET))
	{
		my_socket = NetworkSystem::MakeSocket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
		if (my_socket != INVALID_SOCKET)
		{

			// Instead of binding - we call connect, which will do the TCP/IP
			// handshake.  Be aware this can block and cause a frame hickup, which
			// is fine for now.
			int result = NetworkSystem::SocketConnect(my_socket, iter->ai_addr, (int)(iter->ai_addrlen));
			if (SOCKET_ERROR != result)
			{
				u_long non_blocking = 1;
				NetworkSystem::IOctlSocket(my_socket, FIONBIO, non_blocking);

				// We do not listen on on this socket - we are not
				// accepting new connections.

				ASSERT_OR_DIE(iter->ai_addrlen == sizeof(sockaddr_in), "In SocketJoin, ai_addrlen does not equal the size of sockaddr_in!");
				//if (nullptr != out_addr)
				{
					memcpy(&my_addr, iter->ai_addr, iter->ai_addrlen);
				}
			}
			else
			{
				NetworkSystem::CloseSocket(my_socket);
				my_socket = INVALID_SOCKET;
			}
		}
		iter = iter->ai_next;
	}

	NetworkSystem::FreeAddresses(info_list);
}


//*!*----------------------------------------------------------------------------
//*!*----------------------------------------------------------------------------


