#pragma once

#ifndef QUESTTRIGGER_HPP
#define QUESTTRIGGER_HPP
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

class Quest;
class QuestTrigger
{
private:
protected:
	std::string m_TriggerName = "";
	Quest* m_owningQuest = nullptr;
public:
	//Constructors
	QuestTrigger();
	QuestTrigger(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTrigger(const QuestTrigger& other, Quest* owningQuest = nullptr);
	virtual ~QuestTrigger();

	//Setters
	void SetOwningQuest(Quest* quest);

	//Getters
	const Quest* GetOwningQuest() const;
	Quest* GetEditableOwningQuest();

	//Operations
	virtual bool PerformQuestTrigger();
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const = 0;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const = 0;
};
#endif