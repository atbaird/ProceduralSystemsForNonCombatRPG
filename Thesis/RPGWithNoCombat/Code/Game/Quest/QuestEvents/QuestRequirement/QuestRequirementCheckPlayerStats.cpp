#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckPlayerStats.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Creation Funcs
QuestRequirement* CheckPlayerStatsCreationFunc(const XMLNode& node)
{
	QuestRequirementCheckPlayerStats* val = new QuestRequirementCheckPlayerStats(node);
	return val;
}

QuestRequirement* CheckPlayerStatsDefaultCreationFunc()
{
	QuestRequirementCheckPlayerStats* val = new QuestRequirementCheckPlayerStats();
	return val;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Static Variables
STATIC QuestRequirementRegistration 
QuestRequirementCheckPlayerStats::s_CheckPlayerStatsRegistration = QuestRequirementRegistration("CheckPlayerStats",
	CheckPlayerStatsCreationFunc, CheckPlayerStatsDefaultCreationFunc);


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Constructors
QuestRequirementCheckPlayerStats::QuestRequirementCheckPlayerStats()
	: QuestRequirement(Check_Player_Stats)
{

}

QuestRequirementCheckPlayerStats::QuestRequirementCheckPlayerStats(const XMLNode& node)
	: QuestRequirement(Check_Player_Stats, node)
{

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = (child.getName());
		AgentStatsCheckingStruct stat(childName);
		int value = 0;
		SpecialCaseOperations operation = IS_SAME;
		for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = child.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			std::string val = std::string(attr.lpszValue);
			if (attrName == "value")
			{
				ParseTextForSpecialCaseOperationsAndValue(val, operation, value);
			}
		}
		stat.SetSpecialCaseOperations(operation);
		stat.SetStatValue(value);
		m_statRequirements.push_back(stat);
	}
}

QuestRequirementCheckPlayerStats::QuestRequirementCheckPlayerStats(const QuestRequirementCheckPlayerStats& other,
	Quest* owningQuest)
	: QuestRequirement(other, owningQuest),
	m_statRequirements(other.m_statRequirements)
{

}

QuestRequirementCheckPlayerStats::~QuestRequirementCheckPlayerStats()
{
	QuestRequirement::~QuestRequirement();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Setters
void QuestRequirementCheckPlayerStats::AddStatRequirement(const AgentStatsCheckingStruct& statRequirement)
{
	for (size_t idx = 0; idx < m_statRequirements.size(); idx++)
	{
		AgentStatsCheckingStruct& stat = m_statRequirements[idx];
		if (statRequirement.GetLowerCaseStatName() == stat.GetLowerCaseStatName())
		{
			return;
		}
	}
	m_statRequirements.push_back(statRequirement);
}

void QuestRequirementCheckPlayerStats::SetStatRequirementIfExists(const std::string& statName, int statVal)
{
	std::string lowerCase = MakeLower(statName);
	for (size_t idx = 0; idx < m_statRequirements.size() ; idx++)
	{
		AgentStatsCheckingStruct& stat = m_statRequirements[idx];
		if (lowerCase == stat.GetLowerCaseStatName())
		{
			stat.SetStatValue(statVal);
			return;
		}
	}
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Getters
const std::vector<AgentStatsCheckingStruct>& QuestRequirementCheckPlayerStats::GetStatRequirements() const
{
	return m_statRequirements;
}

const std::vector<AgentStatsCheckingStruct>* QuestRequirementCheckPlayerStats::GetStatRequirementsPtr() const
{
	return &m_statRequirements;
}

std::vector<AgentStatsCheckingStruct>* QuestRequirementCheckPlayerStats::GetStatRequirementsEditablePtr()
{
	return &m_statRequirements;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Operations
void QuestRequirementCheckPlayerStats::WriteQuestRequirementToString(std::string& str, 
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Check Player Stats\n";
	indentationAmt += 3;

	int nextLevel = indentationAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of Stats to check: " + std::to_string(m_statRequirements.size()) + "\n";
	
	if (m_statRequirements.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Desired Stat Values: \n";
		for (size_t i = 0; i < m_statRequirements.size(); i++)
		{
			m_statRequirements[i].WriteToString(str, nextLevel);
		}
	}


}

const bool QuestRequirementCheckPlayerStats::DeterminePlayerMeetsQuestRequirement() const
{
	const BaseGameEntity* player = BaseGameEntity::GetPointerToPlayer();
	if (player == nullptr)
	{
		return false;
	}
	const Agent* agePlayer = dynamic_cast<const Agent*>(player);
	if (agePlayer == nullptr)
	{
		return false;
	}
	const std::vector<AgentStat>* stats = agePlayer->GetStatsPtr();

	for (size_t statReqIdx = 0; statReqIdx < m_statRequirements.size(); statReqIdx++)
	{
		const AgentStatsCheckingStruct& statTest = m_statRequirements[statReqIdx];
		std::string lowerCaseTestStatName = statTest.GetLowerCaseStatName();
		bool foundStat = false;
		for (size_t playerStatIdx = 0; playerStatIdx < stats->size(); playerStatIdx++)
		{
			const AgentStat& stat = stats->at(playerStatIdx);
			std::string lowerCaseStatName = stat.GetLowerCaseStatName();
			if (lowerCaseStatName == lowerCaseTestStatName)
			{
				foundStat = true;
				int statAmt = stat.GetValue();

				if (statTest.TestValAgainstStat(statAmt) == false)
				{
					return false;
				}
			}
		}
		if (foundStat == false)
		{
			return false;
		}
	}
	return true;
}

QuestRequirement* QuestRequirementCheckPlayerStats::Clone(Quest* owningQuest) const
{
	QuestRequirementCheckPlayerStats* clone = new QuestRequirementCheckPlayerStats(*this, owningQuest);
	return clone;
}