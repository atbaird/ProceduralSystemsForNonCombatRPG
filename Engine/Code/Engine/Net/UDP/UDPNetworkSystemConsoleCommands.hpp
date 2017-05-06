#pragma once

#ifndef UDPNETWORKSYSTEMCONSOLECOMMANDS_HPP
#define UDPNETWORKSYSTEMCONSOLECOMMANDS_HPP
#include "Engine/Console/RegisterCommandHelper.hpp"

class UDPNetworkSystemConsoleCommands
{
private:
protected:
public:
	//Register Console Commands
	static void RegisterConsoleCommands();

	//Console Commands
	static void ConsoleCommandUDPStartSession(Command& com);
	static void ConsoleCommandUDPEndSession(Command& com);
	static void ConsoleCommandUDPPingIP(Command& com);
	static void ConsoleCommandUDPSetPacketDropRate(Command& com);
	static void ConsoleCommandUDPSetPacketSimLag(Command& com);
	static void ConsoleCommandUDPToggleConnectionTimeOut(Command& com);
	static void ConsoleNetSessionCreateConnection(Command& com);
	static void ConsoleNetSessionDestroyConnection(Command& com);
	static void ConsoleNetSessionHost(Command& com);
	static void ConsoleNetSessionJoin(Command& com);
	static void ConsoleNetSessionLeave(Command& com);

	//Help Console Commands
	static void HelpConsoleCommandUDPStartSession(Command& com);
	static void HelpConsoleCommandUDPEndSession(Command& com);
	static void HelpConsoleCommandUDPPingIP(Command& com);
	static void HelpConsoleCommandUDPSetPacketDropRate(Command& com);
	static void HelpConsoleCommandUDPSetPacketSimLag(Command& com);
	static void HelpConsoleCommandUDPToggleConnectionTimeOut(Command& com);
	static void HelpConsoleNetSessionCreateConnection(Command& com);
	static void HelpConsoleNetSessionDestroyConnection(Command& com);
	static void HelpConsoleNetSessionHost(Command& com);
	static void HelpConsoleNetSessionJoin(Command& com);
	static void HelpConsoleNetSessionLeave(Command& com);

};
#endif