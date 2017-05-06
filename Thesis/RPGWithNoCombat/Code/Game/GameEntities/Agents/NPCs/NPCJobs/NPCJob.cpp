#include "Game/GameEntities/Agents/NPCs/NPCJobs/NPCJob.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/TheGame.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Static Variables
STATIC std::vector<NPCJob*>* NPCJob::s_AllRegisteredJobs = nullptr;
STATIC std::vector<NPCJob*>* NPCJob::s_ActiveJobs = nullptr;
STATIC NPCJob* NPCJob::s_JoblessJob = nullptr;
STATIC NPCJobLoadingData* NPCJob::s_loadingData = nullptr;

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Constructors
NPCJob::NPCJob()
{
}

NPCJob::NPCJob(const XMLNode& node)
{
	SetJobTitle(node.getName());

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "npcjob"
			|| attrName == "name"
			|| attrName == "npcjobname")
		{
			m_jobTitle = std::string(attr.lpszValue);
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "statrequirements")
		{
			for (int grandChildIdx = 0; grandChildIdx < child.nChildNode(); grandChildIdx++)
			{
				XMLNode statXMLNode = child.getChildNode(grandChildIdx);
				std::string statName = statXMLNode.getName();
				AgentStat stat = AgentStat();
				stat.SetStatName(statName);

				for (int attrIdx = 0; attrIdx < statXMLNode.nAttribute(); attrIdx++)
				{
					XMLAttribute attr = statXMLNode.getAttribute(attrIdx);
					std::string attrName = MakeLower(attr.lpszName);
					if (attrName == "value")
					{
						int val = atoi(attr.lpszValue);
						stat.SetStatValue(val);
					}
				}



				m_stats.push_back(stat);
			}


		}
	}
}

NPCJob::~NPCJob()
{

}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Setters
void NPCJob::SetJobTitle(const std::string& jobTitle)
{
	m_jobTitle = (jobTitle);
	m_jobTitleLowerCase = MakeLower(m_jobTitle);
}

void NPCJob::SetStatByStat(const AgentStat& stat, bool addIfNotFound)
{
	for (size_t statIdx = 0; statIdx < m_stats.size(); statIdx++)
	{
		AgentStat& testStat = m_stats[statIdx];
		if (testStat.GetLowerCaseStatAbreviation() == stat.GetLowerCaseStatAbreviation()
			|| testStat.GetLowerCaseStatName() == stat.GetLowerCaseStatName())
		{
			testStat.SetStatValue(stat.GetValue());
			return;
		}
	}
	if (addIfNotFound)
	{
		m_stats.push_back(stat);
	}
}

void NPCJob::SetStatByName(const std::string& name, int val, bool addIfNotFound)
{
	std::string lowerCase = MakeLower(name);
	for (size_t statIdx = 0; statIdx < m_stats.size(); statIdx++)
	{
		AgentStat& testStat = m_stats[statIdx];
		if (testStat.GetLowerCaseStatName() == lowerCase)
		{
			testStat.SetStatValue(val);
			return;
		}
	}
	if (addIfNotFound)
	{
		AgentStat stat;
		stat.SetStatValue(val, true);
		stat.SetStatName(name);
		m_stats.push_back(stat);
	}
}

void NPCJob::SetStatByAbbreviation(const std::string& abbreviation, int val, bool addIfNotFound)
{
	std::string lowerCase = MakeLower(abbreviation);
	for (size_t statIdx = 0; statIdx < m_stats.size(); statIdx++)
	{
		AgentStat& testStat = m_stats[statIdx];
		if (testStat.GetLowerCaseStatAbreviation() == lowerCase)
		{
			testStat.SetStatValue(val);
			return;
		}
	}

	if (addIfNotFound)
	{
		AgentStat stat;
		stat.SetStatValue(val, true);
		stat.SetStatAbreviation(abbreviation);
		m_stats.push_back(stat);
	}

}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Getters
bool NPCJob::GetIfJobIsJobless() const
{
	return this == s_JoblessJob;
}

bool NPCJob::GetIfJobIsJoblessByTitle() const
{
	return s_JoblessJob != nullptr && m_jobTitleLowerCase == s_JoblessJob->m_jobTitleLowerCase;
}

const std::string& NPCJob::GetJobTitle() const
{
	return m_jobTitle;
}

const std::string& NPCJob::GetLowerCaseJobTitle() const
{
	return m_jobTitleLowerCase;
}

