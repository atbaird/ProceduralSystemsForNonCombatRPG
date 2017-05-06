#include "Engine/Net/ConnectionIDs.hpp"


ConnectionIDs::ConnectionIDs()
	: m_address(sockaddr_in()),
	m_index(0)
{
	for (size_t i = 0; i < 32; i++)
	{
		m_guid[i] = '\0';
	}
}