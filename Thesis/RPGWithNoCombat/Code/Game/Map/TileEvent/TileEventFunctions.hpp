#pragma once

#ifndef TILEEVENTFUNCTIONS_HPP
#define TILEEVENTFUNCTIONS_HPP
#include <vector>
#include "Engine/Core/EngineXMLParser.hpp"

class NamedProperties;
class TileEvent;
class Tile;
class TileEventFunctions
{
private:
protected:
	static const TileEvent s_ChangeTileImageEvent;
	static const TileEvent s_ChangeMapsEvent;
public:
	//Game calls
	static void RegisterAllTileEventsAtOnce();
	static void UnregisterTileEvents();

	//Event calls
	static void EventFuncChangeTileImage(Tile* tile, NamedProperties& params, const std::vector<StrXMLAttr>& attrs);
	static void EventFuncChangeMaps(Tile* tile, NamedProperties& params, const std::vector<StrXMLAttr>& attrs);
};

#endif