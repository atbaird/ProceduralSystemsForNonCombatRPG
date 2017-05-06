#pragma once

#ifndef SPECIALCASETESTRESULTS_HPP
#define SPECIALCASETESTRESULTS_HPP
#include "Engine/Math/IntVector2.hpp"
#define STATIC
#define TILE_COORDS IntVector2

class TileDefinition;
struct SpecialCaseTestResults
{
private:
protected:
public:
	//Static Variables
	static const SpecialCaseTestResults s_Empty;

	//Variables
	const TileDefinition* m_def = nullptr;
	TILE_COORDS m_direction = TILE_COORDS::ivec2_zeros;
	TILE_COORDS m_startCoords = TILE_COORDS::ivec2_zeros;
	size_t m_resultsFound = 0;

	//Constructors
	SpecialCaseTestResults();

	//Getters
	bool GetIsThisCorrectTestResult(const TileDefinition* def, const TILE_COORDS& direction,
		const TILE_COORDS& startCoords) const;
};

#endif