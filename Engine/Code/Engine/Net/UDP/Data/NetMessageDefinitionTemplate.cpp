#include "Engine/Net/UDP/Data/NetMessageDefinitionTemplate.hpp"
#include "Engine/Net/UDP/NetMessage.hpp"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors
NetMessageDefinitionTemplate::NetMessageDefinitionTemplate()
	: my_idx(0),
	m_debug_name(nullptr),
	m_cb(nullptr),
	m_set(false),
	m_Reliable(false),
	m_inOrder(false),
	m_RequiresConnection(false),
	m_channelID(0)
{

}