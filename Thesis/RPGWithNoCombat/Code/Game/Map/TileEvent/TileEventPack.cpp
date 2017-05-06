#include "Game/Map/TileEvent/TileEventPack.hpp"
#include "Engine/Math/MathToStringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/Debugging/NamedPropertiesToString.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Constructors
TileEventPack::TileEventPack()
{

}

TileEventPack::TileEventPack(TileEvent* onInteract, TileEvent* onTouch, TileEvent* onEnter, TileEvent* onExit,
	const std::vector<StrXMLAttr>& onInteractParams, const std::vector<StrXMLAttr>& onTouchParams,
	const std::vector<StrXMLAttr>& onEnterParams,
	const std::vector<StrXMLAttr>& onExitParams,
	const std::string& OnInteractEventName,
	const std::string& OnTouchEventName,
	const std::string& OnEnterEventName,
	const std::string& OnExitEventName,
	const TILE_COORDS& coords, int tileIdx)
	: m_onInteract(onInteract),
	m_onExit(onExit),
	m_onTouch(onTouch),
	m_onEnter(onEnter),
	m_OnInteractParams(onInteractParams),
	m_OnTouchParams(onTouchParams),
	m_OnEnterParams(onEnterParams),
	m_OnExitParams(onExitParams),
	m_OnInteractEventName(OnInteractEventName),
	m_OnTouchEventName(OnTouchEventName),
	m_OnEnterEventName(OnEnterEventName),
	m_onExitEventName(OnExitEventName),
	m_coordsToEffect(coords),
	m_tileIdx(tileIdx)
{

}

TileEventPack::~TileEventPack()
{

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Setters
void TileEventPack::SetTileCoords(const TILE_COORDS& tileCoords)
{
	m_coordsToEffect = tileCoords;
}

void TileEventPack::SetTileIdx(int tileIdx)
{
	m_tileIdx = tileIdx;
}

void TileEventPack::SetOnInteractEvent(TileEvent* tileEvent, const std::string& eventName)
{
	m_onInteract = tileEvent;
	m_OnInteractEventName = eventName;
}

void TileEventPack::SetOnTouchEvent(TileEvent* tileEvent, const std::string& eventName)
{
	m_onTouch = tileEvent;
	m_OnTouchEventName = eventName;
}

void TileEventPack::SetOnEnterEvent(TileEvent* tileEvent, const std::string& eventName)
{
	m_onEnter = tileEvent;
	m_OnEnterEventName = eventName;
}

void TileEventPack::SetOnExitEvent(TileEvent* tileEvent, const std::string& eventName)
{
	m_onExit = tileEvent;
	m_onExitEventName = eventName;
}

void TileEventPack::SetOnInteractParams(const std::vector<StrXMLAttr>& params)
{
	m_OnInteractParams = params;
}

void TileEventPack::SetOnTouchParams(const std::vector<StrXMLAttr>& params)
{
	m_OnTouchParams = params;
}

void TileEventPack::SetOnEnterParams(const std::vector<StrXMLAttr>& params)
{
	m_OnEnterParams = params;
}

void TileEventPack::SetOnExitParams(const std::vector<StrXMLAttr>& params)
{
	m_OnExitParams = params;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Getters
const TILE_COORDS& TileEventPack::GetTileCoords() const
{
	return m_coordsToEffect;
}

const int TileEventPack::GetTileIdx() const
{
	return m_tileIdx;
}

const TileEvent* TileEventPack::GetOnInteractEvent() const
{
	return m_onInteract;
}

const TileEvent* TileEventPack::GetOnTouchEvent() const
{
	return m_onTouch;
}

const TileEvent* TileEventPack::GetOnEnterEvent() const
{
	return m_onEnter;
}

const TileEvent* TileEventPack::GetOnExitEvent() const
{
	return m_onExit;
}

TileEvent* TileEventPack::GetEditableOnInteractEvent() const
{
	return m_onInteract;
}

TileEvent* TileEventPack::GetEditableOnTouchEvent() const
{
	return m_onTouch;
}

TileEvent* TileEventPack::GetEditableOnEnterEvent() const
{
	return m_onEnter;
}

TileEvent* TileEventPack::GetEditableOnExitEvent() const
{
	return m_onExit;
}

const std::vector<StrXMLAttr>& TileEventPack::GetOnInteractParams() const
{
	return m_OnInteractParams;
}

const std::vector<StrXMLAttr>& TileEventPack::GetOnTouchParams() const
{
	return m_OnTouchParams;
}

const std::vector<StrXMLAttr>& TileEventPack::GetOnEnterParams() const
{
	return m_OnEnterParams;
}

const std::vector<StrXMLAttr>& TileEventPack::GetOnExitParams() const
{
	return m_OnExitParams;
}


void TileEventPack::WriteTileEventPackToString(std::string& str, int indentationAmt, bool writeTileCoords) const
{
	if (writeTileCoords)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Tile Coords to effect: " + MathToStringUtils::ToString(m_coordsToEffect) + "\n";
	}

	int nextLevel = indentationAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "On Interact Event Name: " + m_OnInteractEventName + "\n";
	GameDebuggingCommons::AddIndentation(str, nextLevel);
	str += "On Interact Event is Null: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_onInteract == nullptr);
	if (m_onInteract != nullptr
		&& m_OnInteractParams.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += "On Interact Variables: ";
		for (size_t i = 0; i < m_OnInteractParams.size(); i++)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel + 3);
			str += m_OnInteractParams[i].name + " : " + m_OnInteractParams[i].val + "\n";
		}
		//std::vector<StrXMLAttr> m_OnInteractParams;
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "On Touch Event Name: " + m_OnTouchEventName + "\n";
	GameDebuggingCommons::AddIndentation(str, nextLevel);
	str += "On Touch Event is Null: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_onTouch == nullptr);
	if (m_onTouch != nullptr
		&& m_OnTouchParams.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += "On Touch Variables: ";
		for (size_t i = 0; i < m_OnTouchParams.size(); i++)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel + 3);
			str += m_OnTouchParams[i].name + " : " + m_OnTouchParams[i].val + "\n";
		}
	}
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "On Enter Event Name: " + m_OnEnterEventName + "\n";
	GameDebuggingCommons::AddIndentation(str, nextLevel);
	str += "On Enter Event is Null: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_onEnter == nullptr);
	if (m_onEnter != nullptr
		&& m_OnEnterParams.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += "On Enter Variables: ";
		for (size_t i = 0; i < m_OnEnterParams.size(); i++)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel + 3);
			str += m_OnEnterParams[i].name + " : " + m_OnEnterParams[i].val + "\n";
		}
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "On Exit Event Name: " + m_onExitEventName + "\n";
	GameDebuggingCommons::AddIndentation(str, nextLevel);
	str += "On Exit Event is Null: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_onExit == nullptr);
	if (m_onExit != nullptr
		&& m_OnExitParams.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += "On Exit Variables: ";
		for (size_t i = 0; i < m_OnExitParams.size(); i++)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel + 3);
			str += m_OnExitParams[i].name + " : " + m_OnExitParams[i].val + "\n";
		}
	}
}