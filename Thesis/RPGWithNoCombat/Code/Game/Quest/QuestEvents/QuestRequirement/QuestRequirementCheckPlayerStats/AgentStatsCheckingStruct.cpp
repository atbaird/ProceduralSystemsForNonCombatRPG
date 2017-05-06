#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckPlayerStats/AgentStatsCheckingStruct.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Constructors
AgentStatsCheckingStruct::AgentStatsCheckingStruct(const std::string& statName)
{
	SetStatName(statName);
}

AgentStatsCheckingStruct::~AgentStatsCheckingStruct()
{

}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Setters
void AgentStatsCheckingStruct::SetStatName(const std::string& name)
{
	m_statName = name;
	m_lowerCaseStatName = MakeLower(name);
}

void AgentStatsCheckingStruct::SetSpecialCaseOperations(SpecialCaseOperations operation)
{
	m_operationToPerform = operation;
}

void AgentStatsCheckingStruct::SetStatValue(int statVal)
{
	m_statVal = statVal;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Getters
const std::string& AgentStatsCheckingStruct::GetStatName() const
{
	return m_statName;
}

const std::string& AgentStatsCheckingStruct::GetLowerCaseStatName() const
{
	return m_lowerCaseStatName;
}

const SpecialCaseOperations AgentStatsCheckingStruct::GetOperationToPerform() const
{
	return m_operationToPerform;
}

int AgentStatsCheckingStruct::GetStatValue() const
{
	return m_statVal;
}

bool AgentStatsCheckingStruct::TestValAgainstStat(int val) const
{
	switch (m_operationToPerform)
	{
	case IS_SAME:						return val == m_statVal;
	case IS_LESS_THAN:					return val < m_statVal;
	case IS_GREATER_THAN:				return val > m_statVal;
	case IS_GREATER_THAN_OR_EQUAL_TO:	return val >= m_statVal;
	case IS_LESS_THAN_OR_EQUAL_TO:		return val <= m_statVal;
	case IS_NOT_EQUAL_TO:				return val != m_statVal;
	default:							return false;
	}
}

void AgentStatsCheckingStruct::WriteToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Stat Name: " + m_statName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value Desired: ";
	GameDebuggingCommons::WriteSpecialCaseOperatorToString(str, m_operationToPerform);
	str += std::to_string(m_statVal) + "\n";
}