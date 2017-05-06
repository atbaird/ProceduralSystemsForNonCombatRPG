#pragma once

#ifndef NETPACKET_HPP
#define NETPACKET_HPP
#include "Engine/Utils/BytePacker.hpp"
#include "Engine/Net/NetworkSystem.hpp"
#include "Engine/Net/UDP/Data/NetPacketHeader.hpp"

#define PACKET_MTU 1232
#define STATIC
class NetPacket : public BytePacker
{
private:
protected:
	NetPacketHeader m_packetHeader;
	
public:
	bool m_treatMessagesAsAlreadyRead;
	//Constructors
	NetPacket();
	virtual ~NetPacket();

	//Write
	size_t WriteOutNumOfMsgs();
	void WriteOutSockAddr(const sockaddr* addr);
	void WritePacketHeader();
	void* WritePacketHeaderWithReserve();
	void WriteEndianness();

	//Read
	size_t ReadInNumOfMsgs();
	void ReadInSockAddr(sockaddr_in* out_addr);
	bool ReadPacketHeader();
	bool ReadEndianness();

	//Setters
	void SetConnectionID(uint8_t connID);
	void SetNumOfMsgs(size_t msgCnt);
	void IncrementNumOfMsgs();
	void ResetNumOfMsgs();
	void IncrementNumOfReadMsgs();
	void ResetNumOfReadMsgs();
	void SetAck(uint16_t ack);
	uint16_t GetNextAck(uint16_t& nextAvailableAck);
	void SetHighestRecvAck(uint16_t);
	void SetPreviousRecvAck(uint16_t);

	//Getters
	uint8_t GetNetworkID() const;
	size_t GetNumOfMsgs() const;
	size_t GetNumOfReadMsgs() const;
	uint16_t GetAck() const;
	uint16_t GetHighestRecvAck() const;
	uint16_t GetPreviousRecvAckBf() const;
	NetPacketHeader GetHeader() const;
	static const size_t GetExpectedHeaderSize();

};
#endif