#pragma once

#ifndef QUESTTRIGGERSETQUESTVARIABLEFLOAT_HPP
#define QUESTTRIGGERSETQUESTVARIABLEFLOAT_HPP
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerWhatToDoWithStatOptions.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerSetQuestStatFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultSetQuestStatFunc();

class QuestTriggerSetQuestVariableFloat : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_QuestTriggerSetQuestStatRegistration;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	float m_value = 0.f;
	QuestTriggerWhatToDoWithStatOptions m_StatOption = SET;
public:
	//Constructors
	QuestTriggerSetQuestVariableFloat();
	QuestTriggerSetQuestVariableFloat(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerSetQuestVariableFloat(const QuestTriggerSetQuestVariableFloat& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerSetQuestVariableFloat();

	//Setters
	void SetValue(float val);
	void SetVariableName(const std::string& variableName);
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