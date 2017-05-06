#include "Engine/Net/UDP/UDPNetworkSystemConsoleCommands.hpp"
#include "Engine/Net/UDP/NetSession.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/StringUtils.hpp"

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Register Console Commands
STATIC void UDPNetworkSystemConsoleCommands::RegisterConsoleCommands()
{
	//RegisterCommandHelper(const std::string& name = "", console_command_cb cb = nullptr, console_command_cb helpMessage = DefaultHelpMessage);
	//std::string, console_command_cb, console_command_cb
	//console_command_cb == typedef void(*console_command_cb)(Command&);
	//static RegisterCommandHelper _NetListTCPAddresses("NetListUDPAddresses", ConsoleCommandListUDPAddresses, HelpConsoleCommandListUDPAddresses);
	
	static RegisterCommandHelper _UDPStartSession("UDPStartSession", ConsoleCommandUDPStartSession, HelpConsoleCommandUDPStartSession);
	static RegisterCommandHelper _UDPEndSession("UDPEndSession", ConsoleCommandUDPEndSession, HelpConsoleCommandUDPEndSession);
	static RegisterCommandHelper _UDPMessageAll("UDPPingIP", ConsoleCommandUDPPingIP, HelpConsoleCommandUDPPingIP);
	static RegisterCommandHelper _UDPSetDropRate("UDPSetDropRate", ConsoleCommandUDPSetPacketDropRate, HelpConsoleCommandUDPSetPacketDropRate);
	static RegisterCommandHelper _UDPSetSimLag("UDPSetSimLag", ConsoleCommandUDPSetPacketSimLag, HelpConsoleCommandUDPSetPacketSimLag);
	static RegisterCommandHelper _UDPToggleConnectionTimeOuts("UDPToggleConnectionTimeOuts", ConsoleCommandUDPToggleConnectionTimeOut, HelpConsoleCommandUDPToggleConnectionTimeOut);
	static RegisterCommandHelper _CreateConnection("CreateConnection", ConsoleNetSessionCreateConnection, HelpConsoleNetSessionCreateConnection);
	static RegisterCommandHelper _DestroyConnection("DestroyConnection", ConsoleNetSessionDestroyConnection, HelpConsoleNetSessionDestroyConnection);
	static RegisterCommandHelper _NetHost("NetHost", ConsoleNetSessionHost, HelpConsoleNetSessionHost);
	static RegisterCommandHelper _NetJoin("NetJoin", ConsoleNetSessionJoin, HelpConsoleNetSessionJoin);
	static RegisterCommandHelper _NetLeave("NetLeave", ConsoleNetSessionLeave, HelpConsoleNetSessionLeave);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Console Commands
STATIC void UDPNetworkSystemConsoleCommands::ConsoleCommandUDPStartSession(Command& com)
{
	std::string portStr = com.get_string_by_index(0);
	std::string minPortStr = com.get_string_by_index(1);
	std::string maxPortStr = com.get_string_by_index(2);
	if (SimpleStrCmp(portStr, "") == true)
	{
		portStr = std::string(NetConstants::s_defaultPort);
		minPortStr = std::string(NetConstants::s_minPortDefault);
		maxPortStr = std::string(NetConstants::s_maxPortDefault);
	}
	else
	{
		if (SimpleStrCmp(minPortStr, "") == true)
		{
			minPortStr = std::string(NetConstants::s_minPortDefault);
			maxPortStr = std::string(NetConstants::s_maxPortDefault);
		}
		else
		{
			if (SimpleStrCmp(maxPortStr, "") == true)
			{
				maxPortStr = std::string(NetConstants::s_maxPortDefault);
			}
		}
	}
	bool successfullyStarted = NetSession::NetSessionCreateSocketAndDontCareAboutHostingOrJoining(portStr.c_str(), minPortStr.c_str(), maxPortStr.c_str());
	if (successfullyStarted)
	{
		sockaddr_in nonPtrAddr = NetSession::g_NetSession->GetSocketAddress();
		sockaddr* myAddr = (sockaddr*)&nonPtrAddr;
		const char* car = NetworkSystem::SockAddrToString(myAddr);
		g_Console->ConsolePrintf(UNIMPORTANT, "Successfully started UDP Socket at: " +
			std::string(car));
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Failed to start a UDP Socket");

	}
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleCommandUDPEndSession(Command& com)
{
	com;
	bool shutnetworkDown = NetSession::NetSessionEnd();

	if (shutnetworkDown)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "UDP NetSession closed.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "UDP NetSession was not active in the first place.");
	}
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleCommandUDPPingIP(Command& com)
{
	if (NetSession::g_NetSession == nullptr ||
		!NetSession::g_NetSession->IsRunning())
	{
		return;
	}
	std::string ip = com.get_string_by_index(0);
	size_t numPeriods = 0;
	const char* ipAddr = NetworkSystem::GetHostAddressFromIP(ip.c_str(), numPeriods);
	const char* ipPort = NetworkSystem::GetServiceFromIP(ip.c_str());
	if (ipAddr == nullptr || ipPort == nullptr)
	{
		g_Console->ConsolePrintf(WARNING, "Can NOT send a message to an invalid/incomplete IP");
		g_Console->ConsolePrintf(WARNING, "address!");
		if (ipAddr == nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "The IP address is invalid.");
		}
		else
		{
			delete ipAddr;
		}
		if (ipPort == nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "The IP port is invalid.");
		}
		else
		{
			delete ipPort;
		}
		return;
	}
	if (numPeriods != 3)
	{
		g_Console->ConsolePrintf(WARNING, "Did not send, as IP is not valid!");
		delete ipAddr;
		delete ipPort;
		return;
	}
	std::string pingMsg = com.rebuild_string_from_variables(1, com.get_number_of_variables() -1);
	if (strlen(pingMsg.c_str()) > MTU_MSG)
	{
		g_Console->ConsolePrintf(WARNING, "Message not sent! it is too long.");
		return;
	}
	NetMessage msg = NetMessage(NETMSG_PING);
	msg.WriteString(pingMsg.c_str());
	g_Console->ConsolePrintf(UNIMPORTANT, "Sending Message to" + ip);
	sockaddr_in ipaddr = NetworkSystem::StringToSockAddrIPv4(ipAddr, (uint16_t)atoi(ipPort));
	bool sent = NetSession::g_NetSession->SendMessageDirect(ipaddr, msg);
	if (sent == false)
	{
		g_Console->ConsolePrintf(WARNING, "Failed to send data, likely invalid IP");
	}

	delete ipAddr;
	delete ipPort;
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleCommandUDPSetPacketDropRate(Command& com)
{
	std::string str = com.get_string_by_index(0);
	if (SimpleStrCmp(str, "") == true)
	{
		g_Console->ConsolePrintf(WARNING, "Did not change drop rate; drop rate value not set.");
		return;
	}
	float val = (float)atof(str.c_str());
	PacketChannel::SetPacketSimulatedDropRate(val);
	g_Console->ConsolePrintf(UNIMPORTANT, "Drop rate changed to " + std::to_string(val));
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleCommandUDPSetPacketSimLag(Command& com)
{
	std::string strMin = com.get_string_by_index(0);
	std::string strMax = com.get_string_by_index(1);
	if (SimpleStrCmp(strMin, "") == true)
	{
		g_Console->ConsolePrintf(WARNING, "Sim Lag not changed; no minimum range set.");
		return;
	}
	if (SimpleStrCmp(strMax, "") == true)
	{
		g_Console->ConsolePrintf(WARNING, "Sim Lag not changed; no maximum range set.");
		return;
	}
	double min = atof(strMin.c_str());
	double max = atof(strMax.c_str());
	PacketChannel::SetPacketSimulatedAdditionalLag(Range<double>(min, max));
	g_Console->ConsolePrintf(UNIMPORTANT, "Sim Lag set to " + std::to_string(min) + " ~ " + std::to_string(max));
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleCommandUDPToggleConnectionTimeOut(Command& com)
{
	com;
	bool enabled = NetSession::ToggleEnableConnectionTimeOuts();
	if (g_Console != nullptr)
	{
		if (enabled)
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "Connection Time Out enabled!");
		}
		else
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "Connection Time Out disabled!");
		}
	}
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleNetSessionCreateConnection(Command& com)
{
	if (NetSession::g_NetSession == nullptr)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! Game Net session is null.");
		}
		return;
	}
	if (NetSession::g_NetSession->IsRunning() == false)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! Game Net Session is not running.");
		}
		return;
	}
	//<idx> <guid> address
	std::string idxStr = com.get_string_by_index(0);
	std::string guidStr = com.get_string_by_index(1);
	std::string IPStr = com.get_string_by_index(2);

	//Idx test
	if (idxStr.size() == 0)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "idx not set.");
		}
		return;
	}
	uint8_t idx = (uint8_t)atoi(idxStr.c_str());
	int max = NetSession::g_NetSession->GetMaxNumOfConnectionsAllowed() - 1;
	if (idx < 0 || idx > max)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! wanted index is not in valid range.");
			g_Console->ConsolePrintf(WARNING, "index must between 0 and " + std::to_string(max));
		}
		return;
	}
	bool isAvailableIdx = NetSession::g_NetSession->TestIfIdxIsAvailable(idx);
	if (isAvailableIdx == false)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! wanted index is not available.");
		}
		return;
	}

	//guidStr

	if (guidStr.size() == 0)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! Guid was not set.");
		}
		return;
	}
	if (guidStr.size() >= MAX_GUID_LENGTH)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! Guid is too long.");
		}
		return;
	}
	bool nameTaken = NetSession::g_NetSession->TestIfNameIsTaken(guidStr);
	if (nameTaken)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! name already taken!");
		}
		return;
	}

	//ip address
	if (IPStr.size() == 0)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! IP address not set.");
		}
		return;
	}
	size_t numPeriods;
	const char* ipaddr = NetworkSystem::GetHostAddressFromIP(IPStr.c_str(), numPeriods);
	const char* ipPort = NetworkSystem::GetServiceFromIP(IPStr.c_str());
	if (numPeriods < 3 || numPeriods > 3 || ipaddr == nullptr || ipPort == nullptr)
	{
		if (ipaddr != nullptr)
		{
			delete ipaddr;
		}
		if (ipPort != nullptr)
		{
			delete ipPort;
		}
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! ip address not valid!");
		}
		return;
	}
	if (NetSession::g_NetSession->TestIfIPAlreadyExists(IPStr.c_str()) == true)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not created! IP address already assigned a connection");
		}
		return;
	}

	sockaddr_in addr = NetworkSystem::StringToSockAddrIPv4(ipaddr, (uint16_t)atoi(ipPort));
	UDPNetConnection* conn = NetSession::g_NetSession->CreateUDPNetConnectionFromSockAddr(addr);
	conn->SetGuid(guidStr.c_str());
	conn->m_index = idx;
	NetSession::g_NetSession->AddConnection(conn, idx);

	delete ipaddr;
	delete ipPort;
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleNetSessionDestroyConnection(Command& com)
{
	if (NetSession::g_NetSession == nullptr)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not destroyed! Game Net session is null.");
		}
		return;
	}
	if (NetSession::g_NetSession->IsRunning() == false)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not destroyed! Game Net Session is not running.");
		}
		return;
	}
	//<idx>
	std::string idxStr = com.get_string_by_index(0);
	if (idxStr.size() == 0)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Connection not destroyed! idx not set.");
		}
		return;
	}
	uint8_t idx = (uint8_t)atoi(idxStr.c_str());
	NetSession::g_NetSession->DestroyConnection(idx);
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleNetSessionHost(Command& com)
{
	if (NetSession::g_NetSession == nullptr)
	{
		g_Console->ConsolePrintf(WARNING, "Host failed, session not created.");
		return;
	}
	std::string guid = com.get_string_by_index(0);
	if (SimpleStrCmp(guid, "") == true
		|| guid.length() > MAX_GUID_LENGTH)
	{
		g_Console->ConsolePrintf(WARNING, "Host failed, invalid username.");
		return;
	}
	if (NetSession::g_NetSession->GetCurrentSessionState() != DISCONNECTED_NETSESSION)
	{
		g_Console->ConsolePrintf(WARNING, "Host failed, session in incorrect state.");
		return;
	}
	bool succeeded = NetSession::g_NetSession->NetSessionHost(guid.c_str(), NetConstants::s_localHost, NetConstants::s_defaultPort);
	if (succeeded == true)
	{
		const char* ip = NetworkSystem::SockAddrToString((sockaddr*)&(NetSession::g_NetSession->hostConnection->address));
		g_Console->ConsolePrintf(UNIMPORTANT, "Hosting succeeded. IP Address is: " + std::string(ip));
	}
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleNetSessionJoin(Command& com)
{
	if (NetSession::g_NetSession == nullptr)
	{
		g_Console->ConsolePrintf(WARNING, "Join failed, session not created.");
		return;
	}
	std::string guid = com.get_string_by_index(0);
	if (SimpleStrCmp(guid, "") == true
		|| guid.length() > MAX_GUID_LENGTH)
	{
		g_Console->ConsolePrintf(WARNING, "Join failed, invalid username.");
		return;
	}

	std::string ipAddr = com.get_string_by_index(1);
	size_t numPeriods = 0;
	const char* joinAddr = NetworkSystem::GetHostAddressFromIP(ipAddr.c_str(), numPeriods);
	const char* joinPort = NetworkSystem::GetServiceFromIP(ipAddr.c_str());

	if(numPeriods != 3
		|| joinAddr == nullptr
		|| joinPort == nullptr)
	{
		if (joinAddr != nullptr)
		{
			delete joinAddr;
		}
		if (joinPort != nullptr)
		{
			delete joinPort;
		}
		g_Console->ConsolePrintf(WARNING, "Failed to Join, Invalid IP for joining.");
		return;
	}

	std::string ipAddr2 = com.get_string_by_index(2);

	if (SimpleStrCmp(ipAddr, ipAddr2) == true)
	{
		g_Console->ConsolePrintf(WARNING, "Failed to Join, can not connect to self!");
		delete joinAddr;
		delete joinPort;
		return;
	}

	const char* hostAddr = NetworkSystem::GetHostAddressFromIP(ipAddr2.c_str(), numPeriods);
	const char* hostPort = NetworkSystem::GetServiceFromIP(ipAddr2.c_str());
	if (numPeriods != 3
		|| hostAddr == nullptr
		|| hostPort == nullptr)
	{
		if (hostAddr != nullptr)
		{
			delete hostAddr;
		}
		if (hostPort != nullptr)
		{
			delete hostPort;
		}
		delete joinAddr;
		delete joinPort;
		g_Console->ConsolePrintf(WARNING, "Failed to Join, Invalid Host IP address.");
		return;
	}



	NetSession::g_NetSession->NetSessionJoin(guid.c_str(), joinAddr, joinPort, hostAddr, hostPort);
	delete hostAddr;
	delete hostPort;
	delete joinAddr;
	delete joinPort;
}

STATIC void UDPNetworkSystemConsoleCommands::ConsoleNetSessionLeave(Command& com)
{
	com;
	if (NetSession::g_NetSession == nullptr)
	{
		g_Console->ConsolePrintf(WARNING, "Did not leave, NetSession is null!");
		return;
	}
	if (NetSession::g_NetSession->GetCurrentSessionState() != CONNECTED_NETSESSION)
	{
		g_Console->ConsolePrintf(WARNING, "Did not leave, Netsession not in correct state.");
		return;
	}

	NetSession::g_NetSession->NetSessionLeave();
	g_Console->ConsolePrintf(UNIMPORTANT, "Left Connection");
}



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Help Console Commands
STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleCommandUDPStartSession(Command& com)
{
	com;
	//const char* port, const char* minPort = NetConstants::s_minPortDefault, const char* maxPort = NetConstants::s_maxPortDefault);
	g_Console->ConsolePrintf(UNIMPORTANT, "Takes a port, minPort, and maxPort so as to try and create a UDPSocket");
	g_Console->ConsolePrintf(UNIMPORTANT, "That follows those specifications");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleCommandUDPEndSession(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "Ends the UDP connection.");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleCommandUDPPingIP(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "takes a full IP address and tries to send the ping message to it.");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleCommandUDPSetPacketDropRate(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "takes a percentage and sets the simulated drop rate to it.");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleCommandUDPSetPacketSimLag(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "takes a minimum double and a maximum double and sets ");
	g_Console->ConsolePrintf(UNIMPORTANT, "the simulated lag to that range.");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleCommandUDPToggleConnectionTimeOut(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "Enable or Disables Net Connection time outs.");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleNetSessionCreateConnection(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "takes in a network ID, a screen name (no spaces), and an IP address.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Then it attempts to create a Connection with it, and uses that to start");
	g_Console->ConsolePrintf(UNIMPORTANT, "listening to the given IP");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleNetSessionDestroyConnection(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "Takes in a network ID, and destroys the connection that is tied to it.");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleNetSessionHost(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "Takes in a desired username, and attempts to host at the first available");
	g_Console->ConsolePrintf(UNIMPORTANT, "local ip address");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleNetSessionJoin(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "Takes in a desired username, and a full host ip address.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Then attempts to connect with said ip address, while setting up ");
	g_Console->ConsolePrintf(UNIMPORTANT, "the session\'s own ip at the first available address and port");
}

STATIC void UDPNetworkSystemConsoleCommands::HelpConsoleNetSessionLeave(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "If the NetSession is in the Connected state, causes the NetSession to ");
	g_Console->ConsolePrintf(UNIMPORTANT, "switch to the Disconnected state.");
}