#pragma once

#ifndef NETSESSIONSTATEMACHINEEND_HPP
#define NETSESSIONSTATEMACHINEEND_HPP

class NetSession;

class NetSessionStateMachineEnd
{
public:

	//State Machine end functions
	static void InvalidStateEnd(NetSession* session);
	static void SetupStateEnd(NetSession* session);
	static void DisconnectedStateEnd(NetSession* session);
	static void ConnectedStateEnd(NetSession* session);
	static void HostingStateEnd(NetSession* session);
	static void JoiningStateEnd(NetSession* session);
};
#endif