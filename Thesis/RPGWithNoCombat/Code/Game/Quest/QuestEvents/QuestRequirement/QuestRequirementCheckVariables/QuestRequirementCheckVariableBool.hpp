#pragma once

#ifndef QUESTREQUIREMENTCHECKVARIABLEBASEGAMEENTITY_HPP
#define QUESTREQUIREMENTCHECKVARIABLEBASEGAMEENTITY_HPP
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariableBase.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* CheckVariableBoolCreationFunc(const XMLNode& node);
QuestRequirement* CheckVariableBoolDefaultCreationFunc();

class QuestRequirementCheckVariableBool : public QuestRequirementCheckVariableBase
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_CheckVariableBoolRegistration;

	//Variables
	bool m_value = true;
public:
	//Constructors
	QuestRequirementCheckVariableBool();
	QuestRequirementCheckVariableBool(const XMLNode& node);
	QuestRequirementCheckVariableBool(const QuestRequirementCheckVariableBool& other, Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckVariableBool();

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