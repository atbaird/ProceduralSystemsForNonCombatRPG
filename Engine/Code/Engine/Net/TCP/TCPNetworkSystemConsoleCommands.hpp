#pragma once

#ifndef NETWORKSYSTEMCONSOLECOMMANDS_HPP
#define NETWORKSYSTEMCONSOLECOMMANDS_HPP
#include "Engine/Console/RegisterCommandHelper.hpp"

class TCPNetworkSystemConsoleCommands
{
private:
public:
	//Registration

	static void RegisterConsoleCommands();

	//Console Commands
	static void ConsoleCommandListTCPAddresses(Command& com);
	static void ConsoleCommandTCPHost(Command& com);
	static void ConsoleCommandTCPStop(Command& com);
	static void ConsoleCommandTCPJoin(Command& com);
	static void ConsoleCommandTCPLeave(Command& com);
	static void ConsoleCommandTCPInfo(Command& com);
	static void ConsoleCommandTCPGetLocalHostName(Command& com);
	static void ConsoleCommandTCPNetSendCommand(Command& com);
	static void ConsoleCommandTCPMessageAll(Command& com);

	//Help Console Commands
	static void HelpConsoleCommandListTCPAddresses(Command& com);
	static void HelpConsoleCommandTCPHost(Command& com);
	static void HelpConsoleCommandTCPStop(Command& com);
	static void HelpConsoleCommandTCPJoin(Command& com);
	static void HelpConsoleCommandTCPLeave(Command& com);
	static void HelpConsoleCommandTCPInfo(Command& com);
	static void HelpConsoleCommandTCPGetLocalHostName(Command& com);
	static void HelpConsoleCommandTCPNetSendCommand(Command& com);
	static void HelpConsoleCommandTCPMessageAll(Command& com);
};

#endif