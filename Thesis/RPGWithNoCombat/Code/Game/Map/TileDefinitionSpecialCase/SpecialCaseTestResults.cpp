#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseTestResults.hpp"
#include "Game/Map/TileDefinition.hpp"

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Static Variables
STATIC const SpecialCaseTestResults SpecialCaseTestResults::s_Empty = SpecialCaseTestResults();

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Constructors
SpecialCaseTestResults::SpecialCaseTestResults()
{

}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Getters
bool SpecialCaseTestResults::GetIsThisCorrectTestResult(const TileDefinition* def, const TILE_COORDS& direction,
	const TILE_COORDS& coords) const
{
	return def == m_def && direction == m_direction
		&& m_startCoords == coords;
}
