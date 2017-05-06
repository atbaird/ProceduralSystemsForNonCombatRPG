#pragma once

#ifndef TILEDEFINITIONSPECIALCASEDIRECTIONINFO_HPP
#define TILEDEFINITIONSPECIALCASEDIRECTIONINFO_HPP
#include "Engine/Math/IntVector2.hpp"
#include "Game/Map/TileDefinitionSpecialCase/TileDefinitionSpecialCaseRequirement.hpp"
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseOperations.hpp"
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseTestResults.hpp"
#include <string>
#include <vector>

#define TILE_COORDS IntVector2

class Map;
struct TileDefinitionSpecialCaseDirectionInfo
{
public:
	//Variables
	std::string m_TileDefinitionLookingFor = "";
	TILE_COORDS m_direction = TILE_COORDS::ivec2_zeros;
	size_t m_countInThatDirection = 0;
	TileDefinitionSpecialCaseRequirement m_specialCase = TILEDEFINITIONS_ARE_SAME;
	SpecialCaseOperations m_specialCaseOperation = IS_SAME;
	bool m_usesSameType = true;

	//Constructors
	TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS direction = TILE_COORDS::ivec2_zeros);
	virtual ~TileDefinitionSpecialCaseDirectionInfo();

	//Getters
	bool VerifyNumberInDirectionIsCorrectNumberComparison(size_t number) const;
	bool VerifyNumberInDirectionIsCorrect(Map* map, const TILE_COORDS& coordsTestingFrom,
		std::vector<SpecialCaseTestResults>& results) const;
	void WriteTestInfoToString(std::string& str, int indentationAmt) const;
};

#endif