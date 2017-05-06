#pragma once

#ifndef NETMESSAGE_HPP
#define NETMESSAGE_HPP
#include "Engine/Utils/BytePacker.hpp"
#include "Engine/Net/NetworkSystem.hpp"
#include "Engine/Net/UDP/Data/NetSenderTemplate.hpp"
#include "Engine/Net/UDP/Data/NetMessageDefinitionTemplate.hpp"
#include "Engine/Net/UDP/Data/eNetCoreMessageTypes.hpp"


#define MTU_MSG 1024

class NetSession;
class NetPacket;

class NetMessage : public BytePacker
{
private:
protected:
	uint8_t m_msgId;
	bool m_canWriteMore;
	uint16_t m_sequenceId; //only for Sequence in order
	uint16_t m_reliableID;
	uint16_t m_ackID;
	uint8_t m_channelID;
	uint64_t m_timeFirstSent;
	uint64_t m_timeSent;
public:
	NetSenderTemplate m_sender;
	NetMessageDefinitionTemplate* m_defn;

	//Constructors
	NetMessage(void* packetHead = nullptr, size_t msgSize = MTU_MSG);
	NetMessage(uint8_t msgType);
	virtual ~NetMessage();

	//Setters
	void SetMsgID(uint8_t id);
	void Finalize(NetSession* session);
	void SetReliableID(uint16_t id);
	void SetAckID(uint16_t id);
	void SetSequenceID(uint16_t seqID);
	void SetChannelID(uint8_t chanID);
	void StampTimeSent();
	void CopyTimeSentToFirstTimeSent();

	//Getters
	const uint8_t GetMessageID() const;
	const bool GetCanWriteMore() const;
	uint16_t GetReliableID() const;
	uint16_t GetAckID() const;
	uint16_t GetSequenceID() const;
	uint8_t GetChannelID() const;
	uint64_t GetTimeSent() const;
	uint64_t GetTimeFirstSent() const;
	static NetMessage* MakeCopy(const NetMessage& original);

	//operations
	void WriteMessageIntoPacket(NetPacket& packet, uint16_t msgSize, uint16_t payloadSize);
	bool ReadMessageFromPacket(NetPacket& packet, sockaddr_in* out_addr, bool& actuallyReadIt);
	
protected:
	bool ReadMessageHeaderFromPacket(NetPacket& packet, sockaddr_in* out_addr, bool& actuallyReadIt, size_t& msgHeader);
	bool ReadMessageContentsFromPacket(NetPacket& packet, sockaddr_in* out_addr, bool& actuallyReadIt, size_t& msgHeader);

};

#endif