int NPCJob::GetStatRequirementByName(const std::string& name) const
{
	std::string lowerCase = MakeLower(name);
	for (size_t statIdx = 0; statIdx < m_stats.size(); statIdx++)
	{
		const AgentStat& stat = m_stats[statIdx];
		if (stat.GetLowerCaseStatAbreviation() == lowerCase)
		{
			return stat.GetValue();
		}
	}
	return -1;
}

int NPCJob::GetStatRequirementByAbbreviation(const std::string& abbreviation) const
{
	std::string lowerCase = MakeLower(abbreviation);
	for (size_t statIdx = 0; statIdx < m_stats.size(); statIdx++)
	{
		const AgentStat& stat = m_stats[statIdx];
		if (stat.GetLowerCaseStatAbreviation() == lowerCase)
		{
			return stat.GetValue();
		}
	}
	return -1;
}

bool NPCJob::GetHasStatRequirementByName(const std::string& name) const
{
	return GetStatRequirementByName(name) != -1;
}

bool NPCJob::GetHasStatRequirementByAbbreviation(const std::string& abbreviation) const
{
	return GetStatRequirementByAbbreviation(abbreviation) != -1;
}

const std::vector<AgentStat>& NPCJob::GetStats() const
{
	return m_stats;
}

const std::vector<AgentStat>* NPCJob::GetStatsPtr() const
{
	return &m_stats;
}

