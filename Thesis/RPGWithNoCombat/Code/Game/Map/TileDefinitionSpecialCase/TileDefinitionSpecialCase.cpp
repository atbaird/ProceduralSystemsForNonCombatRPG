#include "Game/Map/TileDefinitionSpecialCase/TileDefinitionSpecialCase.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Constructors
TileDefinitionSpecialCase::TileDefinitionSpecialCase()
{

}

TileDefinitionSpecialCase::TileDefinitionSpecialCase(const XMLNode& node)
{
	ReadXMLAttributesFromNode(node);
	ReadXMLChildrenFromNode(node);
}

TileDefinitionSpecialCase::~TileDefinitionSpecialCase()
{
}


void TileDefinitionSpecialCase::ReadXMLAttributesFromNode(const XMLNode& node)
{
	TileDefinitionSpecialCaseToSet defaultCase = TileDefinitionSpecialCaseToSet();
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "ifsame"
			|| attrName == "same")
		{
			ReadXMLAttributeForIfSameValues(attr);
		}
		else if (attrName == "ifneighbortypeis"
			|| attrName == "neighbortype"
			|| attrName == "neighboristype")
		{
			ReadXMLAttributeForNeighborTypeValues(attr);
		}
		else if (ReadAttributeOffNodeAndOntoSetCase(attr, defaultCase))
		{
			if (attrName == "debugname")
			{
				m_debugName = std::string(attr.lpszValue);
			}
			continue;
		}
	}
	m_ChangesToPerform.push_back(defaultCase);
}

void TileDefinitionSpecialCase::ReadXMLAttributeForIfSameValues(const XMLAttribute& attr)
{
	TileDefinitionSpecialCaseDirectionInfo North = TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS(0, 1));
	TileDefinitionSpecialCaseDirectionInfo South = TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS(0, -1));
	TileDefinitionSpecialCaseDirectionInfo East = TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS(1, 0));
	TileDefinitionSpecialCaseDirectionInfo West = TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS(-1, 0));
	bool usedNorth = false;
	bool usedSouth = false;
	bool usedEast = false;
	bool usedWest = false;

	std::string val = MakeLower(attr.lpszValue);
	std::vector<std::string> parsed = ParseString(val, ',');
	for (size_t parseIdx = 0; parseIdx < parsed.size(); parseIdx++)
	{
		std::string dirAndValExpected = parsed[parseIdx];
		size_t size = dirAndValExpected.size();
		int charsBeforeEncounterAnOperator = -1;
		int charsAfterOperatorEnd = -1;
		ParseSpecialOperator(dirAndValExpected, size, charsBeforeEncounterAnOperator, charsAfterOperatorEnd);
		if (charsBeforeEncounterAnOperator == -1)
		{
			continue;
		}
		std::string direction = dirAndValExpected.substr(0, charsBeforeEncounterAnOperator);
		std::string number = "0";
		SpecialCaseOperations operation = IS_SAME;
		ReadXMLAttributeCheckStatementReadOperationAndNumber(operation, charsBeforeEncounterAnOperator, 
			charsAfterOperatorEnd, dirAndValExpected, number, size);
		

		int cnt = atoi(number.c_str());
		DetermineOperationToSet(North, South, East, West,
			usedNorth, usedSouth, usedEast, usedWest,
			direction, cnt, operation);
	}
	ReadXMLAttributeCheckStatementCommonFinish(North, South, East, West, 
		usedNorth, usedSouth, usedEast, usedWest);
}


