#include "Game/Quest/QuestGenerator.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Game/Quest/Quest.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/TheGame.hpp"
#include "Game/GameSettings.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Static Variables
STATIC std::vector<QuestGenerator>* QuestGenerator::s_QuestGenerators = nullptr;
STATIC QuestGeneratorLoadingSave* QuestGenerator::s_loadingSave = nullptr;

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Constructors
QuestGenerator::QuestGenerator()
{

}

QuestGenerator::QuestGenerator(const XMLNode& QuestNode)
	: m_Template(QuestNode)
{
	m_QuestName = QuestNode.getName();
	m_QuestLowerCase = MakeLower(m_QuestName);
}

QuestGenerator::QuestGenerator(const QuestGenerator& other)
	: m_Template(other.m_Template),
	m_QuestName(other.m_QuestName),
	m_QuestLowerCase(other.m_QuestLowerCase)
{

}

QuestGenerator::~QuestGenerator()
{

}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Setters
void QuestGenerator::SetQuestName(const std::string& name)
{
	m_QuestName = name;
	m_QuestLowerCase = MakeLower(name);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Getters
const std::string& QuestGenerator::GetQuestName() const
{
	return m_QuestName;
}

const std::string& QuestGenerator::GetQuestNameLowerCase() const
{
	return m_QuestLowerCase;
}

const QuestTemplate& QuestGenerator::GetQuestTemplate() const
{
	return m_Template;
}

QuestTemplate* QuestGenerator::GetEditableQuestTemplatePtr()
{
	return &m_Template;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Operations
Quest* QuestGenerator::SpawnQuest(const std::string& currentMapFileName, 
	const XMLNode& mapQuestNode, MapQuestImportantData& data,
	MapFiles* files)
{
	Quest* quest = new Quest();
	m_Template.CopyVariablesOntoQuest(quest, mapQuestNode, currentMapFileName, data, files);
	return quest;
}

void QuestGenerator::WriteQuestGeneratorToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Generator Name: " + m_QuestName + "\n";

	m_Template.WriteQuestTemplateToString(str, indentationAmt + 3);

}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Static Functions
STATIC void QuestGenerator::WriteAllQuestGenerators(std::string& str, int indentationAmt)
{
	if (s_QuestGenerators == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < s_QuestGenerators->size(); i++)
	{
		const QuestGenerator& gen = s_QuestGenerators->at(i);
		gen.WriteQuestGeneratorToString(str, indentationAmt);
	}
}

STATIC void QuestGenerator::ClearAllGenerators()
{
	if (s_QuestGenerators != nullptr)
	{
		delete s_QuestGenerators;
		s_QuestGenerators = nullptr;
	}
	if (s_loadingSave != nullptr)
	{
		delete s_loadingSave;
		s_loadingSave = nullptr;
	}
}

STATIC void QuestGenerator::ReadInGeneratorsFromCommonQuestGeneratorsFolderAllAtOnce()
{
	std::string commonQuestXMLFolder = "Data/XML/Quests/";
	std::string commonQuestXMLRootNode = "Quests";
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonQuestXMLFolder = settings->m_commonQuestXMLFolder;
		commonQuestXMLRootNode = settings->m_commonQuestXMLRoot;
	}

	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(commonQuestXMLFolder, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadInQuestGeneratorFromFileLocation(settings, fileStr, commonQuestXMLRootNode);
	}
}

