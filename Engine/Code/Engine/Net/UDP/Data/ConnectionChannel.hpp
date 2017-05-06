#pragma once

#ifndef CONNECTIONCHANNEL_HPP
#define CONNECTIONCHANNEL_HPP
#include <vector>
#include <stdint.h>

class NetMessage;

struct ConnectionChannel
{
public:
	std::vector<NetMessage*> m_receivedOrderedMsgs;
	uint8_t m_channelID;
	uint16_t m_nextAvailableSequenceMessageID;
	uint16_t m_nextExpectedSequenceMessageID;

public:
	ConnectionChannel(uint8_t chanID = 0);

	void ClearStoredMessages();
};

#endif