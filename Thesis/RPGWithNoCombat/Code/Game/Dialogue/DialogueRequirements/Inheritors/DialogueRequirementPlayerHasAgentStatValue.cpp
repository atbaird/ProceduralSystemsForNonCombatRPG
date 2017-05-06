#include "Game/Dialogue/DialogueRequirements/Inheritors/DialoguerequirementPlayerHasAgentStatValue.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Spawning Functions
DialogueRequirement* DialogueReqPlayerHasAgentStatValueCreationFunc(const XMLNode& node)
{
	return new DialogueRequirementPlayerHasAgentStatValue(node);
}

DialogueRequirement* DialogueReqPlayerHasAgentStatValueDefaultCreationFunc()
{
	return new DialogueRequirementPlayerHasAgentStatValue();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Static Variables
STATIC const DialogueRequirementRegistrationHelper 
	DialogueRequirementPlayerHasAgentStatValue::s_PlayerHasAgentStatValueRegHelper
		= DialogueRequirementRegistrationHelper("PlayerHasAgentStatValue",
			DialogueReqPlayerHasAgentStatValueCreationFunc,
			DialogueReqPlayerHasAgentStatValueDefaultCreationFunc);

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Constructors
DialogueRequirementPlayerHasAgentStatValue::DialogueRequirementPlayerHasAgentStatValue()
	: DialogueRequirement()
{

}

DialogueRequirementPlayerHasAgentStatValue::DialogueRequirementPlayerHasAgentStatValue(const XMLNode& node)
	: DialogueRequirement(node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variable"
			|| attrName == "variablename")
		{
			SetStatName(std::string(attr.lpszValue));
		}
		else if (attrName == "value")
		{
			std::string val = std::string(attr.lpszValue);
			QuestRequirement::ParseTextForSpecialCaseOperationsAndValue(val,
				m_specialCaseOperation, m_statValue);
		}
	}
}

DialogueRequirementPlayerHasAgentStatValue::DialogueRequirementPlayerHasAgentStatValue(
	const DialogueRequirementPlayerHasAgentStatValue& other)
	: DialogueRequirement(other),
	m_statName(other.m_statName),
	m_lowerCaseStatName(other.m_lowerCaseStatName),
	m_statValue(other.m_statValue),
	m_specialCaseOperation(other.m_specialCaseOperation)
{

}

DialogueRequirementPlayerHasAgentStatValue::~DialogueRequirementPlayerHasAgentStatValue()
{
	DialogueRequirement::~DialogueRequirement();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Setters
void DialogueRequirementPlayerHasAgentStatValue::SetStatName(const std::string& statName)
{
	m_statName = statName;
	m_lowerCaseStatName = MakeLower(statName);
}

void DialogueRequirementPlayerHasAgentStatValue::SetStatValue(int statValue)
{
	m_statValue = statValue;
}

void DialogueRequirementPlayerHasAgentStatValue::SetSpecialCaseOperation(SpecialCaseOperations operation)
{
	m_specialCaseOperation = operation;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Getters
const std::string& DialogueRequirementPlayerHasAgentStatValue::GetStatName() const
{
	return m_statName;
}

const std::string& DialogueRequirementPlayerHasAgentStatValue::GetLowerCaseStatName() const
{
	return m_lowerCaseStatName;
}

int DialogueRequirementPlayerHasAgentStatValue::GetStatValue() const
{
	return m_statValue;
}

SpecialCaseOperations DialogueRequirementPlayerHasAgentStatValue::GetSpecialCaseOperation() const
{
	return m_specialCaseOperation;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Operations
void DialogueRequirementPlayerHasAgentStatValue::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Requirement Name: Player Has Agent Stat Value\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Stat Name: " + m_statName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value searching for: ";
	GameDebuggingCommons::WriteSpecialCaseOperatorToString(str, m_specialCaseOperation);
	str += std::to_string(m_statValue) + "\n";
}

bool DialogueRequirementPlayerHasAgentStatValue::CheckIfRequirementIsMet() const
{
	const BaseGameEntity* entityPly = BaseGameEntity::GetPointerToPlayer();
	if (entityPly == nullptr)
	{
		return false;
	}
	const Player* player = dynamic_cast<const Player*>(entityPly);
	if (player == nullptr)
	{
		return false;
	}
	const std::vector<AgentStat>* stats = player->GetStatsPtr();
	if (stats == nullptr)
	{
		return false;
	}

	for (size_t statIdx = 0; statIdx < stats->size(); statIdx++)
	{
		const AgentStat& stat = stats->at(statIdx);
		if (stat.GetLowerCaseStatAbreviation() == m_lowerCaseStatName
			|| stat.GetLowerCaseStatName() == m_lowerCaseStatName)
		{
			int value = stat.GetValue();
			bool equalToCase = m_specialCaseOperation == IS_SAME && value == m_statValue;
			bool greaterThanCase = m_specialCaseOperation == IS_GREATER_THAN && value > m_statValue;
			bool lessThanCase = m_specialCaseOperation == IS_LESS_THAN && value < m_statValue;
			bool greaterThanOrEqualToCase = m_specialCaseOperation == IS_GREATER_THAN_OR_EQUAL_TO
				&& value >= m_statValue;
			bool lessThanOrEqualToCase = m_specialCaseOperation == IS_LESS_THAN_OR_EQUAL_TO
				&& value <= m_statValue;
			bool notEqualToCase = m_specialCaseOperation == IS_NOT_EQUAL_TO && value != m_statValue;
			bool combined = equalToCase || greaterThanCase || lessThanCase
				|| greaterThanOrEqualToCase || lessThanOrEqualToCase || notEqualToCase;
			return combined;
		}
	}
	return false;
}

DialogueRequirement* DialogueRequirementPlayerHasAgentStatValue::Clone() const
{
	return new DialogueRequirementPlayerHasAgentStatValue(*this);
}