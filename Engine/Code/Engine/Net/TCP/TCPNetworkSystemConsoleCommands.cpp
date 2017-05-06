#include "Engine/Net/TCP/TCPNetworkSystemConsoleCommands.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Net/NetworkSystem.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Net/TCP/RemoteCommandService.hpp"

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Registeration

//Register Console Commands
void TCPNetworkSystemConsoleCommands::RegisterConsoleCommands()
{
	//RegisterCommandHelper(const std::string& name = "", console_command_cb cb = nullptr, console_command_cb helpMessage = DefaultHelpMessage);
	//std::string, console_command_cb, console_command_cb
	//console_command_cb == typedef void(*console_command_cb)(Command&);
	static RegisterCommandHelper _NetListTCPAddresses("NetListTCPAddresses", ConsoleCommandListTCPAddresses, HelpConsoleCommandListTCPAddresses);
	static RegisterCommandHelper _NetHost("NetTCPHost", ConsoleCommandTCPHost, HelpConsoleCommandTCPHost);
	static RegisterCommandHelper _NetStop("NetTCPStop", ConsoleCommandTCPStop, HelpConsoleCommandTCPStop);
	static RegisterCommandHelper _NetJoin("NetTCPJoin", ConsoleCommandTCPJoin, HelpConsoleCommandTCPJoin);
	static RegisterCommandHelper _NetLeave("NetTCPLeave", ConsoleCommandTCPLeave, HelpConsoleCommandTCPLeave);
	static RegisterCommandHelper _NetInfo("NetTCPInfo", ConsoleCommandTCPInfo, HelpConsoleCommandTCPInfo);
	static RegisterCommandHelper _NetGetLocalHostName("NetTCPGetLocalHostName", ConsoleCommandTCPGetLocalHostName, HelpConsoleCommandTCPGetLocalHostName);
	static RegisterCommandHelper _NetSendCommand("NetTCPSendCommand", ConsoleCommandTCPNetSendCommand, HelpConsoleCommandTCPNetSendCommand);
	static RegisterCommandHelper _NetMessageAll("NetTCPMessageAll", ConsoleCommandTCPMessageAll, HelpConsoleCommandTCPMessageAll);
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Console Commands

//Console Command List TCP Addresses
void TCPNetworkSystemConsoleCommands::ConsoleCommandListTCPAddresses(Command& com)
{
	com;
}

//Console Command Host
void TCPNetworkSystemConsoleCommands::ConsoleCommandTCPHost(Command& com)
{
	com;
	std::string ip = com.get_string_by_index(0);
	const char* addr = nullptr;
	const char* port = nullptr;
	bool deleteAddr = false;
	bool deletePort = false;
	if (SimpleStrCmp(ip, "") == true)
	{
		addr = NetConstants::s_localHost;
		port = NetConstants::s_defaultPort;
	}
	else
	{
		deleteAddr = true;
		deletePort = true;
		size_t numPeriods = 0;
		addr = NetworkSystem::GetHostAddressFromIP(ip.c_str(), numPeriods);
		port = NetworkSystem::GetServiceFromIP(ip.c_str());
		if (addr == nullptr)
		{
			addr = NetConstants::s_localHost;
			deleteAddr = false;
		}
		if (port == nullptr)
		{
			port = NetConstants::s_defaultPort;
			deletePort = false;
		}
	}
	bool hosting = RemoteCommandService::g_globalCommandService.Host(port, addr);

	if (hosting)
	{
		//Socket Addr to string (plug in the ai_addr)
		sockaddr_in address = RemoteCommandService::g_globalCommandService.GetHostAddr();
		const char* adr = NetworkSystem::SockAddrToString((const sockaddr*)&address);
		g_Console->ConsolePrintf(UNIMPORTANT, "Succeeded in hosting!");
		g_Console->ConsolePrintf(UNIMPORTANT, "Hosting from " + std::string(adr));
	}
	else
	{
		g_Console->ConsolePrintf(WARNING, "Failed at hosting!");
	}

	if (deleteAddr)
	{
		delete addr;
	}
	if (deletePort)
	{
		delete port;
	}
}

//Console Command Stop
void TCPNetworkSystemConsoleCommands::ConsoleCommandTCPStop(Command& com)
{
	com;
	if (RemoteCommandService::g_globalCommandService.CurrentlyHosting() == false)
	{
		g_Console->ConsolePrintf(WARNING, "Not currently hosting!");
		return;
	}
	RemoteCommandService::g_globalCommandService.StopHosting();
	g_Console->ConsolePrintf(UNIMPORTANT, "Ended hosting session.");
}

//Console Command Join
void TCPNetworkSystemConsoleCommands::ConsoleCommandTCPJoin(Command& com)
{
	com;
	std::string ip = com.get_string_by_index(0);
	{
		size_t numPeriods = 0;
		const char* addr = NetworkSystem::GetHostAddressFromIP(ip.c_str(), numPeriods);
		const char* port = NetworkSystem::GetServiceFromIP(ip.c_str());
		bool deleteAddr = true;
		bool deletePort = true;
		if (addr == nullptr)
		{
			addr = NetConstants::s_localHost;
			deleteAddr = false;
		}
		if (port == nullptr)
		{
			port = NetConstants::s_defaultPort;
			deletePort = false;
		}

		bool joined = RemoteCommandService::g_globalCommandService.Join(port, addr);
		if (joined)
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "Successfully joined " + ip);
		}
		else
		{
			g_Console->ConsolePrintf(WARNING, "Failed to join " + ip);
		}

		if (deleteAddr)
		{
			delete addr;
		}
		if (deletePort)
		{
			delete port;
		}
	}
}