void TileDefinitionSpecialCase::ReadXMLAttributeForNeighborTypeValues(const XMLAttribute& attr)
{
	TileDefinitionSpecialCaseDirectionInfo North = TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS(0, 1));
	TileDefinitionSpecialCaseDirectionInfo South = TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS(0, -1));
	TileDefinitionSpecialCaseDirectionInfo East = TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS(1, 0));
	TileDefinitionSpecialCaseDirectionInfo West = TileDefinitionSpecialCaseDirectionInfo(TILE_COORDS(-1, 0));
	North.m_usesSameType = false;
	South.m_usesSameType = false;
	East.m_usesSameType = false;
	West.m_usesSameType = false;
	bool usedNorth = false;
	bool usedSouth = false;
	bool usedEast = false;
	bool usedWest = false;

	std::string val = MakeLower(attr.lpszValue);
	std::vector<std::string> parsed = ParseString(val, ',');
	for (size_t parseIdx = 0; parseIdx < parsed.size(); parseIdx++)
	{
		std::string dirAndValExpected = parsed[parseIdx];
		size_t size = dirAndValExpected.size();
		int charsBeforeEncounterAnOperator = -1;
		int charsAfterOperatorEnd = -1;
		ParseSpecialOperator(dirAndValExpected, size, charsBeforeEncounterAnOperator, charsAfterOperatorEnd);
		if (charsBeforeEncounterAnOperator == -1)
		{
			continue;
		}
		std::string direction = dirAndValExpected.substr(0, charsBeforeEncounterAnOperator);
		std::string number = "0";
		SpecialCaseOperations operation = IS_SAME;
		ReadXMLAttributeCheckStatementReadOperationAndNumber(operation, charsBeforeEncounterAnOperator,
			charsAfterOperatorEnd, dirAndValExpected, number, size);


		int cnt = atoi(number.c_str());
		if (DetermineOperationToSet(North, South, East, West,
			usedNorth, usedSouth, usedEast, usedWest,
			direction, cnt, operation) == false)
		{
			if (direction == "type"
				|| direction == "tiledefinition"
				|| direction == "definition")
			{
				std::string tileDef = number;
				if (tileDef.length() > 0 &&
					(tileDef.substr(0, 1) == "\""
					|| tileDef.substr(0,1) == "'"))
				{
					tileDef = tileDef.substr(1, tileDef.length() - 1);
				}
				if (tileDef.length() > 0 &&
					(tileDef.substr(tileDef.length() - 1, 1) == "\""
						|| tileDef.substr(tileDef.length() - 1, 1) == "'"))
				{
					tileDef = tileDef.substr(0, tileDef.length() - 1);
				}

				North.m_TileDefinitionLookingFor = tileDef;
				South.m_TileDefinitionLookingFor = tileDef;
				East.m_TileDefinitionLookingFor = tileDef;
				West.m_TileDefinitionLookingFor = tileDef;
			}
		}
	}
	ReadXMLAttributeCheckStatementCommonFinish(North, South, East, West,
		usedNorth, usedSouth, usedEast, usedWest);
}


bool TileDefinitionSpecialCase::DetermineOperationToSet(TileDefinitionSpecialCaseDirectionInfo& North,
	TileDefinitionSpecialCaseDirectionInfo& South,
	TileDefinitionSpecialCaseDirectionInfo& East,
	TileDefinitionSpecialCaseDirectionInfo& West,
	bool& usedNorth, bool& usedSouth, bool& usedEast, bool& usedWest,
	const std::string& direction, int cnt, SpecialCaseOperations operation)
{
	if (direction == "n"
		|| direction == "north")
	{
		North.m_countInThatDirection = cnt;
		North.m_specialCaseOperation = operation;
		usedNorth = true;
		return true;
	}
	else if (direction == "s"
		|| direction == "south")
	{
		South.m_countInThatDirection = cnt;
		South.m_specialCaseOperation = operation;
		usedSouth = true;
		return true;
	}
	else if (direction == "e"
		|| direction == "east")
	{
		East.m_countInThatDirection = cnt;
		East.m_specialCaseOperation = operation;
		usedEast = true;
		return true;
	}
	else if (direction == "w"
		|| direction == "west")
	{
		West.m_countInThatDirection = cnt;
		West.m_specialCaseOperation = operation;
		usedWest = true;
		return true;
	}
	return false;
}

