#include "Game/Map/TileEvent/TileEvent.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/Map/Tile.hpp"

//STATIC std::vector<std::string> TileEvent::s_AllEventNames;
STATIC std::vector<TileEvent*> TileEvent::s_AllTileEvents;
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//Constructors
TileEvent::TileEvent(const std::string& eventName, TileEventFunc* function)
	: m_eventName(eventName),
	m_function(function)
{
	if (m_function == nullptr || strcmp(m_eventName.c_str(), "") == 0)
	{
		return;
	}
	RegisterTileEvent(this);
}

TileEvent::~TileEvent()
{
}


//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//operations
void TileEvent::CallFunction(Tile* tile, NamedProperties& params, const std::vector<StrXMLAttr>& attrs)
{
	if (m_function != nullptr)
	{
		m_function(tile, params, attrs);
	}
}


//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//Static Methods
STATIC int TileEvent::GetIndexForEventIfExists(const std::string& eventName)
{
	for (size_t i = 0; i < s_AllTileEvents.size(); i++)
	{
		TileEvent* event = s_AllTileEvents[i];
		if (event != nullptr && event->m_eventName == eventName)
		{
			return i;
		}
	}
	return -1;
}

STATIC bool TileEvent::DoesEventExist(const std::string& eventName)
{
	for (size_t i = 0; i < s_AllTileEvents.size(); i++)
	{
		TileEvent* event = s_AllTileEvents[i];
		if (event != nullptr && event->m_eventName == eventName)
		{
			return true;
		}
	}
	return false;
}


STATIC TileEvent* TileEvent::GetTileEventAtIdx(int idx)
{
	if (idx < 0 || (size_t)idx >= s_AllTileEvents.size())
	{
		return nullptr;
	}
	return s_AllTileEvents[idx];
}

STATIC TileEvent* TileEvent::GetTileEventForName(const std::string& eventName)
{
	int idx = GetIndexForEventIfExists(eventName);
	return GetTileEventAtIdx(idx);
}

STATIC void TileEvent::RegisterTileEvent(TileEvent* event)
{
	if (event == nullptr)
	{
		return;
	}
	int i = GetIndexForEventIfExists(event->m_eventName);
	if (i != -1)
	{
		DebuggerPrintf("Warning! %s already registered once! Setting to latest version.", event->m_eventName);
		s_AllTileEvents[i] = event;
	}
	else
	{
		//s_AllEventNames.push_back(event->m_eventName);
		s_AllTileEvents.push_back(event);
	}
}

STATIC void TileEvent::UnregisterTileEvent(TileEvent* even)
{
	if (even == nullptr)
	{
		return;
	}
	UnregisterTileEvent(even->m_eventName);
}

STATIC void TileEvent::UnregisterTileEvent(const std::string& eventName)
{
	int i = GetIndexForEventIfExists(eventName);
	if (i != -1)
	{
		//s_AllEventNames.erase(s_AllEventNames.begin() + i);
		s_AllTileEvents.erase(s_AllTileEvents.begin() + i);
	}
}

STATIC void TileEvent::ClearAllTileEvents()
{
	while (s_AllTileEvents.size() > 0)
	{
		TileEvent* event = s_AllTileEvents.front();
		if (event != nullptr)
		{
			s_AllTileEvents.erase(s_AllTileEvents.begin());
			//UnregisterTileEvent(event->m_eventName);
		}

	}
	//s_AllEventNames.clear();
	s_AllTileEvents.clear();
	//s_AllEventNames.shrink_to_fit();
	s_AllTileEvents.shrink_to_fit();
}