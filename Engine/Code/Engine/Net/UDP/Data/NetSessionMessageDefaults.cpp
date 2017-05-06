#include "Engine/Net/UDP/Data/NetSessionMessageDefaults.hpp"
#include "Engine/Net/UDP/NetMessage.hpp"
#include "Engine/Net/UDP/NetSession.hpp"
#include "Engine/Console/Console.hpp"

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//DefaultMessageFunctions
STATIC void NetSessionMessageDefaults::OnPingReceived(const NetSenderTemplate& sender, NetMessage &msg)
{
	//char buffer[256];
	std::string msgStr;
	msg.ResetOffset();
	char *str = new char[msg.GetReadableBytes()];
	size_t read = msg.ReadForwards<char>(str, msg.GetReadableBytes() - 1);
	msgStr += std::string(str);

	if (g_Console != nullptr)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Ping received from " + std::string(NetworkSystem::SockAddrToString((const sockaddr*)&sender.m_address)) + ".");
		if (str == nullptr || read == 0)
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "null");
		}
		else
		{
			g_Console->ConsolePrintf(UNIMPORTANT, msgStr);
		}
	}

	NetMessage pong(NETMSG_PONG);

	if (NetSession::g_NetSession != nullptr)
	{
		NetSession::g_NetSession->SendMessageDirect(sender.m_address, pong);
	}
	delete str;
}

STATIC void NetSessionMessageDefaults::OnPongReceived(const NetSenderTemplate& sender, NetMessage& msg)
{
	msg;
	if (g_Console != nullptr)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Pong received from " + std::string(NetworkSystem::SockAddrToString((const sockaddr*)&sender.m_address)) + ".");
	}
}

STATIC void NetSessionMessageDefaults::OnHeartbeatReceived(const NetSenderTemplate& sender, NetMessage& msg)
{
	msg;
	NetMessage ack(NETMSG_ACK);
	if (NetSession::g_NetSession != nullptr)
	{
		NetSession::g_NetSession->SendMessageDirect(sender.m_address, ack);
	}
}

STATIC void NetSessionMessageDefaults::OnAckReceived(const NetSenderTemplate& sender, NetMessage& msg)
{
	sender;
	msg;
}


STATIC void NetSessionMessageDefaults::OnJoinRequestReceived(const NetSenderTemplate& sender, NetMessage& msg)
{
	if (NetSession::g_NetSession == nullptr)
	{
		return;
	}
	//guarantee host side first.
	if (NetSession::g_NetSession->GetAmHost() == false)
	{
		NetMessage msgSe = NetMessage(NETMSG_JOIN_DENY);
		DisconnectionReason reason = DisconnectionReason_JoinDeniedNotHost;
		msgSe.WriteForward<DisconnectionReason>(&reason, sizeof(DisconnectionReason));
		NetSession::g_NetSession->SendMessageDirect(sender.m_address, msgSe);
		return;
	}

	//guarantee host accepting connections
	if (NetSession::g_NetSession->GetHostStillAcceptingConnections() == false)
	{
		NetMessage msgSe = NetMessage(NETMSG_JOIN_DENY);
		DisconnectionReason reason = DisconnectionReason_JoinDeniedNotAcceptingNewConnections;
		msgSe.WriteForward<DisconnectionReason>(&reason, sizeof(DisconnectionReason));
		NetSession::g_NetSession->SendMessageDirect(sender.m_address, msgSe);
		return;
	}

	std::vector<UDPNetConnection*> connections = NetSession::g_NetSession->GetConnections();
	//Guarantee have enough room to add connection
	if (connections.size() >= NetSession::g_NetSession->GetMaxNumOfConnectionsAllowed())
	{
		NetMessage msgSe = NetMessage(NETMSG_JOIN_DENY);
		DisconnectionReason reason = DisconnectionReason_JoinDeniedFull;
		msgSe.WriteForward<DisconnectionReason>(&reason, sizeof(DisconnectionReason));
		NetSession::g_NetSession->SendMessageDirect(sender.m_address, msgSe);
		return;
	}

	//Need to handle case when Connection already connected.

	//guarantee desired guid does not already exist
	char desiredName[MAX_GUID_LENGTH];
	for (size_t i = 0; i < MAX_GUID_LENGTH; i++)
	{
		desiredName[i] = '\0';
	}
	size_t len;
	msg.ReadForwards<size_t>(&len, sizeof(len));
	msg.ReadForwards<char>(desiredName, len);
	for (size_t i = 0; i < connections.size(); i++)
	{
		UDPNetConnection* conn = connections.at(i);
		if (conn != nullptr)
		{
			if (NetworkSystem::DetermineIfSockAddrsAreTheSame(conn->address, sender.m_address))
			{
				//connection already added.
				return;
			}
			else if (strcmp(conn->guid, desiredName) == 0)
			{
				NetMessage msgSe = NetMessage(NETMSG_JOIN_DENY);
				DisconnectionReason reason = DisconnectionReason_JoinDeniedGuidInUse;
				msgSe.WriteForward<DisconnectionReason>(&reason, sizeof(DisconnectionReason));
				NetSession::g_NetSession->SendMessageDirect(sender.m_address, msgSe);
				return;
			}
		}
	}



	sockaddr_in copyAddr = sender.m_address;
	UDPNetConnection* conn = new UDPNetConnection(copyAddr);
	NetSession::g_NetSession->AddConnection(conn);
	conn->SetGuid(desiredName);
	uint8_t connID = conn->m_index;

	//Prep message to inform connections about new connection

	//Prep message to send to joined client.
	size_t strLen = strlen(NetSession::g_NetSession->myConn->guid);
	NetMessage msgSe = NetMessage(NETMSG_JOIN_ACCEPT);
	msgSe.WriteForward<uint8_t>(&connID, sizeof(uint8_t));
	msgSe.WriteForward<size_t>(&strLen, sizeof(size_t));
	msgSe.WriteForward<char>(NetSession::g_NetSession->myConn->guid, strLen);
	conn->ConnSendMessage(msgSe);

	if (NetSession::g_NetSession->GetCurrentSessionState() == CONNECTED_NETSESSION)
	{
		NetSession::g_NetSession->on_connection_joined.Trigger(conn);
	}

	//Update on all other IP addresses
	if (NetSession::g_NetSession->GetPeerToPeer())
	{
		NetSessionMessageDefaults::SendNewConnectionStatusUpdateFromHostMessages(sender, conn);
	}
	else
	{
		NetSession::g_NetSession->on_connection_join_non_peer_to_peer.Trigger(conn);
	}
}

