#pragma once

#ifndef TIMESTAMPEDPACKET_HPP
#define TIMESTAMPEDPACKET_HPP
#include "Engine/Net/UDP/NetPacket.hpp"
#include "Engine/Core/Performance/Performance.hpp"

struct TimeStampedPacket
{
public:
	NetPacket m_packet;
	uint64_t m_timeStamp;
	uint64_t m_endTimeStamp;
	sockaddr_in m_from_addr;
public:
	TimeStampedPacket();

	void StampPacket();
};
#endif