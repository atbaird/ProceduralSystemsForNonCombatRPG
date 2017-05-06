#include "Game/Quest/QuestEvent.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementCheckVariableType.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
//Constructors
QuestEvent::QuestEvent()
{

}

QuestEvent::QuestEvent(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "eventname")
		{
			m_QuestEventName = std::string(attr.lpszValue);
		}
	}

	ReadXMLNodeQuestEvent(node, false);
}

QuestEvent::QuestEvent(const QuestEvent& other, Quest* owningQuest)
	: m_owningQuest(owningQuest),
	m_QuestEventName(other.m_QuestEventName)
{
	for (size_t otherRequirementsIdx = 0; otherRequirementsIdx
		< other.m_questRequirementsForEventToRun.size(); otherRequirementsIdx++)
	{
		QuestRequirement* requirements = other.m_questRequirementsForEventToRun[otherRequirementsIdx];
		if (requirements == nullptr)
		{
			continue;
		}
		QuestRequirement* copy = requirements->Clone(owningQuest);
		copy->SetQuestOwnedBy(m_owningQuest);
		m_questRequirementsForEventToRun.push_back(copy);
	}

	for (size_t otherTriggerIdx = 0; otherTriggerIdx < other.m_questTriggers.size();
		otherTriggerIdx++)
	{
		QuestTrigger* trigger = other.m_questTriggers[otherTriggerIdx];
		if (trigger == nullptr)
		{
			continue;
		}
		QuestTrigger* clone = trigger->Clone(owningQuest);
		clone->SetOwningQuest(m_owningQuest);
		m_questTriggers.push_back(clone);
	}

	for (size_t otherTriggerIdx = 0; otherTriggerIdx < other.m_postInteractQuestTriggers.size(); otherTriggerIdx++)
	{
		QuestTrigger* trigger = other.m_postInteractQuestTriggers[otherTriggerIdx];
		if (trigger == nullptr)
		{
			continue;
		}
		QuestTrigger* clone = trigger->Clone(owningQuest);
		clone->SetOwningQuest(m_owningQuest);
		m_postInteractQuestTriggers.push_back(clone);
	}

}

QuestEvent::~QuestEvent()
{
	ClearData();
}

void QuestEvent::ClearData()
{
	for (size_t i = 0; i < m_questRequirementsForEventToRun.size(); i++)
	{
		QuestRequirement* req = m_questRequirementsForEventToRun[i];
		if (req != nullptr)
		{
			delete req;
		}
	}
	m_questRequirementsForEventToRun.clear();
	for (size_t i = 0; i < m_questTriggers.size(); i++)
	{
		QuestTrigger* trigger = m_questTriggers[i];
		if (trigger != nullptr)
		{
			delete trigger;
		}
	}
	m_questTriggers.clear();

	for (size_t i = 0; i < m_postInteractQuestTriggers.size(); i++)
	{
		QuestTrigger* trigger = m_postInteractQuestTriggers[i];
		if (trigger != nullptr)
		{
			delete trigger;
		}
	}
	m_postInteractQuestTriggers.clear();
}

void QuestEvent::ReadXMLNodeQuestEvent(const XMLNode& node, bool clearData)
{
	if (clearData)
	{
		ClearData();
	}
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "requirements"
			|| childName == "requirement")
		{
			ReadChildrenForRequirements(child);
		}
		else if (childName == "trigger"
			|| childName == "triggers")
		{
			ReadChildrenForTriggers(child);
		}
		else if (childName == "posttrigger"
			|| childName == "posttriggers")
		{
			ReadChildrenForPostTriggers(child);
		}
	}
}

void QuestEvent::ReadChildrenForRequirements(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string type = QuestRequirement::GetQuestRequirementTypeAndQuestRequirementCheckVariableType(child);
		QuestRequirement* req
			= QuestRequirementRegistration::CreateQuestRequirementByName(type, child);//QuestRequirementRegistration::CreateQuestRequirementByType(child, reqType, varType);


		if (req != nullptr)
		{
			m_questRequirementsForEventToRun.push_back(req);
		}
	}
}

void QuestEvent::ReadChildrenForTriggers(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = (child.getName());
		QuestTrigger* trigger = QuestTriggerRegistration::CreateQuestTriggerByName(childName, child);
		if (trigger != nullptr)
		{
			m_questTriggers.push_back(trigger);
		}
	}
}

