#include "Game/Map/TileDefinitionSpecialCase/TileDefinitionSpecialCaseDirectionInfo.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Engine/Math/MathToStringUtils.hpp"

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Constructors
TileDefinitionSpecialCaseDirectionInfo::TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS direction)
	: m_direction(direction)
{

}

TileDefinitionSpecialCaseDirectionInfo::~TileDefinitionSpecialCaseDirectionInfo()
{

}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Getters
bool TileDefinitionSpecialCaseDirectionInfo::VerifyNumberInDirectionIsCorrectNumberComparison(size_t number) const
{
	switch (m_specialCaseOperation)
	{
	case IS_SAME:						return number == m_countInThatDirection;
	case IS_GREATER_THAN:				return number > m_countInThatDirection;
	case IS_GREATER_THAN_OR_EQUAL_TO:	return number >= m_countInThatDirection;
	case IS_LESS_THAN:					return number < m_countInThatDirection;
	case IS_LESS_THAN_OR_EQUAL_TO:		return number <= m_countInThatDirection;
	case IS_NOT_EQUAL_TO:				return number != m_countInThatDirection;
	default:							return false;
	}

}

bool TileDefinitionSpecialCaseDirectionInfo::VerifyNumberInDirectionIsCorrect(Map* map, 
	const TILE_COORDS& coordsTestingFrom, std::vector<SpecialCaseTestResults>& results) const
{
	if (map == nullptr)
	{
		return false;
	}

	const Tile* tile = map->GetTileAtCoords(coordsTestingFrom);
	const char* tileType = m_TileDefinitionLookingFor.c_str();
	const TileDefinition* definition = nullptr;
	if (m_usesSameType)
	{
		definition = tile->GetTileDefinition();
		if (definition != nullptr)
		{
			tileType = definition->GetDebugName().c_str();
		}
	}
	else
	{
		bool found = false;
		definition = TileDefinition::GetTileDefinitionByName(tileType, found);
	}

	if (definition == nullptr)
	{
		return false;
	}

	size_t numberInDirection = 0;
	bool found = false;
	for (size_t resultIdx = 0; resultIdx < results.size(); resultIdx++)
	{
		const SpecialCaseTestResults& result = results[resultIdx];
		if (result.GetIsThisCorrectTestResult(definition, m_direction, coordsTestingFrom))
		{
			found = true;
			numberInDirection = result.m_resultsFound;
			break;
		}
	}

	if (!found)
	{
		int numberToAlter = 0;
		SpecialCaseTestResults result =
			map->CheckNumberOfTilesSameInDirection(numberInDirection, coordsTestingFrom, 
				m_direction, definition, numberToAlter);
		result.m_resultsFound = numberInDirection;

		//if(result.m_resultsFound != 0)
		//{
		//	Map* unconst_map = const_cast<Map*>(map);
		//	unconst_map->PassResultInDirection(coordsTestingFrom, m_direction, numberInDirection, numberToAlter,
		//		definition);
		//}

		results.push_back(result);
	}
	return VerifyNumberInDirectionIsCorrectNumberComparison(numberInDirection);
}


void TileDefinitionSpecialCaseDirectionInfo::WriteTestInfoToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tile Definition looking for: " + m_TileDefinitionLookingFor + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Looking for same type as current Tile Definition: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_usesSameType);

	indentationAmt += 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Direction testing in: " + MathToStringUtils::ToString(m_direction) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Test case: ";

	switch (m_specialCaseOperation)
	{
	case IS_GREATER_THAN:
		str += ">";
		break;
	case IS_GREATER_THAN_OR_EQUAL_TO:
		str += ">=";
		break;
	case IS_LESS_THAN:
		str += "<";
		break;
	case IS_LESS_THAN_OR_EQUAL_TO:
		str += "<=";
		break;
	case IS_NOT_EQUAL_TO:
		str += "!=";
		break;
	case IS_SAME:
		str += "==";
		break;
	default:
		break;
	}
	str += std::to_string(m_countInThatDirection) + "\n";
}