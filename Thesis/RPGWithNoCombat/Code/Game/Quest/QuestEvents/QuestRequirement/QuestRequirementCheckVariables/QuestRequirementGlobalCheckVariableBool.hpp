#pragma once

#ifndef QUESTREQUIREMENTGLOBALCHECKVARIABLEBOOL_HPP
#define QUESTREQUIREMENTGLOBALCHECKVARIABLEBOOL_HPP
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariableBase.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* GlobalCheckVariableBoolCreationFunc(const XMLNode& node);
QuestRequirement* GlobalCheckVariableBoolDefaultCreationFunc();

class QuestRequirementGlobalCheckVariableBool : public QuestRequirementCheckVariableBase
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_GlobalCheckVariableBoolRegistration;

	//Variables
	bool m_value = true;
public:
	//Constructors
	QuestRequirementGlobalCheckVariableBool();
	QuestRequirementGlobalCheckVariableBool(const XMLNode& node);
	QuestRequirementGlobalCheckVariableBool(const QuestRequirementGlobalCheckVariableBool& other, 
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementGlobalCheckVariableBool();

	//Setters
	void SetVariable(bool var);

	//Getters
	bool GetVariable() const;

	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest) const override;
};

#endif