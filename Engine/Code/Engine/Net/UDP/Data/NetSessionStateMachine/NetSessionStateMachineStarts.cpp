#include "Engine/Net/UDP/Data/NetSessionStateMachine/NetSessionStateMachineStarts.hpp"
#include "Engine/Net/UDP/NetSession.hpp"


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//State Machine start functions
STATIC void NetSessionStateMachineStarts::InvalidStateStart(NetSession* session)
{
	session;
}

STATIC void NetSessionStateMachineStarts::SetupStateStart(NetSession* session)
{
	session;
}

STATIC void NetSessionStateMachineStarts::DisconnectedStateStart(NetSession* session)
{
	std::vector<UDPNetConnection*> connections = session->GetConnections();
	for (size_t i = 0; i < connections.size(); i = i)
	{
		session->DestroyConnection(connections.at(i));
		connections.erase(connections.begin() + i);
	}
}

STATIC void NetSessionStateMachineStarts::ConnectedStateStart(NetSession* session)
{
	std::vector<UDPNetConnection*> connections = session->GetConnections();
	uint64_t time = Performance::GetCurrentPerformanceCount();
	session->SetLastUpdateTime(time);
	for (size_t i = 0; i < connections.size(); i++)
	{
		if (connections.at(i) != nullptr)
		{
			session->on_connection_joined.Trigger(connections.at(i));
		}
	}
}

STATIC void NetSessionStateMachineStarts::HostingStateStart(NetSession* session)
{
	session->SetLastError(DisconnectionReason_None);
	session->SetAmHost(true);
	
	sockaddr_in addr = session->GetSocketAddress();
	UDPNetConnection* conn = new UDPNetConnection(addr);
	conn->SetGuid(session->myUserName.c_str());
	conn->m_connState.m_is_local = true;
	conn->SetIsHost(true);
	session->myConn = conn;
	session->hostConnection = conn;
	session->AddConnection(conn, 0);
}

STATIC void NetSessionStateMachineStarts::JoiningStateStart(NetSession* session)
{
	uint64_t time = Performance::GetCurrentPerformanceCount();
	session->SetLastUpdateTime(time);
	session->SetLastError(DisconnectionReason_None);
	session->SetAmHost(false);

	PacketChannel* chan = session->GetMyConnect();
	sockaddr_in addr = chan->GetAddress();
	UDPNetConnection* mConn = new UDPNetConnection(addr);
	mConn->m_connState.m_is_local = true;

	sockaddr_in hostAddr = NetworkSystem::StringToSockAddrIPv4(session->hostIPAddr.c_str(), (uint16_t)atoi(session->hostIPPort.c_str()));

	UDPNetConnection* hostConn = new UDPNetConnection(hostAddr);
	hostConn->SetIsHost(true);
	mConn->SetGuid(session->myUserName.c_str());
	session->AddConnection(hostConn, 0);
	session->AddConnection(mConn, 1);
	session->hostConnection = hostConn;
	session->myConn = mConn;

	NetMessage msg = NetMessage(NETMSG_JOIN_REQUEST);
	size_t len = strlen(session->myUserName.c_str());
	msg.WriteForward<size_t>(&len, sizeof(size_t));
	msg.WriteForward<char>(session->myUserName.c_str(), sizeof(const char) * len);
	session->hostConnection->ConnSendMessage(msg);
}
