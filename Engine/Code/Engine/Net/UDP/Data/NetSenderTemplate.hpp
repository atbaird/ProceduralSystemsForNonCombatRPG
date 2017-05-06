#pragma once

#ifndef NETSENDERTEMPLATE_HPP
#define NETSENDERTEMPLATE_HPP
#include "Engine/Net/NetworkSystem.hpp"

class NetSession;
struct UDPNetConnection;

struct NetSenderTemplate
{
public:
	//Variables
	NetSession* m_session;
	UDPNetConnection* m_connection;
	sockaddr_in m_address;
	//NetConnection* connection;
public:
	//Constructors
	NetSenderTemplate();
};

#endif