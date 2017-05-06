#pragma once

#ifndef MAPQUESTIMPORTANTDATA_HPP
#define MAPQUESTIMPORTANTDATA_HPP
#include "Game/Quest/TilePositionsByEntityName.hpp"
#include "Engine/Math/Vector2.hpp"
#include <vector>
#include <string>

struct MapQuestImportantData
{
public:
	//Variables
	std::vector<TilePositionsByEntityName> acceptablePositionsPerMap;
public:
	//Constructors


	//Operations
	void AddPositionData(const std::vector<TilePositionsByEntityName>& positionsForEntity);
};

#endif