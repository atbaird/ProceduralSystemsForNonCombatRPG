#include "Game/Dialogue/DialogueGroupings.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/Dialogue/DialogueSegment.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"
#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Constructors
DialogueGroupings::DialogueGroupings(const std::string& groupingName)
{
	SetName(groupingName);
}

DialogueGroupings::DialogueGroupings(const XMLNode& node)
{
	SetName(node.getName());

	ReadXMLChildren(node);
}

DialogueGroupings::DialogueGroupings(const DialogueGroupings& other)
	: m_GroupingName(other.m_GroupingName),
	m_lowerCaseGroupingName(other.m_lowerCaseGroupingName),
	m_allDialogueSegmentNames(other.m_allDialogueSegmentNames)
{
	for (size_t actionIdx = 0; actionIdx < other.m_actionsForWhenDialogueClosed.size(); actionIdx++)
	{
		DialogueTrigger* action = other.m_actionsForWhenDialogueClosed[actionIdx];
		if (action == nullptr)
		{
			continue;
		}
		DialogueTrigger* clone = action->Clone();
		m_actionsForWhenDialogueClosed.push_back(clone);
	}
}

DialogueGroupings::~DialogueGroupings()
{
	for (size_t actionIdx = 0; actionIdx < m_actionsForWhenDialogueClosed.size(); actionIdx++)
	{
		DialogueTrigger* action = m_actionsForWhenDialogueClosed[actionIdx];
		if (action == nullptr)
		{
			continue;
		}
		delete action;
	}
	m_actionsForWhenDialogueClosed.clear();
}

void DialogueGroupings::ReadXMLChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "dialoguesegments"
			|| childName == "segments"
			|| childName == "dialoguenames"
			|| childName == "names")
		{
			ReadXMLForDialogueSegmentNames(child);
		}
		else if (childName == "actions"
			|| childName == "action"
			|| childName == "triggers"
			|| childName == "trigger")
		{
			ReadXMLForDialogueActions(child);
		}
	}
}

void DialogueGroupings::ReadXMLForDialogueSegmentNames(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = child.getName();
		AddDialogueSegmentName(childName);
	}
}

