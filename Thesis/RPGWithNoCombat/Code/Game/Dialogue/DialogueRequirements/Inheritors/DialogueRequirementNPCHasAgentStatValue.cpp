#include "Game/Dialogue/DialogueRequirements/Inheritors/DialogueRequirementNPCHasAgentStatValue.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Spawning Functions
DialogueRequirement* DialogueReqNPCHasAgentStatValueCreationFunc(const XMLNode& node)
{
	return new DialogueRequirementNPCHasAgentStatValue(node);
}

DialogueRequirement* DialogueReqNPCHasAgentStatValueDefaultCreationFunc()
{
	return new DialogueRequirementNPCHasAgentStatValue();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Static Variables
STATIC const DialogueRequirementRegistrationHelper
	DialogueRequirementNPCHasAgentStatValue::s_NPCHasAgentStatValueRegHelper
		= DialogueRequirementRegistrationHelper("NPCHasAgentStatValue", 
			DialogueReqNPCHasAgentStatValueCreationFunc,
			DialogueReqNPCHasAgentStatValueDefaultCreationFunc);

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Constructors
DialogueRequirementNPCHasAgentStatValue::DialogueRequirementNPCHasAgentStatValue()
	: DialogueRequirement()
{

}

DialogueRequirementNPCHasAgentStatValue::DialogueRequirementNPCHasAgentStatValue(const XMLNode& node)
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

DialogueRequirementNPCHasAgentStatValue::DialogueRequirementNPCHasAgentStatValue(
	const DialogueRequirementNPCHasAgentStatValue& other)
	: DialogueRequirement(other),
	m_statName(other.m_statName),
	m_lowerCaseStatName(other.m_lowerCaseStatName),
	m_statValue(other.m_statValue),
	m_specialCaseOperation(other.m_specialCaseOperation)
{

}

DialogueRequirementNPCHasAgentStatValue::~DialogueRequirementNPCHasAgentStatValue()
{
	DialogueRequirement::~DialogueRequirement();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Setters
void DialogueRequirementNPCHasAgentStatValue::SetStatName(const std::string& statName)
{
	m_statName = statName;
	m_lowerCaseStatName = MakeLower(statName);
}

void DialogueRequirementNPCHasAgentStatValue::SetStatValue(int statValue)
{
	m_statValue = statValue;
}

void DialogueRequirementNPCHasAgentStatValue::SetSpecialCaseOperation(SpecialCaseOperations operation)
{
	m_specialCaseOperation = operation;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Getters
const std::string& DialogueRequirementNPCHasAgentStatValue::GetStatName() const
{
	return m_statName;
}

const std::string& DialogueRequirementNPCHasAgentStatValue::GetLowerCaseStatName() const
{
	return m_lowerCaseStatName;
}

int DialogueRequirementNPCHasAgentStatValue::GetStatValue() const
{
	return m_statValue;
}

SpecialCaseOperations DialogueRequirementNPCHasAgentStatValue::GetSpecialCaseOperation() const
{
	return m_specialCaseOperation;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Operations
void DialogueRequirementNPCHasAgentStatValue::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Requirement Name: NPC Has Agent Stat Value\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Stat Name: " + m_statName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value searching for: ";
	GameDebuggingCommons::WriteSpecialCaseOperatorToString(str, m_specialCaseOperation);
	str += std::to_string(m_statValue) + "\n";

}

bool DialogueRequirementNPCHasAgentStatValue::CheckIfRequirementIsMet() const
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
	Agent* agent = player->GetTalkingTo();
	if (agent == nullptr)
	{
		return false;
	}
	NPC* npc = dynamic_cast<NPC*>(agent);
	if (npc == nullptr)
	{
		return false;
	}
	const std::vector<AgentStat>* stats = npc->GetStatsPtr();
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

			switch (m_specialCaseOperation)
			{
			case IS_SAME:						return value == m_statValue;
			case IS_LESS_THAN:					return value < m_statValue;
			case IS_GREATER_THAN:				return value > m_statValue;
			case IS_GREATER_THAN_OR_EQUAL_TO:	return value >= m_statValue;
			case IS_LESS_THAN_OR_EQUAL_TO:		return value <= m_statValue;
			case IS_NOT_EQUAL_TO:				return value != m_statValue;
			default:							return false;
			}
		}
	}
	return false;
}

DialogueRequirement* DialogueRequirementNPCHasAgentStatValue::Clone() const
{
	return new DialogueRequirementNPCHasAgentStatValue(*this);
}