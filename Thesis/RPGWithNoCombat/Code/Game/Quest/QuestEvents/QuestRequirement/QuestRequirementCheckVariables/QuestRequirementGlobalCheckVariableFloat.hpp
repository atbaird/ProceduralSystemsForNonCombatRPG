#pragma once

#ifndef QUESTREQUIREMENTGLOBALCHECKVARIABLEFLOAT_HPP
#define QUESTREQUIREMENTGLOBALCHECKVARIABLEFLOAT_HPP
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariableBase.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* GlobalCheckVariableFloatCreationFunc(const XMLNode& node);
QuestRequirement* GlobalCheckVariableFloatDefaultCreationFunc();

class QuestRequirementGlobalCheckVariableFloat : public QuestRequirementCheckVariableBase
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_GlobalCheckVariableFloatRegistration;

	//variables
	float m_value = 0.f;
public:
	//Constructors
	QuestRequirementGlobalCheckVariableFloat();
	QuestRequirementGlobalCheckVariableFloat(const XMLNode& node);
	QuestRequirementGlobalCheckVariableFloat(const QuestRequirementGlobalCheckVariableFloat& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementGlobalCheckVariableFloat();

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