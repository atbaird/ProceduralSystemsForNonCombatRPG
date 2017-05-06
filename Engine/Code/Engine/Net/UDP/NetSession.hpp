#pragma once

#ifndef NETSESSION_HPP
#define NETSESSION_HPP
#include "Engine/Net/UDP/NetMessage.hpp"
#include "Engine/Net/UDP/NetPacket.hpp"
#include "Engine/Net/UDP/PacketChannel.hpp"
#include "Engine/Net/UDP/UDPNetConnection.hpp"
#include "Engine/EventSystem/Event.hpp"
#include "Engine/Core/States/StateMachine.hpp"
#include "Engine/Net/UDP/Data/ObjConnectionPair.hpp"
#include "Engine/Net/UDP/Data/ObjAndIDPair.hpp"
#include "Engine/Net/UDP/Data/NetSessionState.hpp"
#include "Engine/Net/UDP/Data/DisconnectionReason.hpp"

#define GAME_PORT 4334
#define STATIC

typedef void(*net_message_cb)(const NetSenderTemplate& NetSender, NetMessage& msg);

class NetSession
{
private:
protected:
	static bool s_EnableConnectionTimeOuts;
	static const uint8_t s_numOfMsgDefs;
	static int s_maxNumPacketsToReadInARow;
	PacketChannel* my_socket;
	NetMessageDefinitionTemplate msg_definitions[256];
	std::vector<UDPNetConnection*> connections;
	std::vector<ObjConnectionPair*> m_objects;
	std::vector<ObjAndIDPair*> m_nonConnObjects;
	std::vector<uint8_t> availableConnectionIDs;
	size_t my_networkID;
	size_t numPacketsReadInARow;
	size_t maxNumConnectionsAllowed;
	uint8_t nextAvailableNetworkID;
	bool m_amHost;
	bool m_peerToPeer;
	bool m_enableDebugDisplay;
	bool m_asHostStillAcceptingConnections;
	bool m_wantsToDisconnect;
	uint64_t last_update_time;
	float tick_rate;

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
	Event<UDPNetConnection*> on_connection_joined;
	Event<UDPNetConnection*> on_connection_leave;
	Event<UDPNetConnection*> on_net_tick;
	Event<UDPNetConnection*> on_connection_join_non_peer_to_peer;
	Event<float> in_mid_update;
	static NetSession* g_NetSession;
	//Constructions
	NetSession();
	virtual ~NetSession();

	//Running
	static bool NetSessionCreate();
	static bool NetSessionSetup();
	static bool NetSessionCreateSocketAndDontCareAboutHostingOrJoining(const char* port,
		const char* minPort = NetConstants::s_minPortDefault, const char* maxPort = NetConstants::s_maxPortDefault);
	static bool NetSessionHost(const char* username, const char* addr, const char* port,
		const char* minPort = NetConstants::s_minPortDefault, const char* maxPort = NetConstants::s_maxPortDefault);
	static bool NetSessionJoin(const char* username, const char* myAdr, const char* myPort, const char* hostAdr, const char* hostPort);
	static bool NetSessionStop();
	static bool NetSessionLeave();
	
	static bool NetSessionEnd();
	static void NetUpdate(float dt);
	static void NetSessionDestroy();
	virtual bool Start(const char* port, const char* minPort = NetConstants::s_minPortDefault, 
		const char* maxPort = NetConstants::s_maxPortDefault);
	virtual bool End();
	virtual void Update(float dt);
	virtual bool IsRunning() const;
	virtual void Listen();
	virtual void StopListening();

	//Connection related
	virtual bool AddConnection(UDPNetConnection* conn, uint8_t idx);
	virtual bool AddConnection(UDPNetConnection* conn);
	virtual bool DestroyConnection(UDPNetConnection* conn);
	virtual bool DestroyConnection(uint8_t networkID);
	virtual UDPNetConnection* RemoveConnection(uint8_t networkID);
	virtual void SetMaxNumberOfConnections(uint8_t connCount);
	virtual bool TestIfIdxIsAvailable(uint8_t idx) const;
	virtual bool TestIfNameIsTaken(std::string& name) const;
	virtual bool TestIfIPAlreadyExists(const char* fullIPAddr) const;
	virtual UDPNetConnection* CreateUDPNetConnectionFromSockAddr(sockaddr_in& addr);
	virtual UDPNetConnection* GetUDPNetConnectionForSockAddr(sockaddr_in& addr);
	virtual UDPNetConnection* GetUDPNetConnectionForSockAddr(sockaddr_in* addr);
	virtual UDPNetConnection* GetUDPNetConnectionForNetID(uint8_t netID);
	virtual void SetObjectForNetID(void* object, uint8_t netID);
	virtual void AddNonConnObject(void* object, int uniqueID);
	virtual void RemoveNonConnObjectByID(int uniqueID);
	virtual void RemoveNonConnObjectByObject(void* object);
	virtual void RemoveNonConnObjectByObjAndID(void* object, int uniqueID);
	virtual void ClearAllConnections();

