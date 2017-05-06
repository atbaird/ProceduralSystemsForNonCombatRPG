#include "Game/Quest/MapQuestImportantData.hpp"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Operations
void MapQuestImportantData::AddPositionData(
	const std::vector<TilePositionsByEntityName>& positionsForEntity)
{
	for (size_t tilePositionSetIdx = 0; tilePositionSetIdx < positionsForEntity.size(); tilePositionSetIdx++)
	{
		bool found = false;
		int indexFoundAt = 0;
		const TilePositionsByEntityName& val = positionsForEntity[tilePositionSetIdx];
		for (size_t i = 0; i < acceptablePositionsPerMap.size(); i++)
		{
			TilePositionsByEntityName& myVal = acceptablePositionsPerMap[i];
			if (myVal.m_entityName == val.m_entityName
				&& myVal.m_mapName == val.m_mapName)
			{
				found = true;
				indexFoundAt = i;
				break;
			}
		}
		if (found)
		{
			acceptablePositionsPerMap[indexFoundAt].AddAllPositions(val.m_possiblePositions);
			continue;
		}
		acceptablePositionsPerMap.push_back(val);

	}
}