#pragma once

#ifndef NETPACKETHEADER_HPP
#define NETPACKETHEADER_HPP
#include "Engine/Net/NetworkSystem.hpp"

struct NetPacketHeader
{
public:
	static uint16_t INVALID_PACKET_ACK;
	static uint16_t maxPacketDifference;
	//Variables
	size_t numOfMsgs;
	size_t numOfReadMsgs;
	uint8_t connID;
	uint16_t Ack;
	uint16_t highest_recv_ack;
	uint16_t previous_recv_ack; //bitfield
public:
	//Constructors
	NetPacketHeader();

	size_t GetSizeOfHeaderSent() const;

};
#endif