STATIC void NetSessionMessageDefaults::OnJoinAcceptReceived(const NetSenderTemplate& sender, NetMessage& msg)
{
	msg;
	if (NetSession::g_NetSession == nullptr || NetSession::g_NetSession->GetMachine()->GetCurrentStateID() != JOINING_NETSESSION)
	{
		return;
	}
	if (NetworkSystem::DetermineIfSockAddrsAreTheSame(NetSession::g_NetSession->hostConnection->address, sender.m_address) == false)
	{
		return;
	}

	UDPNetConnection* conn = NetSession::g_NetSession->RemoveConnection(1);

	msg.ReadForwards<uint8_t>(&conn->m_index, sizeof(uint8_t));
	NetSession::g_NetSession->AddConnection(conn, conn->m_index);
	NetSession::g_NetSession->myConn = conn;

	size_t strLen = 0;
	char guid[MAX_GUID_LENGTH];
	for (size_t i = 0; i < MAX_GUID_LENGTH; i++)
	{
		guid[i] = '\0';
	}
	msg.ReadForwards<size_t>(&strLen, sizeof(size_t));
	msg.ReadForwards<char>(guid, strLen);
	NetSession::g_NetSession->hostConnection->SetGuid(guid);

	NetSession::g_NetSession->GetMachine()->ChangeStates(CONNECTED_NETSESSION);
}

STATIC void NetSessionMessageDefaults::OnJoinDeniedReceived(const NetSenderTemplate& sender, NetMessage& msg)
{
	if (NetSession::g_NetSession == nullptr || NetSession::g_NetSession->GetMachine()->GetCurrentStateID() != JOINING_NETSESSION)
	{
		return;
	}
	if (NetworkSystem::DetermineIfSockAddrsAreTheSame(NetSession::g_NetSession->hostConnection->address, sender.m_address) == false)
	{
		return;
	}
	DisconnectionReason reason;
	msg.ReadForwards<DisconnectionReason>(&reason, sizeof(DisconnectionReason));
	NetSession::g_NetSession->SetLastError(reason);
	NetSession::g_NetSession->GetMachine()->ChangeStates(DISCONNECTED_NETSESSION);
}

