#include "Engine/Net/NetworkSystem.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Startup and Shutdown

//Net System Startup
const bool NetworkSystem::NetSystemStartup()
{
	WSADATA wsa_data;

	// Startup Winsock version 2.2
	int error = WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (error == 0)
	{
		return true;
	}
	else {
		// Error( "Failed to initialize WinSock.  Error[%u]\n", error );
		//int errorCode = WSAGetLastError();
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Failed to initialize WinSock!");
		}
		return false;
	}

}

//Net System Shutdown
void NetworkSystem::NetSystemShutdown()
{
	WSACleanup();
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Information

//Get Local Host Name
const char* NetworkSystem::GetLocalHostName()
{
	static char buffer[256];
	if (::gethostname(buffer, 256) == 0)
	{
		return buffer;
	}
	else
	{
		// failed to grab local host name - you can check the error WSAGetLastError()
		// if you're curious as to why.
		//int errorCode = WSAGetLastError();
		return NetConstants::s_localHost;
	}

}

//GetInAddr
void* NetworkSystem::GetInAddr(const sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((sockaddr_in*)sa)->sin_addr);
	}
	else
	{
		return &(((sockaddr_in6*)sa)->sin6_addr);
	}
}

//SockAddrToString
const char* NetworkSystem::SockAddrToString(const sockaddr* addr)
{
	static char buffer[256];

	// Hard coding this for sockaddr_in for brevity
	// You can make this work for IPv6 as well
	const sockaddr_in *addr_in = (const sockaddr_in*)addr;

	// inet_ntop converts an address type to a human readable string,
	// ie 0x7f000001 => "127.0.0.1"
	// GetInAddr (defined below) gets the pointer to the address part of the sockaddr
	char hostname[256];
	inet_ntop(addr_in->sin_family, GetInAddr(addr), hostname, 256);

	// Combine the above with the port.  
	// Port is stored in network order, so convert it to host order
	// using ntohs (Network TO Host Short)
	sprintf_s(buffer, 256, "%s:%u", hostname, ntohs(addr_in->sin_port));

	// buffer is static - so will not go out of scope, but that means this is not thread safe.
	return buffer;
}

sockaddr_in NetworkSystem::StringToSockAddrIPv4(char const *ip, const uint16_t port)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	return addr;
}

//Socket Error Should Disconnect
// Ignoring Non-Critical Errors
// These errors are non-fatal and are more or less ignorable.
bool NetworkSystem::SocketErrorShouldDisconnect(const int32_t error)
{
	switch (error)
	{
	case WSAEWOULDBLOCK: // nothing to do - would've blocked if set to blocking
	case WSAEMSGSIZE:    // UDP message too large - ignore that packet.
	case WSAECONNRESET:  // Other side reset their connection.
		return false;

	default:
		return true;
	}
}

STATIC bool NetworkSystem::DetermineIfSockAddrsAreTheSame(const sockaddr_in& addr1, const sockaddr_in& addr2)
{
	//const char* addr1Str = SockAddrToString((sockaddr*)&addr1);
	//std::string addr1String = std::string(addr1Str);
	//const char* addr2Str = SockAddrToString((sockaddr*)&addr2);
	//std::string addr2String = std::string(addr2Str);
	//if (SimpleStrCmp(addr1String, addr2String))
	//{
	//	return true;
	//}

	if ((addr1.sin_addr.S_un.S_addr == addr2.sin_addr.S_un.S_addr)
		&& (addr1.sin_port == addr2.sin_port))
	{
		return true;
	}
	return false;
}

const char* NetworkSystem::GetServiceFromIP(const char* IP)
{
	std::string original = std::string(IP);
	int len = strlen(IP);
	int countAfterColun = 0;
	int indexOfFirstAfterColun = -1;
	for (int i = 0; i < len; i++)
	{
		std::string test = original.substr(i, 1);
		if (indexOfFirstAfterColun == -1 && SimpleStrCmp(test, ":") == true)
		{
			indexOfFirstAfterColun = i + 1;
			break;
		}
	}
	countAfterColun = len - (indexOfFirstAfterColun);
	if (indexOfFirstAfterColun == -1 || countAfterColun == 0)
	{
		return nullptr;
	}
	char* str = new char[countAfterColun + 1];

	strcpy(str, original.substr(indexOfFirstAfterColun, countAfterColun).c_str());
	str[countAfterColun] = '\0';
	return str;
}

