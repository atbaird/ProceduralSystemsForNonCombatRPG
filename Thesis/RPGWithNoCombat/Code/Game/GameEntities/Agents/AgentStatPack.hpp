#pragma once

#ifndef AGENTSTATPACK_HPP
#define AGENTSTATPACK_HPP
#include "Game/GameEntities/Agents/AgentTypes.hpp"
#include "Game/GameEntities/Agents/AgentStats/AgentStat.hpp"

struct AgentStatPack
{
private:
protected:
	//static variables
	static constexpr int s_MaxGeneralStatScore = 100;

	//Variables
	float m_trueMovementSpeed = 1.f;
	float m_currentMovementSpeed = 1.f;
	float m_regularMovementSpeed = 1.f;
	float m_SprintingMovementSpeed = 2.f;
	AgentTypes m_agentType = INVALID_AGENTTYPE;
	std::vector<AgentStat> m_AllStats;
	bool m_male = true;
public:

	//Constructors
	AgentStatPack(AgentTypes type = INVALID_AGENTTYPE);
	virtual ~AgentStatPack();

	//Setters
	void SetTrueMovementSpeed(float speed, bool setCurrentSpeedToTrue = true);
	void SetCurrentMovementSpeed(float speed);
	void SetSprintingMovementSpeed(float speed);
	void ResetCurrentMovementSpeed();
	void ResetCurrentMovementSpeedSprinting();
	void RandomizeAllStats();
	void RestoreMaxAndMinStatValuesToGlobalDefaults();
	void ClearAndCopyAllStatsFromCommonStats();
	void SetOrAddAgentStat(const AgentStat& stat);
	void SetOrAddAgentStat(const std::string& statName, int value);
	void SetOrAddAgentStatAbbreviation(const std::string& statAbbreviation, int value);
	void SetAgentType(AgentTypes type);
	void SetIsMale(bool male);

	//Getters
	float GetTrueMovementSpeed() const;
	float GetCurrentMovementSpeed() const;
	int GetStatValueByStatName(const std::string& name, bool& found) const;
	int GetStatValueByStatAbbreviation(const std::string& abbreviation, bool& found) const;
	int GetStatValueByStatName(const std::string& name) const;
	int GetStatValueByStatAbbreviation(const std::string& abbreviation) const;
	AgentTypes GetAgentType() const;
	bool GetIsMale() const;
	const std::vector<AgentStat>& GetAllStats() const;
	const std::vector<AgentStat>* GetAllStatsPtr() const;
	std::vector<AgentStat>* GetAllStatsEditablePtr();
};

#endif