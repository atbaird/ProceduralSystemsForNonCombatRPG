#include "Engine/Net/TCP/TCPListener.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*!*----------------------------------------------------------------------------
//*!*----------------------------------------------------------------------------
//TCPListener

//-----------------------------------------------------------
//-----------------------------------------------------------
//Constructors
TCPListener::TCPListener(const char* host, const char* port, int queue_count)
	: m_disconnectMe(false)
{
	TCPListener::SocketTCPHost(queue_count, port, host);
}
TCPListener::TCPListener(const char* port, int queue_count) //
	: m_disconnectMe(false)
{
	TCPListener::SocketTCPHost(queue_count, port);
}

//-----------------------------------------------------------
//-----------------------------------------------------------
void TCPListener::Stop() // Close Socket.
{
	NetworkSystem::CloseSocket(my_socket);
}
bool TCPListener::Is_Listening() // socket is connected <-
{
	/*
	SOCKET my_socket;
	sockaddr_in my_addr;
	*/
	char val;
	socklen_t len = sizeof(val);
	if (getsockopt(my_socket, SOL_SOCKET, SO_ACCEPTCONN, &val, &len) == -1)
	{
		//Not a socket
		return false;
	}
	else if (val)
	{
		//socket is listening
		return true;
	}
	else
	{
		//socket is not listening
		return false;
	}
}

TCPConnection* TCPListener::Accept()
{
	sockaddr_in their_addr;
	SOCKET sock = NetworkSystem::AcceptConnection(my_socket, &their_addr);
	if (sock != INVALID_SOCKET)
	{
		return new TCPConnection(sock, their_addr);
	}
	return nullptr;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//Setters
const bool TCPListener::SetMyNickname(const std::string& nickname)
{
	if (SimpleStrCmp(nickname, "") == true)
	{
		return false;
	}
	my_nickname = nickname;
	return true;
}


//-----------------------------------------------------------
//-----------------------------------------------------------
//Getters
SOCKET* TCPListener::GetSocketAsPtr()
{
	return &my_socket;
}

sockaddr_in* TCPListener::GetSocketAddrAsPtr()
{
	return &my_addr;
}

SOCKET TCPListener::GetSocket()
{
	return my_socket;
}

sockaddr_in TCPListener::GetSocketAddr()
{
	return my_addr;
}

const std::string TCPListener::GetMyNickName() const
{
	return my_nickname;
}

const bool TCPListener::GetDisconnectMe() const
{
	return m_disconnectMe;
}

void TCPListener::SocketTCPHost(int queue_count, const char* port, const char* host)
{
	// First, try to get network addresses for this
	addrinfo *info_list = NetworkSystem::AllocAddressesForHost(host, // an address for this machine
		port, // service, which for TCP/IP is the port as a string (ex: "80")
		AF_INET, // We're doing IPv4 in class
		SOCK_STREAM, // TCP for now
		AI_PASSIVE);  // And something we can bind (and therefore listen on)


	if (info_list == nullptr)
	{
		// no addresses match - FAIL
		return;
	}

	// Alright, try to create a SOCKET from this addr info
	my_socket = INVALID_SOCKET;
	addrinfo *iter = info_list;
	while ((iter != nullptr) && (my_socket == INVALID_SOCKET))
	{

		// First, create a socket for this address.
		// family, socktype, and protocol are provided by the addrinfo
		// if you wanted to be manual, for an TCP/IPv4 socket you'd use
		// AF_INET, SOCK_STREAM, IPPROTO_TCP
		my_socket = NetworkSystem::MakeSocket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
		if (my_socket != INVALID_SOCKET)
		{
			// Okay, we were able to create it,
			// Now try to bind it (associates the address (ex: 192.168.1.52:4325) to this 
			// socket so it will receive information for it.
			int result = NetworkSystem::BindSocket(my_socket, iter->ai_addr, (int)(iter->ai_addrlen));
			if (SOCKET_ERROR != result)
			{

				// Set it to non-block - since we'll be working with this on our main thread
				u_long non_blocking = 1;
				NetworkSystem::IOctlSocket(my_socket, FIONBIO, non_blocking);

				// Set it to listen - this will allow people to connect to us
				result = NetworkSystem::SocketListen(my_socket, queue_count);
				ASSERT_OR_DIE(result != SOCKET_ERROR, "Error!: SOCKET ERROR!"); // sanity check

																				// Save off the address if available.
				ASSERT_OR_DIE(iter->ai_addrlen == sizeof(sockaddr_in), "ai_addrlen in host creation does not have a size equal to sockaddr_in!");
				//if (nullptr != out_addr)
				{
					memcpy(&my_addr, iter->ai_addr, iter->ai_addrlen);
				}
			}
			else
			{
				// Cleanup on Fail.
				NetworkSystem::CloseSocket(my_socket);
				my_socket = INVALID_SOCKET;
			}
		}
		iter = iter->ai_next;
	}
	// If we allocted, we must free eventually
	NetworkSystem::FreeAddresses(info_list);
}

//*!*----------------------------------------------------------------------------
//*!*----------------------------------------------------------------------------