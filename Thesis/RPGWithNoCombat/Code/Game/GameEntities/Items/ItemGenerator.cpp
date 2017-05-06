#include "Game/GameEntities/Items/ItemGenerator.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Items/Item.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/TheGame.hpp"

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Static variables
STATIC std::vector<ItemGenerator>* ItemGenerator::s_ItemGenerators = nullptr;
STATIC ItemGeneratorLoadingData* ItemGenerator::s_loadingData = nullptr;

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Constructors
ItemGenerator::ItemGenerator()
{

}

ItemGenerator::ItemGenerator(const XMLNode& node)
{
	m_itemTemplate.SetAndReadXMLNode(node);
	std::string itemName = m_itemTemplate.GetItemName();
	ItemType type = m_itemTemplate.GetItemType();
	if (itemName == ""
		|| type <= INVALID_ITEM_TYPE
		|| type >= NUMBER_OF_ITEM_TYPES)
	{
		return;
	}
	m_valid = true;
	m_lowerCaseItemName = MakeLower(itemName);
}

ItemGenerator::~ItemGenerator()
{

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Operations
Item* ItemGenerator::SpawnItemAtPosition(const Vector2& position) const
{
	if (m_valid == false)
	{
		return nullptr;
	}
	Item* item = m_itemTemplate.SpawnItem(position);
	item->SetGeneratorUsed(m_lowerCaseItemName);
	return item;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Getters
const std::string& ItemGenerator::GetName()
{
	return m_itemTemplate.GetItemName();
}

const std::string& ItemGenerator::GetLowerCaseName() const
{
	return m_lowerCaseItemName;
}

const bool ItemGenerator::GetIsValid() const
{
	return m_valid;
}

void ItemGenerator::WriteGeneratorToString(std::string& str, int currentIndentationLevel) const
{
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += m_lowerCaseItemName + "\n";
	currentIndentationLevel += 3;
	m_itemTemplate.WriteGeneratorToString(str, currentIndentationLevel);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Static Functions
STATIC void ItemGenerator::WriteAllItemGeneratorsToString(std::string& str, int currentIndentationLevel)
{
	if (s_ItemGenerators == nullptr)
	{
		return;
	}

	str += "Number of Item Generators: " + std::to_string(s_ItemGenerators->size()) + "\n";
	currentIndentationLevel += 3;
	for (size_t i = 0; i < s_ItemGenerators->size(); i++)
	{
		const ItemGenerator& generator = s_ItemGenerators->at(i);
		generator.WriteGeneratorToString(str, currentIndentationLevel);
	}
}

STATIC void ItemGenerator::ClearAllGenerators()
{
	ItemTypeSpawnerRegistration::ClearAllItemTypeSpawnerRegistrations();

	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}

	if (s_ItemGenerators == nullptr)
	{
		return;
	}
	std::vector<ItemGenerator>* ItemGenerators = s_ItemGenerators;
	s_ItemGenerators = nullptr;
	delete ItemGenerators;
	ItemGenerators = nullptr;
}

STATIC void ItemGenerator::ReadInGeneratorsFromCommonItemGeneratorsFolderAllAtOnce()
{
	std::string commonItemGeneratorsFolder = "Data/XML/ItemGenerators/";
	std::string commonItemRootNodeName = "ItemGenerators";
	std::string commonItemNodeName = "ItemGenerator";

	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonItemGeneratorsFolder = settings->m_commonItemGeneratorsFolder;
		commonItemRootNodeName = settings->m_commonItemRootNodeName;
		commonItemNodeName = settings->m_commonItemNodeName;
	}


	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(commonItemGeneratorsFolder, "*", true);
	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadInItemGeneratorsFromXMLFileLocation(settings, fileStr, commonItemRootNodeName, commonItemNodeName);
	}
}

