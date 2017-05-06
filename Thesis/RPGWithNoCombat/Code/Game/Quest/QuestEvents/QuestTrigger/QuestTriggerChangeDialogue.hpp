#pragma once

#ifndef QUESTTRIGGERCHANGEDDIALOGUE_HPP
#define QUESTTRIGGERCHANGEDDIALOGUE_HPP
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include <string>

//Registration Funcs
QuestTrigger* QuestTriggerChangeDialogueFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultChangeDialogueFunc();

class Quest;
class QuestTriggerChangedDialogue : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_ChangeDialogueRegistration;

	//Variables
	std::string m_EntityVariableName = "";
	std::string m_DialogueToChangeTo = "";
public:
	//Constructors
	QuestTriggerChangedDialogue();
	QuestTriggerChangedDialogue(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerChangedDialogue(const QuestTriggerChangedDialogue& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerChangedDialogue();

	//Setters
	void SetEntityVariableName(const std::string& name);
	void SetDialogueToChangeTo(const std::string& dialogue);

	//Getters
	const std::string& GetEntityVariableName() const;
	const std::string& GetDialogueToChangeTo() const;

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};
#endif