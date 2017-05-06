#pragma once

#ifndef QUESTEVENT_HPP
#define QUESTEVENT_HPP
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include <vector>

struct XMLNode;
class Quest;
class QuestEvent
{
private:
protected:
	std::string m_QuestEventName = "";
	Quest* m_owningQuest = nullptr;
	std::vector<QuestRequirement*> m_questRequirementsForEventToRun;
	std::vector<QuestTrigger*> m_questTriggers;
	std::vector<QuestTrigger*> m_postInteractQuestTriggers;
public:
	//Constructors
	QuestEvent();
	QuestEvent(const XMLNode& node);
	QuestEvent(const QuestEvent& other, Quest* owningQuest = nullptr);
	virtual ~QuestEvent();

	void ClearData();

	void ReadXMLNodeQuestEvent(const XMLNode& node, bool clearData = true);
protected:
	void ReadChildrenForRequirements(const XMLNode& node);
	void ReadChildrenForTriggers(const XMLNode& node);
	void ReadChildrenForPostTriggers(const XMLNode& node);

public:
	//Setters
	void AddQuestRequirement(QuestRequirement* requirement);
	void SetOwningQuest(Quest* quest);

	//Getters
	const std::vector<QuestRequirement*>& GetQuestRequirementsForEventToRun() const;
	const std::vector<QuestRequirement*>* GetQuestRequirementsForEventToRunPtr() const;
	std::vector<QuestRequirement*>* GetQuestRequirementsForEventToRunEditablePtr();
	bool TestIfMatchesQuestEventsRequirements() const;

	//Operations
	void TriggerEvent();
	void PostTriggerEvent();
	void WriteQuestEventToString(std::string& str, int indentationAmt) const;
};
#endif