STATIC bool QuestGenerator::OnUpdateReadInGeneratorsFromCommonQuestGeneratorsFolder()
{
	if (s_loadingSave == nullptr)
	{
		s_loadingSave = new STATIC QuestGeneratorLoadingSave();
	}
	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingSave->m_readInFiles)
	{
		s_loadingSave->m_readInFiles = true;
		if (settings != nullptr)
		{
			s_loadingSave->m_commonQuestXMLFolder = settings->m_commonQuestXMLFolder;
			s_loadingSave->m_commonQuestXMLRootNode = settings->m_commonQuestXMLRoot;
			s_loadingSave->m_amountOfTimeToTakeToReadInQuestsPerFrame = settings->m_amountOfTimeLoadingQuestsPerFrame;
		}
		s_loadingSave->m_files = FileUtils::EnumerateFilesInDirectory(s_loadingSave->m_commonQuestXMLFolder,
			"*", true);
		if (s_loadingSave->m_files.size() == 0)
		{
			return true;
		}
		return false;
	}
	int numberOfQuestsReadThisFrame = 0;
	if (s_loadingSave->m_amountOfTimeToTakeToReadInQuestsPerFrame <= 0)
	{
		return true;
	}

	while ((size_t)s_loadingSave->m_currentFileIndex < s_loadingSave->m_files.size())
	{
		std::string fileStr = s_loadingSave->m_files[s_loadingSave->m_currentFileIndex];
		ReadInQuestGeneratorFromFileLocation(settings, fileStr, s_loadingSave->m_commonQuestXMLRootNode);
		s_loadingSave->m_currentFileIndex++;
		numberOfQuestsReadThisFrame++;

		s_loadingSave->m_amountOfTimeToTakeToReadInQuestsPerFrame;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingSave->m_amountOfTimeToTakeToReadInQuestsPerFrame)
		{
			return false;
		}
	}

	if ((size_t)s_loadingSave->m_currentFileIndex >= s_loadingSave->m_files.size())
	{
		return true;
	}
	return false;
}

STATIC void QuestGenerator::ReadInQuestGeneratorFromFileLocation(
	GameSettings* settings,
	const std::string& fileStr,
	const std::string& commonQuestXMLRootNode)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, commonQuestXMLRootNode.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode QuestNode = root.getChildNode(childIdx);
		{
			QuestGenerator newGenerator = QuestGenerator(QuestNode);
			if (s_QuestGenerators == nullptr)
			{
				s_QuestGenerators = new std::vector<QuestGenerator>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountQuestGenerators;
				}
				s_QuestGenerators->reserve(preallocationAmt);
			}
			s_QuestGenerators->push_back(newGenerator);
		}
	}
}

STATIC const QuestGenerator* QuestGenerator::GetQuestGeneratorByQuestName(const char* name)
{
	if (name == nullptr)
	{
		return nullptr;
	}
	std::string strName = std::string(name);
	return GetQuestGeneratorByQuestName(strName);
}

STATIC const QuestGenerator* QuestGenerator::GetQuestGeneratorByQuestName(const std::string& name)
{
	if (s_QuestGenerators == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);
	for (size_t questGenIdx = 0; questGenIdx < s_QuestGenerators->size(); questGenIdx++)
	{
		QuestGenerator* gen = &s_QuestGenerators->at(questGenIdx);
		if (gen == nullptr)
		{
			continue;
		}
		if (gen->m_QuestLowerCase == lowerCase)
		{
			return gen;
		}
	}
	return nullptr;
}

STATIC const QuestGenerator* QuestGenerator::GetQuestGeneratorByIndex(size_t index)
{
	if (s_QuestGenerators == nullptr
		|| index >= s_QuestGenerators->size())
	{
		return nullptr;
	}

	return &s_QuestGenerators->at(index);
}

STATIC Quest* QuestGenerator::SpawnQuestByName(const std::string& name, 
	const XMLNode& mapQuestNode, const std::string& currentMapFileName,
	MapFiles* files)
{
	if (s_QuestGenerators == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);
	for (size_t idx = 0; idx < s_QuestGenerators->size(); idx++)
	{
		QuestGenerator* generator = &s_QuestGenerators->at(idx);
		if (generator != nullptr && generator->GetQuestNameLowerCase() == lowerCase)
		{
			MapQuestImportantData data;
			ParseMapQuestNodeForImportantQuestData(mapQuestNode, data);
			return generator->SpawnQuest(currentMapFileName, mapQuestNode, data, files);
		}
	}
	return nullptr;
}

STATIC Quest* QuestGenerator::SpawnQuestByIndex(size_t index, 
	const XMLNode& mapQuestNode, const std::string& currentMapFileName,
	MapFiles* files)
{
	if (s_QuestGenerators == nullptr
		|| index >= s_QuestGenerators->size())
	{
		return nullptr;
	}
	MapQuestImportantData data;
	ParseMapQuestNodeForImportantQuestData(mapQuestNode, data);
	return s_QuestGenerators->at(index).SpawnQuest(currentMapFileName, mapQuestNode, data, files);
}

