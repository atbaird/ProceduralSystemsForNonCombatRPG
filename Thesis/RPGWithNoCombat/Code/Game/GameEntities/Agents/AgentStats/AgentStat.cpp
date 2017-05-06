#include "Game/GameEntities/Agents/AgentStats/AgentStat.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/TheGame.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//Static Variables
STATIC std::vector<AgentStat>* AgentStat::s_AgentStats = nullptr;
STATIC AgentStatLoadingData* AgentStat::s_loadingData = nullptr;
STATIC int AgentStat::s_CommonMin = 0;
STATIC int AgentStat::s_CommonMax = 100;
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//Constructors
AgentStat::AgentStat()
{

}

AgentStat::AgentStat(int min, int max)
	: m_absoluteMin(min),
	m_absoluteMax(max)
{

}

AgentStat::~AgentStat()
{
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//Setters
void AgentStat::SetStatName(const std::string& name)
{
	m_statName = name;
	m_LowerCaseStatName = MakeLower(name);
}

void AgentStat::SetStatAbreviation(const std::string& name)
{
	m_statAbreviation = name;
	m_LowerCaseStatAbreviation = MakeLower(name);
}

void AgentStat::SetStatValue(int value, bool ignoreClamping)
{
	if (ignoreClamping 
		|| (value >= m_absoluteMin && value <= m_absoluteMax))
	{
		m_statVal = value;
	}
	else if (value < m_absoluteMin)
	{
		m_statVal = m_absoluteMin;
	}
	else if (value > m_absoluteMax)
	{
		m_statVal = m_absoluteMax;
	}
}

void AgentStat::SetStatAbsoluteMax(int max)
{
	m_absoluteMax = max;
}

void AgentStat::SetStatAbsoluteMin(int min)
{
	m_absoluteMin = min;
}

void AgentStat::RandomizeValue()
{
	m_statVal = GetRandomIntInRange(m_absoluteMin, m_absoluteMax);
}

void AgentStat::RandomizeValue(int extraMin, int extraMax)
{
	if (extraMin < m_absoluteMin)
	{
		extraMin = m_absoluteMin;
	}
	else if (extraMin > m_absoluteMax)
	{
		extraMin = m_absoluteMax;
	}

	if (extraMax > m_absoluteMax)
	{
		extraMax = m_absoluteMax;
	}
	else if (extraMax < m_absoluteMin)
	{
		extraMax = m_absoluteMin;
	}

	if (extraMax < extraMin)
	{
		int temp = extraMin;
		extraMin = extraMax;
		extraMax = temp;
	}

	m_statVal = GetRandomIntInRange(extraMin, extraMax);
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//Getters
void AgentStat::WriteAgentStatToString(std::string& str, int indentationLvl, bool writeValue) const
{
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "AgentStat Name: " + m_statName + "\n";
	indentationLvl += 3;

	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "AgentStat Max: " + std::to_string(m_absoluteMax) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "AgentStat Min: " + std::to_string(m_absoluteMin) + "\n";

	if (writeValue)
	{
		GameDebuggingCommons::AddIndentation(str, indentationLvl);
		str += "AgentStat Value: " + std::to_string(m_statVal) + "\n";
	}
}

const std::string& AgentStat::GetStatName() const
{
	return m_statName;
}

const std::string& AgentStat::GetLowerCaseStatName() const
{
	return m_LowerCaseStatName;
}

const std::string& AgentStat::GetStatAbreviation() const
{
	return m_statAbreviation;
}

const std::string& AgentStat::GetLowerCaseStatAbreviation() const
{
	return m_LowerCaseStatAbreviation;
}

int AgentStat::GetValue() const
{
	return m_statVal;
}

int AgentStat::GetValueMax() const
{
	return m_absoluteMax;
}

int AgentStat::GetValueMin() const
{
	return m_absoluteMin;
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//Static Functions
STATIC void AgentStat::WriteAllAgentStatsToString(std::string& str, int indentation)
{
	if (s_AgentStats == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < s_AgentStats->size(); i++)
	{
		const AgentStat& stat = s_AgentStats->at(i);
		stat.WriteAgentStatToString(str, indentation, false);
	}
}

STATIC void AgentStat::ResetCommonMinMax()
{
	s_CommonMax = 100;
	s_CommonMin = 0;
}

STATIC void AgentStat::ReadInAllAgentStatsFromCommonFolderAllAtOnce()
{
	std::string commonAgentStatFile = s_CommonAgentStatFile;
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonAgentStatFile = settings->m_CommonAgentStatFolder;
	}

	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(commonAgentStatFile, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadSingleAgentStatFromXMLFile(fileStr);
	}
}

STATIC bool AgentStat::OnUpdateReadInAllAgentStatsFromCommonFolder()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new STATIC AgentStatLoadingData();
	}
	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingData->m_readData)
	{
		s_loadingData->m_readData = true;
		if (settings != nullptr)
		{
			s_loadingData->m_fileDirectory = settings->m_CommonAgentStatFolder;
			s_loadingData->m_amountOfTimeToTakePerFrame = 
				settings->m_amountOfTimeLoadingAgentStatsPerFrame;
		}
		s_loadingData->m_files = 
			FileUtils::EnumerateFilesInDirectory(s_loadingData->m_fileDirectory, "*", true);;
		return s_loadingData->m_files.size() == 0;
	}

	if (s_loadingData->m_amountOfTimeToTakePerFrame <= 0)
	{
		return true;
	}
	int numberLoadedThisFrame = 0;

	while ((size_t)s_loadingData->m_index < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
		ReadSingleAgentStatFromXMLFile(fileStr);
		numberLoadedThisFrame++;
		s_loadingData->m_index++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakePerFrame)
		{
			return false;
		}

	}

	return (size_t)s_loadingData->m_index >= s_loadingData->m_files.size();
}

