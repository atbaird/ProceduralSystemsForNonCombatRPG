#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckCurrentDialogueSegment.hpp"
#include "Game/Dialogue/Dialogue.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Dialogue/DialogueSegment.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Creation Funcs
QuestRequirement* CheckCurrentDialogueSegmentCreationFunc(const XMLNode& node)
{
	return new QuestRequirementCheckCurrentDialogueSegment(node);
}

QuestRequirement* CheckCurrentDialogueSegmentDefaultCreationFunc()
{
	return new QuestRequirementCheckCurrentDialogueSegment();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Static Variables
STATIC QuestRequirementRegistration
	QuestRequirementCheckCurrentDialogueSegment::s_CheckPlayerStatsRegistration
		= QuestRequirementRegistration("CheckCurrentDialogueSegment", 
			CheckCurrentDialogueSegmentCreationFunc, CheckCurrentDialogueSegmentDefaultCreationFunc);

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Constructors
QuestRequirementCheckCurrentDialogueSegment::QuestRequirementCheckCurrentDialogueSegment()
	: QuestRequirement(Current_Dialogue_Segment_Is)
{

}

QuestRequirementCheckCurrentDialogueSegment::QuestRequirementCheckCurrentDialogueSegment(
	const XMLNode& node)
	: QuestRequirement(Current_Dialogue_Segment_Is, node)
{
	for (int i = 0; i < node.nAttribute(); i++)
	{
		XMLAttribute attr = node.getAttribute(i);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "dialoguesegment"
			|| attrName == "segment")
		{
			m_wantedDialogueSegmentName = MakeLower(attr.lpszValue);
		}
	}
}

QuestRequirementCheckCurrentDialogueSegment::QuestRequirementCheckCurrentDialogueSegment(
	const QuestRequirementCheckCurrentDialogueSegment& other,
	Quest* owningQuest)
	: QuestRequirement(other, owningQuest),
	m_wantedDialogueSegmentName(other.m_wantedDialogueSegmentName)
{

}
QuestRequirementCheckCurrentDialogueSegment::~QuestRequirementCheckCurrentDialogueSegment()
{
	QuestRequirement::~QuestRequirement();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Operations
void QuestRequirementCheckCurrentDialogueSegment::WriteQuestRequirementToString(std::string& str,
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Check Current Dialogue Segment\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Wanted Dialogue Segment Name: " + m_wantedDialogueSegmentName + "\n";
}

const bool QuestRequirementCheckCurrentDialogueSegment::DeterminePlayerMeetsQuestRequirement() const
{
	Dialogue* dialogue = Dialogue::SpawnOrGetCentralDialogue();
	if (dialogue == nullptr)
	{
		return false;
	}
	DialogueSegment* dialogueSegment = dialogue->GetCurrentDialogueSegment();
	if (dialogueSegment == nullptr)
	{
		return false;
	}
	return MakeLower(dialogueSegment->GetDialogueSegmentName()) == m_wantedDialogueSegmentName;
}

QuestRequirement* QuestRequirementCheckCurrentDialogueSegment::Clone(Quest* owningQuest) const
{
	return new QuestRequirementCheckCurrentDialogueSegment(*this, owningQuest);
}