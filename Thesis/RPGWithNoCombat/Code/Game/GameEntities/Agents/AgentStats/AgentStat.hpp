#pragma once

#ifndef AGENTSTAT_HPP
#define AGENTSTAT_HPP
#include <vector>
#include <string>

struct AgentStatLoadingData
{
	std::vector<std::string> m_files;
	std::string m_fileDirectory = "";
	std::string m_rootNodeName = "";
	std::string m_NodeName = "";
	int m_index = 0;
	double m_amountOfTimeToTakePerFrame = 0.25;
	bool m_readData = false;
};

#define STATIC
struct XMLNode;
class AgentStat
{
private:
protected:
	//Static Variables
	static std::vector<AgentStat>* s_AgentStats;
	static AgentStatLoadingData* s_loadingData;
	static constexpr char* s_CommonAgentStatFile = "Data/XML/AgentStats/";
	static constexpr char* s_CommonAgentRootNode = "AgentStats";
	static int s_CommonMin;
	static int s_CommonMax;
public:

private:

	//Variables
	std::string m_statName = "";
	std::string m_LowerCaseStatName = "";
	std::string m_statAbreviation = "";
	std::string m_LowerCaseStatAbreviation = "";
	int m_statVal = 0;
	int m_absoluteMax = 100;
	int m_absoluteMin = 0;
public:
	//Constructors
	AgentStat();
	AgentStat(int min, int max);
	virtual ~AgentStat();

	//Setters
	void SetStatName(const std::string& name);
	void SetStatAbreviation(const std::string& name);
	void SetStatValue(int value, bool ignoreClamping = false);
	void SetStatAbsoluteMax(int max);
	void SetStatAbsoluteMin(int min);
	void RandomizeValue();
	void RandomizeValue(int extraMin, int extraMax);

	//Getters
	void WriteAgentStatToString(std::string& str, int indentationLvl, bool writeValue = true) const;
	const std::string& GetStatName() const;
	const std::string& GetLowerCaseStatName() const;
	const std::string& GetStatAbreviation() const;
	const std::string& GetLowerCaseStatAbreviation() const;
	int GetValue() const;
	int GetValueMax() const;
	int GetValueMin() const;

	//Static Functions
	static void WriteAllAgentStatsToString(std::string& str, int indentation);
	static void ResetCommonMinMax();
	static void ReadInAllAgentStatsFromCommonFolderAllAtOnce();
	static bool OnUpdateReadInAllAgentStatsFromCommonFolder();
protected:
	static void ReadSingleAgentStatFromXMLFile(const std::string& fileStr);
public:
	static void ReadXMLNodeAndRegisterNewStat(const XMLNode& node);
	static void ClearAllAgentStats();
	static void RegisterStatNew(const AgentStat& stat);
	static AgentStat GetStatByName(const std::string& name, bool& found);
	static AgentStat GetStatByAbreviation(const std::string& abbreviation, bool& found);
	static const std::vector<AgentStat> GetCopyOfAllAgentStats();
	static const std::vector<AgentStat>* GetAllAgentStats();
};
#endif