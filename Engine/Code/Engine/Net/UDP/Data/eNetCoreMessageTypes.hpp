#pragma once

#ifndef ENETCOREMESSAGETYPES_HPP
#define ENETCOREMESSAGETYPES_HPP
#include <stdint.h>

enum eNetCoreMessageTypes : byte_t
{
	NETMSG_PING = 0,
	NETMSG_PONG = 1,
	NETMSG_HEARTBEAT = 2,
	NETMSG_ACK = 3,
	NETMSG_JOIN_REQUEST = 4,
	NETMSG_JOIN_DENY = 5,
	NETMSG_JOIN_ACCEPT = 6,
	NETMSG_CONNECTION_LEFT = 7,
	NETMSG_New_Connection_Status_Update_From_Host = 8,
	NETMSG_LEAVE = 9,
	NETMSG_KICK = 10,
	NETMSG_CORE_COUNT
};

#endif