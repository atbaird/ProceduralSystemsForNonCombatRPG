#include "Engine/Net/UDP/NetMessage.hpp"
#include "Engine/Net/UDP/NetSession.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Net/UDP/NetSession.hpp"
#include "Engine/Net/UDP/NetPacket.hpp"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors

/*

uint8_t msgId;
bool canWriteMore;
uint16_t sequenceId; //only for Sequence in order
uint16_t reliableID;
uint16_t ackID;
uint8_t channelID;
uint64_t timeFirstSent;
uint64_t timeSent;
*/

NetMessage::NetMessage(void* packetHead, size_t msgSize)
	: BytePacker(packetHead, msgSize),
	m_msgId(0),
	m_canWriteMore(true),
	m_sequenceId(0),
	m_reliableID(0),
	m_ackID(0),
	m_channelID(0),
	m_timeFirstSent(0),
	m_timeSent(0),
	m_sender(NetSenderTemplate()),
	m_defn(nullptr)
{
}

NetMessage::NetMessage(uint8_t msgType)
	: BytePacker(new byte_t[MTU_MSG], MTU_MSG),
	m_msgId(msgType),
	m_canWriteMore(true),
	m_sequenceId(0),
	m_reliableID(0),
	m_ackID(0),
	m_channelID(0),
	m_timeFirstSent(0),
	m_timeSent(0),
	m_sender(NetSenderTemplate()),
	m_defn(nullptr)
{

	for (size_t i = 0; i < MTU_MSG; i++)
	{
		byte_t* ptr = ((byte_t*)GetTail()) + i;
		*ptr = 0;
	}
}

