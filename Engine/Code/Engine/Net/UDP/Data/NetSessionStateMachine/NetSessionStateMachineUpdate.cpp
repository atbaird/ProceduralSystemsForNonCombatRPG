#include "Engine/Net/UDP/Data/NetSessionStateMachine/NetSessionStateMachineUpdate.hpp"
#include "Engine/Net/UDP/NetSession.hpp"


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//State Machine update functions
STATIC void NetSessionStateMachineUpdate::InvalidStateUpdate(NetSession* session)
{
	session;
	//NetSession not started yet
}

STATIC void NetSessionStateMachineUpdate::SetupStateUpdate(NetSession* session)
{
	if (NetworkSystem::NetSystemStartup() == true)
	{
		session->GetMachine()->ChangeStates(DISCONNECTED_NETSESSION);
	}
	else
	{
		session->GetMachine()->ChangeStates(INVALID_NETSESSION);
		session->SetLastError(DisconnectReason_FailedToStartNetSession);
	}
}

STATIC void NetSessionStateMachineUpdate::DisconnectedStateUpdate(NetSession* session)
{
	//Socket is Ready
	//Can call Join to go to Joining, or Host to go to Hosting
	session;
}

STATIC void NetSessionStateMachineUpdate::ConnectedStateUpdate(NetSession* session)
{
	session;
	//Leave call or disconnect sends to Disconnected State
	uint64_t currentTime = Performance::GetCurrentPerformanceCount();
	uint64_t timeDiff = currentTime - session->GetLastUpdateTime();
	session->SetLastUpdateTime(currentTime);
	session->Update((float)Performance::PerformanceCountToSeconds(timeDiff));
}

STATIC void NetSessionStateMachineUpdate::HostingStateUpdate(NetSession* session)
{
	//If host fails, go back to disconnect
	//if host succeeds, go to connected
	session->Listen();
	session->GetMachine()->ChangeStates(CONNECTED_NETSESSION);
}

STATIC void NetSessionStateMachineUpdate::JoiningStateUpdate(NetSession* session)
{
	//if joining fails, go to disconnected
	//If joining succeed go to connected
	uint64_t currentTime = Performance::GetCurrentPerformanceCount();
	uint64_t timeDiff = currentTime - session->GetLastUpdateTime();
	session->SetLastUpdateTime(currentTime);
	session->Update((float)Performance::PerformanceCountToSeconds(timeDiff));

	if (session->hostConnection == nullptr)
	{
		if (session->myConn != nullptr)
		{
			session->DestroyConnection(session->myConn);
			session->myConn = nullptr;
		}
		session->GetMachine()->ChangeStates(DISCONNECTED_NETSESSION);
	}
	else if (session->hostConnection->GetConnStatus() != UNCOMFIRMED)
	{
		//Add in more here
		session->GetMachine()->ChangeStates(CONNECTED_NETSESSION);
	}
	else
	{
		uint64_t time = session->hostConnection->m_createdAtTime;
		uint64_t curPerfCnt = Performance::GetCurrentPerformanceCount();
		uint64_t diff = curPerfCnt - time;
		double timeSec = Performance::PerformanceCountToSeconds(diff);
		if (timeSec >= ConnectionState::s_secondsBeforeMarkDisconnectMe)
		{
			session->GetMachine()->ChangeStates(DISCONNECTED_NETSESSION);
			session->SetLastError(DisconnectionReason_HostConnectionTimeOut);
		}
	}

}