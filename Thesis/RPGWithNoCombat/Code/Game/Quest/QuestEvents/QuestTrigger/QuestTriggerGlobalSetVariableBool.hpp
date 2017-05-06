#pragma once

#ifndef QUESTTRIGGERGLOBALSETVARIABLEBOOL_HPP
#define QUESTTRIGGERGLOBALSETVARIABLEBOOL_HPP
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerWhatToDoWithStatOptions.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerGlobalSetVariableBoolFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultGlobalSetVariableBoolFunc();


class QuestTriggerGlobalSetVariableBool : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_QuestTriggerGlobalSetVariableBoolRegistration;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	bool m_value = false;
public:
	//Constructors
	QuestTriggerGlobalSetVariableBool();
	QuestTriggerGlobalSetVariableBool(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerGlobalSetVariableBool(const QuestTriggerGlobalSetVariableBool& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerGlobalSetVariableBool();

	//Setters
	void SetValue(bool val);
	void SetVariableName(const std::string variableName);


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