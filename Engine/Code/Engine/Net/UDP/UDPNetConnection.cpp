#include "Engine/Net/UDP/UDPNetConnection.hpp"
#include "Engine/Net/UDP/NetSession.hpp"

const double UDPNetConnection::s_secondsBetweenHeartBeats = 3.0;
const double UDPNetConnection::s_secondsBeforeResendReliables = 1.0;
const double UDPNetConnection::s_secondsBeforeDumpReceivedReliables = 2.0;
size_t UDPNetConnection::s_MaxActiveReliables = 32;
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//Constructors

/*
//General Connection
private:
float timeSinceLastUpdate;
uint64_t lastSentHeartbeatPacketAt;
bool m_isHost;
bool m_updateNow;
*/
UDPNetConnection::UDPNetConnection()
	: m_connState(ConnectionState()),
	m_nextAvailableAck(0),
	m_highestRecvAck(0),
	m_prevRecvAck(0),
	m_lastReportedHighestAck(NetPacketHeader::INVALID_PACKET_ACK),
	m_lastReportedRecvdAck(NetPacketHeader::INVALID_PACKET_ACK),
	m_nextAvailableReliableMessageID(0),
	m_nextExpectedReliableID(0),
	m_timeLastDumpedReliables(0),
	m_timeLastSentMessage(0),
	m_timeLastRecvMessage(0),
	m_createdAtTime(0),
	m_index(0),
	session(nullptr),
	socket(nullptr),
	address(sockaddr_in()),
	tick_rate(1.f / 20.f),
	m_timeSinceLastUpdate(0.f),
	m_lastSentHeartbeatPacketAt(0),
	m_isHost(false),
	m_updateNow(false)
{
	m_channels.resize(1);
	m_timeLastSentMessage = Performance::GetCurrentPerformanceCount();
	m_timeLastRecvMessage = m_timeLastSentMessage;
	m_lastSentHeartbeatPacketAt = m_timeLastSentMessage;
	m_timeLastDumpedReliables = m_timeLastRecvMessage;
	m_createdAtTime = m_timeLastRecvMessage;
	for (size_t i = 0; i < MAX_GUID_LENGTH; i++)
	{
		guid[i] = '\0';
	}
}

UDPNetConnection::UDPNetConnection(sockaddr_in& addr)
	: m_connState(ConnectionState()),
	m_nextAvailableAck(0),
	m_highestRecvAck(0),
	m_prevRecvAck(0),
	m_lastReportedHighestAck(NetPacketHeader::INVALID_PACKET_ACK),
	m_lastReportedRecvdAck(NetPacketHeader::INVALID_PACKET_ACK),
	m_nextAvailableReliableMessageID(0),
	m_nextExpectedReliableID(0),
	m_timeLastDumpedReliables(0),
	m_timeLastSentMessage(0),
	m_timeLastRecvMessage(0),
	m_createdAtTime(0),
	m_index(0),
	session(nullptr),
	socket(nullptr),
	address(addr),
	tick_rate(1.f / 20.f),
	m_timeSinceLastUpdate(0.f),
	m_lastSentHeartbeatPacketAt(0),
	m_isHost(false),
	m_updateNow(false)
{
	m_channels.resize(1);
	m_timeLastSentMessage = Performance::GetCurrentPerformanceCount();
	m_timeLastRecvMessage = m_timeLastSentMessage;
	m_lastSentHeartbeatPacketAt = m_timeLastSentMessage;
	m_timeLastDumpedReliables = m_timeLastRecvMessage;
	m_createdAtTime = m_timeLastRecvMessage;
	const char* adr = NetworkSystem::SockAddrToString((sockaddr*)&addr);
	SetGuid(adr);
}

