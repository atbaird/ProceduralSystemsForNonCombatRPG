#include "Engine/Net/UDP/Data/NetSenderTemplate.hpp"
#include "Engine/Net/UDP/NetSession.hpp"
#include "Engine/Net/UDP/UDPNetConnection.hpp"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors
NetSenderTemplate::NetSenderTemplate()
	: m_session(nullptr),
	m_connection(nullptr),
	m_address(sockaddr_in())
{

}