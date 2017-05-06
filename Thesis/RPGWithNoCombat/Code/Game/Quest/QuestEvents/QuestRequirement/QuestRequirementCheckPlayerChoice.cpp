#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckPlayerChoice.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Creation Funcs
QuestRequirement* CheckPlayerChoiceCreationFunc(const XMLNode& node)
{
	QuestRequirementCheckPlayerChoice* req = new QuestRequirementCheckPlayerChoice(node);
	return req;
}

QuestRequirement* CheckPlayerChoiceDefaultCreationFunc()
{
	QuestRequirementCheckPlayerChoice* req = new QuestRequirementCheckPlayerChoice();
	return req;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Static Variables
STATIC QuestRequirementRegistration QuestRequirementCheckPlayerChoice::s_CheckPlayerStatsRegistration
	= QuestRequirementRegistration("CheckPlayerChoice", 
		CheckPlayerChoiceCreationFunc, CheckPlayerChoiceDefaultCreationFunc);

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Constructors
QuestRequirementCheckPlayerChoice::QuestRequirementCheckPlayerChoice()
	: QuestRequirement(Check_Player_Choice)
{

}

QuestRequirementCheckPlayerChoice::QuestRequirementCheckPlayerChoice(const XMLNode& node)
	: QuestRequirement(Check_Player_Choice, node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);

		if (attrName == "segment"
			|| attrName == "dialoguesegment")
		{
			m_DialogueSegment = MakeLower(attr.lpszValue);
		}
		else if (attrName == "choice"
			|| attrName == "dialoguechoice")
		{
			m_ChoiceSelected = MakeLower(attr.lpszValue);
		}
	}
}

QuestRequirementCheckPlayerChoice::QuestRequirementCheckPlayerChoice(
	const QuestRequirementCheckPlayerChoice& other,
	Quest* owningQuest)
	: QuestRequirement(other, owningQuest),
	m_DialogueSegment(other.m_DialogueSegment),
	m_ChoiceSelected(other.m_ChoiceSelected)
{

}

QuestRequirementCheckPlayerChoice::~QuestRequirementCheckPlayerChoice()
{
	QuestRequirement::~QuestRequirement();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Setters
void QuestRequirementCheckPlayerChoice::SetDialogueSegmentName(const std::string& name)
{
	m_DialogueSegment = name;
}

void QuestRequirementCheckPlayerChoice::SetChoiceSelected(const std::string& name)
{
	m_ChoiceSelected = name;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Getters
const std::string& QuestRequirementCheckPlayerChoice::GetDialogueSegmentName() const
{
	return m_DialogueSegment;
}

const std::string& QuestRequirementCheckPlayerChoice::GetChoiceSelected() const
{
	return m_ChoiceSelected;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Operations
void QuestRequirementCheckPlayerChoice::WriteQuestRequirementToString(std::string& str, 
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Check Player Choice\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Desired Dialogue Segment: " + m_DialogueSegment + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Desired Dialogue Choice: " + m_ChoiceSelected + "\n";
}

const bool QuestRequirementCheckPlayerChoice::DeterminePlayerMeetsQuestRequirement() const
{
	const BaseGameEntity* player = BaseGameEntity::GetPointerToPlayer();
	if (player == nullptr)
	{
		return false;
	}
	const Player* agePlayer = dynamic_cast<const Player*>(player);
	if (agePlayer == nullptr)
	{
		return false;
	}

	std::string lowerCaseDialogueSegment = MakeLower(m_DialogueSegment);
	std::string lowerCaseDialogueChoice = MakeLower(m_ChoiceSelected);
	std::string lowerCaseDialogueSegmentPlayer = MakeLower(agePlayer->GetPlayerDialogueChoicePicked());
	std::string lowerCaseDialogueChoicePlayer = MakeLower(agePlayer->GetPlayerChoiceSegmentName());

	if (lowerCaseDialogueChoice == lowerCaseDialogueChoicePlayer
		&& lowerCaseDialogueSegment == lowerCaseDialogueSegmentPlayer)
	{
		return true;
	}

	lowerCaseDialogueSegmentPlayer = MakeLower(agePlayer->GetLastPlayerChoiceSegmentName());
	lowerCaseDialogueChoicePlayer = MakeLower(agePlayer->GetLastPlayerChoiceSegmentName());

	if (lowerCaseDialogueChoice == lowerCaseDialogueChoicePlayer
		&& lowerCaseDialogueSegment == lowerCaseDialogueSegmentPlayer)
	{
		return true;
	}

	return false;
}

QuestRequirement* QuestRequirementCheckPlayerChoice::Clone(Quest* owningQuest) const
{
	QuestRequirementCheckPlayerChoice* clone = new QuestRequirementCheckPlayerChoice(*this, owningQuest);
	return clone;
}