NetMessage::~NetMessage()
{
	BytePacker::~BytePacker();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Setters
void NetMessage::SetMsgID(uint8_t id)
{
	m_msgId = id;
}

void NetMessage::Finalize(NetSession* session)
{
	session;
	m_canWriteMore = false;
}

void NetMessage::SetReliableID(uint16_t id)
{
	m_reliableID = id;
}

void NetMessage::SetAckID(uint16_t id)
{
	m_ackID = id;
}

void NetMessage::SetSequenceID(uint16_t seqID)
{
	m_sequenceId = seqID;
}

void NetMessage::SetChannelID(uint8_t chanID)
{
	m_channelID = chanID;
}

void NetMessage::StampTimeSent()
{
	m_timeSent = Performance::GetCurrentPerformanceCount();
}

void NetMessage::CopyTimeSentToFirstTimeSent()
{
	m_timeFirstSent = m_timeSent;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Getters
const uint8_t NetMessage::GetMessageID() const
{
	return m_msgId;
}

const bool NetMessage::GetCanWriteMore() const
{
	return m_canWriteMore;
}

uint16_t NetMessage::GetReliableID() const
{
	return m_reliableID;
}

uint16_t NetMessage::GetAckID() const
{
	return m_ackID;
}

uint16_t NetMessage::GetSequenceID() const
{
	return m_sequenceId;
}

uint8_t NetMessage::GetChannelID() const
{
	return m_channelID;
}

uint64_t NetMessage::GetTimeSent() const
{
	return m_timeSent;
}

uint64_t NetMessage::GetTimeFirstSent() const
{
	return m_timeFirstSent;
}

/*

uint16_t ackID;
*/

STATIC NetMessage* NetMessage::MakeCopy(const NetMessage& original)
{
	NetMessage* copy = new NetMessage(original.GetMessageID());
	memcpy((char*)copy->GetTail(), (char*)original.GetTail(), MTU_MSG);
	copy->SetSequenceID(original.GetSequenceID());
	copy->SetReliableID(original.GetReliableID());
	copy->SetChannelID(original.GetChannelID());
	copy->SetReadableBytes(original.GetTotalReadableBytes());
	copy->SetOffset(original.GetOffset());
	copy->SetEndianness(original.GetEndianness());
	return copy;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//operations
void NetMessage::WriteMessageIntoPacket(NetPacket& packet, uint16_t msg_size, uint16_t payloadSize)
{
	StampTimeSent();
	SetAckID(packet.GetAck());

	packet.Write<uint16_t>(&msg_size, sizeof(uint16_t));
	packet.Write<uint8_t>(&m_msgId, sizeof(uint8_t));
	if (NetSession::g_NetSession->GetIfMsgTypeIsReliable(m_msgId))
	{
		packet.Write<uint16_t>(&m_reliableID, sizeof(uint16_t));
	}
	if (NetSession::g_NetSession->GetIfMsgTypeIsInOrder(m_msgId))
	{
		uint8_t chanID = NetSession::g_NetSession->GetMsgTypeChannelID(m_msgId);
		packet.Write<uint16_t>(&m_sequenceId, sizeof(uint16_t));
		packet.Write<uint8_t>(&chanID, sizeof(uint8_t));
	}
	byte_t* car = (byte_t*)GetTail();
	packet.WriteForward<byte_t>(car, payloadSize);
}


bool NetMessage::ReadMessageFromPacket(NetPacket& packet, sockaddr_in* out_addr, bool& actuallyReadIt)
{
	size_t msgHeader = 0;
	if (ReadMessageHeaderFromPacket(packet, out_addr, actuallyReadIt, msgHeader) == false)
	{
		return false;
	}
	return ReadMessageContentsFromPacket(packet, out_addr, actuallyReadIt, msgHeader);

}

bool NetMessage::ReadMessageHeaderFromPacket(NetPacket& packet, sockaddr_in* out_addr, bool& actuallyReadIt, size_t& msgHeader)
{
	out_addr;
	actuallyReadIt;
	msgHeader = NetSession::g_NetSession->GetHeaderSize(NETMSG_PING);
	if (packet.GetReadableBytes() < msgHeader
		|| packet.GetNumOfReadMsgs() >= packet.GetNumOfMsgs())
	{
		return false;
	}

	packet.IncrementNumOfReadMsgs();
	uint16_t msg_size = 0;
	packet.Read<uint16_t>(&msg_size, sizeof(uint16_t));

	if (msg_size > MTU_MSG)
	{
		return false;
	}
	packet.Read<uint8_t>(&m_msgId, sizeof(uint8_t));
	size_t realHeaderSize = NetSession::g_NetSession->GetHeaderSize(m_msgId);
	msgHeader = realHeaderSize;
	if (msg_size < msgHeader)
	{
		return false;
	}
	msgHeader -= sizeof(uint8_t);
	size_t realMsgSize = msg_size - realHeaderSize;
	SetReadableBytes(realMsgSize);

	return true;
}

bool NetMessage::ReadMessageContentsFromPacket(NetPacket& packet, sockaddr_in* out_addr, bool& actuallyReadIt, size_t& msgHeader)
{
	size_t realHeaderSize = NetSession::g_NetSession->GetHeaderSize(m_msgId);
	size_t realMsgSize = GetTotalReadableBytes() - realHeaderSize - sizeof(uint8_t);
	if (NetSession::g_NetSession->GetIfMsgTypeRequiresConnection(m_msgId) == true
		&& NetSession::g_NetSession->DoesConnectionExist((sockaddr*)out_addr) == false)
	{
		actuallyReadIt = false;
		return true;
	}
	UDPNetConnection* conn = NetSession::g_NetSession->GetUDPNetConnectionForSockAddr(out_addr);
	bool msgTypeReliable = NetSession::g_NetSession->GetIfMsgTypeIsReliable(m_msgId);
	bool msgTypeInOrder = NetSession::g_NetSession->GetIfMsgTypeIsInOrder(m_msgId);
	bool msgTypeReliableAndInOrder = msgTypeReliable && msgTypeInOrder;
	msgTypeReliableAndInOrder;

	//Read Header
	if (msgTypeReliable && msgHeader >= sizeof(uint16_t))
	{
		uint16_t reliableID = 0;
		packet.Read<uint16_t>(&reliableID, sizeof(uint16_t));
		msgHeader -= sizeof(uint16_t);
		SetReliableID(reliableID);
	}

	if (msgTypeInOrder && msgHeader >= (sizeof(uint8_t) + sizeof(uint16_t)))
	{
		msgHeader -= sizeof(uint16_t);
		msgHeader -= sizeof(uint8_t);
		packet.Read<uint16_t>(&m_sequenceId, sizeof(uint16_t));
		packet.Read<uint8_t>(&m_channelID, sizeof(uint8_t));
	}


	if (msgTypeReliable == true)
	{
		if (conn != nullptr && conn->DontReadReliableID(GetReliableID()))
		{
			actuallyReadIt = false;
			return true;
		}
	}

	if (realMsgSize > 0)
	{
		char* newBuffer = new char[realMsgSize];
		char* car = (char*)packet.GetHead();
		car;
		packet.ReadForwards<char>(newBuffer, realMsgSize);
		WriteForward<char>(newBuffer, realMsgSize);
		delete newBuffer;
	}
	return true;
}