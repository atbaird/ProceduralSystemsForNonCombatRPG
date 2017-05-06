#pragma once

#ifndef TILEPOSITIONSBYENTITYNAME_HPP
#define TILEPOSITIONSBYENTITYNAME_HPP
#include "Engine/Math/IntVector2.hpp"
#include <vector>

#define TILE_COORDS IntVector2
struct TilePositionsByEntityName
{
public:
	//Variables
	std::string m_entityName = "";
	std::string m_mapName = "";
	std::vector<TILE_COORDS> m_possiblePositions;
public:
	//Constructors
	TilePositionsByEntityName(const std::string& entityName = "");
	virtual ~TilePositionsByEntityName();

	//Operations
	void AddAllPositions(const std::vector<TILE_COORDS>& positions);
};

#endif