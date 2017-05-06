#include "Engine/Net/UDP/NetPacket.hpp"


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Constructors
NetPacket::NetPacket()
	: BytePacker(new byte_t[PACKET_MTU], PACKET_MTU),
	m_treatMessagesAsAlreadyRead(false),
	m_packetHeader(NetPacketHeader())
{
	for (size_t i = 0; i < PACKET_MTU; i++)
	{
		byte_t* ptr = ((byte_t*)GetTail()) + i;
		*ptr = 0;
	}
}

NetPacket::~NetPacket()
{
	BytePacker::~BytePacker();
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Write
size_t NetPacket::WriteOutNumOfMsgs()
{
	size_t written = Write<size_t>(&m_packetHeader.numOfMsgs, sizeof(size_t));
	return written;
}

void NetPacket::WriteOutSockAddr(const sockaddr* addr)
{
	const char* IP = NetworkSystem::SockAddrToString(addr);
	WriteString(IP);
}

void NetPacket::WritePacketHeader()
{
	const size_t packetHeaderSize = GetExpectedHeaderSize();
	//WriteEndiannes();
	Write<size_t>(&packetHeaderSize, sizeof(size_t));

	Write<size_t>(&m_packetHeader.numOfMsgs, sizeof(size_t));
	Write<uint16_t>(&m_packetHeader.Ack, sizeof(uint16_t));
	Write<uint16_t>(&m_packetHeader.highest_recv_ack, sizeof(uint16_t));
	Write<uint16_t>(&m_packetHeader.previous_recv_ack, sizeof(uint16_t));
}

void* NetPacket::WritePacketHeaderWithReserve()
{
	const size_t packetHeaderSize = GetExpectedHeaderSize();
	//WriteEndiannes();
	Write<size_t>(&packetHeaderSize, sizeof(size_t));

	void* bk = ReserveWriteBookmark<size_t>(&m_packetHeader.numOfMsgs, sizeof(size_t));
	Write<uint16_t>(&m_packetHeader.Ack, sizeof(uint16_t));
	Write<uint16_t>(&m_packetHeader.highest_recv_ack, sizeof(uint16_t));
	Write<uint16_t>(&m_packetHeader.previous_recv_ack, sizeof(uint16_t));
	return bk;
}

void NetPacket::WriteEndianness()
{
	uint8_t en = 1;
	Write<uint8_t>(&en, sizeof(uint8_t));
}


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Read
size_t NetPacket::ReadInNumOfMsgs()
{
	return Read<size_t>(&m_packetHeader.numOfMsgs, sizeof(m_packetHeader.numOfMsgs));
}

void NetPacket::ReadInSockAddr(sockaddr_in* out_addr)
{
	const char* IP = ReadString();
	if (IP == nullptr)
	{
		return;
	}
	size_t numPeriods = 0;
	const char* IPaddr = NetworkSystem::GetHostAddressFromIP(IP, numPeriods);
	const char* IPport = NetworkSystem::GetServiceFromIP(IP);
	*out_addr = NetworkSystem::StringToSockAddrIPv4(IPaddr, (uint16_t)atoi(IPport));
	delete IPaddr;
	delete IPport;
}

bool NetPacket::ReadPacketHeader()
{
	const size_t expectedPacketHeaderSize = GetExpectedHeaderSize();
	size_t packetHeaderSize = 0;
	//bool readEndianness = ReadEndianness();
	//if (readEndianness == false)
	//{
	//	return false;
	//}
	Read<size_t>(&packetHeaderSize, sizeof(size_t));
	if (packetHeaderSize != expectedPacketHeaderSize)
	{
		return false;
	}
	Read<size_t>(&m_packetHeader.numOfMsgs, sizeof(size_t));
	Read<uint16_t>(&m_packetHeader.Ack, sizeof(uint16_t));
	Read<uint16_t>(&m_packetHeader.highest_recv_ack, sizeof(uint16_t));
	Read<uint16_t>(&m_packetHeader.previous_recv_ack, sizeof(uint16_t));
	return true;
}

bool NetPacket::ReadEndianness()
{
	uint8_t test = 0;
	Read<uint8_t>(&test, sizeof(uint8_t));
	if (test == 0)
	{
		return false;
	}

	if (test == 128)
	{
		endianness = GetOppositeEndianness(endianness);
	}
	else if (test != 1)
	{
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Setters
void NetPacket::SetConnectionID(uint8_t connID)
{
	m_packetHeader.connID = connID;
}

void NetPacket::SetNumOfMsgs(size_t msgCnt)
{
	m_packetHeader.numOfMsgs = msgCnt;
}

void NetPacket::IncrementNumOfMsgs()
{
	m_packetHeader.numOfMsgs++;
}

void NetPacket::ResetNumOfMsgs()
{
	m_packetHeader.numOfMsgs = 0;
}

void NetPacket::IncrementNumOfReadMsgs()
{
	m_packetHeader.numOfReadMsgs++;
}

void NetPacket::ResetNumOfReadMsgs()
{
	m_packetHeader.numOfReadMsgs = 0;
}


void NetPacket::SetAck(uint16_t ack)
{
	m_packetHeader.Ack = ack;
}

uint16_t NetPacket::GetNextAck(uint16_t& nextAvailableAck)
{
	m_packetHeader.Ack = nextAvailableAck;
	nextAvailableAck++;
	if (nextAvailableAck == NetPacketHeader::INVALID_PACKET_ACK)
	{
		nextAvailableAck++; //I honestly think this should be a nextAvailableAck = 0;
	}
	return m_packetHeader.Ack;
}

void NetPacket::SetHighestRecvAck(uint16_t val)
{
	m_packetHeader.highest_recv_ack = val;
}

void NetPacket::SetPreviousRecvAck(uint16_t val)
{
	m_packetHeader.previous_recv_ack = val;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Getters
uint8_t NetPacket::GetNetworkID() const
{
	return m_packetHeader.connID;
}

size_t NetPacket::GetNumOfMsgs() const
{
	return m_packetHeader.numOfMsgs;
}

size_t NetPacket::GetNumOfReadMsgs() const
{
	return m_packetHeader.numOfReadMsgs;
}


uint16_t NetPacket::GetAck() const
{
	return m_packetHeader.Ack;
}

uint16_t NetPacket::GetHighestRecvAck() const
{
	return m_packetHeader.highest_recv_ack;
}

uint16_t NetPacket::GetPreviousRecvAckBf() const
{
	return m_packetHeader.previous_recv_ack;
}

NetPacketHeader NetPacket::GetHeader() const
{
	return m_packetHeader;
}

const size_t NetPacket::GetExpectedHeaderSize()
{
	NetPacketHeader packetHeader;
	return packetHeader.GetSizeOfHeaderSent();
}