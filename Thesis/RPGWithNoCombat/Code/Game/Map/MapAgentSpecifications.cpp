#include "Game/Map/MapAgentSpecifications.hpp"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Constructors
MapAgentSpecifications::MapAgentSpecifications()
{

}

MapAgentSpecifications::~MapAgentSpecifications()
{

}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Setters
void MapAgentSpecifications::SetStatAndOverwrite(const AgentStat& stat, bool overwrite)
{
	for (size_t i = 0; i < m_overwritingStats.size(); i++)
	{
		AgentStat& testStat = m_overwritingStats[i];
		if (testStat.GetLowerCaseStatAbreviation() == stat.GetLowerCaseStatAbreviation()
			|| testStat.GetLowerCaseStatName() == stat.GetLowerCaseStatName())
		{
			m_overwritingStats[i] = stat;
			m_overwriteStats[i] = overwrite;
			return;
		}
	}

	m_overwritingStats.push_back(stat);
	m_overwriteStats.push_back(overwrite);
}