void QuestEvent::ReadChildrenForPostTriggers(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = (child.getName());
		QuestTrigger* trigger = QuestTriggerRegistration::CreateQuestTriggerByName(childName, child);
		if (trigger != nullptr)
		{
			 m_postInteractQuestTriggers.push_back(trigger);
		}
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
//Setters
void QuestEvent::AddQuestRequirement(QuestRequirement* requirement)
{
	if (requirement == nullptr)
	{
		return;
	}
	m_questRequirementsForEventToRun.push_back(requirement);
}

void QuestEvent::SetOwningQuest(Quest* quest)
{
	m_owningQuest = quest;
	for (size_t questReqIdx = 0; questReqIdx < m_questRequirementsForEventToRun.size(); questReqIdx++)
	{
		QuestRequirement* req = m_questRequirementsForEventToRun[questReqIdx];
		if (req != nullptr)
		{
			req->SetQuestOwnedBy(m_owningQuest);
		}
	}

	for (size_t questTriggerIdx = 0; questTriggerIdx < m_questTriggers.size(); questTriggerIdx++)
	{
		QuestTrigger* trigger = m_questTriggers[questTriggerIdx];
		if (trigger != nullptr)
		{
			trigger->SetOwningQuest(quest);
		}
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
//Getters
const std::vector<QuestRequirement*>& QuestEvent::GetQuestRequirementsForEventToRun() const
{
	return m_questRequirementsForEventToRun;
}

const std::vector<QuestRequirement*>* QuestEvent::GetQuestRequirementsForEventToRunPtr() const
{
	return &m_questRequirementsForEventToRun;
}

std::vector<QuestRequirement*>* QuestEvent::GetQuestRequirementsForEventToRunEditablePtr()
{
	return &m_questRequirementsForEventToRun;
}

bool QuestEvent::TestIfMatchesQuestEventsRequirements() const
{
	for (size_t i = 0; i < m_questRequirementsForEventToRun.size(); i++)
	{
		QuestRequirement* req = m_questRequirementsForEventToRun[i];
		if (req == nullptr)
		{
			continue;
		}
		if (req->DeterminePlayerMeetsQuestRequirement() == false)
		{
			return false;
		}
	}

	return true;
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
//Operations
void QuestEvent::TriggerEvent()
{
	for (size_t triggerIdx = 0; triggerIdx < m_questTriggers.size(); triggerIdx++)
	{
		QuestTrigger* trigger = m_questTriggers[triggerIdx];
		if (trigger == nullptr)
		{
			continue;
		}
		trigger->PerformQuestTrigger();
	}
}

void QuestEvent::PostTriggerEvent()
{
	for (size_t triggerIdx = 0; triggerIdx < m_postInteractQuestTriggers.size(); triggerIdx++)
	{
		QuestTrigger* trigger = m_postInteractQuestTriggers[triggerIdx];
		if (trigger == nullptr)
		{
			continue;
		}
		trigger->PerformQuestTrigger();
	}
}

void QuestEvent::WriteQuestEventToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Event Name: " + m_QuestEventName + "\n";
	indentationAmt += 3;

	int nextLevel = indentationAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Event Requirements: \n";
	for (size_t i = 0; i < m_questRequirementsForEventToRun.size(); i++)
	{
		const QuestRequirement* req = m_questRequirementsForEventToRun[i];
		if (req == nullptr)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel);
			str += "NULL QUEST REQUIREMENT\n";
		}
		else
		{
			req->WriteQuestRequirementToString(str, nextLevel);
		}
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Event Triggers: \n";
	for (size_t i = 0; i < m_questTriggers.size(); i++)
	{
		const QuestTrigger* trigger = m_questTriggers[i];
		if (trigger == nullptr)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel);
			str += "NULL QUEST TRIGGER\n";
		}
		else
		{
			trigger->WriteQuestTriggerToString(str, nextLevel);
		}
	}

	for (size_t i = 0; i < m_postInteractQuestTriggers.size(); i++)
	{
		const QuestTrigger* trigger = m_postInteractQuestTriggers[i];
		if (trigger == nullptr)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel);
			str += "NULL QUEST TRIGGER\n";
		}
		else
		{
			trigger->WriteQuestTriggerToString(str, nextLevel);
		}
	}
}