	//Message Related
	virtual void RegisterMessage(uint8_t msgType, const char* debug_name, net_message_cb func, 
		bool requiresConnection = false, bool reliable = false, bool inOrder = false, uint8_t chanID = (uint8_t)0);
	virtual int RegisterMessage(const char* debug_name, net_message_cb func, bool requiresConnection = false,
		bool reliable = false, bool inOrder = false, uint8_t chanID = (uint8_t)0);
	virtual bool SendMessageDirect(const sockaddr_in& addr, NetMessage& msg);
	virtual void SendMessageAll(NetMessage& msg);
	virtual void SendMessageAllExcept(NetMessage& msg, UDPNetConnection* conn);
	virtual bool BroadcastMessageDirect(NetMessage& msg);
	virtual void WriteMessageToPacket(NetPacket* packet, const NetMessage& msg);
	virtual void ProcessIncomingPackets();
	virtual void ProcessOutgoingPackets(float& dt);
	virtual bool ReadNextPacketFromSocket(NetPacket* out_packet, sockaddr_in* out_addr, UDPNetConnection*& conn);
	virtual UDPNetConnection* UpdateUDPNetConnection(NetPacket* packet, sockaddr_in* addr);
	virtual bool ReadNextMessageFromPacket(NetMessage* msg, NetPacket& packet, size_t& newMsgSize, sockaddr_in* out_addr,
		bool& actuallyReadIt);

	//Setters
	static bool ToggleEnableConnectionTimeOuts();
	static void SetEnableConnectionTimeOuts(bool enable);
	virtual void SetLastError(DisconnectionReason reason);
	virtual void SetLastUpdateTime(uint64_t time);
	virtual void SetAmHost(bool host);
	virtual void SetWantToDisconnect(bool disconnect);
	virtual void SetMyConnection(PacketChannel* chan); 
	virtual void SetPeerToPeer(bool peerToPeer);

	//Getters
	virtual sockaddr_in GetSocketAddress();
	virtual uint16_t GetHeaderSize(byte_t msgID) const;
	virtual uint16_t GetPayloadSize(const NetMessage& msg);
	virtual NetMessageDefinitionTemplate* FindDefinition(byte_t msgId);
	virtual PacketChannel* GetMyConnect() const;
	virtual size_t GetMaxNumOfConnectionsAllowed() const;
	virtual bool GetAmHost() const;
	virtual bool DoesConnectionExist(sockaddr* addr) const;
	virtual const std::vector<UDPNetConnection*> GetConnections() const;
	virtual const std::vector<ObjAndIDPair*> GetNonConnNetObjects() const;
	static bool GetEnableConnectionTimeOuts();
	virtual bool GetIfMsgTypeRequiresConnection(uint8_t msgType);
	virtual bool GetIfMsgTypeIsReliable(uint8_t msgType);
	virtual bool GetIfMsgTypeIsInOrder(uint8_t msgType);
	virtual uint8_t GetMsgTypeChannelID(uint8_t msgType);
	virtual unsigned int GetCurrentSessionState() const;
	virtual DisconnectionReason GetDisconnectionReasonCode() const;
	virtual StateMachine<NetSession*>* GetMachine();
	virtual UDPNetConnection* GetHostConnection() const;
	virtual bool GetHostStillAcceptingConnections() const;
	virtual uint64_t GetLastUpdateTime() const;
	virtual void* GetObjectForNetID(uint8_t netID);
	virtual void* GetObjectForSockaddr(const sockaddr_in& addr);
	virtual void* GetMyConnectionObject();
	virtual uint8_t GetNetIDForObject(void* obj, bool& found);
	virtual bool GetPeerToPeer() const;
	virtual ObjAndIDPair* GetNonConnObjForUniqueID(int uniqueID) const;

	static void WriteUDPNetConnectionToMessage(UDPNetConnection* conn, NetMessage& msg);
	static UDPNetConnection* ReadUDPNetConnectionFromMessage(NetMessage& msg, bool& alreadyHad, uint8_t& orgIdxIfAlreadyHad,
		char* wantedGuid);

};
//void NetCommand(net_ping)
//{
//	sockaddr_in to = AddrFromString(args.get_next_string());
//	NetMessage ping(PING);
//	ping.write_string(args.get_next_string());
//	session.send_message_direct(to, ping);
//}

#endif