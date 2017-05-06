#pragma once

#ifndef NETSESSIONSTATEMACHINSTARTS_HPP
#define NETSESSIONSTATEMACHINSTARTS_HPP

class NetSession;

class NetSessionStateMachineStarts
{
public:
	//State Machine start functions
	static void InvalidStateStart(NetSession* session);
	static void SetupStateStart(NetSession* session);
	static void DisconnectedStateStart(NetSession* session);
	static void ConnectedStateStart(NetSession* session);
	static void HostingStateStart(NetSession* session);
	static void JoiningStateStart(NetSession* session);

};
#endif