void TileDefinitionSpecialCase::ParseSpecialOperator(const std::string& dirAndValExpected, 
	size_t& size, int& charsBeforeEncounterAnOperator, int& charsAfterOperatorEnd)
{
	size = dirAndValExpected.size();
	charsBeforeEncounterAnOperator = -1;
	charsAfterOperatorEnd = -1;
	for (int charIdx = 0; charIdx < (int)size; charIdx++)
	{
		unsigned char car = dirAndValExpected[charIdx];
		bool carIsAnOperator = car == '='
			|| car == '<'
			|| car == '>'
			|| car == '!';
		if (charsBeforeEncounterAnOperator == -1
			&& carIsAnOperator)
		{
			charsBeforeEncounterAnOperator = charIdx;
		}
		else if (charsBeforeEncounterAnOperator != -1
			&& charsAfterOperatorEnd == -1
			&& !carIsAnOperator)
		{
			charsAfterOperatorEnd = charIdx;
		}
	}
}

void TileDefinitionSpecialCase::ReadXMLAttributeCheckStatementReadOperationAndNumber(
	SpecialCaseOperations& operation,
	int charsBeforeEncounterAnOperator,
	int charsAfterOperatorEnd,
	const std::string& dirAndValExpected,
	std::string& number,
	size_t size)
{
	std::string operatorVal = "";
	if (charsAfterOperatorEnd == -1)
	{
		operatorVal = dirAndValExpected.substr(charsAfterOperatorEnd, size - charsBeforeEncounterAnOperator);
	}
	else
	{
		operatorVal = dirAndValExpected.substr(charsBeforeEncounterAnOperator,
			charsAfterOperatorEnd - charsBeforeEncounterAnOperator);
		number = dirAndValExpected.substr(charsAfterOperatorEnd, size - charsAfterOperatorEnd);
	}

	if (operatorVal == "=="
		|| operatorVal == "=")
	{
		operation = IS_SAME;
	}
	else if (operatorVal == ">")
	{
		operation = IS_GREATER_THAN;
	}
	else if (operatorVal == "<")
	{
		operation = IS_LESS_THAN;
	}
	else if (operatorVal == ">=")
	{
		operation = IS_GREATER_THAN_OR_EQUAL_TO;
	}
	else if (operatorVal == "<=")
	{
		operation = IS_LESS_THAN_OR_EQUAL_TO;
	}
	else if (operatorVal == "!=")
	{
		operation = IS_NOT_EQUAL_TO;
	}
}

void TileDefinitionSpecialCase::ReadXMLAttributeCheckStatementCommonFinish(
	TileDefinitionSpecialCaseDirectionInfo& North,
	TileDefinitionSpecialCaseDirectionInfo& South,
	TileDefinitionSpecialCaseDirectionInfo& East,
	TileDefinitionSpecialCaseDirectionInfo& West,
	bool usedNorth, bool usedSouth, bool usedEast, bool usedWest)
{
	if (usedWest)
	{
		m_WestBound.push_back(West);
	}
	if (usedSouth)
	{
		m_SouthBound.push_back(South);
	}
	if (usedNorth)
	{
		m_NorthBound.push_back(North);
	}
	if (usedEast)
	{
		m_EastBound.push_back(East);
	}
}

void TileDefinitionSpecialCase::ReadXMLChildrenFromNode(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "toset")
		{
			ReadToSetNode(child);

		}
	}
}

