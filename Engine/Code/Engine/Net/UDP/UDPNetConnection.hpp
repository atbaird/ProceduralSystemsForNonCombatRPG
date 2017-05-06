#pragma once

#ifndef UDPNETCONNECTION_HPP
#define UDPNETCONNECTION_HPP
#include "Engine/Net/UDP/NetMessage.hpp"
#include "Engine/Net/ConnectionIDs.hpp"
#include "Engine/Net/UDP/Data/ConnectionState.hpp"
#include "Engine/Net/UDP/Data/ConnectionChannel.hpp"
#include <vector>

class NetSession;
class NetPacket;
struct NetPacketHeader;
class PacketChannel;

#define MAX_GUID_LENGTH 32
#define MAX_RELIABLE_RANGE 1024


struct UDPNetConnection
{
public:
	static const double s_secondsBetweenHeartBeats;
	static const double s_secondsBeforeResendReliables;
	static const double s_secondsBeforeDumpReceivedReliables;
	static size_t s_MaxActiveReliables;
	ConnectionState m_connState;
	//Ack Related
	uint16_t m_nextAvailableAck;
	uint16_t m_highestRecvAck;
	uint16_t m_prevRecvAck;
	uint16_t m_lastReportedHighestAck;
	uint16_t m_lastReportedRecvdAck;
	uint16_t m_nextAvailableReliableMessageID;
	uint16_t m_nextExpectedReliableID;

	//Time Stamps
	uint64_t m_timeLastDumpedReliables;
	uint64_t m_timeLastSentMessage;
	uint64_t m_timeLastRecvMessage;
	uint64_t m_createdAtTime;

	//General Connection
	uint8_t m_index;
	NetSession* session;
	PacketChannel* socket;
	sockaddr_in address;
	float tick_rate;
	char guid[MAX_GUID_LENGTH]; //Screen Name
	std::vector<NetMessage*> m_unreliables;
	std::vector<NetMessage*> m_unsentReliables;
	std::vector<NetMessage*> m_sentReliables;
	std::vector<ConnectionChannel> m_channels;
	std::vector<uint16_t> m_receivedReliables;
private:
	float m_timeSinceLastUpdate;
	uint64_t m_lastSentHeartbeatPacketAt;
	bool m_isHost;
	bool m_updateNow;
public:
	//Constructors
	UDPNetConnection();
	UDPNetConnection(sockaddr_in& addr);
	virtual ~UDPNetConnection();

	//Connection related
	void ConnSendMessage(NetMessage& msg);
	void Tick(float dt);
protected:
	size_t UDPNetConnection::SendTo(sockaddr_in to_addr, void* data, size_t data_size);
public:
	void SendPacket();
protected:
	void HeartbeatAndConnectionStatusUpdate();
	void LoadUnreliablesInPacket(NetPacket& packet, size_t& msgCnt);
	void ResendSentReliablesInPacket(NetPacket& packet, size_t& msgCnt);
	void LoadUnsentReliablesInPacket(NetPacket& packet, size_t& msgCnt);
public:
	void MarkMessageRecvd(NetMessage& msg);
	bool MarkMessageRecvdInOrder(NetMessage& msg);
	NetMessage* PopMessageRecvdInOrder(uint8_t channelID);
	void AddInOrderReliableMessageToBuffer(NetMessage& msg);
	void MarkPacketRecvd(NetPacketHeader& header);
	void ResizeNumOfChannels(uint8_t numChannels);
protected:
	void MarkReliableReceived(uint16_t reliableID);
	bool CycleGreaterThanOrEqual(uint16_t a, uint16_t b);
	void RemoveAllReliableIDsLessThan(uint16_t lessThanThis);
	void UpdateHighestReceived(uint16_t& curHighestRecvdAck, uint16_t& prevRecvdAcks, const uint16_t& ack);
	void ProcessConfirmedAcks(uint16_t& highestRecvdAcks, uint16_t& previousRecvdAcksBf);
	bool IsReliableConfirmed(uint16_t ackID);
	bool IsBitSet(size_t idx, size_t bitfield);
	void CleanReliables();
public:
	bool IsMe() const;
	bool DontReadReliableID(uint16_t reliableId) const;
	size_t GetNumberOfUnsentReliables() const;
	size_t GetNumberOfSentReliables() const;
	size_t GetNumberOfUnreliables() const;


	//operators
	const bool operator==(const UDPNetConnection& conn) const;

	//Setters
	void SetIsHost(const bool& isHost);
	void SetGuid(const char* gui);
	void StampLastRecvPacket();
	void UpdateLastRecvPacketInfo(NetPacket* packet);

	//Getters
	bool GetIsHost() const;
	bool GetUpdateNow() const;
	bool GetDisconnectMe() const;
	eConnectionStatus GetConnStatus() const; //m_connState.connStatus
	uint16_t GetHighestRecvAck() const;
	uint16_t GetPrevRecvAck() const;
	uint16_t GetLastRecvAck() const;
	uint64_t GetTimeLastRecvMessage() const;
	uint64_t GetTimeLastSentMessage() const;
	ConnectionState GetConnectionState() const;

};
#endif