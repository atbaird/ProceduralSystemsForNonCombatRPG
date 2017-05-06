#include "Game/GameEntities/Hair/HairGenerator.hpp"
#include "Game/GameEntities/Hair/Hair.hpp"
#include "Game/GameEntities/Hair/HairTemplate.hpp"
#include "Game/GameEntities/Hair/HairColorNameValuePair.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/GameEntities/Agents/AgentGenerator.hpp"
#include "Game/TheGame.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Static Variables
STATIC std::vector<HairGenerator>* HairGenerator::s_HairGenerators = nullptr;
STATIC HairGeneratorLoadingData* HairGenerator::s_loadingData = nullptr;

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Constructors
HairGenerator::HairGenerator()
{

}

HairGenerator::HairGenerator(const XMLNode& node)
{
	m_template.SetAndReadXMLNode(node);
	m_lowerCaseHairName = MakeLower(m_template.GetHairName());
}

HairGenerator::~HairGenerator()
{

}

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Operations
Hair* HairGenerator::SpawnHair() const
{
	return m_template.SpawnHair();
}

Hair* HairGenerator::SpawnHair(const std::vector<std::string>& colorChoiceLimits) const
{
	return m_template.SpawnHair(colorChoiceLimits);
}

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Setters

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Getters
void HairGenerator::WriteGeneratorToString(std::string& str, int indentationLevel) const
{
	m_template.WriteGeneratorToString(str, indentationLevel);
}

const std::string& HairGenerator::GetName()
{
	return m_template.GetHairName();
}

const std::string& HairGenerator::GetLowerCaseName() const
{
	return m_lowerCaseHairName;
}

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Static Functions
STATIC void HairGenerator::WriteAllHairGeneratorsToString(std::string& str, int indentationLevel)
{
	if (s_HairGenerators == nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationLevel);
		str += "No hair generators\n";
		return;
	}

	str += "\nNumber of Hair Generators: " + std::to_string(s_HairGenerators->size());
	str += "\n";

	for (size_t i = 0; i < s_HairGenerators->size(); i++)
	{
		const HairGenerator& generator = s_HairGenerators->at(i);
		generator.WriteGeneratorToString(str, indentationLevel);
	}
}

STATIC void HairGenerator::ClearAllGenerators()
{
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}
	if (s_HairGenerators == nullptr)
	{
		return;
	}

	delete s_HairGenerators;
	s_HairGenerators = nullptr;
}

STATIC void HairGenerator::ReadInGeneratorsFromCommonHairGeneratorsFolderAllAtOnce()
{
	std::string commonHairGeneratorsFolder = "";
	std::string commonHairRootNodeName = "";
	std::string commonHairNodeName = "";

	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonHairGeneratorsFolder = settings->m_commonHairGeneratorsFolder;
		commonHairRootNodeName = settings->m_commonHairRootNodeName;
		commonHairNodeName = settings->m_commonHairNodeName;
	}


	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(commonHairGeneratorsFolder, "*", true);
	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadSingleHairGeneratorFromXMLFile(settings, fileStr, commonHairRootNodeName, commonHairNodeName);
	}
}

STATIC bool HairGenerator::OnUpdateReadInGeneratorsFromCommonHairGeneratorsFolder()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new STATIC HairGeneratorLoadingData();
	}
	GameSettings* settings = TheGame::GetGameSettings();

	if (!s_loadingData->m_readData)
	{
		s_loadingData->m_readData = true;
		if (settings != nullptr)
		{
			s_loadingData->m_fileDirectory = settings->m_commonHairGeneratorsFolder;
			s_loadingData->m_rootNodeName = settings->m_commonHairRootNodeName;
			s_loadingData->m_NodeName = settings->m_commonHairNodeName;
			s_loadingData->m_amountOfTimeToTakeReadingHairPerFrame = settings->m_amountOfTimeLoadingHairGeneratorsPerFrame;
		}
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(s_loadingData->m_fileDirectory,
			"*", true);
		return s_loadingData->m_files.size() == 0;
	}

	if (s_loadingData->m_amountOfTimeToTakeReadingHairPerFrame <= 0)
	{
		return true;
	}

	int numberOfHairGeneratorsReadThisFrame = 0;
	while ((size_t)s_loadingData->m_index < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
		ReadSingleHairGeneratorFromXMLFile(settings, fileStr, 
			s_loadingData->m_rootNodeName, s_loadingData->m_NodeName);
		s_loadingData->m_index++;
		numberOfHairGeneratorsReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakeReadingHairPerFrame)
		{
			return false;
		}
	}

	return (size_t)s_loadingData->m_index >= s_loadingData->m_files.size();
}

STATIC void HairGenerator::ReadSingleHairGeneratorFromXMLFile(GameSettings* settings,
	const std::string fileStr,
	const std::string& commonHairRootNodeName,
	const std::string& commonHairNodeName)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, commonHairRootNodeName.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		if (SimpleStrCmpLower(child.getName(), commonHairNodeName))
		{
			HairGenerator newGenerator = HairGenerator(child);
			if (s_HairGenerators == nullptr)
			{
				s_HairGenerators = new std::vector<HairGenerator>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountHairGenerators;
				}
				s_HairGenerators->reserve(preallocationAmt);
			}
			s_HairGenerators->push_back(newGenerator);
			if (newGenerator.m_template.IsDefaultHair())
			{
				AgentGenerator::RegisterDefaultHair(newGenerator.GetName());
			}
		}
	}
}

STATIC const HairGenerator* HairGenerator::GetHairGeneratorByItemName(const char* name)
{
	if (name == nullptr
		|| s_HairGenerators == nullptr)
	{
		return nullptr;
	}
	for (size_t idx = 0; idx < s_HairGenerators->size(); idx++)
	{
		HairGenerator* gen = &s_HairGenerators->at(idx);
		if (gen != nullptr
			&& SimpleStrCmpLower(gen->GetLowerCaseName(), std::string(name)))
		{
			return gen;
		}
	}
	return nullptr;
}

STATIC const HairGenerator* HairGenerator::GetHairGeneratorByIndex(size_t index)
{
	if (s_HairGenerators == nullptr
		|| index >= s_HairGenerators->size())
	{
		return nullptr;
	}
	return &s_HairGenerators->at(index);
}

STATIC BaseGameEntity* HairGenerator::SpawnHairByHairName(const char* name)
{
	const HairGenerator* gen = GetHairGeneratorByItemName(name);
	if (gen == nullptr)
	{
		return nullptr;
	}
	return gen->SpawnHair();
}

STATIC BaseGameEntity* HairGenerator::SpawnHairByIndex(size_t index)
{
	const HairGenerator* gen = GetHairGeneratorByIndex(index);
	if (gen == nullptr)
	{
		return nullptr;
	}
	return gen->SpawnHair();
}


STATIC BaseGameEntity* HairGenerator::SpawnHairByHairName(const char* name,
	const std::vector<std::string>& colorChoiceLimits)
{
	const HairGenerator* gen = GetHairGeneratorByItemName(name);
	if (gen == nullptr)
	{
		return nullptr;
	}
	return gen->SpawnHair(colorChoiceLimits);
}

STATIC BaseGameEntity* HairGenerator::SpawnHairByIndex(size_t index, 
	const std::vector<std::string>& colorChoiceLimits)
{
	const HairGenerator* gen = GetHairGeneratorByIndex(index);
	if (gen == nullptr)
	{
		return nullptr;
	}
	return gen->SpawnHair(colorChoiceLimits);
}