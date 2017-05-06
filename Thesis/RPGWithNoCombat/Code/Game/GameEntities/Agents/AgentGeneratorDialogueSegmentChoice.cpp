#include "Game/GameEntities/Agents/AgentGeneratorDialogueSegmentChoice.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Constructors
AgentGeneratorDialogueSegmentChoice::AgentGeneratorDialogueSegmentChoice(const std::string& dialogueChoice)
	: m_DialogueSegment(dialogueChoice)
{

}

AgentGeneratorDialogueSegmentChoice::~AgentGeneratorDialogueSegmentChoice()
{

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Setters
void AgentGeneratorDialogueSegmentChoice::AddDialogueSegment(const std::string segment)
{
	m_DialogueSegment = segment;
}

void AgentGeneratorDialogueSegmentChoice::SetNumberOfUsesAllowed(int limit)
{
	m_numberOfUsesLimit = limit;
	m_limitUses = limit > 0;
}

void AgentGeneratorDialogueSegmentChoice::SetNumberOfUsesSoFar(int uses)
{
	m_numberOfUsesSoFar = uses;
}

void AgentGeneratorDialogueSegmentChoice::IncrementNumberOfUsesSoFar() const
{
	m_numberOfUsesSoFar++;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Getters
void AgentGeneratorDialogueSegmentChoice::WriteDialogueSegmentChoiceToString(std::string& str, int indentationLvl) const
{
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Dialogue Sequence or Group Name: " + m_DialogueSegment + "\n";
}

const std::string& AgentGeneratorDialogueSegmentChoice::GetDialogueChoice() const
{
	return m_DialogueSegment;
}

int AgentGeneratorDialogueSegmentChoice::GetNumberOfUsesLimit() const
{
	return m_numberOfUsesLimit;
}

int AgentGeneratorDialogueSegmentChoice::GetNumberOfUsesSoFar() const
{
	return m_numberOfUsesSoFar;
}

bool AgentGeneratorDialogueSegmentChoice::GetLimitUses() const
{
	return m_limitUses;
}

bool AgentGeneratorDialogueSegmentChoice::GetShouldBeAllowedToUseThisChoice() const
{
	if (m_limitUses == false
		|| (m_numberOfUsesSoFar < m_numberOfUsesLimit))
	{
		return true;
	}
	return false;
}