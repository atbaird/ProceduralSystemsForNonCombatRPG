#pragma once

#ifndef CONNECTIONSTATE_HPP
#define CONNECTIONSTATE_HPP

enum eConnectionStatus
{
	UNCOMFIRMED,
	CONFIRMED,
	BAD,
	NUM_OF_CONNECTION_STATUSES
};

#define STATIC
struct ConnectionState
{
public:
	//Static Vars
	static const double s_secondsBeforeConnectionBad;
	static const double s_secondsBeforeMarkDisconnectMe;
	//Variables
	eConnectionStatus m_connStatus;
	bool m_is_local;
	bool m_disconnect_me;
public:
	//Constructors
	ConnectionState();
};
#endif