#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckPlayerInteraction.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Creation Funcs
QuestRequirement* CheckPlayerInteractionStatsCreationFunc(const XMLNode& node)
{
	QuestRequirementCheckPlayerInteraction* interaction = new QuestRequirementCheckPlayerInteraction(node);
	return interaction;
}

QuestRequirement* CheckPlayerInteractionStatsDefaultCreationFunc()
{
	QuestRequirementCheckPlayerInteraction* interaction = new QuestRequirementCheckPlayerInteraction();
	return interaction;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
STATIC QuestRequirementRegistration 
	QuestRequirementCheckPlayerInteraction::s_CheckInteractionRegistration 
	= QuestRequirementRegistration("CheckPlayerInteraction", 
		CheckPlayerInteractionStatsCreationFunc, CheckPlayerInteractionStatsDefaultCreationFunc);

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Constructors
QuestRequirementCheckPlayerInteraction::QuestRequirementCheckPlayerInteraction()
	: QuestRequirement(Player_Interacting_With)
{

}

QuestRequirementCheckPlayerInteraction::QuestRequirementCheckPlayerInteraction(const XMLNode& node)
	: QuestRequirement(Player_Interacting_With, node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "interact")
		{
			SetVariableName(attr.lpszValue);
		}
	}
}

QuestRequirementCheckPlayerInteraction::QuestRequirementCheckPlayerInteraction(const QuestRequirementCheckPlayerInteraction& other,
	Quest* owningQuest)
	: QuestRequirement(other, owningQuest),
	m_VariableName(other.m_VariableName),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName)
{

}

QuestRequirementCheckPlayerInteraction::~QuestRequirementCheckPlayerInteraction()
{
	QuestRequirement::~QuestRequirement();
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Setters
void QuestRequirementCheckPlayerInteraction::SetVariableName(const std::string& variableName)
{
	m_VariableName = variableName;
	m_lowerCaseVariableName = MakeLower(variableName);
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Getters
const std::string& QuestRequirementCheckPlayerInteraction::GetVariableName() const
{
	return m_VariableName;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Operations
void QuestRequirementCheckPlayerInteraction::WriteQuestRequirementToString(std::string& str, 
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Check Player Interaction\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable player should be interacting with: " + m_VariableName + "\n";
}

const bool QuestRequirementCheckPlayerInteraction::DeterminePlayerMeetsQuestRequirement() const
{
	if (m_owningQuest == nullptr)
	{
		return false;
	}
	const BaseGameEntity* player = BaseGameEntity::GetPointerToPlayer();
	BaseGameEntity* entity = nullptr;
	m_owningQuest->GetVariable<BaseGameEntity*>(m_lowerCaseVariableName, entity, nullptr);
	if (entity == nullptr
		|| player == nullptr)
	{
		return false;
	}
	const Player* playerPtr = dynamic_cast<const Player*>(player);
	if (playerPtr == nullptr)
	{
		return false;
	}
	const BaseGameEntity* entityInteractingWith = playerPtr->GetEntityLastInteractedWith();
	if (entityInteractingWith != nullptr 
		&& (entityInteractingWith == entity))
	{
		return true;
	}
	 
	return false;
}

QuestRequirement* QuestRequirementCheckPlayerInteraction::Clone(Quest* owningQuest) const
{
	QuestRequirementCheckPlayerInteraction* clone = new QuestRequirementCheckPlayerInteraction(*this, owningQuest);
	return clone;
}