std::vector<AgentStat>* NPCJob::GetStatsEditablePtr()
{
	return &m_stats;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Operations
void NPCJob::WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel) const
{
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Job Title: " + m_jobTitle + "\n";
	currentIndentationLevel += 3;
	int nextLevel = currentIndentationLevel + 3;

	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Stat Requirements: \n";
	for (size_t i = 0; i < m_stats.size(); i++)
	{
		const AgentStat& stat = m_stats[i];
		stat.WriteAgentStatToString(str, nextLevel);
	}
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Tests
bool NPCJob::TestPassesStatRequirement(const AgentStat& stat) const
{
	for (size_t statIdx = 0; statIdx < m_stats.size(); statIdx++)
	{
		const AgentStat& testStat = m_stats[statIdx];
		if (testStat.GetLowerCaseStatAbreviation() == stat.GetLowerCaseStatAbreviation()
			|| testStat.GetLowerCaseStatName() == stat.GetLowerCaseStatName())
		{
			if (stat.GetValue() >= testStat.GetValue())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Static Functions
STATIC void NPCJob::WriteAllNPCJobsToString(std::string& str, int currentIndentationLevel)
{
	if (s_AllRegisteredJobs == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < s_AllRegisteredJobs->size(); i++)
	{
		const NPCJob* job = s_AllRegisteredJobs->at(i);
		if(job == nullptr)
			continue;
		job->WriteOutEntityImportantDataToStr(str, currentIndentationLevel);
	}
}

STATIC std::string NPCJob::ClampJobTitleSize(const std::string& jobTitle)
{
	if (jobTitle.length() <= s_MaxCharsForJobTitle)
	{
		return jobTitle;
	}
	else
	{
		return jobTitle.substr(0, s_MaxCharsForJobTitle);
	}
}

STATIC unsigned int NPCJob::GetMaxCharsForJobTitle()
{
	return s_MaxCharsForJobTitle;
}

STATIC void NPCJob::RegisterJob(NPCJob* job)
{
	if (job == nullptr)
	{
		return;
	}
}

STATIC NPCJob* NPCJob::GetJobByTitle(const std::string& jobTitle)
{
	if (s_AllRegisteredJobs == nullptr)
	{
		return nullptr;
	}

	std::string shortenedJobTitle = ClampJobTitleSize(jobTitle);
	shortenedJobTitle = MakeLower(shortenedJobTitle);

	for (size_t jobIdx = 0; jobIdx < s_AllRegisteredJobs->size(); jobIdx++)
	{
		NPCJob* job = s_AllRegisteredJobs->at(jobIdx);
		if (job != nullptr && job->m_jobTitleLowerCase == shortenedJobTitle)
		{
			return job;
		}
	}

	return nullptr;
}

STATIC void NPCJob::ActivateJob(NPCJob* job)
{
	if (job == nullptr)
	{
		return;
	}
}

STATIC void NPCJob::UnactivateJob(NPCJob* job)
{
	if (job == nullptr)
	{
		return;
	}
}

STATIC void NPCJob::ActivateJob(std::string jobTitle)
{
	NPCJob* job = GetJobByTitle(jobTitle);
	if (job == nullptr)
	{
		return;
	}
	if (s_ActiveJobs == nullptr)
	{
		s_ActiveJobs = new std::vector<NPCJob *>();
	}
	s_ActiveJobs->push_back(job);
}

STATIC void NPCJob::UnactivateJob(std::string jobTitle)
{
	if (s_ActiveJobs == nullptr)
	{
		return;
	}
	std::string shortenedJobTitle = ClampJobTitleSize(jobTitle);
	shortenedJobTitle = MakeLower(shortenedJobTitle);

	for (size_t jobIdx = 0; jobIdx < s_ActiveJobs->size(); jobIdx++)
	{
		NPCJob* job = s_ActiveJobs->at(jobIdx);
		if (job != nullptr && job->m_jobTitleLowerCase == shortenedJobTitle)
		{
			s_ActiveJobs->erase(s_ActiveJobs->begin() + jobIdx);
			return;
		}
	}
}

STATIC void NPCJob::ClearActiveJobs()
{
	if (s_ActiveJobs != nullptr)
	{
		s_ActiveJobs->clear();
	}
}

STATIC void NPCJob::DestroyAllJobs()
{
	if (s_ActiveJobs != nullptr)
	{
		delete s_ActiveJobs;
		s_ActiveJobs = nullptr;
	}
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}

	if (s_AllRegisteredJobs != nullptr)
	{
		for (size_t jobIdx = 0; jobIdx < s_AllRegisteredJobs->size(); jobIdx++)
		{
			NPCJob* job = s_AllRegisteredJobs->at(jobIdx);
			if (job != nullptr)
			{
				delete job;
				job = nullptr;
			}
		}
		delete s_AllRegisteredJobs;
		s_AllRegisteredJobs = nullptr;
	}
	s_JoblessJob = nullptr;
}

STATIC void NPCJob::RegisterAllJobsFromXMLFileAllAtOnce()
{
	std::string commonNPCJobFileDirectory = s_CommonNPCJobFileDirectory;
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonNPCJobFileDirectory = settings->m_CommonNPCJobFileDirectory;
	}
	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(commonNPCJobFileDirectory, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
	}
}

STATIC bool NPCJob::OnUpdateRegisterAllJobsFromXMLFile()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new STATIC NPCJobLoadingData();
	}
	GameSettings* settings = TheGame::GetGameSettings();

	if (!s_loadingData->m_readData)
	{
		s_loadingData->m_readData = true;
		if (settings != nullptr)
		{
			s_loadingData->m_fileDirectory = settings->m_CommonNPCJobFileDirectory;
			s_loadingData->m_amountOfTimeToTakeLoadingPerFrame = settings->m_amountOfTimeLoadingNPCJobsPerFrame;
		}
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(s_loadingData->m_fileDirectory, "*", true);
		return s_loadingData->m_files.size() == 0;
	}

	if (s_loadingData->m_amountOfTimeToTakeLoadingPerFrame <= 0)
	{
		return true;
	}
	int numberReadThisFrame = 0;

	while ((size_t)s_loadingData->m_index < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
		ReadSingleXMLFileForNPCJob(fileStr);
		s_loadingData->m_index++;
		numberReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakeLoadingPerFrame)
		{
			return false;
		}
	}
	return (size_t)s_loadingData->m_index >= s_loadingData->m_files.size();
}

STATIC void NPCJob::ReadSingleXMLFileForNPCJob(const std::string& fileStr)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, s_NPCJobRootNodeName, errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}
	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);

		NPCJob* job = new NPCJob(child);
		if (s_AllRegisteredJobs == nullptr)
		{
			s_AllRegisteredJobs = new std::vector<NPCJob *>();
			if (s_JoblessJob == nullptr)
			{
				s_JoblessJob = new NPCJob();
				s_JoblessJob->SetJobTitle("Jobless");
				s_AllRegisteredJobs->push_back(s_JoblessJob);
			}
		}
		s_AllRegisteredJobs->push_back(job);
	}
}

