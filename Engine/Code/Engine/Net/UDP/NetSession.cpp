#include "Engine/Net/UDP/NetSession.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Net/UDP/Data/NetSessionMessageDefaults.hpp"
#include "Engine/Net/UDP/Data/NetSessionStateMachine/NetSessionStateMachineStarts.hpp"
#include "Engine/Net/UDP/Data/NetSessionStateMachine/NetSessionStateMachineEnd.hpp"
#include "Engine/Net/UDP/Data/NetSessionStateMachine/NetSessionStateMachineUpdate.hpp"

NetSession* NetSession::g_NetSession = nullptr;
bool NetSession::s_EnableConnectionTimeOuts = true;


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Constructors

const uint8_t NetSession::s_numOfMsgDefs = (const uint8_t)256;
int NetSession::s_maxNumPacketsToReadInARow = 20;

/*

DisconnectionReason lastError;
StateMachine<NetSession*> machine;

public:
std::string hostIPPort;
std::string hostIPAddr;
std::string wantedIPPort;
std::string wantedIPAddr;
std::string minIPPortRan;
std::string maxIPPortRan;
std::string myUserName;
UDPNetConnection* hostConnection;
UDPNetConnection* myConn;
*/

NetSession::NetSession()
	: my_socket(nullptr),
	my_networkID(0),
	numPacketsReadInARow(0),
	maxNumConnectionsAllowed(4),
	nextAvailableNetworkID(1),
	m_amHost(false),
	m_peerToPeer(true),
	m_enableDebugDisplay(false),
	m_asHostStillAcceptingConnections(true),
	m_wantsToDisconnect(false),
	last_update_time(0),
	tick_rate(1.f / 20.f),
	lastError(DisconnectionReason_None),
	machine(StateMachine<NetSession*>()),
	hostIPAddr(""),
	hostIPPort(""),
	wantedIPAddr(""),
	wantedIPPort(""),
	minIPPortRan(""),
	maxIPPortRan(""),
	myUserName(""),
	myConn(nullptr),
	hostConnection(nullptr)
{
	for (size_t i = 0; i < 256; i++)
	{
		msg_definitions[i] = NetMessageDefinitionTemplate();
	}
	State<NetSession*>* invalidState = new State<NetSession*>();
	State<NetSession*>* setupState = new State<NetSession*>();
	State<NetSession*>* disconnectedState = new State<NetSession*>();
	State<NetSession*>* connectedState = new State<NetSession*>();
	State<NetSession*>* hostingState = new State<NetSession*>();
	State<NetSession*>* joiningState = new State<NetSession*>();

	//Set up start states
	invalidState->start_up_state_trigger.RegisterFunction(NetSessionStateMachineStarts::InvalidStateStart, 0);
	setupState->start_up_state_trigger.RegisterFunction(NetSessionStateMachineStarts::SetupStateStart, 0);
	disconnectedState->start_up_state_trigger.RegisterFunction(NetSessionStateMachineStarts::DisconnectedStateStart, 0);
	connectedState->start_up_state_trigger.RegisterFunction(NetSessionStateMachineStarts::ConnectedStateStart, 0);
	hostingState->start_up_state_trigger.RegisterFunction(NetSessionStateMachineStarts::HostingStateStart, 0);
	joiningState->start_up_state_trigger.RegisterFunction(NetSessionStateMachineStarts::JoiningStateStart, 0);

	//Set up end states
	invalidState->end_state_trigger.RegisterFunction(NetSessionStateMachineEnd::InvalidStateEnd, 0);
	setupState->end_state_trigger.RegisterFunction(NetSessionStateMachineEnd::SetupStateEnd, 0);
	disconnectedState->end_state_trigger.RegisterFunction(NetSessionStateMachineEnd::DisconnectedStateEnd, 0);
	connectedState->end_state_trigger.RegisterFunction(NetSessionStateMachineEnd::ConnectedStateEnd, 0);
	hostingState->end_state_trigger.RegisterFunction(NetSessionStateMachineEnd::HostingStateEnd, 0);
	joiningState->end_state_trigger.RegisterFunction(NetSessionStateMachineEnd::JoiningStateEnd, 0);

	//Set up Update states
	invalidState->state_trigger.RegisterFunction(NetSessionStateMachineUpdate::InvalidStateUpdate, 0);
	setupState->state_trigger.RegisterFunction(NetSessionStateMachineUpdate::SetupStateUpdate, 0);
	disconnectedState->state_trigger.RegisterFunction(NetSessionStateMachineUpdate::DisconnectedStateUpdate, 0);
	connectedState->state_trigger.RegisterFunction(NetSessionStateMachineUpdate::ConnectedStateUpdate, 0);
	hostingState->state_trigger.RegisterFunction(NetSessionStateMachineUpdate::HostingStateUpdate, 0);
	joiningState->state_trigger.RegisterFunction(NetSessionStateMachineUpdate::JoiningStateUpdate, 0);

	machine.RegisterState(INVALID_NETSESSION, invalidState);
	machine.RegisterState(SETUP_NETSESSION, setupState);
	machine.RegisterState(DISCONNECTED_NETSESSION, disconnectedState);
	machine.RegisterState(CONNECTED_NETSESSION, connectedState);
	machine.RegisterState(HOSTING_NETSESSION, hostingState);
	machine.RegisterState(JOINING_NETSESSION, joiningState);
	machine.ChangeStates(INVALID_NETSESSION);

	machine.Update(this);

	RegisterMessage(NETMSG_PING, "Ping", NetSessionMessageDefaults::OnPingReceived, false, false, false);
	RegisterMessage(NETMSG_PONG, "Pong", NetSessionMessageDefaults::OnPongReceived, false, false, false);
	RegisterMessage(NETMSG_HEARTBEAT, "Heartbeat", NetSessionMessageDefaults::OnHeartbeatReceived, true, false, false);
	RegisterMessage(NETMSG_ACK, "Ack", NetSessionMessageDefaults::OnAckReceived, false, false, false);
	RegisterMessage(NETMSG_JOIN_REQUEST, "JoinRequest", NetSessionMessageDefaults::OnJoinRequestReceived, false, true, false);
	RegisterMessage(NETMSG_JOIN_ACCEPT, "JoinAccept", NetSessionMessageDefaults::OnJoinAcceptReceived, true, true, false);
	RegisterMessage(NETMSG_JOIN_DENY, "JoinDeny", NetSessionMessageDefaults::OnJoinDeniedReceived, true, false, false);
	RegisterMessage(NETMSG_LEAVE, "Leave", NetSessionMessageDefaults::OnLeaveReceived, false, false, false);
	RegisterMessage(NETMSG_New_Connection_Status_Update_From_Host, "NewConnectionAddedReportFromHost",
		NetSessionMessageDefaults::OnNewConnectionStatusUpdateFromHost, true, true, false, 1);


}

