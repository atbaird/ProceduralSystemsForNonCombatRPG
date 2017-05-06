#include "Game/GameEntities/Agents/AgentStatPack.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/StringUtils.hpp"

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//Constructors
AgentStatPack::AgentStatPack(AgentTypes type)
	: m_agentType(type)
{
	m_AllStats = AgentStat::GetCopyOfAllAgentStats();
}

AgentStatPack::~AgentStatPack()
{

}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//Setters
void AgentStatPack::SetTrueMovementSpeed(float speed, bool setCurrentSpeedToTrue)
{
	m_trueMovementSpeed = speed;
	if (setCurrentSpeedToTrue)
	{
		m_currentMovementSpeed = m_trueMovementSpeed;
	}
}

void AgentStatPack::SetCurrentMovementSpeed(float speed)
{
	m_currentMovementSpeed = speed;
}

void AgentStatPack::SetSprintingMovementSpeed(float speed)
{
	m_SprintingMovementSpeed = speed;
}

void AgentStatPack::ResetCurrentMovementSpeed()
{
	m_currentMovementSpeed = m_trueMovementSpeed;
}

void AgentStatPack::ResetCurrentMovementSpeedSprinting()
{
	m_currentMovementSpeed = m_SprintingMovementSpeed;
}

void AgentStatPack::RandomizeAllStats()
{
	for (size_t statIdx = 0; statIdx < m_AllStats.size(); statIdx++)
	{
		AgentStat& stat = m_AllStats[statIdx];
		stat.RandomizeValue();
	}
}

void AgentStatPack::RestoreMaxAndMinStatValuesToGlobalDefaults()
{
	const std::vector<AgentStat>* allAgentStats = AgentStat::GetAllAgentStats();
	if (allAgentStats == nullptr)
	{
		return;
	}

	for (size_t agentStatIdx = 0; agentStatIdx < allAgentStats->size(); agentStatIdx++)
	{
		const AgentStat& stat = allAgentStats->at(agentStatIdx);
		for (size_t myAgentStatIdx = 0; myAgentStatIdx < m_AllStats.size(); myAgentStatIdx++)
		{
			AgentStat& myStat = m_AllStats[myAgentStatIdx];
			if (myStat.GetLowerCaseStatAbreviation() == stat.GetLowerCaseStatAbreviation()
				|| myStat.GetLowerCaseStatName() == stat.GetLowerCaseStatName())
			{
				myStat.SetStatAbsoluteMax(stat.GetValueMax());
				myStat.SetStatAbsoluteMin(stat.GetValueMin());
				break;
			}
		}
	}
}

void AgentStatPack::ClearAndCopyAllStatsFromCommonStats()
{
	m_AllStats = AgentStat::GetCopyOfAllAgentStats();
}

void AgentStatPack::SetOrAddAgentStat(const AgentStat& stat)
{
	for (size_t statIdx = 0; statIdx < m_AllStats.size(); statIdx++)
	{
		AgentStat& testStat = m_AllStats[statIdx];
		if (testStat.GetLowerCaseStatName() == stat.GetLowerCaseStatName()
			|| testStat.GetLowerCaseStatAbreviation() == stat.GetLowerCaseStatAbreviation())
		{
			m_AllStats[statIdx] = stat;
			return;
		}
	}
	m_AllStats.push_back(stat);
}

void AgentStatPack::SetOrAddAgentStat(const std::string& statName, int value)
{
	std::string lowerCase = MakeLower(statName);
	for (size_t statIdx = 0; statIdx < m_AllStats.size(); statIdx++)
	{
		AgentStat& testStat = m_AllStats[statIdx];
		if (testStat.GetLowerCaseStatName() == lowerCase)
		{
			testStat.SetStatValue(value);
			return;
		}
	}
}

void AgentStatPack::SetOrAddAgentStatAbbreviation(const std::string& statAbbreviation, int value)
{
	std::string lowerCase = MakeLower(statAbbreviation);
	for (size_t statIdx = 0; statIdx < m_AllStats.size(); statIdx++)
	{
		AgentStat& testStat = m_AllStats[statIdx];
		if (testStat.GetLowerCaseStatAbreviation() == lowerCase)
		{
			testStat.SetStatValue(value);
			return;
		}
	}
}

void AgentStatPack::SetAgentType(AgentTypes type)
{
	m_agentType = type;
}

void AgentStatPack::SetIsMale(bool male)
{
	m_male = male;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//Getters
float AgentStatPack::GetTrueMovementSpeed() const
{
	return m_trueMovementSpeed;
}

float AgentStatPack::GetCurrentMovementSpeed() const
{
	return m_currentMovementSpeed;
}

int AgentStatPack::GetStatValueByStatName(const std::string& name) const
{
	bool found = false;
	return GetStatValueByStatName(name, found);
}

int AgentStatPack::GetStatValueByStatAbbreviation(const std::string& abbreviation) const
{
	bool found = false;
	return GetStatValueByStatAbbreviation(abbreviation, found);
}

int AgentStatPack::GetStatValueByStatName(const std::string& name, bool& found) const
{
	found = false;
	std::string lowerCase = MakeLower(name);
	for (size_t statIdx = 0; statIdx < m_AllStats.size(); statIdx++)
	{
		const AgentStat& stat = m_AllStats[statIdx];
		if (stat.GetLowerCaseStatName() == lowerCase)
		{
			found = true;
			return stat.GetValue();
		}
	}
	return 0;
}

int AgentStatPack::GetStatValueByStatAbbreviation(const std::string& abbreviation, bool& found) const
{
	found = false;
	std::string lowerCase = MakeLower(abbreviation);
	for (size_t statIdx = 0; statIdx < m_AllStats.size(); statIdx++)
	{
		const AgentStat& stat = m_AllStats[statIdx];
		if (stat.GetLowerCaseStatAbreviation() == lowerCase)
		{
			found = true;
			return stat.GetValue();
		}
	}
	return 0;
}

AgentTypes AgentStatPack::GetAgentType() const
{
	return m_agentType;
}

bool AgentStatPack::GetIsMale() const
{
	return m_male;
}

const std::vector<AgentStat>& AgentStatPack::GetAllStats() const
{
	return m_AllStats;
}

const std::vector<AgentStat>* AgentStatPack::GetAllStatsPtr() const
{
	return &m_AllStats;
}

std::vector<AgentStat>* AgentStatPack::GetAllStatsEditablePtr()
{
	return &m_AllStats;
}