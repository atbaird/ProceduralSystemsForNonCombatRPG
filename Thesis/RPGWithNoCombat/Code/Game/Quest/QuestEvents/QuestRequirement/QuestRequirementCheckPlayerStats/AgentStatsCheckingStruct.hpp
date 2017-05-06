#pragma once

#ifndef AGENTSTATSCHECKINGSTRUCT_HPP
#define AGENTSTATSCHECKINGSTRUCT_HPP
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseOperations.hpp"
#include <string>

struct AgentStatsCheckingStruct
{
private:
protected:
	//Variables
	std::string m_statName = "";
	std::string m_lowerCaseStatName = "";
	SpecialCaseOperations m_operationToPerform = IS_SAME;
	int m_statVal = 0;
public:
	//Constructors
	AgentStatsCheckingStruct(const std::string& statName = "");
	virtual ~AgentStatsCheckingStruct();

	//Setters
	void SetStatName(const std::string& name);
	void SetSpecialCaseOperations(SpecialCaseOperations operation);
	void SetStatValue(int statVal);

	//Getters
	const std::string& GetStatName() const;
	const std::string& GetLowerCaseStatName() const;
	const SpecialCaseOperations GetOperationToPerform() const;
	int GetStatValue() const;
	bool TestValAgainstStat(int val) const;
	void WriteToString(std::string& str, int indentationAmt) const;
};

#endif