NetSession::~NetSession()
{
	if (my_socket != nullptr)
	{
		End();
	}
	ClearAllConnections();
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		if (m_objects.at(i) != nullptr)
		{
			delete m_objects[i];
		}
	}
	m_objects.clear();

	for (size_t i = 0; i < m_nonConnObjects.size(); i++)
	{
		if (m_nonConnObjects.at(i) != nullptr)
		{
			delete m_nonConnObjects[i];
		}
	}
	m_nonConnObjects.clear();
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Running
STATIC bool NetSession::NetSessionCreate()
{
	//In Invalid state.
	if (g_NetSession != nullptr)
	{
		return false;
	}
	g_NetSession = new NetSession();
	return true;
}

STATIC bool NetSession::NetSessionSetup()
{
	if (g_NetSession == nullptr || g_NetSession->machine.GetCurrentStateID() != INVALID_NETSESSION)
	{
		return false;
	}
	g_NetSession->machine.ChangeStates(SETUP_NETSESSION);
	g_NetSession->machine.Update(g_NetSession);
	if (g_NetSession->machine.GetCurrentStateID() == INVALID_NETSESSION)
	{
		return false;
	}
	else
	{
		return true;
	}
}

STATIC bool NetSession::NetSessionCreateSocketAndDontCareAboutHostingOrJoining(const char* port, const char* minPort, const char* maxPort)
{
	//Go to Disconnected State
	if (g_NetSession == nullptr || g_NetSession->machine.GetCurrentStateID() != DISCONNECTED_NETSESSION)
	{
		return false;
	}
	return g_NetSession->Start(port, minPort, maxPort);
}

STATIC bool NetSession::NetSessionHost(const char* username, const char* addr, const char* port, const char* minPort, const char* maxPort)
{
	if (g_NetSession == nullptr || g_NetSession->machine.GetCurrentStateID() != DISCONNECTED_NETSESSION)
	{
		return false;
	}
	g_NetSession->wantedIPPort = std::string(port);
	g_NetSession->wantedIPAddr = std::string(addr);
	g_NetSession->minIPPortRan = std::string(minPort);
	g_NetSession->maxIPPortRan = std::string(maxPort);
	g_NetSession->myUserName = std::string(username);
	g_NetSession->machine.ChangeStates(HOSTING_NETSESSION);
	g_NetSession->machine.Update(g_NetSession);
	if (g_NetSession->machine.GetCurrentStateID() == HOSTING_NETSESSION)
	{
		return true;
	}
	return false;
}

STATIC bool NetSession::NetSessionJoin(const char* username, const char* myAdr, const char* myPort, const char* hostAdr,
	const char* hostPort)
{
	if (g_NetSession == nullptr || g_NetSession->machine.GetCurrentStateID() != DISCONNECTED_NETSESSION)
	{
		return false;
	}
	g_NetSession->hostIPAddr = std::string(hostAdr);
	g_NetSession->hostIPPort = std::string(hostPort);
	g_NetSession->myUserName = std::string(username);
	g_NetSession->wantedIPAddr = std::string(myAdr);
	g_NetSession->wantedIPPort = std::string(myPort);
	g_NetSession->minIPPortRan = std::string(NetConstants::s_minPortDefault);
	g_NetSession->minIPPortRan = std::string(NetConstants::s_maxPortDefault);
	g_NetSession->machine.ChangeStates(JOINING_NETSESSION);
	g_NetSession->machine.Update(g_NetSession);
	if (g_NetSession->machine.GetCurrentStateID() == JOINING_NETSESSION)
	{
		return true;
	}
	return false;
}

STATIC bool NetSession::NetSessionStop()
{
	if (g_NetSession != nullptr && g_NetSession->machine.GetCurrentStateID() == DISCONNECTED_NETSESSION)
	{
		g_NetSession->machine.ChangeStates(INVALID_NETSESSION);
		return true;
	}
	return false;
}


STATIC bool NetSession::NetSessionLeave()
{
	if (g_NetSession == nullptr || g_NetSession->machine.GetCurrentStateID() != CONNECTED_NETSESSION)
	{
		return false;
	}

	g_NetSession->machine.ChangeStates(DISCONNECTED_NETSESSION);
	g_NetSession->m_wantsToDisconnect = true;
	g_NetSession->machine.Update(g_NetSession);
	return true;
}

STATIC bool NetSession::NetSessionEnd()
{
	//Go from connected to disconnected state.
	if (g_NetSession == nullptr) 
	{
		return false;
	}
	return g_NetSession->End();
}


STATIC void NetSession::NetUpdate(float dt)
{
	dt;
	//Called from Connected State
	if (g_NetSession != nullptr)
	{
		//g_NetSession->Update(dt);
		g_NetSession->machine.Update(g_NetSession);
	}
}

STATIC void NetSession::NetSessionDestroy()
{
	//Go from any state through to disconnected, then INVALID
	if (g_NetSession == nullptr)
	{
		return;
	}
	NetSessionEnd();
	delete g_NetSession;
	g_NetSession = nullptr;
	NetworkSystem::NetSystemShutdown();
}

bool NetSession::Start(const char* port, const char* minPort, const char* maxPort)
{
	if (my_socket != nullptr)
	{
		End();
	}
	my_socket = new PacketChannel(port, minPort, maxPort);
	my_socket->m_session = this;
	return my_socket->IsBound();
}

