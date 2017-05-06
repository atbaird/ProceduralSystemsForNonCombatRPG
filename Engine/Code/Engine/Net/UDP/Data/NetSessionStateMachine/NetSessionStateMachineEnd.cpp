#include "Engine/Net/UDP/Data/NetSessionStateMachine/NetSessionStateMachineEnd.hpp"
#include "Engine/Net/UDP/NetSession.hpp"


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//State Machine end functions
STATIC void NetSessionStateMachineEnd::InvalidStateEnd(NetSession* session)
{
	if (SETUP_NETSESSION != session->GetMachine()->GetNextStateID())
	{
		session->GetMachine()->ChangeStates(INVALID_NETSESSION);
	}
	session->GetMachine()->SetCurrentStateFinished();
}

STATIC void NetSessionStateMachineEnd::SetupStateEnd(NetSession* session)
{
	PacketChannel* my_socket = session->GetMyConnect();
	if (session->GetMachine()->GetNextStateID() == DISCONNECTED_NETSESSION)
	{
		my_socket = new PacketChannel();
		session->SetMyConnection(my_socket);
	}
	else if (session->GetMachine()->GetNextStateID() != INVALID_NETSESSION)
	{
		session->GetMachine()->ChangeStates(SETUP_NETSESSION);
	}
	else
	{
		NetworkSystem::NetSystemShutdown();
	}
	session->GetMachine()->SetCurrentStateFinished();
}

STATIC void NetSessionStateMachineEnd::DisconnectedStateEnd(NetSession* session)
{
	PacketChannel* my_socket = session->GetMyConnect();
	if (session->GetMachine()->GetNextStateID() == HOSTING_NETSESSION
		|| session->GetMachine()->GetNextStateID() == JOINING_NETSESSION)
	{
		my_socket->Bind(session->wantedIPAddr.c_str(), session->wantedIPPort.c_str(), session->minIPPortRan.c_str(), session->maxIPPortRan.c_str());
	}
	else if (session->GetMachine()->GetNextStateID() == INVALID_NETSESSION)
	{
		NetworkSystem::NetSystemShutdown();
		if (my_socket != nullptr)
		{
			delete my_socket;
			session->SetMyConnection(nullptr);
			my_socket = nullptr;
		}
	}
	else
	{
		session->GetMachine()->ChangeStates(DISCONNECTED_NETSESSION);
	}
	session->wantedIPAddr = "";
	session->wantedIPPort = "";
	session->GetMachine()->SetCurrentStateFinished();
	session->SetWantToDisconnect(false);
}

STATIC void NetSessionStateMachineEnd::ConnectedStateEnd(NetSession* session)
{
	PacketChannel* my_socket = session->GetMyConnect();
	if (session->GetMachine()->GetNextStateID() != DISCONNECTED_NETSESSION)
	{
		session->GetMachine()->ChangeStates(CONNECTED_NETSESSION);
		return;
	}
	else if (my_socket != nullptr && my_socket->IsBound())
	{
		NetMessage msg = NetMessage(NETMSG_LEAVE);
		std::vector<UDPNetConnection*> connections = session->GetConnections();
		for (size_t i = 0; i < connections.size(); i++)
		{
			UDPNetConnection* conn = connections.at(i);
			if (conn != nullptr)
			{
				conn->ConnSendMessage(msg);
				session->on_connection_leave.Trigger(conn);
			}
		}
		uint64_t currentTime = Performance::GetCurrentPerformanceCount();
		uint64_t timeDiff = currentTime - session->GetLastUpdateTime();
		session->SetLastUpdateTime(currentTime);
		session->Update((float)Performance::PerformanceCountToSeconds(timeDiff));
		my_socket->Unbind();
	}
	session->GetMachine()->SetCurrentStateFinished();
}

STATIC void NetSessionStateMachineEnd::HostingStateEnd(NetSession* session)
{
	if (session->GetMachine()->GetNextStateID() == CONNECTED_NETSESSION
		|| session->GetMachine()->GetNextStateID() == DISCONNECTED_NETSESSION)
	{
		session->wantedIPPort = "";
		session->wantedIPAddr = "";
		session->minIPPortRan = "";
		session->maxIPPortRan = "";
	}
	else
	{
		session->GetMachine()->ChangeStates(HOSTING_NETSESSION);
	}
	session->GetMachine()->SetCurrentStateFinished();
}

STATIC void NetSessionStateMachineEnd::JoiningStateEnd(NetSession* session)
{
	PacketChannel* my_socket = session->GetMyConnect();
	if (session->GetMachine()->GetNextStateID() == CONNECTED_NETSESSION)
	{
		session->hostIPAddr = "";
		session->hostIPPort = "";
	}
	else if (session->GetMachine()->GetNextStateID() == DISCONNECTED_NETSESSION)
	{
		session->hostIPAddr = "";
		session->hostIPPort = "";
		session->DestroyConnection((uint8_t)0);
		session->DestroyConnection((uint8_t)1);
		my_socket->Unbind();
	}
	else
	{
		session->GetMachine()->ChangeStates(JOINING_NETSESSION);
	}
	session->GetMachine()->SetCurrentStateFinished();
}