#pragma once

#ifndef QUESTTRIGGERSETQUESTVARIABLEBOOL_HPP
#define QUESTTRIGGERSETQUESTVARIABLEBOOL_HPP
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerWhatToDoWithStatOptions.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerSetQuestVariableBoolFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultSetQuestVariableBoolFunc();

class QuestTriggerSetQuestVariableBool : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_QuestTriggerSetQuestStatRegistration;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	bool m_value = false;
public:
	//Constructors
	QuestTriggerSetQuestVariableBool();
	QuestTriggerSetQuestVariableBool(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerSetQuestVariableBool(const QuestTriggerSetQuestVariableBool& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerSetQuestVariableBool();

	//Setters
	void SetValue(bool val);
	void SetVariableName(const std::string& variableName);


	//Getters
	const std::string& GetVariableName() const;
	const std::string& GetLowerCaseVariableName() const;
	bool GetValue() const;

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};

#endif