const char* NetworkSystem::GetHostAddressFromIP(const char* IP, size_t& numPeriods)
{
	numPeriods = 0;
	if (IP == nullptr)
	{
		return nullptr;
	}
	std::string original = std::string(IP);
	int len = strlen(IP);
	int CountBeforeHitColun = -1;
	bool lastFrameWasPeriod = false;
	for (int i = 0; i < len; i++)
	{
		std::string test = original.substr(i, 1);
		if (CountBeforeHitColun == -1 && SimpleStrCmp(test, ":") == true)
		{
			CountBeforeHitColun = i;
			break;
		}
		if (SimpleStrCmp(test, ".") == true)
		{
			numPeriods++;
			lastFrameWasPeriod = true;
		}
		else
		{
			lastFrameWasPeriod = false;
		}
	}
	if (CountBeforeHitColun <= 0)
	{
		return nullptr;
	}
	char* str = new char[CountBeforeHitColun + 1];
	strcpy(str, original.substr(0, CountBeforeHitColun).c_str());
	str[CountBeforeHitColun] = '\0';
	return str;
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Host Specific


//Alloc Addresses For Host
addrinfo* NetworkSystem::AllocAddressesForHost(const char* host,
	const char* service, // usually is the port number as a string
	int family,      // Connection Family, AF_INET (IPv4) for this assignment
	int socktype,    // Socket Type, SOCK_STREAM or SOCK_DGRAM (TCP or UDP) for this class
	int flags)  // Search flag hints, we use this for AI_PASSIVE (bindable addresses)
{
	// Also, family of AF_UNSPEC will return all address that support the 
	// sock type (so both IPv4 and IPv6 adddress).

	// Define the hints - this is what it will use
	// for determining what addresses to return
	addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = family;
	hints.ai_socktype = socktype;
	hints.ai_flags = flags;

	// This will allocate all addresses into a single linked list
	// with the head put into result.
	addrinfo *result = nullptr;
	int status = getaddrinfo(host, service, &hints, &result);
	if (status != 0) {
		/* Warning( "net", "Failed to find addresses for [%s:%s].  Error[%s]",
		host, service, gai_strerror(status) ); */
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Failed to find addresses for " + std::string(host) + ".");
		}
	}

	return result;
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Sockets

//Accept Connection
SOCKET NetworkSystem::AcceptConnection(SOCKET host_socket, sockaddr_in *out_their_addr)
{
	sockaddr_storage their_addr;
	int their_addr_len = sizeof(their_addr);

	SOCKET their_socket = ::accept(host_socket, (sockaddr*)&their_addr, &their_addr_len);
	if (their_socket != INVALID_SOCKET) 
	{

		// If you want to support IPv6, this is no longer a valid check
		if (out_their_addr != nullptr) 
		{
			ASSERT_OR_DIE(their_addr_len == sizeof(sockaddr_in), "their_addr_len in AcceptConnection does not have a size of sockaddr_in!");
			memcpy(out_their_addr, &their_addr, their_addr_len);
		}

		return their_socket;
	}
	else 
	{
		// if we fail to accept, it might be we lost
		// connection - you can check the same we we'll do it
		// for send and recv below, and potentially return 
		// that error code somehow (if you move this code into a method
		// you could disonnect directly)
		/*
		int err = WSAGetLastError();
		if (SocketErrorShouldDisconnect(err)) {
		disconnect();
		}
		*/
		int err = WSAGetLastError();
		if (SocketErrorShouldDisconnect(err))
		{
			CloseSocket(their_socket);
			their_socket = INVALID_SOCKET;
		}
	}

	return their_socket;

}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Sockets

//Make Socket
SOCKET NetworkSystem::MakeSocket(int socketFamily, int socketType, int protocol)
{
	return socket(socketFamily, socketType, protocol);
}

//Bind Socket
int NetworkSystem::BindSocket(SOCKET& socket, sockaddr* addr, int addrLength)
{
	return bind(socket, addr, addrLength);
}

//IOctlSocket
int NetworkSystem::IOctlSocket(SOCKET& socket, u_long option, u_long active)
{
	return ioctlsocket(socket, option, &active);
}

//Socket Listen
int NetworkSystem::SocketListen(SOCKET& socket, int backlog)
{
	return listen(socket, backlog);
}

//Socket Connect
int NetworkSystem::SocketConnect(SOCKET& socket, const sockaddr* addr, int addrlen)
{
	return connect(socket, addr, addrlen);
}

//Free Addresses
void NetworkSystem::FreeAddresses(addrinfo *addresses)
{
	if (nullptr != addresses)
	{
		freeaddrinfo(addresses);
	}
}

//Close Socket
void NetworkSystem::CloseSocket(SOCKET sock)
{
	closesocket(sock);
}

//Socket Send
size_t NetworkSystem::SocketSend(bool *out_should_disconnect,
	SOCKET my_socket,
	void const *data,
	size_t const data_size)
{
	*out_should_disconnect = false;
	if (my_socket != INVALID_SOCKET)
	{
		// send will return the amount of data actually sent.
		// It SHOULD match, or be an error.  
		int size = ::send(my_socket, (char const*)data, (int)data_size, 0);
		if (size < 0) {
			int32_t error = WSAGetLastError();
			if (SocketErrorShouldDisconnect(error)) {
				// If the error is critical - disconnect this socket
				*out_should_disconnect = true;
			}
		}
		else 
		{
			if ((size_t)size != data_size && g_Console != nullptr)
			{
				g_Console->ConsolePrintf(WARNING, "Socket data_size and sent message does not have the same size!");
			}
		}

		return (size_t)size;
	}
	return 0;
}

//Socket Receive
size_t NetworkSystem::SocketReceive(bool* out_should_disconnect,
	SOCKET my_socket,
	void* buffer,
	const size_t buffer_size)
{
	*out_should_disconnect = false;
	if (my_socket != INVALID_SOCKET) 
	{
		// recv will return amount of data read, should always be <= buffer_size
		// Also, if you send, say, 3 KB with send, recv may actually
		// end up returning multiple times (say, 1KB, 512B, and 1.5KB) because 
		// the message got broken up - so be sure you application watches for it
		int size = ::recv(my_socket, (char*)buffer, buffer_size, 0);
		if (size < 0) 
		{
			int32_t error = WSAGetLastError();
			if (SocketErrorShouldDisconnect(error)) 
			{
				*out_should_disconnect = true;
			}
			return 0U;
		}
		else 
		{
			return (size_t)size;
		}
	}
	else {
		return 0U;
	}
}

