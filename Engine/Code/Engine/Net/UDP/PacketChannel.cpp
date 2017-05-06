#include "Engine/Net/UDP/PacketChannel.hpp"
#include "Engine/Net/UDP/NetSession.hpp"

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------

size_t PacketChannel::s_numOfInboundPacketCnt = 500;
float PacketChannel::s_drop_rate = 0.f;
Range<double> PacketChannel::s_additional_lag = Range<double>(0.0, 0.0);

/*

UDPSocket* my_socket;
std::vector<TimeStampedPacket*> inboundPackets;
ObjectPool<TimeStampedPacket> pool;
*/

//Constructors
PacketChannel::PacketChannel()
	: my_socket(nullptr),
	m_inboundPackets(std::vector<TimeStampedPacket*>())
{
	m_pool.init(s_numOfInboundPacketCnt);
}
PacketChannel::PacketChannel(const char* port, const char* minPort, const char* maxPort, const char* addr)
	: my_socket(nullptr),
	m_inboundPackets(std::vector<TimeStampedPacket*>())
{
	my_socket = new UDPSocket(port, minPort, maxPort, addr);
	m_pool.init(s_numOfInboundPacketCnt);
}

PacketChannel::~PacketChannel()
{
	if (my_socket != nullptr)
	{
		delete my_socket;
		my_socket = nullptr;
	}
	for (size_t i = 0; i < m_inboundPackets.size(); i++)
	{
		if (m_inboundPackets[i] != nullptr)
		{
			m_pool.Dealloc(m_inboundPackets[i]);
		}
	}
	m_inboundPackets.clear();
	m_pool.exit();
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Binding
bool PacketChannel::Bind(const char* addr, const char* port, const char* minPort, const char* maxPort)
{
	if (my_socket == nullptr)
	{
		my_socket = new UDPSocket(port, minPort, maxPort, addr);
		return true;
	}
	return my_socket->Bind(addr, port);
}

void PacketChannel::Unbind()
{
	if (my_socket == nullptr)
	{
		return;
	}
	my_socket->Unbind();
}

bool PacketChannel::IsBound()
{
	if (my_socket == nullptr)
	{
		return false;
	}
	return my_socket->IsBound();
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Send Recv
size_t PacketChannel::SendTo(sockaddr_in to_addr, void* data, size_t datalength)
{
	if (my_socket == nullptr)
	{
		return 0;
	}
	return my_socket->SendTo(to_addr, data, datalength);
}

size_t PacketChannel::RecvFrom(sockaddr_in *from_addr, NetPacket* buffer, const size_t buffer_size)
{
	//Read in and register packets for timed delay and drop
	bool forceRead = ReadInTimeStampedPacket(buffer_size);
	forceRead;
	//Read out packets that are ready to be read.
	if (buffer == nullptr)
	{
		return 0;
	}
	if (m_inboundPackets.size() > 0)
	{
		uint64_t currentTime = Performance::GetCurrentPerformanceCount();
		TimeStampedPacket* packet = m_inboundPackets[0];
		//Test if current time is beyond when we're suppose to be able to receive it.
		if (currentTime >= packet->m_endTimeStamp)
		{
			//If is, copy over data, and dealloc original packet.
			*from_addr = packet->m_from_addr;
			size_t size = packet->m_packet.GetTotalReadableBytes();
			memcpy(buffer->GetTail(), packet->m_packet.GetTail(), size);
			buffer->SetReadableBytes(size);
			buffer->SetOffset(packet->m_packet.GetOffset());
			buffer->SetAck(packet->m_packet.GetAck());
			buffer->SetConnectionID(packet->m_packet.GetNetworkID());
			buffer->SetNumOfMsgs(packet->m_packet.GetNumOfMsgs());
			char* carr = (char*)packet->m_packet.GetHead();
			char* car = (char*)buffer->GetHead();
			car;
			carr;

			m_inboundPackets.erase(m_inboundPackets.begin());
			m_pool.Dealloc(packet);
			return size;
		}
	}
	
	return 0;
}

bool PacketChannel::ReadInTimeStampedPacket(size_t buffer_size)
{
	bool forceRead = false;
	if (my_socket != nullptr)
	{
		if (m_inboundPackets.size() >= s_numOfInboundPacketCnt)
		{
			//If no packets available, clear the oldest one out.
			m_pool.Dealloc(m_inboundPackets[0]);
			m_inboundPackets.erase(m_inboundPackets.begin());
		}

		//Read in packet data.
		TimeStampedPacket* packet = m_pool.Alloc();
		size_t readamt = my_socket->RecvFrom(&packet->m_from_addr, packet->m_packet.GetTail(), buffer_size);
		int errorCode = WSAGetLastError();
		errorCode;

		if (readamt > NetPacket::GetExpectedHeaderSize() && readamt <= PACKET_MTU)
		{
			if (rand() < s_drop_rate || m_inboundPackets.size() >= s_numOfInboundPacketCnt)
			{
				//Dealloc because we "dropped it"
				m_pool.Dealloc(packet);
			}
			else
			{
				//Read packet header
				bool headerExists = packet->m_packet.ReadPacketHeader();
				packet->m_packet.m_treatMessagesAsAlreadyRead = false;

				if (headerExists == false)
				{
					//if need a connection, but don't have one, dealloc
					m_pool.Dealloc(packet);
				}
				else
				{
					//Add to the que stack and add in the simulated amount of lag.
					double delay = s_additional_lag.Roll();
					packet->StampPacket();
					packet->m_endTimeStamp = packet->m_timeStamp + (uint64_t)delay;
					packet->m_packet.SetReadableBytes(readamt);
					forceRead = InsertInOrder(packet);
				}
			}
		}
		else
		{
			//Dealloc because read nothing.
			m_pool.Dealloc(packet);
		}
	}
	return forceRead;
}

bool PacketChannel::InsertInOrder(TimeStampedPacket* packet)
{
	if (packet == nullptr)
	{
		return false;
	}
	bool forceRead = false;
	if (m_inboundPackets.size() >= s_numOfInboundPacketCnt)
	{
		m_pool.Dealloc(m_inboundPackets[0]);
		m_inboundPackets.erase(m_inboundPackets.begin());
		forceRead = true;
	}
	bool inserted = false;
	for (size_t i = 0; i < m_inboundPackets.size(); i++)
	{
		if (packet->m_endTimeStamp < m_inboundPackets[i]->m_endTimeStamp)
		{
			inserted = true;
			m_inboundPackets.insert(m_inboundPackets.begin() + i, packet);
		}
	}
	if (inserted == false)
	{
		m_inboundPackets.push_back(packet);
	}
	return forceRead;
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Getters

const UDPSocket* PacketChannel::GetUDPSocket() const
{
	return my_socket;
}

UDPSocket* PacketChannel::GetEditableUDPSocket() const
{
	return my_socket;
}

SOCKET PacketChannel::GetSocket() const
{
	if (my_socket == nullptr)
	{
		return INVALID_SOCKET;
	}
	return my_socket->GetSocket();
}

sockaddr_in PacketChannel::GetAddress() const
{
	if (my_socket == nullptr)
	{
		return sockaddr_in();
	}
	return my_socket->GetAddress();
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Static methods
STATIC void PacketChannel::SetPacketSimulatedDropRate(float dropRate)
{
	s_drop_rate = dropRate;
}

STATIC void PacketChannel::SetPacketSimulatedAdditionalLag(Range<double> additionalLag)
{
	s_additional_lag = additionalLag;
}

STATIC Range<double> PacketChannel::GetAdditionalLag()
{
	return s_additional_lag;
}

STATIC float PacketChannel::GetDropRate()
{
	return s_drop_rate;
}