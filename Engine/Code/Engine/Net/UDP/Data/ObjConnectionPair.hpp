#pragma once

#ifndef OBJCONNECTIONPAIR_HPP
#define OBJCONNECTIONPAIR_HPP
#include <stdint.h>

struct UDPNetConnection;


struct ObjConnectionPair
{
public:
	void* object;
	UDPNetConnection* connection;
	uint8_t connectionID;
public:
	ObjConnectionPair();
};

#endif