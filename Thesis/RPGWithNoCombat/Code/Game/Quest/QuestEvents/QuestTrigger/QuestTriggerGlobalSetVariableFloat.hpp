#pragma once

#ifndef QUESTTRIGGERGLOBALSETVARIABLEFLOAT_HPP
#define QUESTTRIGGERGLOBALSETVARIABLEFLOAT_HPP
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerWhatToDoWithStatOptions.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerGlobalSetVariableFloatFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultGlobalSetVariableFloatFunc();

class QuestTriggerGlobalSetVariableFloat : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_QuestTriggerGlobalSetVariableFloatRegistration;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	QuestTriggerWhatToDoWithStatOptions m_StatOption = SET;
	float m_value = 0.f;
public:
	//Constructors
	QuestTriggerGlobalSetVariableFloat();
	QuestTriggerGlobalSetVariableFloat(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerGlobalSetVariableFloat(const QuestTriggerGlobalSetVariableFloat& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerGlobalSetVariableFloat();

	//Setters
	void SetValue(float val);
	void SetVariableName(const std::string variableName);
	void SetWhatToDoWithStatOptions(QuestTriggerWhatToDoWithStatOptions statOption);


	//Getters
	const std::string& GetVariableName() const;
	const std::string& GetLowerCaseVariableName() const;
	float GetValue() const;
	QuestTriggerWhatToDoWithStatOptions GetWhatToDoWithStat() const;

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};
#endif