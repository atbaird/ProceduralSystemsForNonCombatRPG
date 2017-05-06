#include "Engine/Net/TCP/RSC.hpp"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Constructors
RSC::RSC(TCPConnection* TCPConn)
	: m_TCPConn(TCPConn)
{
	m_on_message.RegisterFunction(&RSC::OnMessage);
}
RSC::~RSC()
{
	if (m_TCPConn != nullptr)
	{
		m_TCPConn->Disconnect();
		delete m_TCPConn;
		m_TCPConn = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Send/Receive
void RSC::Send(const void* msg, byte msg_id)
{
	if (m_TCPConn == nullptr)
	{
		return;
	}
	m_TCPConn->Send(msg, msg_id);
}

void RSC::Receive()
{
	if (m_TCPConn == nullptr)
	{
		return;
	}
	const size_t BUFFER_SIZE = 1024;
	byte buffer[BUFFER_SIZE];
	size_t read = m_TCPConn->Receive(buffer, BUFFER_SIZE);
	while(read > 0)
	{
		for (size_t i = 0; i < read; ++i)
		{
			char c = buffer[i];
			m_next_message.push_back(c);
			if (c == NULL)
			{
				m_on_message.Trigger(this, m_next_message[0], &m_next_message[1]);
				m_next_message.clear();
			}
		}
		read = m_TCPConn->Receive(buffer, BUFFER_SIZE);
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Tests
void RSC::Disconnect() // close socket
{
	if (m_TCPConn == nullptr)
	{
		return;
	}
	m_TCPConn->Disconnect();
	delete m_TCPConn;
	m_TCPConn = nullptr;
}

bool RSC::Is_Connected() // socket != INVALID_SOCKET
{
	if (m_TCPConn == nullptr)
	{
		return false;
	}
	return m_TCPConn->Is_Connected();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Setters
TCPConnection* RSC::SetTCPConnection(TCPConnection* conn)
{
	TCPConnection* org = m_TCPConn;
	m_TCPConn = nullptr;
	m_TCPConn = conn;
	return org;
}

void RSC::IncrementFramesSinceLastConnected()
{
	if (m_TCPConn == nullptr)
	{
		return;
	}
	m_TCPConn->IncrementFramesSinceLastConnected();
}

void RSC::ResetFramesSinceLastConnected()
{
	if (m_TCPConn == nullptr)
	{
		return;
	}
	m_TCPConn->ResetFramesSinceLastConnected();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Getters
const TCPConnection* RSC::GetTCPConn() const
{
	return m_TCPConn;
}

TCPConnection* RSC::GetEditableTCPConn() const
{
	return m_TCPConn;
}

SOCKET* RSC::GetSocketAsPtr()
{
	if (m_TCPConn == nullptr)
	{
		return nullptr;
	}
	return m_TCPConn->GetSocketAsPtr();
}

sockaddr_in* RSC::GetSocketAddrAsPtr()
{
	if (m_TCPConn == nullptr)
	{
		return nullptr;
	}
	return m_TCPConn->GetSocketAddrAsPtr();
}

SOCKET RSC::GetSocket()
{
	if (m_TCPConn == nullptr)
	{
		return INVALID_SOCKET;
	}
	return m_TCPConn->GetSocket();
}

sockaddr_in RSC::GetSocketAddr()
{
	if (m_TCPConn == nullptr)
	{
		return sockaddr_in();
	}
	return m_TCPConn->GetSocketAddr();
}

const std::string RSC::GetMyNickname() const
{
	if (m_TCPConn == nullptr)
	{
		return "";
	}
	return m_TCPConn->GetMyNickname();
}

const size_t RSC::GetFramesSinceLastConnected() const
{
	if (m_TCPConn == nullptr)
	{
		return 0;
	}
	return m_TCPConn->GetFramesSinceLastConnected();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Static Methods
void RSC::OnMessage(RSC* rsc, byte messageID, const char* message)
{
	if (rsc == nullptr || strlen(message) == 0)
	{
		return;
	}
	rsc->Send(message, messageID);
}