#include "Game/Quest/TilePositionsByEntityName.hpp"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Constructors
TilePositionsByEntityName::TilePositionsByEntityName(const std::string& entityName)
	: m_entityName(entityName)
{

}

TilePositionsByEntityName::~TilePositionsByEntityName()
{

}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Operations
void TilePositionsByEntityName::AddAllPositions(const std::vector<TILE_COORDS>& positions)
{
	for (size_t posIdx = 0; posIdx < positions.size(); posIdx++)
	{
		bool found = false;
		const TILE_COORDS& tilePos = positions[posIdx];
		for (size_t i = 0; i < m_possiblePositions.size(); i++)
		{
			if (tilePos == m_possiblePositions[i])
			{
				found = true;
				break;
			}
		}
		if (found)
		{
			continue;
		}
		m_possiblePositions.push_back(tilePos);
	}
}