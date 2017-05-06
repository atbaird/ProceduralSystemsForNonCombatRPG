#pragma once

#ifndef QUESTREQUIREMENTCHECKVARIABLEFLOAT_HPP
#define QUESTREQUIREMENTCHECKVARIABLEFLOAT_HPP
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariableBase.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* CheckVariableFloatCreationFunc(const XMLNode& node);
QuestRequirement* CheckVariableFloatDefaultCreationFunc();

class QuestRequirementCheckVariableFloat : public QuestRequirementCheckVariableBase
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_CheckVariableFloatRegistration;

	//variables
	float m_value = 0.f;
public:
	//Constructors
	QuestRequirementCheckVariableFloat();
	QuestRequirementCheckVariableFloat(const XMLNode& node);
	QuestRequirementCheckVariableFloat(const QuestRequirementCheckVariableFloat& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckVariableFloat();

	//Setters
	void SetVariable(float var);

	//Getters
	float GetVariable() const;

	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest = nullptr) const override;

};
#endif