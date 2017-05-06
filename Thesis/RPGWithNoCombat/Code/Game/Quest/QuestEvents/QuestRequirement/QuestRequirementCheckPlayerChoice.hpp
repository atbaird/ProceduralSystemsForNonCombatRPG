#pragma once

#ifndef QUESTREQUIREMENTCHECKPLAYERCHOICE_HPP
#define QUESTREQUIREMENTCHECKPLAYERCHOICE_HPP
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckPlayerStats/AgentStatsCheckingStruct.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* CheckPlayerChoiceCreationFunc(const XMLNode& node);
QuestRequirement* CheckPlayerChoiceDefaultCreationFunc();

class QuestRequirementCheckPlayerChoice : public QuestRequirement
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_CheckPlayerStatsRegistration;

	//Variables
	std::string m_DialogueSegment = "";
	std::string m_ChoiceSelected = "";
public:

	//Constructors
	QuestRequirementCheckPlayerChoice();
	QuestRequirementCheckPlayerChoice(const XMLNode& node);
	QuestRequirementCheckPlayerChoice(const QuestRequirementCheckPlayerChoice& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckPlayerChoice();

	//Setters
	void SetDialogueSegmentName(const std::string& name);
	void SetChoiceSelected(const std::string& name);

	//Getters
	const std::string& GetDialogueSegmentName() const;
	const std::string& GetChoiceSelected() const;

	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest = nullptr) const override;
};

#endif