void DialogueGroupings::ReadXMLForDialogueActions(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = child.getName();
		DialogueTrigger* action = DialogueActionRegistrationHelper::CreateDialogueActionByName(childName, child);
		if (action != nullptr)
		{
			m_actionsForWhenDialogueClosed.push_back(action);
		}
	}
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Operations
void DialogueGroupings::TriggerActionsForAfterMemberOfGroupingCloses()
{
	for (size_t actionIdx = 0; actionIdx < m_actionsForWhenDialogueClosed.size(); actionIdx++)
	{
		DialogueTrigger* action = m_actionsForWhenDialogueClosed[actionIdx];
		if (action == nullptr)
		{
			continue;
		}
		action->TriggerAction();
	}
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Setters
void DialogueGroupings::SetName(const std::string& groupingName)
{
	m_GroupingName = groupingName;
	m_lowerCaseGroupingName = MakeLower(groupingName);
}

void DialogueGroupings::AddDialogueSegmentName(const std::string& name)
{
	int idx = GetIdxOfNameIfExists(name);
	if (idx != -1)
	{
		return;
	}
	std::string lowerCase = MakeLower(name);
	m_allDialogueSegmentNames.push_back(lowerCase);
	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Getters
const std::string& DialogueGroupings::GetGroupingName() const
{
	return m_GroupingName;
}

const std::string& DialogueGroupings::GetLowerCaseGroupingName() const
{
	return m_lowerCaseGroupingName;
}

int DialogueGroupings::GetIdxOfNameIfExists(const std::string& name) const
{
	std::string lowerCase = MakeLower(name);
	for (size_t nameIdx = 0; nameIdx < m_allDialogueSegmentNames.size(); nameIdx++)
	{
		const std::string& lowerCaseDialogueName = m_allDialogueSegmentNames[nameIdx];
		if (lowerCase == lowerCaseDialogueName)
		{
			return nameIdx;
		}
	}
	return -1;
}

std::string DialogueGroupings::GetRandomNameFromListingWithListeningForRequirements() const
{
	std::vector<std::string> availableDialogueSegments;
	std::vector<int> dialogueSegmentIdxs;
	PullOutAllAvailableDialogueSegmentsBasedOnRequirementsMet(availableDialogueSegments, dialogueSegmentIdxs);
	if (availableDialogueSegments.size() == 0)
	{
		return "";
	}
	int randIdx = GetRandomIntLessThan(availableDialogueSegments.size());
	std::string out_dialogueSegmentName = availableDialogueSegments[randIdx];//m_lastDialogueSegmentIdxUsed
	m_lastDialogueSegmentIdxUsed = dialogueSegmentIdxs[randIdx];
	return out_dialogueSegmentName;
}

std::string DialogueGroupings::GetRandomNameFromListingWithoutListeningForRequirements() const
{
	if (m_allDialogueSegmentNames.size() == 0)
	{
		return "";
	}
	int randIdx = GetRandomIntLessThan(m_allDialogueSegmentNames.size());
	m_lastDialogueSegmentIdxUsed = randIdx;
	return m_allDialogueSegmentNames[randIdx];
}

void DialogueGroupings::PullOutAllAvailableDialogueSegmentsBasedOnRequirementsMet(std::vector<std::string>& list,
	std::vector<int>& indexes) const
{
	for (int idx = 0; (size_t)idx < m_allDialogueSegmentNames.size(); idx++)
	{
		if (idx == m_lastDialogueSegmentIdxUsed)
		{
			continue;
		}
		std::string dialogueSegmentName = m_allDialogueSegmentNames[idx];
		DialogueSegment* segment = DialogueSegment::GetDialogueSegmentByName(dialogueSegmentName);
		if (segment == nullptr
			|| segment->DetermineDialogueRequirementsMet() == false)
		{
			continue;
		}
		list.push_back(dialogueSegmentName);
		indexes.push_back(idx);
	}

	if (m_lastDialogueSegmentIdxUsed != -1 && list.size() == 0)
	{
		std::string dialogueSegmentName = m_allDialogueSegmentNames[m_lastDialogueSegmentIdxUsed];
		DialogueSegment* segment = DialogueSegment::GetDialogueSegmentByName(dialogueSegmentName);
		if (segment != nullptr
			&& segment->DetermineDialogueRequirementsMet() == true)
		{
			list.push_back(dialogueSegmentName);
			indexes.push_back(m_lastDialogueSegmentIdxUsed);
		}
	}
}

void DialogueGroupings::WriteDialogueGroupToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Group Name: " + m_GroupingName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Last Dialogue Segment Ran Idx: " + std::to_string(m_lastDialogueSegmentIdxUsed) + "\n";

	int nextLevel = indentationAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Segments in Group: \n";
	for (size_t i = 0; i < m_allDialogueSegmentNames.size(); i++)
	{
		GameDebuggingCommons::AddIndentation(str,nextLevel);
		str += m_allDialogueSegmentNames[i] + "\n";
	}

	if (m_actionsForWhenDialogueClosed.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Dialogue Triggers for when a member of this group closes: \n";
		for (size_t i = 0; i < m_actionsForWhenDialogueClosed.size(); i++)
		{
			const DialogueTrigger* trigger = m_actionsForWhenDialogueClosed[i];
			if (trigger != nullptr)
			{
				trigger->WriteDialogueTriggerToString(str, nextLevel);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL TRIGGER\n";
			}
		}
	}
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Static Functions
STATIC const std::string DialogueGroupings::GetDialogueSegmentNameWithoutNumber(const std::string& name)
{
	int idxOfPositionAtEndThatsNotANumber = -1;
	for (int idx = name.size() - 1; idx >= 0; idx--)
	{
		unsigned char car = name[idx];
		if (car == '0'
			|| car == '1'
			|| car == '2'
			|| car == '3'
			|| car == '4'
			|| car == '5'
			|| car == '6'
			|| car == '7'
			|| car == '8'
			|| car == '9'
			|| car == '#')
		{
			continue;
		}
		idxOfPositionAtEndThatsNotANumber = idx;
		break;
	}

	if (idxOfPositionAtEndThatsNotANumber == -1)
	{
		return "";
	}
	return name.substr(0, idxOfPositionAtEndThatsNotANumber + 1);
}