#pragma once

#ifndef TILEDEFINITIONSPECIALCASE_HPP
#define TILEDEFINITIONSPECIALCASE_HPP
#include "Game/Map/TileDefinitionSpecialCase/TileDefinitionSpecialCaseDirectionInfo.hpp"
#include "Game/Map/TileDefinitionSpecialCase/TileDefintionSpecialCaseToSet.hpp"
#include <vector>

struct XMLNode;
struct XMLAttribute;
class Map;


struct TileDefinitionSpecialCase
{
public:
	//Variables
	std::string m_debugName = "";
	std::vector<TileDefinitionSpecialCaseToSet> m_ChangesToPerform;
	std::vector<TileDefinitionSpecialCaseDirectionInfo> m_NorthBound;
	std::vector<TileDefinitionSpecialCaseDirectionInfo> m_SouthBound;
	std::vector<TileDefinitionSpecialCaseDirectionInfo> m_EastBound;
	std::vector<TileDefinitionSpecialCaseDirectionInfo> m_WestBound;

	//Constructors
	TileDefinitionSpecialCase();
	TileDefinitionSpecialCase(const XMLNode& node);
	virtual ~TileDefinitionSpecialCase();
protected:
	void ReadXMLAttributesFromNode(const XMLNode& node);
	void ReadXMLAttributeForIfSameValues(const XMLAttribute& attr);
	void ReadXMLAttributeForNeighborTypeValues(const XMLAttribute& attr);
	bool DetermineOperationToSet(TileDefinitionSpecialCaseDirectionInfo& North,
		TileDefinitionSpecialCaseDirectionInfo& South,
		TileDefinitionSpecialCaseDirectionInfo& East,
		TileDefinitionSpecialCaseDirectionInfo& West,
		bool& usedNorth, bool& usedSouth, bool& usedEast, bool& usedWest,
		const std::string& direction, int cnt, SpecialCaseOperations operation);
	void ParseSpecialOperator(const std::string& dirAndValExpected,
		size_t& size,
		int& charsBeforeEncounterAnOperator,
		int& charsAfterOperatorEnd);
	void ReadXMLAttributeCheckStatementReadOperationAndNumber(SpecialCaseOperations& operation,
		int charsBeforeEncounterAnOperator,
		int charsAfterOperatorEnd,
		const std::string& dirAndValExpected,
		std::string& number,
		size_t size);
	void ReadXMLAttributeCheckStatementCommonFinish(TileDefinitionSpecialCaseDirectionInfo& North,
		TileDefinitionSpecialCaseDirectionInfo& South,
		TileDefinitionSpecialCaseDirectionInfo& East,
		TileDefinitionSpecialCaseDirectionInfo& West,
		bool usedNorth, bool usedSouth, bool usedEast, bool usedWest);
	void ReadXMLChildrenFromNode(const XMLNode& node);
	bool ReadAttributeOffNodeAndOntoSetCase(const XMLAttribute& attr, TileDefinitionSpecialCaseToSet& set);
	void ReadToSetNode(const XMLNode& child);
public:

	//Setters
	void SetOrAddSpecialCase(const TileDefinitionSpecialCaseToSet& set);

	//Getters
	bool GetMeetsAllStandardsOfCase(const TILE_COORDS& tilecoords, Map* map) const;
	bool GetMeetsAllStandardsOfCase(const TILE_COORDS& tilecoords, Map* map, 
		std::vector<SpecialCaseTestResults>& results) const;
	int GetIndexOfTileDefinitionSpecialCaseToSetForTileCoords(const TILE_COORDS& coords) const;
	void WriteSpecialCaseToString(std::string& str, int indentationAmt) const;
};

#endif