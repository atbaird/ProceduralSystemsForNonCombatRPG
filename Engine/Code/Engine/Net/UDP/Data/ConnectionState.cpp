#include "Engine/Net/UDP/Data/ConnectionState.hpp"


//Static vars
STATIC const double ConnectionState::s_secondsBeforeConnectionBad = 5.0;
STATIC const double ConnectionState::s_secondsBeforeMarkDisconnectMe = 15.0;

//Constructors
ConnectionState::ConnectionState()
	: m_connStatus(UNCOMFIRMED),
	m_is_local(false),
	m_disconnect_me(false)
{

}