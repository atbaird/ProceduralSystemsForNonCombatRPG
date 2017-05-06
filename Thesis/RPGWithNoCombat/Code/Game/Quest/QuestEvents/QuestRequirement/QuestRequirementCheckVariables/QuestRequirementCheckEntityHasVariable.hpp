#pragma once

#ifndef QUESTREQUIREMENTCHECKENTITYHASVARIABLE_HPP
#define QUESTREQUIREMENTCHECKENTITYHASVARIABLE_HPP
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariableBase.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* CheckEntityHasVariableCreationFunc(const XMLNode& node);
QuestRequirement* CheckEntityHasVariableDefaultCreationFunc();

class QuestRequirementCheckEntityHasVariable : public QuestRequirementCheckVariableBase
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_CheckEntityHasRegistration;

	//Variables
	std::string m_entityName = "player";
	std::string m_itemName = "";
	float m_amountToHave = 0.0f;
public:
	//Constructors
	QuestRequirementCheckEntityHasVariable();
	QuestRequirementCheckEntityHasVariable(const XMLNode& node);
	QuestRequirementCheckEntityHasVariable(const QuestRequirementCheckEntityHasVariable& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckEntityHasVariable();

	//Setters
	void SetEntityName(const std::string& entityName);
	void SetItemName(const std::string& itemName);
	void SetAmountToHave(float amtToHave);

	//Getters
	const std::string& GetEntityName() const;
	const std::string& GetItemName() const;
	float GetAmountToHave() const;

	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest) const override;
};
#endif