STATIC void NPCJob::DetermineMeetsAJobsRequirements(NPC* npc)
{
	if (s_JoblessJob == nullptr)
	{
		s_JoblessJob = new NPCJob();
		s_JoblessJob->SetJobTitle("Jobless");
		s_AllRegisteredJobs->push_back(s_JoblessJob);
	}
	if (npc == nullptr
		|| s_AllRegisteredJobs == nullptr)
	{
		npc->SetJob(s_JoblessJob);
		return;
	}
	const std::vector<AgentStat>* npcStats = npc->GetStatsPtr(); //we know this is not null.
	std::vector<NPCJob*> possibleQualifyingJobs;

	for (size_t jobIdx = 0; jobIdx < s_AllRegisteredJobs->size(); jobIdx++)
	{
		NPCJob* job = s_AllRegisteredJobs->at(jobIdx);
		if (job != nullptr)
		{
			bool failedATest = false;
			for (size_t jobStatIdx = 0; jobStatIdx < job->m_stats.size(); jobStatIdx++)
			{
				const AgentStat& stat = job->m_stats[jobStatIdx];
				bool found = false;
				for (size_t npcStatIdx = 0; npcStatIdx < npcStats->size(); npcStatIdx++)
				{
					const AgentStat& npcStat = npcStats->at(npcStatIdx);
					if (npcStat.GetLowerCaseStatAbreviation() == stat.GetLowerCaseStatAbreviation()
						|| npcStat.GetLowerCaseStatName() == stat.GetLowerCaseStatName())
					{
						found = true;
						if (npcStat.GetValue() < stat.GetValue())
						{
							failedATest = true;
						}

						break;
					}
				}
				if (found == false)
				{
					failedATest = true;
					break;
				}
			}

			if (failedATest == false)
			{
				possibleQualifyingJobs.push_back(job);
			}
		}
	}

	if (possibleQualifyingJobs.size() == 0)
	{
		npc->SetJob(s_JoblessJob);
		return;
	}
	else
	{
		int jobIdx = GetRandomIntLessThan(possibleQualifyingJobs.size());
		NPCJob* job = possibleQualifyingJobs[jobIdx];
		npc->SetJob(job);
	}

}

void NPCJob::DetermineMeetsAJobsRequirements(NPC* npc, const std::vector<std::string>& acceptedJobList)
{
	if (s_JoblessJob == nullptr)
	{
		s_JoblessJob = new NPCJob();
		s_JoblessJob->SetJobTitle("Jobless");
		s_AllRegisteredJobs->push_back(s_JoblessJob);
	}
	if (npc == nullptr
		|| s_AllRegisteredJobs == nullptr)
	{
		npc->SetJob(s_JoblessJob);
		return;
	}
	const std::vector<AgentStat>* npcStats = npc->GetStatsPtr(); //we know this is not null.
	std::vector<NPCJob*> possibleQualifyingJobs;

	for (size_t jobIdx = 0; jobIdx < s_AllRegisteredJobs->size(); jobIdx++)
	{
		NPCJob* job = s_AllRegisteredJobs->at(jobIdx);
		if (job != nullptr)
		{
			std::string jobTitleTestingAgainst = MakeLower(job->GetJobTitle());
			bool isJobOnList = false;
			for (size_t acceptJobIdx = 0; acceptJobIdx < acceptedJobList.size(); acceptJobIdx++)
			{
				std::string jobTitle = MakeLower(acceptedJobList[acceptJobIdx]);
				if (jobTitleTestingAgainst == jobTitle)
				{
					isJobOnList = true;
					break;
				}
			}
			if (isJobOnList == false)
			{
				continue;
			}

			bool failedATest = false;
			for (size_t jobStatIdx = 0; jobStatIdx < job->m_stats.size(); jobStatIdx++)
			{
				const AgentStat& jobStat = job->m_stats[jobStatIdx];
				bool found = false;
				for (size_t npcStatIdx = 0; npcStatIdx < npcStats->size(); npcStatIdx++)
				{
					const AgentStat& npcStat = npcStats->at(npcStatIdx);
					if (npcStat.GetLowerCaseStatAbreviation() == jobStat.GetLowerCaseStatAbreviation()
						|| npcStat.GetLowerCaseStatName() == jobStat.GetLowerCaseStatName())
					{
						found = true;
						if (npcStat.GetValue() < jobStat.GetValue())
						{
							failedATest = true;
						}

						break;
					}
				}
				if (found == false)
				{
					failedATest = true;
					break;
				}
				else if (failedATest == true)
				{
					break;
				}
			}

			if (failedATest == false)
			{
				possibleQualifyingJobs.push_back(job);
			}
		}
	}

	if (possibleQualifyingJobs.size() == 0)
	{
		npc->SetJob(s_JoblessJob);
		return;
	}
	else
	{
		int jobIdx = GetRandomIntLessThan(possibleQualifyingJobs.size());
		NPCJob* job = possibleQualifyingJobs[jobIdx];
		npc->SetJob(job);
	}
}