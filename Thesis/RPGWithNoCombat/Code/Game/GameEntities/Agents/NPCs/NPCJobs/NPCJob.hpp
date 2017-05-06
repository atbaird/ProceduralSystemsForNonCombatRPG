#pragma once

#ifndef NPCJOB_HPP
#define NPCJOB_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/GameEntities/Agents/AgentStats/AgentStat.hpp"
#include <string>
#include <vector>

struct NPCJobLoadingData
{
	std::vector<std::string> m_files;
	std::string m_fileDirectory = "Data/XML/NPCJobs/";
	std::string m_rootNodeName = "";
	std::string m_NodeName = "";
	int m_index = 0;
	double m_amountOfTimeToTakeLoadingPerFrame = 0.25;
	bool m_readData = false;
};

class NPC;
class NPCJob
{
private:
protected:
	//static variables
	static std::vector<NPCJob*>* s_AllRegisteredJobs;
	static std::vector<NPCJob*>* s_ActiveJobs;
	static NPCJob* s_JoblessJob;
	static NPCJobLoadingData* s_loadingData;
	static constexpr unsigned int s_MaxCharsForJobTitle = 8;
	static constexpr char* s_CommonNPCJobFileDirectory = "Data/XML/NPCJobs/";
	static constexpr char* s_NPCJobRootNodeName = "NPCJobs";

	//Variables
	std::string m_jobTitle = "";
	std::string m_jobTitleLowerCase = "";
	std::vector<AgentStat> m_stats;
public:
	//Constructors
	NPCJob();
	NPCJob(const XMLNode& node);
	virtual ~NPCJob();

	//Setters
	void SetJobTitle(const std::string& jobTitle);
	void SetStatByStat(const AgentStat& stat, bool addIfNotFound = false);
	void SetStatByName(const std::string& name, int val, bool addIfNotFound = false);
	void SetStatByAbbreviation(const std::string& abbreviation, int val, bool addIfNotFound = false);

	//Getters
	bool GetIfJobIsJobless() const;
	bool GetIfJobIsJoblessByTitle() const;
	const std::string& GetJobTitle() const;
	const std::string& GetLowerCaseJobTitle() const;
	int GetStatRequirementByName(const std::string& name) const;
	int GetStatRequirementByAbbreviation(const std::string& abbreviation) const;
	bool GetHasStatRequirementByName(const std::string& name) const;
	bool GetHasStatRequirementByAbbreviation(const std::string& abbreviation) const;
	const std::vector<AgentStat>& GetStats() const;
	const std::vector<AgentStat>* GetStatsPtr() const;
	std::vector<AgentStat>* GetStatsEditablePtr();

	//Operations
	void WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel = 0) const;

	//Tests
	bool TestPassesStatRequirement(const AgentStat& stat) const;

	//Static Functions
	static void WriteAllNPCJobsToString(std::string& str, int currentIndentationLevel = 0);
	static std::string ClampJobTitleSize(const std::string& jobTitle);
	static unsigned int GetMaxCharsForJobTitle();
	static void RegisterJob(NPCJob* job);
	static NPCJob* GetJobByTitle(const std::string& jobTitle);
	static void ActivateJob(NPCJob* job);
	static void UnactivateJob(NPCJob* job);
	static void ActivateJob(std::string jobTitle);
	static void UnactivateJob(std::string jobTitle);
	static void ClearActiveJobs();
	static void DestroyAllJobs();
	static void RegisterAllJobsFromXMLFileAllAtOnce();
	static bool OnUpdateRegisterAllJobsFromXMLFile();
protected:
	static void ReadSingleXMLFileForNPCJob(const std::string& fileStr);
public:
	static void DetermineMeetsAJobsRequirements(NPC* npc);
	static void DetermineMeetsAJobsRequirements(NPC* npc, const std::vector<std::string>& acceptedJobList);
};

#endif