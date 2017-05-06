#include "Engine/Net/UDP/Data/NetPacketHeader.hpp"

uint16_t NetPacketHeader::INVALID_PACKET_ACK = 0xffff;
uint16_t NetPacketHeader::maxPacketDifference = 10;

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Constructors
NetPacketHeader::NetPacketHeader()
	: numOfMsgs(0),
	numOfReadMsgs(0),
	connID(0),
	Ack(INVALID_PACKET_ACK),
	highest_recv_ack(0),
	previous_recv_ack(0)
{

}


size_t NetPacketHeader::GetSizeOfHeaderSent() const
{
	return sizeof(numOfMsgs) + sizeof(connID) + sizeof(Ack) + sizeof(highest_recv_ack) + sizeof(previous_recv_ack);
}