bool TileDefinitionSpecialCase::ReadAttributeOffNodeAndOntoSetCase(const XMLAttribute& attr,
	TileDefinitionSpecialCaseToSet& set)
{
	std::string attrName = MakeLower(attr.lpszName);
	if (attrName == "tilecoords"
		|| attrName == "relativetilecoords"
		|| attrName == "tilecoordsdifference"
		|| attrName == "tilecoordsdiff")
	{
		std::string val = ReplaceCharInString(attr.lpszValue, ',', ' ');
		std::vector<std::string> vals = ParseString(val, ' ');
		TILE_COORDS coords = TILE_COORDS::ivec2_zeros;
		if (vals.size() > 0)
		{
			coords.x = atoi(vals[0].c_str());
		}
		if (vals.size() > 1)
		{
			coords.y = atoi(vals[1].c_str());
		}

		set.m_TileCoordsToEditAdjustment = coords;
	}
	else if (attrName == "bottomresourcename"
		|| attrName == "bottomresource")
	{
		std::string resourceName = std::string(attr.lpszValue);
		const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		set.SetBottomResource(res);
	}
	else if (attrName == "altbottomresourcename"
		|| attrName == "altbottomresource")
	{
		std::string resourceName = std::string(attr.lpszValue);
		const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		set.SetAltBottomResource(res);
	}
	else if (attrName == "resourcename"
		|| attrName == "resource")
	{
		std::string resourceName = std::string(attr.lpszValue);
		const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		set.SetResource(res);
	}
	else if (attrName == "altresourcename"
		|| attrName == "altresource")
	{
		std::string resourceName = std::string(attr.lpszValue);
		const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		set.SetAltResource(res);
		return true;
	}
	else if (attrName == "overlayresourcename"
		|| attrName == "overlayresource"
		|| attrName == "topresourcename"
		|| attrName == "topresource")
	{
		std::string resourceName = std::string(attr.lpszValue);
		const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		set.SetTopResource(res);
		return true;
	}
	else if (attrName == "altoverlayresourcename"
		|| attrName == "altoverlayresource"
		|| attrName == "alttopresourcename"
		|| attrName == "alttopresource")
	{
		std::string resourceName = std::string(attr.lpszValue);
		const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		set.SetAltTopResource(res);
		return true;
	}
	else if (attrName == "overlayresource2name"
		|| attrName == "overlayresource2"
		|| attrName == "topresource2name"
		|| attrName == "topresource2")
	{
		std::string resourceName = std::string(attr.lpszValue);
		const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		set.SetTopResource2(res);
		return true;
	}
	else if (attrName == "altoverlayresource2name"
		|| attrName == "altoverlayresource2"
		|| attrName == "alttopresource2name"
		|| attrName == "alttopresource2")
	{
		std::string resourceName = std::string(attr.lpszValue);
		const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		set.SetAltTopResource2(res);
		return true;
	}
	else if (attrName == "altbottomresourcechance")
	{
		float chance = (float)atof(attr.lpszValue);
		set.m_altBottomResourceChance = chance;
		return true;
	}
	else if (attrName == "altresourcechance")
	{
		float chance = (float)atof(attr.lpszValue);
		set.m_altResourceChance = chance;
		return true;
	}
	else if (attrName == "alttopresourcechance"
		|| attrName == "altoverlayresourcechance"
		|| attrName == "alttopchance"
		|| attrName == "altoverlaychance")
	{
		float chance = (float)atof(attr.lpszValue);
		set.m_altTopResourceChance = chance;
		return true;
	}
	else if (attrName == "alttopresource2chance"
		|| attrName == "altoverlayresource2chance"
		|| attrName == "alttop2chance"
		|| attrName == "altoverlay2chance")
	{
		float chance = (float)atof(attr.lpszValue);
		set.m_altTopResource2Chance = chance;
		return true;
	}
	else if (attrName == "solid"
		|| attrName == "issolid")
	{
		bool solid = false;
		std::string attrVal = MakeLower((attr.lpszValue));
		if (attrVal == "true"
			|| attrVal == "t")
		{
			solid = true;
		}
		else if (attrVal == "false"
			|| attrVal == "f")
		{
			solid = false;
		}
		set.SetIsSolid(true, solid);
		return true;
	}
	else if (attrName == "liquid"
		|| attrName == "isliquid")
	{
		bool liquid = false;
		std::string attrVal = MakeLower((attr.lpszValue));
		if (attrVal == "true"
			|| attrVal == "t")
		{
			liquid = true;
		}
		else if (attrVal == "false"
			|| attrVal == "f")
		{
			liquid = false;
		}
		set.SetIsLiquid(true, liquid);
		return true;
	}
	else if (attrName == "debugname")
	{
		set.m_debugName = std::string(attr.lpszValue);
		return true;
	}
	return false;
}


void TileDefinitionSpecialCase::ReadToSetNode(const XMLNode& child)
{
	TileDefinitionSpecialCaseToSet set = TileDefinitionSpecialCaseToSet();
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		ReadAttributeOffNodeAndOntoSetCase(attr, set);
	}
	SetOrAddSpecialCase(set);
}


