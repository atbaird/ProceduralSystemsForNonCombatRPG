#pragma once

#ifndef QUESTTRIGGERCONVERTLOCALVARIABLETOLOCALTEXT_HPP
#define QUESTTRIGGERCONVERTLOCALVARIABLETOLOCALTEXT_HPP

#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerConvertLocalVaraibleToTextFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultConvertLocalVaraibleToTextFunc();

class QuestTriggerConvertLocalVariableToText : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_ConvertLocalVariableToLocalTextRegistration;

	//Variables
	std::string m_TextVariableName = "";
	std::string m_VariableNameConverting = "";
	int m_IfFloatThenUseThisDecimalLimit = 2;
	bool m_IsGlobalText = false;
public:
	//Constructors
	QuestTriggerConvertLocalVariableToText();
	QuestTriggerConvertLocalVariableToText(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerConvertLocalVariableToText(const QuestTriggerConvertLocalVariableToText& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestTriggerConvertLocalVariableToText();

	//Setters
	void SetTextVariableName(const std::string& textVariableName);
	void SetVariableNameConverting(const std::string& variableNameConverting);
	void SetFloatLimit(int limit);
	void SetIsGlobalTextOut(bool globalTextVariable);

	//Getters
	const std::string& GetTextVariableName() const;
	const std::string& GetVariableNameConverting() const;
	int GetFloatDecimalLimit() const;
	bool GetIsGlobalTextVariable() const;

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;

protected:
	void SetStringToVariableName(const std::string& toSet);
	void ParseStringForFloatHandling(std::string& string);
};

#endif