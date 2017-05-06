#pragma once

#ifndef QUESTTRIGGERSETPLAYERSTAT_HPP
#define QUESTTRIGGERSETPLAYERSTAT_HPP
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerWhatToDoWithStatOptions.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerSetPlayerStatFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultSetPlayerStatFunc();

class QuestTriggerSetPlayerStat : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_SetPlayerStatRegistration;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	int m_value = 0;
	QuestTriggerWhatToDoWithStatOptions m_StatOption = INCREMENT;
	bool m_variableNameIsAbbreviation = false;
public:
	//Constructors
	QuestTriggerSetPlayerStat();
	QuestTriggerSetPlayerStat(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerSetPlayerStat(const QuestTriggerSetPlayerStat& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerSetPlayerStat();

	//Setters
	void SetValue(int val);
	void SetVariableName(const std::string variableName, bool variableIsAbbreviation = false);
	void SetWhatToDoWithStatOptions(QuestTriggerWhatToDoWithStatOptions statOption);

	//Getters
	const std::string& GetVariableName() const;
	int GetValue() const;
	QuestTriggerWhatToDoWithStatOptions GetWhatToDoWithStat() const;
	bool GetVariableNameIsAbbreviation() const;

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};

#endif