void TileDefinitionSpecialCase::SetOrAddSpecialCase(const TileDefinitionSpecialCaseToSet& set)
{
	for (size_t changesIdx = 0; changesIdx < m_ChangesToPerform.size(); changesIdx++)
	{
		if (m_ChangesToPerform[changesIdx].m_TileCoordsToEditAdjustment == set.m_TileCoordsToEditAdjustment)
		{
			m_ChangesToPerform[changesIdx] = set;
			return;
		}
	}
	m_ChangesToPerform.push_back(set);
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Getters
bool TileDefinitionSpecialCase::GetMeetsAllStandardsOfCase(const TILE_COORDS& tilecoords, Map* map) const
{
	std::vector<SpecialCaseTestResults> results;
	return GetMeetsAllStandardsOfCase(tilecoords, map, results);
}

bool TileDefinitionSpecialCase::GetMeetsAllStandardsOfCase(const TILE_COORDS& tilecoords, Map* map,
	std::vector<SpecialCaseTestResults>& results) const
{
	//North
	for (size_t idx = 0; idx < m_NorthBound.size(); idx++)
	{
		const TileDefinitionSpecialCaseDirectionInfo& info = m_NorthBound[idx];
		if (info.VerifyNumberInDirectionIsCorrect(map, tilecoords, results) == false)
		{
			return false;
		}
	}

	//East
	for (size_t idx = 0; idx < m_EastBound.size(); idx++)
	{
		const TileDefinitionSpecialCaseDirectionInfo& info = m_EastBound[idx];
		if (info.VerifyNumberInDirectionIsCorrect(map, tilecoords, results) == false)
		{
			return false;
		}
	}

	//West
	for (size_t idx = 0; idx < m_WestBound.size(); idx++)
	{
		const TileDefinitionSpecialCaseDirectionInfo& info = m_WestBound[idx];
		if (info.VerifyNumberInDirectionIsCorrect(map, tilecoords, results) == false)
		{
			return false;
		}
	}

	//South
	for (size_t idx = 0; idx < m_SouthBound.size(); idx++)
	{
		const TileDefinitionSpecialCaseDirectionInfo& info = m_SouthBound[idx];
		if (info.VerifyNumberInDirectionIsCorrect(map, tilecoords, results) == false)
		{
			return false;
		}
	}
	return true;
}

int TileDefinitionSpecialCase::GetIndexOfTileDefinitionSpecialCaseToSetForTileCoords(const TILE_COORDS& coords) const
{
	for (size_t setIdx = 0; setIdx < m_ChangesToPerform.size(); setIdx++)
	{
		if (m_ChangesToPerform[setIdx].m_TileCoordsToEditAdjustment == coords)
		{
			return setIdx;
		}
	}
	return -1;
}

void TileDefinitionSpecialCase::WriteSpecialCaseToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Special Case Debug Name: " + m_debugName + "\n";

	indentationAmt += 3;
	int nextLevel = indentationAmt + 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of North Bound Tests: " + std::to_string(m_NorthBound.size()) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "North Bound Tests: \n";
	for (size_t i = 0; i < m_NorthBound.size(); i++)
	{
		m_NorthBound[i].WriteTestInfoToString(str, nextLevel);
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of East Bound Tests: " + std::to_string(m_EastBound.size()) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "East Bound Tests: \n";
	for (size_t i = 0; i < m_EastBound.size(); i++)
	{
		m_EastBound[i].WriteTestInfoToString(str, nextLevel);
	}


	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of South Bound Tests: " + std::to_string(m_SouthBound.size()) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "South Bound Test: \n";
	for (size_t i = 0; i < m_SouthBound.size(); i++)
	{
		m_SouthBound[i].WriteTestInfoToString(str, nextLevel);
	}


	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of West Bound Test: " + std::to_string(m_WestBound.size()) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "West Bound Tests: \n";
	for (size_t i = 0; i < m_WestBound.size(); i++)
	{
		m_WestBound[i].WriteTestInfoToString(str, nextLevel);
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Changes to perform if all requirements are met: \n";
	for (size_t i = 0; i < m_ChangesToPerform.size(); i++)
	{
		m_ChangesToPerform[i].WriteToSetToString(str, nextLevel);
	}
}