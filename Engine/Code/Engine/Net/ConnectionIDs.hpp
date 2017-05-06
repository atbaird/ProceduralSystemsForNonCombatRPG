#pragma once

#ifndef CONNECTIONIDS_HPP
#define CONNECTIONIDS_HPP
#include "Engine/Net/NetworkSystem.hpp"

struct ConnectionIDs
{
public:
	sockaddr_in m_address;
	uint8_t m_index;
	char m_guid[32];

public:
	ConnectionIDs();
};
#endif