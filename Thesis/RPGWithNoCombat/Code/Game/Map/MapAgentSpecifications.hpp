#pragma once

#ifndef MAPAGENTSPECIFICATIONS_HPP
#define MAPAGENTSPECIFICATIONS_HPP
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/GameEntities/Agents/AgentStats/AgentStat.hpp"
#include "Game/GameEntities/Agents/AgentTemplateNPCAttributesContainer.hpp"
#include <vector>
#include <string>

#define TILE_COORDS IntVector2

struct MapAgentSpecifications
{
public:
	//Variables
	std::vector<AgentTemplateNPCAttributesContainer> m_agentTemplateNPCAttributes;
	std::vector<std::string> m_jobsDesired;
	std::string m_agentGeneratorToUse = "";
	std::vector<TILE_COORDS> m_validSpawnPointTilesToUse;
	std::vector<AgentStat> m_overwritingStats;
	std::vector<bool> m_overwriteStats;
	TILE_COORDS m_maxSpawnPointTileCoords = TILE_COORDS::ivec2_zeros;
	TILE_COORDS m_minSpawnPointTileCoords = TILE_COORDS::ivec2_zeros;
	int m_minNumberToSpawn = 1;
	int m_maxNumberToSpawn = 1;
	bool m_tileCoordsSet = false;
	bool m_isMale = true;
	bool m_overwriteGender = false;

	//Constructors
	MapAgentSpecifications();
	virtual ~MapAgentSpecifications();

	//Setters
	void SetStatAndOverwrite(const AgentStat& stat, bool overwrite);

};

#endif