STATIC void NetSessionMessageDefaults::OnNewConnectionStatusUpdateFromHost(const NetSenderTemplate& sender, NetMessage& msg)
{
	sender;
	if (NetSession::g_NetSession == nullptr
		|| NetSession::g_NetSession->hostConnection == nullptr
		|| NetSession::g_NetSession->GetAmHost() == true
		|| NetworkSystem::DetermineIfSockAddrsAreTheSame(msg.m_sender.m_address, NetSession::g_NetSession->hostConnection->address) == false)
	{
		return;
	}
	size_t numConns = 0;
	msg.ReadForwards<size_t>(&numConns, sizeof(size_t));
	if (numConns == 0)
	{
		g_Console->ConsolePrintf(WARNING, "Failed to add a connection mentioned by host.");
		g_Console->ConsolePrintf(WARNING, "0 number of connections inside message.");
	}
	for (size_t connIdx = 0; connIdx < numConns; connIdx++)
	{
		bool alreadyHad = false;
		uint8_t alreadyHadIdx = 0;
		char* wantedGuid = new char[MAX_GUID_LENGTH];
		UDPNetConnection* conn = NetSession::ReadUDPNetConnectionFromMessage(msg, alreadyHad, alreadyHadIdx, wantedGuid);

		if (conn == nullptr)
		{
			g_Console->ConsolePrintf(UNIMPORTANT, "Failed to add a connection mentioned by host. Conn null");
			delete wantedGuid;
			wantedGuid = nullptr;
			continue;
		}

		std::vector<UDPNetConnection*> connections = NetSession::g_NetSession->GetConnections();
		bool guidTaken = false;
		for (size_t otherConnIdx = 0; otherConnIdx < connections.size(); otherConnIdx++)
		{
			if (connections[otherConnIdx] != nullptr && conn != connections[otherConnIdx] 
				&& strcmp(connections[otherConnIdx]->guid, wantedGuid) == 0)
			{
				if (alreadyHad == false)
				{
					g_Console->ConsolePrintf(UNIMPORTANT, "Failed to add a connection mentioned by host.");
				}
				else
				{
					g_Console->ConsolePrintf(UNIMPORTANT, "Failed to update connection guid mentioned by host.");
				}
				g_Console->ConsolePrintf(UNIMPORTANT, "guid already taken.");
				guidTaken = true;
				delete wantedGuid;
				wantedGuid = nullptr;
				if (alreadyHad == false)
				{
					delete conn;
				}
				continue;
			}
		}

		if (guidTaken == false)
		{
			conn->SetGuid(wantedGuid);
			delete wantedGuid;
			wantedGuid = nullptr;
		}

		if (alreadyHad == false)
		{
			NetSessionMessageDefaults::OnNewConnectionStatusUpdateFromHostDidNotAlreadyHadTests(conn, alreadyHadIdx);
		}
	}
}

STATIC void NetSessionMessageDefaults::OnLeaveReceived(const NetSenderTemplate& sender, NetMessage& msg)
{
	msg;
	std::vector<UDPNetConnection*> connections = NetSession::g_NetSession->GetConnections();
	UDPNetConnection* hostConnection = NetSession::g_NetSession->GetHostConnection();
	for (size_t i = 0; i < connections.size(); i++)
	{
		UDPNetConnection* conn = connections.at(i);
		if (conn != nullptr)
		{
			if (NetworkSystem::DetermineIfSockAddrsAreTheSame(conn->address, sender.m_address) == true)
			{
				if (conn == hostConnection)
				{
					NetSession::g_NetSession->SetLastError(DisconnectionReason_ErrorHostDisconnected);
				}
				NetSession::g_NetSession->DestroyConnection(conn);
				break;
			}
		}
	}
}


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Protected methods
STATIC void NetSessionMessageDefaults::SendNewConnectionStatusUpdateFromHostMessages(const NetSenderTemplate& sender, UDPNetConnection* conn)
{
	sender;
	//Update on all other IP addresses
	const size_t numOfConn = 1;
	NetMessage msgToEveryoneElse = NetMessage(NETMSG_New_Connection_Status_Update_From_Host);
	{
		msgToEveryoneElse.WriteForward<size_t>(&numOfConn, sizeof(size_t));
	}
	NetSession::WriteUDPNetConnectionToMessage(conn, msgToEveryoneElse);

	//Note, this is a fast fix, This will only work so long as the size of all the IP addresses is LESS THAN the max size messages are allowed.
	std::vector<UDPNetConnection*> connections = NetSession::g_NetSession->GetConnections();
	for (size_t i = 0; i < connections.size(); i++)
	{
		UDPNetConnection* connTest = connections.at(i);
		if (connTest != nullptr)
		{
			if (connTest->IsMe() == false
				&& connTest != conn)
			{
				connTest->ConnSendMessage(msgToEveryoneElse);
			}
			else
			{
				continue;
			}
			NetMessage msgToNewConn = NetMessage(NETMSG_New_Connection_Status_Update_From_Host);
			msgToNewConn.WriteForward<size_t>(&numOfConn, sizeof(size_t));

			NetSession::WriteUDPNetConnectionToMessage(connTest, msgToNewConn);
			conn->ConnSendMessage(msgToNewConn);
		}
	}
}


STATIC void NetSessionMessageDefaults::OnNewConnectionStatusUpdateFromHostDidNotAlreadyHadTests(UDPNetConnection* conn, uint8_t alreadyHadIdx)
{
	alreadyHadIdx;
	std::vector<UDPNetConnection*> connections = NetSession::g_NetSession->GetConnections();
	bool isTaken = false;
	bool guidTaken = false;
	for (size_t j = 0; j < connections.size(); j++)
	{
		if (connections.at(j)->m_index == conn->m_index)
		{
			isTaken = true;
			break;
		}
	}
	if (isTaken)
	{
		delete conn;
		return;
	}
	if (guidTaken)
	{
		delete conn;
		return;
	}
	NetSession::g_NetSession->AddConnection(conn, conn->m_index);

	void* obj = NetSession::g_NetSession->GetObjectForNetID(conn->m_index);
	if (obj != nullptr)
	{
		return;
	}
	NetSession::g_NetSession->on_connection_joined.Trigger(conn);
}