STATIC void AgentStat::ReadSingleAgentStatFromXMLFile(const std::string& fileStr)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;

	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, s_CommonAgentRootNode, errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}
	ResetCommonMinMax();
	for (int attrIdx = 0; attrIdx < root.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = root.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "minimum"
			|| attrName == "min")
		{
			s_CommonMin = atoi(attr.lpszValue);
		}
		else if (attrName == "maximum"
			|| attrName == "max")
		{
			s_CommonMax = atoi(attr.lpszValue);
		}
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		ReadXMLNodeAndRegisterNewStat(child);
	}
}

STATIC void AgentStat::ReadXMLNodeAndRegisterNewStat(const XMLNode& node)
{
	AgentStat stat;
	std::string name = node.getName();
	stat.SetStatName(name);
	stat.SetStatAbreviation(name);
	stat.SetStatAbsoluteMin(s_CommonMin);
	stat.SetStatAbsoluteMax(s_CommonMax);

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "abbreviation")
		{
			std::string abbreviation = std::string(attr.lpszValue);
			stat.SetStatAbreviation(abbreviation);
		}
		else if (attrName == "minimum"
			|| attrName == "min")
		{
			int min = atoi(attr.lpszValue);
			stat.SetStatAbsoluteMin(min);
		}
		else if (attrName == "maximum"
			|| attrName == "max")
		{
			int max = atoi(attr.lpszValue);
			stat.SetStatAbsoluteMax(max);
		}
	}

	if (s_AgentStats == nullptr)
	{
		s_AgentStats = new STATIC std::vector<AgentStat>();
	}
	s_AgentStats->push_back(stat);
}

STATIC void AgentStat::ClearAllAgentStats()
{
	if (s_AgentStats != nullptr)
	{
		delete s_AgentStats;
		s_AgentStats = nullptr;
	}
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}
}

STATIC void AgentStat::RegisterStatNew(const AgentStat& stat)
{
	//This is designed to crash the game if you tried to make a stat with a non-unique name and try to register it.
	if (s_AgentStats == nullptr)
	{
		s_AgentStats = new STATIC std::vector<AgentStat>();
	}
	for (size_t statIdx = 0; statIdx < s_AgentStats->size(); statIdx++)
	{
		const AgentStat& testStat = s_AgentStats->at(statIdx);
		if (testStat.m_LowerCaseStatName == stat.m_LowerCaseStatName
			|| testStat.m_LowerCaseStatAbreviation == stat.m_LowerCaseStatAbreviation)
		{
			ERROR_AND_DIE("A stat was attempted to be registered twice!");
		}
	}
	s_AgentStats->push_back(stat);
}

STATIC AgentStat AgentStat::GetStatByName(const std::string& name, bool& found)
{
	found = false;
	if (s_AgentStats == nullptr)
	{
		return AgentStat();
	}
	std::string lowerCaseName = MakeLower(name);
	for (size_t statIdx = 0; statIdx < s_AgentStats->size(); statIdx++)
	{
		const AgentStat& stat = s_AgentStats->at(statIdx);
		if (stat.m_LowerCaseStatName == lowerCaseName)
		{
			found = true;
			return stat;
		}
	}
	return AgentStat();
}

STATIC AgentStat AgentStat::GetStatByAbreviation(const std::string& abbreviation, bool& found)
{
	found = false;
	if (s_AgentStats == nullptr)
	{
		return AgentStat();
	}
	std::string lowerCaseAbbreviation = MakeLower(abbreviation);
	for (size_t statIdx = 0; statIdx < s_AgentStats->size(); statIdx++)
	{
		const AgentStat& stat = s_AgentStats->at(statIdx);
		if (stat.m_LowerCaseStatAbreviation == lowerCaseAbbreviation)
		{
			found = true;
			return stat;
		}
	}
	return AgentStat();

}

const std::vector<AgentStat> AgentStat::GetCopyOfAllAgentStats()
{
	if (s_AgentStats == nullptr)
	{
		return std::vector<AgentStat>();
	}
	return *s_AgentStats;
}

const std::vector<AgentStat>* AgentStat::GetAllAgentStats()
{
	if (s_AgentStats == nullptr)
	{
		return nullptr;
	}
	return s_AgentStats;
}