//Console Command Leave
void TCPNetworkSystemConsoleCommands::ConsoleCommandTCPLeave(Command& com)
{
	com;
	if (RemoteCommandService::g_globalCommandService.CurrentlyJoined() == false)
	{
		g_Console->ConsolePrintf(WARNING, "Currently not joined to a server!");
	}
	RemoteCommandService::g_globalCommandService.LeaveServer();
	g_Console->ConsolePrintf(UNIMPORTANT, "Left the server.");
}

//Console Command Info
void TCPNetworkSystemConsoleCommands::ConsoleCommandTCPInfo(Command& com)
{
	com;
	RemoteCommandService::g_globalCommandService.Info();
}

//Console Command Get Local Host Name
void TCPNetworkSystemConsoleCommands::ConsoleCommandTCPGetLocalHostName(Command& com)
{
	com;
	const char* localHostName = NetworkSystem::GetLocalHostName();
	g_Console->ConsolePrintf(UNIMPORTANT, std::string(localHostName));
}

//Console Command Net Send Command
void TCPNetworkSystemConsoleCommands::ConsoleCommandTCPNetSendCommand(Command& com)
{
	com;
	byte code = 3;
	bool echo = RemoteCommandService::g_globalCommandService.GetEchoCommands();
	std::string message = com.rebuild_string_from_variables(0, com.get_number_of_variables() - 1);

	RemoteCommandService::g_globalCommandService.MessageAll(message.c_str(), code, echo);
}

//Console Command Message All
void TCPNetworkSystemConsoleCommands::ConsoleCommandTCPMessageAll(Command& com)
{
	com;
	byte code = InvalidTCPode;
	std::string messageTypeStr = com.get_string_by_index(0);
	size_t idx = atoi(messageTypeStr.c_str());
	if (idx < InvalidTCPode || idx >= NUMBER_OF_TCPCODES)
	{
		idx = InvalidTCPode;
	}
	code = (byte)idx;
	std::string message = com.rebuild_string_from_variables(1, com.get_number_of_variables() - 1);
	bool echo = false;
	if (code == 3)
	{
		echo = RemoteCommandService::g_globalCommandService.GetEchoCommands();
	}

	RemoteCommandService::g_globalCommandService.MessageAll(message.c_str(), code, echo);
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Console Help Commands

//Help Console Command Host
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandListTCPAddresses(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetListTCPAddresses will list all addresses available to");
	g_Console->ConsolePrintf(UNIMPORTANT, "the current host for a given port over IPv4 Streams.");
}

//Help Console Command Host
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandTCPHost(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetHost calls the command to try and host a server at a");
	g_Console->ConsolePrintf(UNIMPORTANT, "given ip address; if none is provided, then it will try");
	g_Console->ConsolePrintf(UNIMPORTANT, "to host at to localhost:4335.");
}

//Help Console Command Stop
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandTCPStop(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetStop calls the command to stop hosting the server");
}

//Help Console Command Join
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandTCPJoin(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetJoin calls the command to try and join a server at a");
	g_Console->ConsolePrintf(UNIMPORTANT, "given ip address; if none is provided, then it will try");
	g_Console->ConsolePrintf(UNIMPORTANT, "to connect to localhost:4335.");
}

//Help Console Command Leave
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandTCPLeave(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetLeave calls the command to leave a joined server");
}

//Help Console Command Info
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandTCPInfo(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetInfo calls the command to get server information.");
}

//Help Console Command Get Local Host Name
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandTCPGetLocalHostName(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetGetLocalHostName gets the name of the local host.");
}

//Help Console Command Net Send Command
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandTCPNetSendCommand(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetSendCommand sends a Console Command to the connected");
	g_Console->ConsolePrintf(UNIMPORTANT, "host or client.");
}

//Help console Command Message All
void TCPNetworkSystemConsoleCommands::HelpConsoleCommandTCPMessageAll(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "NetMessageAll sends the string of text after console ");
	g_Console->ConsolePrintf(UNIMPORTANT, "command to all available connections.");
	g_Console->ConsolePrintf(UNIMPORTANT, "User must first input a number, followed by the message.");
}