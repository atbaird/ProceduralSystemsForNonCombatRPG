#include "Engine/Net/UDP/UDPSocket.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Console/Console.hpp"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors
UDPSocket::UDPSocket(const char* port, const char* minport, const char* maxPort, const char* ipAddrStr)
	: my_socket(INVALID_SOCKET),
	my_local_address(sockaddr_in())
{
	bool succeeded = GoThroughSocketUDP(ipAddrStr, port, minport, maxPort);
	succeeded;
}
UDPSocket::~UDPSocket()
{
	NetworkSystem::CloseSocket(my_socket);
	my_socket = INVALID_SOCKET;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Binding
bool UDPSocket::Bind(const char* addr, const char* port, const char* minPort, const char* maxPort)
{
	if (my_socket != INVALID_SOCKET)
	{
		return false;
	}
	Unbind();
	return GoThroughSocketUDP(addr, port, minPort, maxPort);
}

void UDPSocket::Unbind()
{
	if (IsBound() == false)
	{
		return;
	}
	NetworkSystem::CloseSocket(my_socket);
	my_socket = INVALID_SOCKET;
	
}

bool UDPSocket::IsBound()
{
	int len = sizeof(my_local_address);
	int result = getsockname(my_socket, (sockaddr*)&my_local_address, &len);
	//getsockname returns 0 if no error, and SOCKET_ERROR otherwise.
	if (result == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Send Recv
size_t UDPSocket::SendTo(sockaddr_in to_addr, void* data, size_t data_size)
{
	if (my_socket != INVALID_SOCKET) 
	{
		// send will return the amount of data actually sent.
		// It SHOULD match, or be an error.  
		//int result = NetworkSystem::BindSocket(my_socket, (sockaddr*)&my_local_address, sizeof(sockaddr_in));
		//if (result == SOCKET_ERROR)
		//{
		//	return 0;
		//}
		int size = ::sendto(my_socket,
			(char const*)data,      // payload
			(int)data_size,         // payload size
			0,                      // flags - unused
			(sockaddr const*)&to_addr, // who we're sending to
			sizeof(sockaddr_in));  // size of that structure

		if (size > 0) 
		{
			return size;
		}
	}

	// Not particularly interested in errors - you can 
	// check this though if you want to see if something
	// has happened to your socket.
	return 0;

}

size_t UDPSocket::RecvFrom(sockaddr_in *from_addr,
	void *buffer,
	const size_t buffer_size)
{
	if (my_socket != INVALID_SOCKET)
	{
		// recv will return amount of data read, should always be <= buffer_size
		// Also, if you send, say, 3 KB with send, recv may actually
		// end up returning multiple times (say, 1KB, 512B, and 1.5KB) because 
		// the message got broken up - so be sure you application watches for it

		sockaddr_storage addr;
		int addrlen = sizeof(addr);

		int size = ::recvfrom(my_socket,
			(char*)buffer,    // what we're reading into
			buffer_size,      // max data we can read
			0,                // optional flags (see docs if you're curious)
			(sockaddr*)&addr, // Who sent the message
			&addrlen);       // length of their address

		if (size > 0) 
		{
			// We're only doing IPv4 - if we got a non-IPv4 address
			// assume it's garbage
			//ASSERT_RETURN_VALUE(addrlen == sizeof(sockaddr_in), 0);

			memcpy(from_addr, &addr, addrlen);
			return size;
		}
	}

	// Again, I don't particularly care about the 
	// error code for now.  It may tell us
	// the guy we're sending to is bad, but we can't really
	// do anything with that yet. 
	return 0;

}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Getters
SOCKET UDPSocket::GetSocket() const
{
	return my_socket;
}
sockaddr_in UDPSocket::GetAddress() const
{
	return my_local_address;
}


bool UDPSocket::GoThroughSocketUDP(const char* addr, const char* port, const char* minPort, const char* maxPort)
{
	SocketUDP(addr, port);
	if (my_socket != INVALID_SOCKET)
	{
		return true;
	}
	u_long minP = atol(minPort);
	u_long maxP = atol(maxPort);
	for (u_long i = minP; i <= maxP; i++)
	{
		std::string por = std::to_string(i);
		SocketUDP(addr, por.c_str());
		if (my_socket != INVALID_SOCKET)
		{
			return true;
		}
	}
	return false;
}

bool UDPSocket::SocketUDP(const char* addr, const char* port)
{
	// First, try to get network addresses for this
	addrinfo *info_list = NetworkSystem::AllocAddressesForHost(addr, // an address for this machine
		port, // service, which for TCP/IP is the port as a string (ex: "80")
		AF_INET, // We're doing IPv4 in class
		SOCK_DGRAM, // UDP for now
		AI_PASSIVE);  // And something we can bind (and therefore listen on)

	if (info_list == nullptr)
	{
		// no addresses match - FAIL
		return false;
	}

	// Alright, try to create a SOCKET from this addr info
	my_socket = INVALID_SOCKET;
	addrinfo *iter = info_list;
	while ((iter != nullptr) && (my_socket == INVALID_SOCKET))
	{

		// First, create a socket for this address.
		// family, socktype, and protocol are provided by the addrinfo
		// if you wanted to be manual, for an TCP/IPv4 socket you'd use
		// AF_INET, SOCK_DGRAM, IPPROTO_UDP
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

				// Save off the address if available.
				ASSERT_OR_DIE(iter->ai_addrlen == sizeof(sockaddr_in), "Address lengths are not equal in SocketUDP creation!");
				//if (nullptr != out_addr)
				{
					memcpy(&my_local_address, iter->ai_addr, iter->ai_addrlen);
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

	// Return the socket we created.
	return true;
}