#pragma once

#ifndef NETSESSIONSTATEMACHINEUPDATE_HPP
#define NETSESSIONSTATEMACHINEUPDATE_HPP

class NetSession;

class NetSessionStateMachineUpdate
{
public:

	//State Machine update functions
	static void InvalidStateUpdate(NetSession* session);
	static void SetupStateUpdate(NetSession* session);
	static void DisconnectedStateUpdate(NetSession* session);
	static void ConnectedStateUpdate(NetSession* session);
	static void HostingStateUpdate(NetSession* session);
	static void JoiningStateUpdate(NetSession* session);
};
#endif