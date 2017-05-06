#include "Engine/Net/UDP/Data/ConnectionChannel.hpp"
#include "Engine/Net/UDP/NetMessage.hpp"


ConnectionChannel::ConnectionChannel(uint8_t chanID)
	: m_channelID(chanID),
	m_nextAvailableSequenceMessageID(0),
	m_nextExpectedSequenceMessageID(0)
{

}

void ConnectionChannel::ClearStoredMessages()
{
	for (size_t i = 0; i < m_receivedOrderedMsgs.size(); i++)
	{
		if (m_receivedOrderedMsgs.at(i) != nullptr)
		{
			delete m_receivedOrderedMsgs[i];
			m_receivedOrderedMsgs[i] = nullptr;
		}
	}
	m_receivedOrderedMsgs.clear();
}