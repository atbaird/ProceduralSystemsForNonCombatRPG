#pragma once

#ifndef NETSESSIONSTATE_HPP
#define NETSESSIONSTATE_HPP

enum NetSessionState
{
	INVALID_NETSESSION,
	SETUP_NETSESSION,
	DISCONNECTED_NETSESSION,
	CONNECTED_NETSESSION,
	HOSTING_NETSESSION,
	JOINING_NETSESSION,
	NUM_OF_NETSESSION_STATES
};

#endif