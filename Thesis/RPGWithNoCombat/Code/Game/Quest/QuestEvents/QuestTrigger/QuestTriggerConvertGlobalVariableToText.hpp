#pragma once

#ifndef QUESTTRIGGERCONVERTGLOBALVARIABLETOLOCALTEXT_HPP
#define QUESTTRIGGERCONVERTGLOBALVARIABLETOLOCALTEXT_HPP

#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerConvertGlobalVaraibleToTextFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultConvertGlobalVaraibleToTextFunc();

class QuestTriggerConvertGlobalVariableToText : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_ConvertGlobalVariableToLocalTextRegistration;

	//Variables
	std::string m_TextVariableName = "";
	std::string m_VariableNameConverting = "";
	int m_IfFloatThenUseThisDecimalLimit = 2;
	bool m_IsGlobalText = false;
public:
	//Constructors
	QuestTriggerConvertGlobalVariableToText();
	QuestTriggerConvertGlobalVariableToText(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerConvertGlobalVariableToText(const QuestTriggerConvertGlobalVariableToText& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestTriggerConvertGlobalVariableToText();

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