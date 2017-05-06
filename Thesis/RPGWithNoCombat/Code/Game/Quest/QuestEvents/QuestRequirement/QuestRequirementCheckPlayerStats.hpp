#pragma once

#ifndef QUESTREQUIREMENTCHECKPLAYERSTATS_HPP
#define QUESTREQUIREMENTCHECKPLAYERSTATS_HPP
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckPlayerStats/AgentStatsCheckingStruct.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* CheckPlayerStatsCreationFunc(const XMLNode& node);
QuestRequirement* CheckPlayerStatsDefaultCreationFunc();

class QuestRequirementCheckPlayerStats : public QuestRequirement
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_CheckPlayerStatsRegistration;

	//Variables
	std::vector<AgentStatsCheckingStruct> m_statRequirements;
public:
	//Constructors
	QuestRequirementCheckPlayerStats();
	QuestRequirementCheckPlayerStats(const XMLNode& node);
	QuestRequirementCheckPlayerStats(const QuestRequirementCheckPlayerStats& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckPlayerStats();

	//Setters
	void AddStatRequirement(const AgentStatsCheckingStruct& statRequirement);
	void SetStatRequirementIfExists(const std::string& statName, int statVal);

	//Getters
	const std::vector<AgentStatsCheckingStruct>& GetStatRequirements() const;
	const std::vector<AgentStatsCheckingStruct>* GetStatRequirementsPtr() const;
	std::vector<AgentStatsCheckingStruct>* GetStatRequirementsEditablePtr();

	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest = nullptr) const override;
};
#endif