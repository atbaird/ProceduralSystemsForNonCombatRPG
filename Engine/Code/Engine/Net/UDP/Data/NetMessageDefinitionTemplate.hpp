#pragma once

#ifndef NETMESSAGEDEFINITIONTEMPLATE_HPP
#define NETMESSAGEDEFINITIONTEMPLATE_HPP
#include "Engine/Net/UDP/Data/NetSenderTemplate.hpp"
#include <stdint.h>

class NetMessage;

typedef void(*net_message_cb)(const NetSenderTemplate& NetSender, NetMessage& msg);

struct NetMessageDefinitionTemplate
{
public:
	//variables
	uint8_t my_idx;
	const char* m_debug_name;
	net_message_cb m_cb;
	bool m_set;
	bool m_Reliable;
	bool m_inOrder;
	bool m_RequiresConnection;
	uint8_t m_channelID;
public:
	//Constructors
	NetMessageDefinitionTemplate();
};

#endif