bool NetSession::End()
{
	if (my_socket == nullptr)
	{
		return false;
	}
	my_socket->Unbind();
	delete my_socket;
	my_socket = nullptr;
	return true;
}

void NetSession::Update(float dt)
{
	ProcessIncomingPackets();
	in_mid_update.Trigger(dt);
	//on_connection_leave.Trigger(conn);
	ProcessOutgoingPackets(dt);
}

bool NetSession::IsRunning() const
{
	if (my_socket == nullptr 
		|| my_socket->IsBound() == false)
	{
		return false;
	}
	return true;
}

void NetSession::Listen()
{
	m_asHostStillAcceptingConnections = true;
}
void NetSession::StopListening()
{
	m_asHostStillAcceptingConnections = false;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Connection related
bool NetSession::AddConnection(UDPNetConnection* conn, uint8_t idx)
{
	if (connections.size() >= maxNumConnectionsAllowed || conn == nullptr)
	{
		return false;
	}
	bool found = false;
	size_t index = 0;
	for (size_t i = 0; i < availableConnectionIDs.size(); i++)
	{
		if (availableConnectionIDs[i] == idx)
		{
			found = true;
			index = i;
			break;
		}
	}
	if (found == false)
	{
		return false;
	}
	availableConnectionIDs.erase(availableConnectionIDs.begin() + index);
	conn->session = this;
	conn->socket = my_socket;
	conn->m_index = idx;
	connections.push_back(conn);
	ObjConnectionPair* par = new ObjConnectionPair();
	par->connection = conn;
	par->connectionID = idx;
	m_objects.push_back(par);
	if (conn->IsMe())
	{
		myConn = conn;
	}
	return true;
}

bool NetSession::AddConnection(UDPNetConnection* conn)
{
	if (connections.size() >= maxNumConnectionsAllowed || conn == nullptr)
	{
		return false;
	}
	conn->session = this;
	conn->socket = my_socket;
	uint8_t index = availableConnectionIDs[0];
	conn->m_index = index;
	availableConnectionIDs.erase(availableConnectionIDs.begin());
	connections.push_back(conn);

	//load up object pair
	ObjConnectionPair* par = new ObjConnectionPair();
	par->connection = conn;
	par->connectionID = index;
	par->object = nullptr;
	m_objects.push_back(par);
	if (conn->IsMe())
	{
		myConn = conn;
	}
	return true;
}

bool NetSession::DestroyConnection(UDPNetConnection* conn)
{
	if (conn == nullptr)
	{
		return false;
	}
	on_connection_leave.Trigger(conn);
	for (size_t connIdx = 0; connIdx < connections.size(); connIdx++)
	{
		UDPNetConnection* testConn = connections.at(connIdx);
		if (testConn != nullptr && conn == testConn)
		{
			bool destoryAllConns = false;
			if (conn == myConn
				|| conn == hostConnection)
			{
				if (conn->GetIsHost())
				{
					lastError = DisconnectionReason_ErrorHostDisconnected;
				}
				else if (conn->m_connState.m_is_local)
				{
					lastError = DisconnectionReason_None;
				}
				myConn = nullptr;
				hostConnection = nullptr;
				destoryAllConns = true;
				if (machine.GetCurrentStateID() == CONNECTED_NETSESSION && machine.GetNextStateID() == CONNECTED_NETSESSION)
				{
					machine.ChangeStates(DISCONNECTED_NETSESSION);
				}
			}
			on_connection_leave.Trigger(testConn);
			for (size_t objectIdx = 0; objectIdx < m_objects.size(); objectIdx++)
			{
				ObjConnectionPair* par = m_objects.at(objectIdx);
				if (par != nullptr && par->connection == testConn)
				{
					delete par;
					m_objects.erase(m_objects.begin() + objectIdx);
					break;
				}
			}
			availableConnectionIDs.push_back(conn->m_index);
			connections.erase(connections.begin() + connIdx);
			delete testConn;
			if (destoryAllConns)
			{
				for (size_t connectIdx = 0; connectIdx < connections.size(); connectIdx++)
				{
					if (connections.at(connectIdx) != nullptr)
					{
						bool addIT = true;
						for (size_t idIdx = 0; idIdx < availableConnectionIDs.size(); idIdx++)
						{

							if (availableConnectionIDs.at(idIdx) == connections.at(connectIdx)->m_index)
							{
								addIT = false;
								break;
							}
						}
						if (addIT)
						{
							availableConnectionIDs.push_back(connections.at(connectIdx)->m_index);
						}
						on_connection_leave.Trigger(connections.at(connectIdx));
						delete connections.at(connectIdx);
						connections.erase(connections.begin() + connectIdx);
						connectIdx--;
					}
				}
				for (size_t objectIdx = 0; objectIdx < m_objects.size(); objectIdx++)
				{
					ObjConnectionPair* par = m_objects.at(objectIdx);
					if (par != nullptr && par->connection == testConn)
					{
						delete par;
						m_objects.erase(m_objects.begin() + objectIdx);
						objectIdx--;
					}
				}
			}
			return true;
		}
	}
	return false;
}

bool NetSession::DestroyConnection(uint8_t networkID)
{
	for (size_t connIdx = 0; connIdx < connections.size(); connIdx++)
	{
		UDPNetConnection* conn = connections.at(connIdx);
		if (conn != nullptr && conn->m_index == networkID)
		{
			bool destoryAllConns = false;
			if (conn == myConn
				|| conn == hostConnection)
			{
				if (conn->GetIsHost())
				{
					lastError = DisconnectionReason_ErrorHostDisconnected;
				}
				else if (conn->m_connState.m_is_local)
				{
					lastError = DisconnectionReason_None;
				}
				myConn = nullptr;
				hostConnection = nullptr;
				destoryAllConns = true;
				if (machine.GetCurrentStateID() == CONNECTED_NETSESSION && machine.GetNextStateID() == CONNECTED_NETSESSION)
				{
					machine.ChangeStates(DISCONNECTED_NETSESSION);
				}
			}
			on_connection_leave.Trigger(conn);
			for (size_t objectIdx = 0; objectIdx < m_objects.size(); objectIdx++)
			{
				ObjConnectionPair* par = m_objects.at(objectIdx);
				if (par != nullptr && par->connection == conn)
				{
					delete par;
					m_objects.erase(m_objects.begin() + objectIdx);
					break;
				}
			}
			availableConnectionIDs.push_back(conn->m_index);
			connections.erase((connections.begin() + connIdx));
			delete conn;
			if (destoryAllConns)
			{
				for (size_t connectIdx = 0; connectIdx < connections.size(); connectIdx++)
				{
					if (connections.at(connectIdx) != nullptr)
					{
						bool addIT = true;
						for (size_t idIdx = 0; idIdx < availableConnectionIDs.size(); idIdx++)
						{

							if (availableConnectionIDs.at(idIdx) == connections.at(connectIdx)->m_index)
							{
								addIT = false;
								break;
							}
						}
						if (addIT)
						{
							availableConnectionIDs.push_back(connections.at(connectIdx)->m_index);
						}
						on_connection_leave.Trigger(connections.at(connectIdx));
						delete connections.at(connectIdx);
						connections.erase(connections.begin() + connectIdx);
						connectIdx--;
					}
				}
				for (size_t objectIdx = 0; objectIdx < m_objects.size(); objectIdx++)
				{
					ObjConnectionPair* par = m_objects.at(objectIdx);
					if (par != nullptr && par->connection == conn)
					{
						delete par;
						m_objects.erase(m_objects.begin() + objectIdx);
						objectIdx--;
					}
				}
			}
			return true;
		}
	}
	return false;
}

UDPNetConnection* NetSession::RemoveConnection(uint8_t networkID)
{
	for (size_t i = 0; i < connections.size(); i++)
	{
		UDPNetConnection* conn = connections.at(i);
		if (conn != nullptr && conn->m_index == networkID)
		{
			if (conn->IsMe())
			{
				myConn = nullptr;
			}
			for (size_t objectIdx = 0; objectIdx < m_objects.size(); objectIdx++)
			{
				ObjConnectionPair* par = m_objects.at(objectIdx);
				if (par != nullptr && par->connection == conn)
				{
					delete par;
					m_objects.erase(m_objects.begin() + objectIdx);
					break;
				}
			}
			availableConnectionIDs.push_back(conn->m_index);
			connections.erase((connections.begin() + i));
			return conn;
		}
	}
	return nullptr;
}

void NetSession::SetMaxNumberOfConnections(uint8_t connCount)
{
	if (IsRunning())
	{
		return;
	}
	availableConnectionIDs.clear();
	for (uint8_t i = 0; i < connCount; i++)
	{
		availableConnectionIDs.push_back(i);
	}

}


bool NetSession::TestIfIdxIsAvailable(uint8_t idx) const
{
	for (size_t i = 0; i < availableConnectionIDs.size(); i++)
	{
		if (availableConnectionIDs.at(i) == idx)
		{
			return true;
		}
	}
	return false;
}

bool NetSession::TestIfNameIsTaken(std::string& name) const
{
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		if (m_objects[i] != nullptr && m_objects[i]->connection != nullptr)
		{
			if (strcmp((m_objects[i]->connection->guid), name.c_str()) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool NetSession::TestIfIPAlreadyExists(const char* fullIPAddr) const
{
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		if (m_objects[i] != nullptr && m_objects[i]->connection != nullptr)
		{
			sockaddr_in ip = m_objects[i]->connection->address;
			const char* ipaddr = NetworkSystem::SockAddrToString((sockaddr*)&ip);
			if (strcmp(ipaddr, fullIPAddr) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

UDPNetConnection* NetSession::CreateUDPNetConnectionFromSockAddr(sockaddr_in& addr)
{
	UDPNetConnection* netConn = new UDPNetConnection(addr);
	netConn->session = this;
	netConn->address = addr;
	netConn->socket = my_socket;
	netConn->tick_rate = tick_rate;
	netConn->m_connState.m_is_local = netConn->IsMe();

	return netConn;
}

UDPNetConnection* NetSession::GetUDPNetConnectionForSockAddr(sockaddr_in& addr)
{
	for (size_t i = 0; i < connections.size(); i++)
	{
		UDPNetConnection* conn = connections.at(i);
		if (conn == nullptr)
		{
			continue;
		}
		sockaddr_in connAddr = conn->address;
		if (NetworkSystem::DetermineIfSockAddrsAreTheSame(addr, connAddr))
		{
			return conn;
		}
	}

	return nullptr;
}

UDPNetConnection* NetSession::GetUDPNetConnectionForSockAddr(sockaddr_in* addr)
{
	if (addr == nullptr)
	{
		return nullptr;
	}
	for (size_t i = 0; i < connections.size(); i++)
	{
		UDPNetConnection* conn = connections.at(i);
		if (conn == nullptr)
		{
			continue;
		}
		sockaddr_in connAddr = conn->address;
		if (NetworkSystem::DetermineIfSockAddrsAreTheSame(*addr, connAddr))
		{
			return conn;
		}
	}

	return nullptr;
}

UDPNetConnection* NetSession::GetUDPNetConnectionForNetID(uint8_t netID)
{
	if (netID >= maxNumConnectionsAllowed)
	{
		return nullptr;
	}
	for (size_t i = 0; i < connections.size(); i++)
	{
		if (connections.at(i) != nullptr && connections.at(i)->m_index == netID)
		{
			return connections.at(i);
		}
	}
	return nullptr;
}

void NetSession::SetObjectForNetID(void* object, uint8_t netID)
{
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		ObjConnectionPair* par = m_objects.at(i);
		if (par != nullptr)
		{
			if (par->connectionID == netID)
			{
				par->object = object;
			}
		}
	}
}

void NetSession::AddNonConnObject(void* object, int uniqueID)
{
	if (object == nullptr)
	{
		return;
	}
	ObjAndIDPair* pair = new ObjAndIDPair();
	pair->object = object;
	pair->uniqueID = uniqueID;
	m_nonConnObjects.push_back(pair);
}

void NetSession::RemoveNonConnObjectByID(int uniqueID)
{
	for (size_t i = 0; i < m_nonConnObjects.size(); i++)
	{
		ObjAndIDPair* pair = m_nonConnObjects.at(i);
		if (pair != nullptr && pair->uniqueID == uniqueID)
		{
			delete pair;
			m_nonConnObjects.erase(m_nonConnObjects.begin() + i);
			break;
		}
	}
}

void NetSession::RemoveNonConnObjectByObject(void* object)
{
	for (size_t i = 0; i < m_nonConnObjects.size(); i++)
	{
		ObjAndIDPair* pair = m_nonConnObjects.at(i);
		if (pair != nullptr && pair->object == object)
		{
			delete pair;
			m_nonConnObjects.erase(m_nonConnObjects.begin() + i);
			break;
		}
	}
}

void NetSession::RemoveNonConnObjectByObjAndID(void* object, int uniqueID)
{
	for (size_t i = 0; i < m_nonConnObjects.size(); i++)
	{
		ObjAndIDPair* pair = m_nonConnObjects.at(i);
		if (pair != nullptr && pair->uniqueID == uniqueID && pair->object == object)
		{
			delete pair;
			m_nonConnObjects.erase(m_nonConnObjects.begin() + i);
			break;
		}
	}
}

void NetSession::ClearAllConnections()
{
	for (size_t i = 0; i < connections.size(); i++)
	{
		UDPNetConnection* conn = connections.at(i);
		if (conn != nullptr)
		{
			on_connection_leave.Trigger(conn);
			delete conn;
		}
	}
	connections.clear();
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		if (m_objects[i] != nullptr)
		{
			availableConnectionIDs.push_back(m_objects[i]->connectionID);
			delete m_objects[i];
		}
	}
	m_objects.clear();
}


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Message Related
void NetSession::RegisterMessage(uint8_t msgType, const char* debug_name, net_message_cb func, bool requiresConnection, bool reliable,
	bool inOrder, uint8_t chanID)
{
	if (INVALID_NETSESSION != machine.GetCurrentStateID())
	{
		return;
	}
	msg_definitions[msgType].m_cb = func;
	msg_definitions[msgType].m_debug_name = debug_name;
	msg_definitions[msgType].my_idx = msgType;
	msg_definitions[msgType].m_set = true;
	msg_definitions[msgType].m_Reliable = reliable;
	msg_definitions[msgType].m_inOrder = inOrder;
	msg_definitions[msgType].m_channelID = chanID;
	msg_definitions[msgType].m_RequiresConnection = requiresConnection;

}
int NetSession::RegisterMessage(const char* debug_name, net_message_cb func, bool requiresConnection, bool reliable,
	bool inOrder, uint8_t chanID)
{
	if (IsRunning() || INVALID_NETSESSION != machine.GetCurrentStateID())
	{
		return -1;
	}
	int id = -1;
	for (uint8_t i = 0; i < s_numOfMsgDefs; i++)
	{
		if (msg_definitions[i].m_set == false)
		{
			msg_definitions[i].m_cb = func;
			msg_definitions[i].m_debug_name = debug_name;
			msg_definitions[i].my_idx = i;
			msg_definitions[i].m_set = true;
			msg_definitions[i].m_Reliable = reliable;
			msg_definitions[i].m_inOrder = inOrder;
			msg_definitions[i].m_channelID = chanID;
			msg_definitions[i].m_RequiresConnection = requiresConnection;
			break;
		}
	}
	return id;
}

bool NetSession::SendMessageDirect(const sockaddr_in& addr, NetMessage& msg)
{
	//This method is intended for sending only ONE message
	if (!IsRunning() || my_socket == nullptr)
	{
		return false;
	}
	byte_t msgID = msg.GetMessageID();
	msg.m_defn = this->FindDefinition(msgID);
	if (msg.m_defn == nullptr)
	{
		return false;
	}
	//Create Packet and fill in packet header
	NetPacket packet;
	packet.SetNumOfMsgs(1);
	uint16_t zero = NetPacketHeader::INVALID_PACKET_ACK;
	packet.GetNextAck(zero);

	/*
	NETMSG_PING = 0,
	NETMSG_PONG = 1,
	NETMSG_HEARTBEAT = 2,
	NETMSG_ACK = 3,
	NETMSG_JOIN_REQUEST = 4,
	NETMSG_JOIN_RESPONSE = 5,
	NETMSG_CONNECTION_JOIN = 6,
	NETMSG_CONNECTION_LEFT = 7,
	NETMSG_KICK = 8,
	NETMSG_QUIT = 9,
	*/
	packet.WritePacketHeader();
	WriteMessageToPacket(&packet, msg);
	size_t msgSize = my_socket->SendTo(addr, packet.GetTail(), packet.GetOffset());

	if (msgSize == 0)
	{
		return false;
	}
	return true;
}


void NetSession::SendMessageAll(NetMessage& msg)
{
	SendMessageAllExcept(msg, nullptr);
}

void NetSession::SendMessageAllExcept(NetMessage& msg, UDPNetConnection* conn)
{
	for (size_t i = 0; i < connections.size(); i++)
	{
		if (connections.at(i) != nullptr && connections.at(i) != conn
			&& connections.at(i) != myConn)
		{
			connections.at(i)->ConnSendMessage(msg);
		}
	}
}

bool NetSession::BroadcastMessageDirect(NetMessage& msg)
{
	if (!IsRunning() || my_socket == nullptr)
	{
		return false;
	}
	msg.m_defn = this->FindDefinition(msg.GetMessageID());
	if (msg.m_defn == nullptr)
	{
		return false;
	}
	for (size_t i = 0; i < connections.size(); i++)
	{
		if (connections.at(i) != nullptr && connections.at(i)->IsMe() == false)
		{
			sockaddr_in addr = (connections.at(i)->address);
			SendMessageDirect(addr, msg);
		}
	}
	return true;
}

void NetSession::WriteMessageToPacket(NetPacket* packet, const NetMessage& msg)
{
	uint16_t headerSize = GetHeaderSize(msg.GetMessageID());
	uint16_t payloadSize = GetPayloadSize(msg);
	uint16_t msg_size = headerSize + payloadSize;
	uint16_t reliableID = msg.GetReliableID();
	if (packet->GetWriteableBytes() >= (msg_size + 1))
	{
		//Fill in message header
		packet->Write<uint16_t>(&msg_size, sizeof(uint16_t));
		packet->Write<uint8_t>(&msg.m_defn->my_idx, sizeof(uint8_t));
		if (headerSize > sizeof(uint8_t))
		{
			packet->Write<uint16_t>(&reliableID, sizeof(uint16_t));
		}
		//Write Message
		packet->WriteForward<byte_t>((byte_t*)msg.GetTail(), payloadSize);
	}
}

void NetSession::ProcessIncomingPackets()
{
	if (IsRunning() == false)
	{
		return;
	}
	NetPacket packet = NetPacket();
	NetSenderTemplate from;
	from.m_session = this;
	numPacketsReadInARow = 0;
	while (ReadNextPacketFromSocket(&(packet), &(from.m_address), from.m_connection) == true)
	{
		
		NetMessage msg = NetMessage(NETMSG_PONG);
		//msg.SetBuffer(nullptr, 1024, false);
		size_t msgSize = 0;
		bool actuallyReadIt = true;
		while (ReadNextMessageFromPacket(&msg, packet, msgSize, &(from.m_address), actuallyReadIt) == true)
		{
			memcpy(&msg.m_sender, &from, sizeof(from));
			UDPNetConnection* conn = GetUDPNetConnectionForSockAddr(from.m_address);

			if (GetIfMsgTypeIsInOrder(msg.GetMessageID()))
			{
				//Specifically for IN Order and reliable
				if (conn != nullptr)
				{
					if (conn->MarkMessageRecvdInOrder(msg))
					{
						uint8_t chanID = msg.GetChannelID();
						NetMessage* popped = conn->PopMessageRecvdInOrder(chanID);
						while (popped != nullptr)
						{
							NetMessageDefinitionTemplate* msg_defn = FindDefinition(msg.GetMessageID());
							if (msg_defn != nullptr)
							{
								popped->ResetOffset();
								msg_defn->m_cb(from, *popped);
								from.m_connection = GetUDPNetConnectionForSockAddr(from.m_address);
							}

							delete popped;
							popped = conn->PopMessageRecvdInOrder(chanID);
						}
					}
				}
				//if (from.m_connection != nullptr)
				//{
				//	from.m_connection->MarkMessageRecvd(msg);
				//}
			}
			else
			{
				if (actuallyReadIt)
				{
					//Specifically for non In order stuff.
					NetMessageDefinitionTemplate* msg_defn = FindDefinition(msg.GetMessageID());
					if (msg_defn != nullptr)
					{
						msg.ResetOffset();
						msg_defn->m_cb(from, msg);
						from.m_connection = GetUDPNetConnectionForSockAddr(from.m_address);
					}
				}
				if (from.m_connection != nullptr)
				{
					from.m_connection->MarkMessageRecvd(msg);
				}
			}
			//packet.Advance(msgSize);
			msg.ResetOffset();
			actuallyReadIt = true;
		}
		packet.SetReadableBytes(PACKET_MTU);
		packet.ClearData(PACKET_MTU);
		packet.ResetOffset();
		packet.ResetNumOfMsgs();
		packet.ResetNumOfReadMsgs();
	}
}

void NetSession::ProcessOutgoingPackets(float& dt)
{
	for (size_t i = 0; i < connections.size(); i++)
	{
		UDPNetConnection* cp = connections.at(i);
		if (cp == nullptr)
		{
			continue;
		}

		cp->Tick(dt);

		if (s_EnableConnectionTimeOuts && cp->GetDisconnectMe())
		{
			if (cp == hostConnection)
			{
				lastError = DisconnectionReason_HostConnectionTimeOut;
				machine.ChangeStates(DISCONNECTED_NETSESSION);
			}
			uint8_t index = cp->m_index;
			DestroyConnection(index);
			i--;
			continue;
		}

		if (cp->GetUpdateNow())
		{
			on_net_tick.Trigger(cp);
			cp->SendPacket();
		}
	}
}

bool NetSession::ReadNextPacketFromSocket(NetPacket* out_packet, sockaddr_in* out_addr, UDPNetConnection*& out_connection)
{
	out_connection = nullptr;
	if (out_addr == nullptr || out_packet == nullptr
		|| my_socket == nullptr || !my_socket->IsBound()
		|| (s_maxNumPacketsToReadInARow > 0 && numPacketsReadInARow >= (size_t)s_maxNumPacketsToReadInARow))
	{
		return false;
	}
	size_t packetSize = my_socket->RecvFrom(out_addr, out_packet, out_packet->GetWriteableBytes());
	if (packetSize > PACKET_MTU || packetSize < NetPacket::GetExpectedHeaderSize())
	{
		return false;
	}
	out_connection = UpdateUDPNetConnection(out_packet, out_addr);
	out_packet->SetReadableBytes(packetSize);
	if (out_packet->GetNumOfMsgs() > 0)
	{
		numPacketsReadInARow++;
		return true;
	}
	return false;
}

UDPNetConnection* NetSession::UpdateUDPNetConnection(NetPacket* packet, sockaddr_in* addr)
{
	UDPNetConnection* conn = GetUDPNetConnectionForSockAddr(addr);
	if (conn == nullptr)
	{
		return nullptr;
	}
	packet->SetConnectionID(conn->m_index);
	if(packet->GetAck() <= conn->GetHighestRecvAck() && conn->m_connState.m_connStatus != UNCOMFIRMED)
	{
		packet->m_treatMessagesAsAlreadyRead = true;
	}
	NetPacketHeader header = packet->GetHeader();
	conn->MarkPacketRecvd(header);
	return conn;
}

bool NetSession::ReadNextMessageFromPacket(NetMessage* msg, NetPacket& packet, size_t& newMsgSize, sockaddr_in* out_addr,
	bool& actuallyReadIt)
{
	if (msg == nullptr)
	{
		return false;
	}
	size_t msgHeader = GetHeaderSize(NETMSG_PING);
	if (packet.GetReadableBytes() < msgHeader
		|| packet.GetNumOfReadMsgs() >= packet.GetNumOfMsgs())
	{
		return false;
	}
	packet.IncrementNumOfReadMsgs();
	uint16_t msg_size = 0;
	uint8_t id = 0;
	packet.Read<uint16_t>(&msg_size, sizeof(uint16_t));

	if (msg_size > MTU_MSG)
	{
		return false;
	}
	packet.Read<uint8_t>(&id, sizeof(uint8_t));
	size_t realHeaderSize = GetHeaderSize(id);
	msgHeader = realHeaderSize;
	if (msg_size < msgHeader)
	{
		return false;
	}
	
	msgHeader -= sizeof(uint8_t);
	size_t realMsgSize = msg_size - realHeaderSize;
	newMsgSize = realMsgSize;
	if (NetSession::GetIfMsgTypeRequiresConnection(id) == true && DoesConnectionExist((sockaddr*)out_addr) == false)
	{
		actuallyReadIt = false;
		return true;
	}
	UDPNetConnection* conn = GetUDPNetConnectionForSockAddr(out_addr);
	bool msgTypeReliable = NetSession::GetIfMsgTypeIsReliable(id);
	bool msgTypeInOrder = NetSession::GetIfMsgTypeIsInOrder(id);
	//bool msgTypeReliableAndInOrder = msgTypeReliable && msgTypeInOrder;

	//Read Header
	if (msgTypeReliable && msgHeader >= sizeof(uint16_t))
	{
		uint16_t reliableID = 0;
		packet.Read<uint16_t>(&reliableID, sizeof(uint16_t));
		msgHeader -= sizeof(uint16_t);
		msg->SetReliableID(reliableID);
	}

	if (msgTypeInOrder && msgHeader >= sizeof(uint8_t) + sizeof(uint16_t))
	{
		msgHeader -= sizeof(uint16_t);
		msgHeader -= sizeof(uint8_t);
		uint16_t sequenceID = 0;
		uint8_t chanID = 0;
		packet.Read<uint16_t>(&sequenceID, sizeof(uint16_t));
		packet.Read<uint8_t>(&chanID, sizeof(uint8_t));
		msg->SetSequenceID(sequenceID);
		msg->SetChannelID(chanID);
	}
	

	if (msgTypeReliable == true)
	{
		if (conn != nullptr && conn->DontReadReliableID(msg->GetReliableID()))
		{
			if (realMsgSize > 0)
			{
				char* buffer = new char[realMsgSize];
				packet.ReadForwards<char>(buffer, realMsgSize);
				delete buffer;
			}
			actuallyReadIt = false;
			return true;
		}
	}

	if (realMsgSize > 0)
	{
		char* buffer = new char[realMsgSize];
		char* car = (char*)packet.GetHead();
		car;
		packet.ReadForwards<char>(buffer, realMsgSize);
		msg->WriteForward<char>(buffer, realMsgSize);
		delete buffer;
	}
	msg->SetReadableBytes(realMsgSize);
	msg->SetMsgID(id);
	

	return true;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Setters
STATIC bool NetSession::ToggleEnableConnectionTimeOuts()
{
	s_EnableConnectionTimeOuts = !s_EnableConnectionTimeOuts;
	return s_EnableConnectionTimeOuts;
}

STATIC void NetSession::SetEnableConnectionTimeOuts(bool enable)
{
	s_EnableConnectionTimeOuts = enable;
}

void NetSession::SetLastError(DisconnectionReason reason)
{
	lastError = reason;
}

void NetSession::SetLastUpdateTime(uint64_t time)
{
	last_update_time = time;
}

void NetSession::SetAmHost(bool host)
{
	m_amHost = host;
}

void NetSession::SetWantToDisconnect(bool disconnect)
{
	m_wantsToDisconnect = disconnect;
}

void NetSession::SetMyConnection(PacketChannel* chan)
{
	my_socket = chan;
}

void NetSession::SetPeerToPeer(bool peerToPeer)
{
	NetSessionState state = (NetSessionState)machine.GetCurrentStateID();
	if (state != CONNECTED_NETSESSION
		&& state != HOSTING_NETSESSION
		&& state != JOINING_NETSESSION)
	{
		m_peerToPeer = peerToPeer;
	}
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Getters
sockaddr_in NetSession::GetSocketAddress()
{
	if (my_socket == nullptr)
	{
		return sockaddr_in();
	}
	return my_socket->GetAddress();
}

uint16_t NetSession::GetHeaderSize(byte_t msgID) const
{
	uint16_t header_size = (sizeof(uint8_t));// sizeof(uint8_t);
	if (msg_definitions[msgID].m_Reliable)
	{
		header_size += sizeof(uint16_t);
	}
	if (msg_definitions[msgID].m_inOrder)
	{
		header_size += sizeof(uint16_t) + sizeof(uint8_t); //Sequince id and channel id
	}
	return header_size;
}

uint16_t NetSession::GetPayloadSize(const NetMessage& msg)
{
	return (uint16_t)msg.GetOffset();
}

NetMessageDefinitionTemplate* NetSession::FindDefinition(byte_t msgId)
{
	if (msgId > 256 || msg_definitions[msgId].m_set == false)
	{
		return nullptr;
	}
	return &msg_definitions[msgId];
}

PacketChannel* NetSession::GetMyConnect() const
{
	return my_socket;
}

size_t NetSession::GetMaxNumOfConnectionsAllowed() const
{
	return maxNumConnectionsAllowed;
}

bool NetSession::GetAmHost() const
{
	return m_amHost;
}

bool NetSession::DoesConnectionExist(sockaddr* addr) const
{
	if (addr == nullptr)
	{
		return false;
	}

	bool connectionExists = false;
	for (size_t i = 0; i < connections.size(); i++)
	{
		if (connections.at(i) != nullptr)
		{
			if (NetworkSystem::DetermineIfSockAddrsAreTheSame(*(sockaddr_in*)addr, connections.at(i)->address))
			{
				connectionExists = true;
				break;
			}
		}
	}
	return connectionExists;
}

const std::vector<UDPNetConnection*> NetSession::GetConnections() const
{
	return connections;
}

const std::vector<ObjAndIDPair*> NetSession::GetNonConnNetObjects() const
{
	return m_nonConnObjects;
}

STATIC bool NetSession::GetEnableConnectionTimeOuts()
{
	return s_EnableConnectionTimeOuts;
}

bool NetSession::GetIfMsgTypeRequiresConnection(uint8_t msgType)
{
	return msg_definitions[msgType].m_RequiresConnection;
}

bool NetSession::GetIfMsgTypeIsReliable(uint8_t msgType)
{
	return msg_definitions[msgType].m_Reliable;
}

bool NetSession::GetIfMsgTypeIsInOrder(uint8_t msgType)
{
	return msg_definitions[msgType].m_inOrder;
}

uint8_t NetSession::GetMsgTypeChannelID(uint8_t msgType)
{
	return msg_definitions[msgType].m_channelID;
}

unsigned int NetSession::GetCurrentSessionState() const
{
	return machine.GetCurrentStateID();
}

DisconnectionReason NetSession::GetDisconnectionReasonCode() const
{
	return lastError;
}

StateMachine<NetSession*>* NetSession::GetMachine()
{
	return &machine;
}

UDPNetConnection* NetSession::GetHostConnection() const
{
	return hostConnection;
}

bool NetSession::GetHostStillAcceptingConnections() const
{
	return m_asHostStillAcceptingConnections;
}

uint64_t NetSession::GetLastUpdateTime() const
{
	return last_update_time;
}

void* NetSession::GetObjectForNetID(uint8_t netID)
{
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		ObjConnectionPair* par = m_objects.at(i);
		if (par != nullptr && par->connectionID == netID)
		{
			return par->object;
		}
	}
	return nullptr;
}

void* NetSession::GetObjectForSockaddr(const sockaddr_in& addr)
{
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		ObjConnectionPair* par = m_objects.at(i);
		if (par != nullptr && par->connection != nullptr)
		{
			if (NetworkSystem::DetermineIfSockAddrsAreTheSame(addr, par->connection->address) == 0)
			{
				return par->object;
			}

		}
	}

	return nullptr;
}

void* NetSession::GetMyConnectionObject()
{
	if (myConn == nullptr)
	{
		return nullptr;
	}
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		ObjConnectionPair* par = m_objects.at(i);
		if (par != nullptr && par->connection == myConn)
		{
			return par->object;
		}
	}
	return nullptr;
}

uint8_t NetSession::GetNetIDForObject(void* obj, bool& found)
{
	found = false;
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		ObjConnectionPair* par = m_objects.at(i);
		if (par != nullptr && par->object == obj)
		{
			found = true;
			return par->connectionID;
		}
	}
	return 0;
}

bool NetSession::GetPeerToPeer() const
{
	return m_peerToPeer;
}

ObjAndIDPair* NetSession::GetNonConnObjForUniqueID(int uniqueID) const
{
	for (size_t i = 0; i < m_nonConnObjects.size(); i++)
	{
		if (m_nonConnObjects.at(i) != nullptr && m_nonConnObjects.at(i)->uniqueID == uniqueID)
		{
			return m_nonConnObjects.at(i);
		}
	}
	return nullptr;
}


STATIC void NetSession::WriteUDPNetConnectionToMessage(UDPNetConnection* conn, NetMessage& msg)
{
	if (conn == nullptr)
	{
		return;
	}
	const char* addrToPrint = NetworkSystem::SockAddrToString((sockaddr*)&conn->address);
	size_t addrToPrintLen = strlen(addrToPrint);
	size_t len = strlen(conn->guid);

	//IP addr
	msg.WriteForward<size_t>(&addrToPrintLen, sizeof(size_t));
	msg.WriteForward<char>(addrToPrint, addrToPrintLen);

	//Net Index
	msg.WriteForward<uint8_t>(&conn->m_index, sizeof(uint8_t));

	//guid
	msg.WriteForward<size_t>(&len, sizeof(size_t));
	msg.WriteForward<char>(conn->guid, len);

}

STATIC UDPNetConnection* NetSession::ReadUDPNetConnectionFromMessage(NetMessage& msg, bool& alreadyHad, uint8_t& orgIdxIfAlreadyHad, 
	char* wantedGuid)
{
	if (NetSession::g_NetSession == nullptr)
	{
		g_Console->ConsolePrintf(WARNING, "Failed to add a connection mentioned by host.");
		g_Console->ConsolePrintf(WARNING, "NetSession is null.");
		return nullptr;
	}

	size_t addrToPrintLen = 0;
	char addrStr[32];
	for (size_t i = 0; i < 32; i++)
	{
		addrStr[i] = '\0';
	}
	msg.ReadForwards<size_t>(&addrToPrintLen, sizeof(size_t));
	msg.ReadForwards<char>(addrStr, addrToPrintLen);

	size_t numPeriods = 0;
	const char* addr = NetworkSystem::GetHostAddressFromIP(addrStr, numPeriods);
	const char* port = NetworkSystem::GetServiceFromIP(addrStr);
	if (addr == nullptr
		|| port == nullptr)
	{
		if (addr != nullptr)
		{
			delete addr;
		}
		if (port != nullptr)
		{
			delete port;
		}
		g_Console->ConsolePrintf(WARNING, "Failed to add a connection mentioned by host.");
		g_Console->ConsolePrintf(WARNING, "Invalid IP.");
		return nullptr;
	}

	sockaddr_in addrSock = NetworkSystem::StringToSockAddrIPv4(addr, (uint16_t)atoi(port));
	delete port;
	delete addr;

	UDPNetConnection* conn = NetSession::g_NetSession->GetUDPNetConnectionForSockAddr(&addrSock);
	alreadyHad = true;
	if (conn == nullptr)
	{
		conn = new UDPNetConnection(addrSock);
		alreadyHad = false;
	}

	uint8_t connIdx = 0;
	msg.ReadForwards<uint8_t>(&connIdx, sizeof(uint8_t));
	orgIdxIfAlreadyHad = conn->m_index;
	conn->m_index = connIdx;

	for (size_t i = 0; i < MAX_GUID_LENGTH; i++)
	{
		wantedGuid[i] = '\0';
	}
	size_t guidLen = 0;
	msg.ReadForwards<size_t>(&guidLen, sizeof(size_t));
	msg.ReadForwards<char>(wantedGuid, guidLen);


	return conn;
}