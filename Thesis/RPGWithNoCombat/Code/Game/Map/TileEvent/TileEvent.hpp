#pragma once

#ifndef TILEEVENT_HPP
#define TILEEVENT_HPP
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include <vector>
#include <string>

#define STATIC
class Tile;
typedef void(TileEventFunc)(Tile* tile, NamedProperties& params, const std::vector<StrXMLAttr>& attrs);

class TileEvent
{
private:
protected:
	//Static Variables
	//static std::vector<std::string> s_AllEventNames;
	static std::vector<TileEvent*> s_AllTileEvents;

	//Variables
	std::string m_eventName = "";
	TileEventFunc* m_function = nullptr;
public:
	//Constructors
	TileEvent(const std::string& eventName, TileEventFunc* function);
	virtual ~TileEvent();

	//operations
	void CallFunction(Tile* tile, NamedProperties& params, const std::vector<StrXMLAttr>& attrs);

	//Static Methods
	static int GetIndexForEventIfExists(const std::string& eventName);
	static bool DoesEventExist(const std::string& eventName);
	static TileEvent* GetTileEventAtIdx(int idx);
	static TileEvent* GetTileEventForName(const std::string& eventName);
	static void RegisterTileEvent(TileEvent* even);
	static void UnregisterTileEvent(TileEvent* even);
	static void UnregisterTileEvent(const std::string& eventName);
	static void ClearAllTileEvents();
};

#endif