UDPNetConnection::~UDPNetConnection()
{
	for (size_t i = 0; i < m_unreliables.size(); i++)
	{
		NetMessage* msg = m_unreliables[i];
		if (msg != nullptr)
		{
			delete msg;
		}
	}
	m_unreliables.clear();

	for (size_t i = 0; i < m_unsentReliables.size(); i++)
	{
		NetMessage* msg = m_unsentReliables[i];
		if (msg != nullptr)
		{
			delete msg;
		}
	}
	m_unsentReliables.clear();

	for (size_t i = 0; i < m_sentReliables.size(); i++)
	{
		NetMessage* msg = m_sentReliables[i];
		if (msg != nullptr)
		{
			delete msg;
		}
	}
	m_sentReliables.clear();
	for (size_t i = 0; i < m_channels.size(); i++)
	{
		m_channels.at(i).ClearStoredMessages();
	}
	m_channels.clear();
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//Connection test
void UDPNetConnection::ConnSendMessage(NetMessage& msg)
{
	NetMessage* copy = NetMessage::MakeCopy(msg);
	copy->Finalize(session);
	if (NetSession::g_NetSession != nullptr)
	{
		if (NetSession::g_NetSession->GetIfMsgTypeIsInOrder(copy->GetMessageID()))
		{
			uint8_t chanID = NetSession::g_NetSession->GetMsgTypeChannelID(copy->GetMessageID());
			if (chanID >= m_channels.size())
			{
				ResizeNumOfChannels(chanID + 1);
			}
			copy->SetChannelID(chanID);
			copy->SetSequenceID(m_channels[chanID].m_nextAvailableSequenceMessageID);
			m_channels[chanID].m_nextAvailableSequenceMessageID = m_channels[chanID].m_nextAvailableSequenceMessageID + 1;
		}
		if (NetSession::g_NetSession->GetIfMsgTypeIsReliable(copy->GetMessageID()))
		{
			copy->SetReliableID(m_nextAvailableReliableMessageID);
			m_nextAvailableReliableMessageID++;
			m_unsentReliables.push_back(copy);
		}
		else
		{
			m_unreliables.push_back(copy);
		}
	}
	else
	{
		m_unreliables.push_back(copy);
	}
}


void UDPNetConnection::Tick(float dt)
{
	if (session->myConn != nullptr && NetworkSystem::DetermineIfSockAddrsAreTheSame(address, session->myConn->address) == true)
	{
		return;
	}
	uint64_t curr = Performance::GetCurrentPerformanceCount();
	uint64_t lastRecv = curr - m_timeLastRecvMessage;
	double seconds = Performance::PerformanceCountToSeconds(lastRecv);
	seconds;

	m_timeSinceLastUpdate += dt;
	if (m_timeSinceLastUpdate >= tick_rate)
	{
		m_timeSinceLastUpdate = 0.f;
		m_updateNow = true;
	}
	else
	{
		m_updateNow = false;
	}
}

void UDPNetConnection::SendPacket()
{
	//CleanReliables();
	HeartbeatAndConnectionStatusUpdate();
	bool connTest = m_connState.m_connStatus != UNCOMFIRMED;
	bool invalAckTest = (m_lastReportedRecvdAck != NetPacketHeader::INVALID_PACKET_ACK);
	bool ackDiff = ((m_lastReportedRecvdAck - m_nextAvailableAck) < NetPacketHeader::maxPacketDifference);
	bool unreliablesSize0 = m_unreliables.size() <= 0;
	bool unsentReliablesSize0 = m_unsentReliables.size() <= 0;
	bool sentReliablesSize0 = m_sentReliables.size() <= 0;
	bool haveNoMessagesTest = (unreliablesSize0 && unsentReliablesSize0 && sentReliablesSize0);
	bool NetSessionIsRunningTest = NetSession::g_NetSession->IsRunning();
	if ((NetSessionIsRunningTest == false) || haveNoMessagesTest == true
		|| (connTest == true && invalAckTest == true && ackDiff == false))
	{
		return;
	}

	//Set up packet
	NetPacket packet;
	packet.SetHighestRecvAck(m_highestRecvAck);
	packet.SetPreviousRecvAck(m_prevRecvAck);
	packet.GetNextAck(m_nextAvailableAck);
	void* bookmark = packet.WritePacketHeaderWithReserve();
	size_t msgCnt = 0;

	//Load content into packets
	ResendSentReliablesInPacket(packet, msgCnt);
	LoadUnsentReliablesInPacket(packet, msgCnt);
	LoadUnreliablesInPacket(packet, msgCnt);

	//Send packet off
	packet.WriteBookmark(bookmark, &msgCnt, sizeof(size_t));
	session->GetMyConnect()->SendTo(address, packet.GetTail(), packet.GetOffset());
	m_timeLastSentMessage = Performance::GetCurrentPerformanceCount();
}


void UDPNetConnection::HeartbeatAndConnectionStatusUpdate()
{
	{
		uint64_t currentPerfCnt = Performance::GetCurrentPerformanceCount();

		//Time since last received packet
		uint64_t recvDiff = currentPerfCnt - m_timeLastRecvMessage;
		double seconds = Performance::PerformanceCountToSeconds(recvDiff);

		if (m_connState.m_connStatus != UNCOMFIRMED)
		{
			if (seconds < ConnectionState::s_secondsBeforeConnectionBad)
			{
				m_connState.m_connStatus = CONFIRMED;
			}
			else
			{
				m_connState.m_connStatus = BAD;
			}
		}
		if (seconds >= ConnectionState::s_secondsBeforeMarkDisconnectMe)
		{
			m_connState.m_disconnect_me = true;
		}
		else
		{
			m_connState.m_disconnect_me = false;
		}

		//Time since last sent heartbeat.
		if (m_connState.m_connStatus == BAD)
		{
			uint64_t heartbeatDiff = currentPerfCnt - m_lastSentHeartbeatPacketAt;
			if (Performance::PerformanceCountToSeconds(heartbeatDiff) >= s_secondsBetweenHeartBeats)
			{
				m_lastSentHeartbeatPacketAt = currentPerfCnt;
				if (NetSession::g_NetSession != nullptr)
				{
					NetMessage* msg = new NetMessage(NETMSG_HEARTBEAT);
					m_unreliables.push_back(msg);
					m_timeLastSentMessage = currentPerfCnt;
				}
			}
		}
	}

}

void UDPNetConnection::LoadUnreliablesInPacket(NetPacket& packet, size_t& msgCnt)
{
	for (size_t i = 0; i < m_unreliables.size(); i++)
	{
		NetMessage* msg = m_unreliables[i];
		if (msg == nullptr)
		{
			m_unreliables.erase(m_unreliables.begin() + i);
			i--;
			continue;
		}
		byte_t msgID = msg->GetMessageID();
		msg->m_defn = session->FindDefinition(msgID);
		if (msg->m_defn == nullptr)
		{
			delete m_unreliables[i];
			m_unreliables.erase(m_unreliables.begin() + i);
			i--;
			continue;
		}


		uint16_t headerSize = session->GetHeaderSize(msgID);
		uint16_t payloadSize = (uint16_t)msg->GetOffset();
		uint16_t msg_size = headerSize + payloadSize;
		if (packet.GetWriteableBytes() >= (msg_size))
		{
			msg->WriteMessageIntoPacket(packet, msg_size, payloadSize);
			msgCnt++;

			delete m_unreliables[i];
			m_unreliables.erase(m_unreliables.begin() + i);
			i--;
		}

	}
}

void UDPNetConnection::ResendSentReliablesInPacket(NetPacket& packet, size_t& msgCnt)
{
	bool sentOne = false;
	uint16_t firstReliableIDResent = 0;
	uint64_t currentTime = Performance::GetCurrentPerformanceCount();
	for (size_t i = 0; i < m_sentReliables.size(); i++)
	{
		NetMessage* msg = m_sentReliables.at(i);
		if (msg == nullptr)
		{
			m_sentReliables.erase(m_sentReliables.begin() + i);
			i--;
			continue;
		}

		uint64_t timeDiff = currentTime - msg->GetTimeSent();
		double time = Performance::PerformanceCountToSeconds(timeDiff);
		if (time < s_secondsBeforeResendReliables)
		{
			continue;
		}

		if (IsReliableConfirmed(msg->GetAckID()))
		{
			delete m_sentReliables[i];
			m_sentReliables.erase(m_sentReliables.begin() + i);
			i--;
			continue;
		}

		byte_t msgID = msg->GetMessageID();
		msg->m_defn = session->FindDefinition(msgID);
		if (msg->m_defn == nullptr)
		{
			delete m_sentReliables[i];
			m_sentReliables.erase(m_sentReliables.begin() + i);
			i--;
			continue;
		}

		uint16_t reliableID = msg->GetReliableID();
		if (sentOne == true && reliableID == firstReliableIDResent)
		{
			break;
		}

		uint16_t headerSize = session->GetHeaderSize(msgID);
		uint16_t payloadSize = (uint16_t)msg->GetOffset();
		uint16_t msg_size = headerSize + payloadSize;
		if (packet.GetWriteableBytes() >= (msg_size))
		{
			msg->WriteMessageIntoPacket(packet, msg_size, payloadSize);
			msgCnt++;
			if (sentOne == false)
			{
				firstReliableIDResent = reliableID;
				sentOne = true;
			}
		}
	}
}

void UDPNetConnection::LoadUnsentReliablesInPacket(NetPacket& packet, size_t& msgCnt)
{
	for (size_t i = 0; i < m_unsentReliables.size(); i++)
	{
		if (m_sentReliables.size() >= s_MaxActiveReliables)
		{
			break;
		}
		NetMessage* msg = m_unsentReliables.at(i);
		if (msg == nullptr)
		{
			m_unsentReliables.erase(m_unsentReliables.begin() + i);
			i--;
			continue;
		}

		byte_t msgID = msg->GetMessageID();
		msg->m_defn = session->FindDefinition(msgID);
		if (msg->m_defn == nullptr)
		{
			delete m_unsentReliables[i];
			m_unsentReliables.erase(m_unsentReliables.begin() + i);
			i--;
			continue;
		}

		uint16_t headerSize = session->GetHeaderSize(msgID);
		uint16_t payloadSize = (uint16_t)msg->GetOffset();
		uint16_t msg_size = headerSize + payloadSize;
		if (packet.GetWriteableBytes() >= (msg_size))
		{
			msg->WriteMessageIntoPacket(packet, msg_size, payloadSize);
			m_sentReliables.push_back(msg);
			m_unsentReliables.erase(m_unsentReliables.begin() + i);
			msgCnt++;
			i--;
		}
	}
}


void UDPNetConnection::MarkMessageRecvd(NetMessage& msg)
{
	if (NetSession::g_NetSession->GetIfMsgTypeIsReliable(msg.GetMessageID()))
	{
		MarkReliableReceived(msg.GetReliableID());
	}
}

bool UDPNetConnection::MarkMessageRecvdInOrder(NetMessage& msg)
{
	//Returns whether or not can try to start reading the in ordered messages
	uint16_t sequence = msg.GetSequenceID();
	uint8_t channel = msg.GetChannelID();
	bool msgReliable = (NetSession::g_NetSession->GetIfMsgTypeIsReliable(msg.GetMessageID()));
	if (msgReliable)
	{
		MarkReliableReceived(msg.GetReliableID());
		AddInOrderReliableMessageToBuffer(msg);
		return m_channels[channel].m_nextExpectedSequenceMessageID == sequence;
	}
	else
	{
		//Unreliable In Order
		if (channel >= m_channels.size())
		{
			ResizeNumOfChannels(channel + 1);
		}
		//if (CycleGreaterThanOrEqual(sequence, channels.at(channel).nextExpectedSequenceMessageID + MAX_RELIABLE_RANGE))
		//{
		//	uint16_t distance = sequence - channels.at(channel).nextExpectedSequenceMessageID;
		//	if (distance > MAX_RELIABLE_RANGE)
		//	{
		//		return; //Drop connection
		//	}
		//	channels.at(channel).nextExpectedSequenceMessageID = sequence + 1;
		//	RemoveAllReliableIDsLessThan(sequence - MAX_RELIABLE_RANGE);
		//}
		//else
		//{
		//	uint16_t diff = channels.at(channel).nextExpectedSequenceMessageID - sequence;
		//	if (diff < MAX_RELIABLE_RANGE)
		//	{
		//		channels.at(channel).nextExpectedSequenceMessageID = sequence;
		//	}
		//}
		if(sequence >= m_channels.at(channel).m_nextExpectedSequenceMessageID)
		{
			NetMessage* copy = NetMessage::MakeCopy(msg);
			m_channels.at(channel).m_receivedOrderedMsgs.push_back(copy);
			return true;
		}
	}
	return false;
}

NetMessage* UDPNetConnection::PopMessageRecvdInOrder(uint8_t channelID)
{
	if (channelID >= m_channels.size())
	{
		return nullptr;
	}
	if (m_channels[channelID].m_receivedOrderedMsgs.size() == 0)
	{
		return nullptr;
	}
	for (size_t i = 0; i < m_channels[channelID].m_receivedOrderedMsgs.size(); i++)
	{
		NetMessage* msg = m_channels[channelID].m_receivedOrderedMsgs[i];
		if (msg == nullptr)
		{
			continue;
		}
		bool msgReliable = (NetSession::g_NetSession->GetIfMsgTypeIsReliable(msg->GetMessageID()));
		if (msgReliable == true)
		{
			if (msg->GetSequenceID() == m_channels[channelID].m_nextExpectedSequenceMessageID)
			{
				m_channels[channelID].m_receivedOrderedMsgs.erase(m_channels[channelID].m_receivedOrderedMsgs.begin() + i);
				m_channels[channelID].m_nextExpectedSequenceMessageID = m_channels[channelID].m_nextExpectedSequenceMessageID + 1;
				return msg;
			}
		}
	}

	return nullptr;
}

void UDPNetConnection::AddInOrderReliableMessageToBuffer(NetMessage& msg)
{
	uint16_t sequence = msg.GetSequenceID();
	uint8_t channel = msg.GetChannelID();

	if (sequence < m_channels[channel].m_nextExpectedSequenceMessageID)
	{
		return;
	}

	if (channel >= m_channels.size())
	{
		ResizeNumOfChannels(channel + 1);
	}
	bool foundIdx = false;
	uint16_t lastSequence = 0;
	for (size_t i = 0; i < m_channels[channel].m_receivedOrderedMsgs.size(); i++)
	{
		NetMessage* msgTest = m_channels[channel].m_receivedOrderedMsgs.at(i);
		if (msgTest != nullptr)
		{
			uint16_t thisSequence = msgTest->GetSequenceID();
			if (thisSequence == sequence)
			{
				//RECEIVED a message we already have!
				foundIdx = true;
				break;
			}
			if ((lastSequence <= sequence && sequence < thisSequence))
			{
				foundIdx = true;
				NetMessage* copy = NetMessage::MakeCopy(msg);
				m_channels[channel].m_receivedOrderedMsgs.insert(m_channels[channel].m_receivedOrderedMsgs.begin() + i, copy);
				break;
			}
			lastSequence = thisSequence;
		}
	}
	if (foundIdx == false)
	{
		NetMessage* copy = NetMessage::MakeCopy(msg);
		m_channels[channel].m_receivedOrderedMsgs.push_back(copy);
	}
}

void UDPNetConnection::MarkPacketRecvd(NetPacketHeader& header)
{
	uint16_t prevRecvac = header.Ack;
	UpdateHighestReceived(m_highestRecvAck, m_prevRecvAck, header.Ack);
	ProcessConfirmedAcks(header.highest_recv_ack, header.previous_recv_ack);
	m_prevRecvAck = prevRecvac;
	m_timeLastRecvMessage = Performance::GetCurrentPerformanceCount();
	if (m_connState.m_connStatus == UNCOMFIRMED)
	{
		m_connState.m_connStatus = CONFIRMED;
	}
}

void UDPNetConnection::ResizeNumOfChannels(uint8_t numChannels)
{
	if ((size_t)numChannels >= m_channels.size())
	{
		size_t curSize = m_channels.size();
		size_t diff = (size_t)numChannels - curSize;
		for (size_t i = 0; i <= diff; i++)
		{
			ConnectionChannel chan = ConnectionChannel((uint8_t)(i + curSize));
			m_channels.push_back(chan);
		}
	}
}


void UDPNetConnection::MarkReliableReceived(uint16_t reliableID)
{
	if (CycleGreaterThanOrEqual(reliableID, m_nextExpectedReliableID + MAX_RELIABLE_RANGE))
	{
		uint16_t distance = reliableID - m_nextExpectedReliableID;
		if (distance > MAX_RELIABLE_RANGE)
		{
			return; //Drop connection
		}
		if (reliableID + 1 >= m_nextExpectedReliableID)
		{
			m_nextExpectedReliableID = reliableID + 1;
		}
		RemoveAllReliableIDsLessThan(reliableID - MAX_RELIABLE_RANGE);
	}
	else
	{
		if (reliableID >= m_nextAvailableReliableMessageID)
		{
			m_nextExpectedReliableID = reliableID + 1;
		}
		uint16_t diff = m_nextExpectedReliableID - reliableID;
		if (diff > 0 && diff < MAX_RELIABLE_RANGE)
		{
			m_receivedReliables.push_back(reliableID);
		}
	}
}

bool UDPNetConnection::CycleGreaterThanOrEqual(uint16_t a, uint16_t b)
{
	uint16_t diff = a - b;
	return (diff >= 0 && diff < 0x7fff);
}

void UDPNetConnection::RemoveAllReliableIDsLessThan(uint16_t lessThanThis)
{
	for (size_t i = 0; i < m_receivedReliables.size(); i++)
	{
		if (m_receivedReliables.at(i) < lessThanThis)
		{
			m_receivedReliables.erase(m_receivedReliables.begin() + i);
			i--;
		}
	}
}

void UDPNetConnection::UpdateHighestReceived(uint16_t& curHighestRecvdAck, uint16_t& prevRecvdAcks, const uint16_t& ack)
{
	if (ack > curHighestRecvdAck)
	{
		curHighestRecvdAck = ack;
	}

	prevRecvdAcks = ack;
}

void UDPNetConnection::ProcessConfirmedAcks(uint16_t& highestRecvdAcks, uint16_t& previousRecvdAcksBf)
{
	std::vector<uint16_t> vals;
	for (uint16_t i = previousRecvdAcksBf; i <= highestRecvdAcks; i++)
	{
		vals.push_back(i);
	}
	for (size_t i = 0; i < m_sentReliables.size(); i++)
	{
		if (m_sentReliables.at(i) != nullptr && m_sentReliables.at(i)->GetAckID() >= previousRecvdAcksBf && m_sentReliables.at(i)->GetAckID() <= m_highestRecvAck)
		{

			delete m_sentReliables.at(i);
			m_sentReliables.erase(m_sentReliables.begin() + i);
			i--;
			continue;
		}
	}

}

bool UDPNetConnection::IsReliableConfirmed(uint16_t ackID)
{
	if (UNCOMFIRMED == m_connState.m_connStatus)
	{
		return false;
	}
	if (ackID > m_lastReportedHighestAck)
	{
		return false;
	}
	else// if (ackID <= m_lastReportedHighestAck)
	{
		return true;
	}
	//else
	//{
	//	uint16_t offset = m_lastReportedHighestAck - ackID;
	//	return IsBitSet(m_lastReportedRecvdAck, offset - 1);
	//}

}

bool UDPNetConnection::IsBitSet(size_t idx, size_t bitfield)
{
	return ((1 << idx) & bitfield) != 0;
}

void UDPNetConnection::CleanReliables()
{
	uint64_t curTimeDiff = Performance::GetCurrentPerformanceCount() - m_timeLastDumpedReliables;
	double timeDiff = Performance::PerformanceCountToSeconds(curTimeDiff);
	if (timeDiff >= s_secondsBeforeDumpReceivedReliables)
	{
		m_receivedReliables.clear();
		//highestReceivedReliable = 0;
		m_timeLastDumpedReliables = curTimeDiff;
	}
}

size_t UDPNetConnection::SendTo(sockaddr_in to_addr, void* data, size_t data_size)
{
	if (socket != nullptr)
	{
		// send will return the amount of data actually sent.
		// It SHOULD match, or be an error.  
		//int result = NetworkSystem::BindSocket(my_socket, (sockaddr*)&my_local_address, sizeof(sockaddr_in));
		//if (result == SOCKET_ERROR)
		//{
		//	return 0;
		//}
		int size = socket->SendTo(to_addr, data, data_size);

		if (size > 0)
		{
			return size;
		}
	}

	// Not particularly interested in errors - you can 
	// check this though if you want to see if something
	// has happened to your socket.
	return 0;

}

bool UDPNetConnection::IsMe() const
{
	if (NetSession::g_NetSession == nullptr
		|| NetSession::g_NetSession->IsRunning() == false)
	{
		return false;
	}
	if (NetworkSystem::DetermineIfSockAddrsAreTheSame(address, NetSession::g_NetSession->GetSocketAddress()))
	{
		return true;
	}
	return false;
}

bool UDPNetConnection::DontReadReliableID(uint16_t reliableId) const
{
	//if (reliableId >= nextExpectedReliableID)
	//{
	//	return false;
	//}
	//return true;
	//if (reliableId >= highestReceivedReliable)
	//{
	//	return true;
	//}
	//return false;
	for (size_t i = 0; i < m_receivedReliables.size(); i++)
	{
		if (m_receivedReliables.at(i) == reliableId)
		{
			return true;
		}
	}

	//if (reliableId < m_nextExpectedReliableID)
	//{
	//	return true;
	//}
	return false;
}


size_t UDPNetConnection::GetNumberOfUnsentReliables() const
{
	return m_unsentReliables.size();
}

size_t UDPNetConnection::GetNumberOfSentReliables() const
{
	return m_sentReliables.size();
}

size_t UDPNetConnection::GetNumberOfUnreliables() const
{
	return m_unreliables.size();
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//operators
const bool UDPNetConnection::operator==(const UDPNetConnection& conn) const
{
	if (NetworkSystem::DetermineIfSockAddrsAreTheSame(address, conn.address))
	{
		return true;
	}
	return false;
}


//Setters
void UDPNetConnection::SetIsHost(const bool& isHost)
{
	m_isHost = isHost;
}

void UDPNetConnection::SetGuid(const char* gui)
{
	size_t len = strlen(gui);
	for (size_t i = 0; i < MAX_GUID_LENGTH; i++)
	{
		if (i < len)
		{
			guid[i] = gui[i];
		}
		else
		{
			guid[i] = (unsigned char)'\0';
		}
	}
}

void UDPNetConnection::StampLastRecvPacket()
{
	m_timeLastRecvMessage = Performance::GetCurrentPerformanceCount();
}


void UDPNetConnection::UpdateLastRecvPacketInfo(NetPacket* packet)
{
	if (packet == nullptr)
	{
		return;
	}
	if (m_connState.m_connStatus == UNCOMFIRMED)
	{
		m_connState.m_connStatus = CONFIRMED; 
	}
	StampLastRecvPacket();
	uint16_t ack = packet->GetAck();
	packet->m_treatMessagesAsAlreadyRead = false;
	if (ack != NetPacketHeader::INVALID_PACKET_ACK)
	{
		m_prevRecvAck = ack;
		if (ack <= m_prevRecvAck)
		{
			packet->m_treatMessagesAsAlreadyRead = true;
		}
		m_lastReportedRecvdAck = packet->GetPreviousRecvAckBf();
		m_lastReportedHighestAck = packet->GetHighestRecvAck();
		if (packet->GetHighestRecvAck() > m_prevRecvAck)
		{
			m_highestRecvAck = (m_prevRecvAck);
		}
	}
}

//Getters
bool UDPNetConnection::GetIsHost() const
{
	return m_isHost;
}

bool UDPNetConnection::GetUpdateNow() const
{
	return m_updateNow;
}

bool UDPNetConnection::GetDisconnectMe() const
{
	return m_connState.m_disconnect_me;
}

eConnectionStatus UDPNetConnection::GetConnStatus() const //m_connState.connStatus
{
	return m_connState.m_connStatus;
}

uint16_t UDPNetConnection::GetHighestRecvAck() const
{
	return m_highestRecvAck;
}

uint16_t UDPNetConnection::GetPrevRecvAck() const
{
	return m_prevRecvAck;
}

uint64_t UDPNetConnection::GetTimeLastRecvMessage() const
{
	return m_timeLastRecvMessage;
}

uint64_t UDPNetConnection::GetTimeLastSentMessage() const
{
	return m_timeLastSentMessage;
}

ConnectionState UDPNetConnection::GetConnectionState() const
{
	return m_connState;
}