STATIC void QuestGenerator::ParseMapQuestNodeForImportantQuestData(
	const XMLNode& mapQuestNode, MapQuestImportantData& data)
{
	for (int childIdx = 0; childIdx < mapQuestNode.nChildNode(); childIdx++)
	{
		XMLNode childNode = mapQuestNode.getChildNode(childIdx);

		
		const Map* currentMap = Map::GetMap();
		std::vector<TilePositionsByEntityName> positions;
		std::string currentMapName = "";
		if (currentMap != nullptr)
		{
			currentMapName = MakeLower(currentMap->GetMapName());
		}
		positions.push_back(TilePositionsByEntityName());
		positions[0].m_mapName = currentMapName;


		for (int mapFileChildIdx = 0; mapFileChildIdx < childNode.nChildNode(); mapFileChildIdx++)
		{
			XMLNode mapFileChildNode = childNode.getChildNode(mapFileChildIdx);
			std::string childName = MakeLower(mapFileChildNode.getName());
			if (childName == "positions")
			{
				ParseMapQuestNodeForPositionalData(mapFileChildNode, positions, currentMapName);
			}
		}
		data.AddPositionData(positions);
	}
}

STATIC void QuestGenerator::ParseMapQuestNodeForPositionalData(const XMLNode& mapFileChildNode,
	std::vector<TilePositionsByEntityName>& positions,
	const std::string& currentMapName)
{
	std::string entityName = "";
	std::string mapName = currentMapName;
	for (int attrIdx = 0; attrIdx < mapFileChildNode.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = mapFileChildNode.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "entity"
			|| attrName == "entityname"
			|| attrName == "forentity"
			|| attrName == "name")
		{
			entityName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "map"
			|| attrName == "mapname")
		{
			mapName = MakeLower(attr.lpszValue);
		}
	}
	bool found = false;
	int idx = 0;
	for (size_t i = 0; i < positions.size(); i++)
	{
		if (positions[i].m_entityName == entityName
			&& positions[i].m_mapName == mapName)
		{
			idx = i;
			found = true;
			break;
		}
	}
	TilePositionsByEntityName* val = nullptr;
	if (found)
	{
		val = &positions[idx];
	}
	else
	{
		TilePositionsByEntityName valIn;
		valIn.m_entityName = entityName;
		valIn.m_mapName = mapName;
		positions.push_back(valIn);
		val = &positions[positions.size() - 1];
	}

	for (int positionIdx = 0; positionIdx < mapFileChildNode.nChildNode(); positionIdx++)
	{
		XMLNode positionNode = mapFileChildNode.getChildNode(positionIdx);

		for (int attrIdx = 0; attrIdx < positionNode.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = positionNode.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "position"
				|| attrName == "pos")
			{
				bool sizeOf1Found = false;
				IntVector2 pos = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1Found);
				if (sizeOf1Found)
				{
					val->m_possiblePositions.push_back(pos);
				}
			}
			else if (attrName == "positionrange"
				|| attrName == "positionsrange")
			{
				IntVector2 min;
				IntVector2 max;
				std::vector<std::string> parsed = ParseString(attr.lpszValue, ',');
				if (parsed.size() > 0)
				{
					min.x = atoi(parsed[0].c_str());
				}
				if (parsed.size() > 1)
				{
					min.y = atoi(parsed[1].c_str());
				}

				if (parsed.size() > 2)
				{
					max.x = atoi(parsed[2].c_str());
				}
				if (parsed.size() > 3)
				{
					max.y = atoi(parsed[3].c_str());
				}

				if (parsed.size() > 0)
				{
					if (min.x > max.x)
					{
						int temp = max.x;
						max.x = min.x;
						min.x = temp;
					}
					if (min.y > max.y)
					{
						int temp = max.y;
						max.y = min.y;
						min.y = temp;
					}

					for (int x = min.x; x <= max.x; x++)
					{
						for (int y = min.y; y <= max.y; y++)
						{
							TILE_COORDS coords(x, y);
							val->m_possiblePositions.push_back(coords);
						}
					}
				}
			}
		}
	}
}