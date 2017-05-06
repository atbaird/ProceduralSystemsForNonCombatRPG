#pragma once

#ifndef QUESTREQUIREMENTINTERACTION_HPP
#define QUESTREQUIREMENTINTERACTION_HPP
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* CheckPlayerInteractionStatsCreationFunc(const XMLNode& node);
QuestRequirement* CheckPlayerInteractionStatsDefaultCreationFunc();

class QuestRequirementCheckPlayerInteraction : public QuestRequirement
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_CheckInteractionRegistration;

	//Variables
	std::string m_VariableName = "";
	std::string m_lowerCaseVariableName = "";
public:
	//Constructors
	QuestRequirementCheckPlayerInteraction();
	QuestRequirementCheckPlayerInteraction(const XMLNode& node);
	QuestRequirementCheckPlayerInteraction(const QuestRequirementCheckPlayerInteraction& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckPlayerInteraction();
	
	//Setters
	void SetVariableName(const std::string& variableName);

	//Getters
	const std::string& GetVariableName() const;

	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest = nullptr) const override;
protected:

};
#endif