#include "Engine/Net/UDP/TimeStampedPacket.hpp"

TimeStampedPacket::TimeStampedPacket()
	:m_packet(NetPacket()),
	m_timeStamp(0),
	m_endTimeStamp(0),
	m_from_addr(sockaddr_in())
{

}

void TimeStampedPacket::StampPacket()
{
	m_timeStamp = Performance::GetCurrentPerformanceCount();
}