STATIC bool ItemGenerator::OnUpdateReadInGeneratorsFromCommonItemGeneratorsFolder()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new ItemGeneratorLoadingData();
	}
	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingData->m_readData)
	{
		s_loadingData->m_readData = true;
		if (settings != nullptr)
		{
			s_loadingData->m_fileDirectory = settings->m_commonItemGeneratorsFolder;
			s_loadingData->m_rootNodeName = settings->m_commonItemRootNodeName;
			s_loadingData->m_NodeName = settings->m_commonItemNodeName;
			s_loadingData->m_amountOfTimeToTakeReadingPerFrame = settings->m_amountOfTimeLoadingItemGeneratorsPerFrame;
		}
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(s_loadingData->m_fileDirectory, "*", true);
		return s_loadingData->m_files.size() == 0;
	}

	if (s_loadingData->m_amountOfTimeToTakeReadingPerFrame <= 0)
	{
		return true;
	}

	int numberReadThisFrame = 0;
	while ((size_t)s_loadingData->m_index < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
		ReadInItemGeneratorsFromXMLFileLocation(settings, fileStr, 
			s_loadingData->m_rootNodeName, s_loadingData->m_NodeName);
		s_loadingData->m_index++;
		numberReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakeReadingPerFrame)
		{
			return false;
		}
	}

	return (size_t)s_loadingData->m_index >= s_loadingData->m_files.size();
}

STATIC void ItemGenerator::ReadInItemGeneratorsFromXMLFileLocation(GameSettings* settings,
	const std::string& fileStr,
	const std::string& commonItemRootNodeName,
	const std::string& commonItemNodeName)
{
	;
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, commonItemRootNodeName.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		if (SimpleStrCmpLower(child.getName(), commonItemNodeName))
		{
			ItemGenerator newGenerator = ItemGenerator(child);
			if (s_ItemGenerators == nullptr)
			{
				s_ItemGenerators = new std::vector<ItemGenerator>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountItemGenerators;
				}
				s_ItemGenerators->reserve(preallocationAmt);
			}
			s_ItemGenerators->push_back(newGenerator);
		}
	}
}

STATIC const ItemGenerator* ItemGenerator::GetItemGeneratorByItemName(const char* name)
{
	if (s_ItemGenerators == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);
	for (size_t i = 0; i < s_ItemGenerators->size(); i++)
	{
		ItemGenerator* gen = &s_ItemGenerators->at(i);
		if (gen == nullptr)
		{
			continue;
		}
		if (lowerCase == gen->GetLowerCaseName())
		{
			return gen;
		}
	}
	return nullptr;
}

STATIC const ItemGenerator* ItemGenerator::GetItemGeneratorByIndex(size_t index)
{
	if (s_ItemGenerators == nullptr)
	{
		return nullptr;
	}

	if (index >= s_ItemGenerators->size())
	{
		return nullptr;
	}
	return &s_ItemGenerators->at(index);
}

STATIC BaseGameEntity* ItemGenerator::SpawnItemByItemName(const char* name, const Vector2& position)
{
	const ItemGenerator* gen = GetItemGeneratorByItemName(name);
	if (gen == nullptr)
	{
		return nullptr;
	}
	return gen->SpawnItemAtPosition(position);
}

STATIC BaseGameEntity* ItemGenerator::SpawnItemByIndex(size_t index, const Vector2& position)
{
	const ItemGenerator* gen = GetItemGeneratorByIndex(index);
	if (gen == nullptr)
	{
		return nullptr;
	}
	return gen->SpawnItemAtPosition(position);
}

STATIC BaseGameEntity* ItemGenerator::SpawnItemAtRandomPosition(const Vector2& position)
{
	if (s_ItemGenerators == nullptr
		|| s_ItemGenerators->size() == 0)
	{
		return nullptr;
	}
	size_t ran = ((size_t)rand() / RAND_MAX) * s_ItemGenerators->size();
	return s_ItemGenerators->at(ran).SpawnItemAtPosition(position);
}