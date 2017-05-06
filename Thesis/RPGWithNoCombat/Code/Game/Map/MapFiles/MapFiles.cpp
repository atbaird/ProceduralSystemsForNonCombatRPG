#include "Game/Map/MapFiles/MapFiles.hpp"
#include "Game/Map/Map.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Game/Common/GameCommonVars.hpp"
#include "Game/Quest/QuestGenerator.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/GameEntities/Agents/AgentGenerator.hpp"
#include "Game/GameEntities/Features/FeatureGenerator.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/TheGame.hpp"
#include "Game/GameEntities/Agents/AgentTemplateNPCAttributesContainer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

void MapLoadingStream::ClearMapLoadingStreamData()
{
	m_rootNode = XMLNode::emptyNode();
	m_loadedBasicData = false;
	m_mapFileName = "";
	m_mapToLoadOn = nullptr;
	m_loadInNewMap = false;
	m_loadedXML = false;
	m_loadedImage = false;
	m_loadedQuestAgents = false;
	m_AddedAllOfTheDependencies = false;
	m_nodeIndex = 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Static Variables
STATIC std::vector<MapFiles>* MapFiles::s_MapFiles = nullptr;
STATIC MapFilesWhileLoadingSave* MapFiles::s_MapFilesWhileLoadingSave = nullptr;
STATIC std::vector<MapLoadingStream>* MapFiles::s_MapStreaming;
STATIC uint64_t MapFiles::s_startFrame = 0;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors
MapFiles::MapFiles()
{

}

MapFiles::MapFiles(const std::string& fileName, const std::string& dialogueMapName,
	const std::string ImageFileLoc, const std::string& mapDataLoc)
	: m_DialogueMapName(dialogueMapName),
	m_ImageFileLoc(ImageFileLoc),
	m_MapDataLoc(mapDataLoc)
{
	SetMapFileName(fileName);
}

MapFiles::MapFiles(const XMLNode& node)
{
	m_MapFileName = MakeLower(node.getName());
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "image" || attrName == "imagedata")
		{
			m_ImageFileLoc = std::string(attr.lpszValue);
		}
		else if (attrName == "map" || attrName == "mapdata")
		{
			m_MapDataLoc = std::string(attr.lpszValue);
		}
		else if (attrName == "dialoguename"
			|| attrName == "dialogue")
		{
			m_DialogueMapName = std::string(attr.lpszValue);
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "dependencies"
			|| childName == "mapdependencies")
		{
			for (int dependencyIdx = 0; dependencyIdx < child.nChildNode(); dependencyIdx++)
			{
				XMLNode dependency = child.getChildNode(dependencyIdx);
				std::string MapDependency = MakeLower(dependency.getName());
				m_DependenciesAkaOtherMaps.push_back(MapDependency);
			}
		}
	}
}

MapFiles::~MapFiles()
{

}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Setters
STATIC void MapFiles::SearchForRandomPosition(GameEntityToGenDataRelatingToQuest& entityData,
	MapQuestImportantData& mapQuestdata,
	const std::string& entityName,
	const std::string& mapNameSearchingFor,
	bool& found)
{
	for (size_t mapIdx = 0; mapIdx < mapQuestdata.acceptablePositionsPerMap.size(); mapIdx++)
	{
		std::string mapName = MakeLower(mapQuestdata.acceptablePositionsPerMap[mapIdx].m_mapName);
		std::string entityNam = MakeLower(mapQuestdata.acceptablePositionsPerMap[mapIdx].m_entityName);
		if (!found
			&& mapName == mapNameSearchingFor
			&& entityNam == entityName)
		{
			if (mapQuestdata.acceptablePositionsPerMap[mapIdx].m_possiblePositions.size() > 0)
			{
				size_t randIdx = GetRandomIntLessThan(
					mapQuestdata.acceptablePositionsPerMap[mapIdx].m_possiblePositions.size());
				IntVector2 TileCoords =
					mapQuestdata.acceptablePositionsPerMap[mapIdx].m_possiblePositions[randIdx];
				Map* map = Map::GetEditableMap();
				mapQuestdata.acceptablePositionsPerMap[mapIdx].m_possiblePositions.erase(
					mapQuestdata.acceptablePositionsPerMap[mapIdx].m_possiblePositions.begin() + randIdx);
				entityData.m_positionSet = true;
				entityData.m_position = map->GetCenterOfTileAtTileCoords(TileCoords);
				found = true;
				break;
			}
		}
	}
}

void MapFiles::AddAgentToGenWhenNextOpen(const XMLNode& node,
	const XMLNode& mapQuestNode, const std::string& questTitle,
	MapQuestImportantData& mapQuestdata, const std::string& entityName,
	bool forceSpawnNew, const std::string& globalVariableName)
{
	GameEntityToGenDataRelatingToQuest entityData;
	entityData.m_node = node;
	entityData.m_questNodeFromMap = mapQuestNode;
	entityData.m_EntityName = node.getName();
	entityData.m_questTitle = questTitle;
	entityData.m_forceSpawn = forceSpawnNew;
	entityData.m_GlobalVariableName = globalVariableName;

	bool found = false;
	if (mapQuestdata.acceptablePositionsPerMap.size() > 0)
	{
		SearchForRandomPosition(entityData, mapQuestdata, entityName, m_MapFileName, found);

		if (!found && entityName != "")
		{
			SearchForRandomPosition(entityData, mapQuestdata, "", m_MapFileName, found);
		}
	}

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variable"
			|| attrName == "variablename")
		{
			entityData.m_variableName = std::string(attr.lpszValue);
		}
		else if (attrName == "dialogue")
		{
			entityData.m_DialogueToSet = std::string(attr.lpszValue);
			entityData.m_overwriteDialogue = true;
		}
	}
	

	m_agentsForQuestsWaitingToBeSpawned.push_back(entityData);
}

void MapFiles::AddFeatureToGenWhenNextOpen(const XMLNode& node,
	const XMLNode& mapQuestNode,
	const std::string& questTitle,
	MapQuestImportantData& mapQuestdata,
	const std::string& entityName, bool forceSpawnNew, const std::string& globalVariableName)
{
	GameEntityToGenDataRelatingToQuest entityData;
	entityData.m_questNodeFromMap = mapQuestNode;
	entityData.m_node = node;
	entityData.m_EntityName = node.getName();
	entityData.m_questTitle = questTitle;
	entityData.m_forceSpawn = forceSpawnNew;
	entityData.m_GlobalVariableName = globalVariableName;

	bool found = false;
	if (mapQuestdata.acceptablePositionsPerMap.size() > 0)
	{
		SearchForRandomPosition(entityData, mapQuestdata, entityName, m_MapFileName, found);

		if (!found && entityName != "")
		{
			SearchForRandomPosition(entityData, mapQuestdata, "", m_MapFileName, found);
		}
	}

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variable"
			|| attrName == "variablename")
		{
			entityData.m_variableName = std::string(attr.lpszValue);
		}
	}

	m_featuresForQuestsWaitingToBeSpawned.push_back(entityData);
}

void MapFiles::SetMapDialogueName(const std::string& dialogueName)
{
	m_DialogueMapName = dialogueName;
}

void MapFiles::SetMapFileName(const std::string& name)
{
	m_MapFileName = MakeLower(name);
}

void MapFiles::SetImageFileLocation(const std::string& loc)
{
	m_ImageFileLoc = loc;
}

void MapFiles::SetMapDataLocation(const std::string& loc)
{
	m_MapDataLoc = loc;
}

Map* MapFiles::LoadThisMapAllAtOnce()
{
	Map* map = new Map();
	GameCommons::s_AllLoadedMaps.insert(std::pair<std::string, Map*>(m_MapFileName, map));
	GameCommons::s_MapNamesInOrderTheyWereLoaded.push_back(m_MapFileName);
	Map::SetCurrentMap(map);
	map->SetMapName(m_MapFileName);
	map->SetMapDialogueName(m_DialogueMapName);

	LoadThisMapOntoMapAllAtOnce(map);

	return map;
}

void MapFiles::LoadThisMapOntoMapAllAtOnce(Map* map)
{
	if (map == nullptr)
	{
		return;
	}
	map->ReadMapFromImageAllAtOnce(m_ImageFileLoc);
	map->ReadMapSpecialDataFromXMLAllAtOnce(m_MapDataLoc);
	LoadEntitiesAllAtOnce(map);
	map->PlayThisMapsMusic();
}

void MapFiles::LoadEntitiesAllAtOnce(Map* map)
{
	LoadAgentsAllAtOnce(map);
	LoadFeaturesAllAtOnce(map);
}

bool MapFiles::OnUpdateLoadQuestEntities(Map* map, int& numberOfActionsTakenThisFrame, double amountOfTimeToTakePerFrame,
	const uint64_t& startFrameTime)
{

	OnUpdateLoadAgents(map, numberOfActionsTakenThisFrame, amountOfTimeToTakePerFrame, startFrameTime);
	uint64_t currentFrameTime = Performance::GetCurrentPerformanceCount();
	uint64_t timeDiff = currentFrameTime - startFrameTime;
	double timeDiffDouble = Performance::PerformanceCountToSeconds(currentFrameTime);
	if (timeDiffDouble >= amountOfTimeToTakePerFrame)
	{
		return false;
	}
	OnUpdateLoadFeatures(map, numberOfActionsTakenThisFrame, amountOfTimeToTakePerFrame, startFrameTime);

	currentFrameTime = Performance::GetCurrentPerformanceCount();
	timeDiff = currentFrameTime - startFrameTime;
	timeDiffDouble = Performance::PerformanceCountToSeconds(currentFrameTime);
	if (timeDiffDouble >= amountOfTimeToTakePerFrame)
	{
		return false;
	}

	m_currentIndexForAgentsAndFeaturesOnUpdate = 0;
	return true;
}

void MapFiles::LoadAgentsAllAtOnce(Map* map)
{
	if (map == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < m_agentsForQuestsWaitingToBeSpawned.size(); i = i)
	{
		GameEntityToGenDataRelatingToQuest& data = m_agentsForQuestsWaitingToBeSpawned[i];
		HandleSingleQuestAgentDataToLoad(data, map);
		m_agentsForQuestsWaitingToBeSpawned.erase(m_agentsForQuestsWaitingToBeSpawned.begin());
	}
}

bool MapFiles::OnUpdateLoadAgents(Map* map,
	int& numberOfActionsTakenThisFrame, double amountOfTimeToTakePerFrame,
	const uint64_t& startFrameTime)
{
	if (map == nullptr
		|| (size_t)m_currentIndexForAgentsAndFeaturesOnUpdate == m_agentsForQuestsWaitingToBeSpawned.size())
	{
		return true;
	}

	
	if (s_MapStreaming != nullptr && s_MapStreaming->size() > 0)
	{
		MapLoadingStream& stream = s_MapStreaming->at(s_MapStreaming->size() - 1);
		stream.m_timeStartedLoadingDataType = Performance::GetCurrentPerformanceCount();
	}

	while ((size_t)m_currentIndexForAgentsAndFeaturesOnUpdate < m_agentsForQuestsWaitingToBeSpawned.size())
	{
		GameEntityToGenDataRelatingToQuest& data = 
			m_agentsForQuestsWaitingToBeSpawned[0];
		HandleSingleQuestAgentDataToLoad(data, map);
		m_agentsForQuestsWaitingToBeSpawned.erase(m_agentsForQuestsWaitingToBeSpawned.begin());
		numberOfActionsTakenThisFrame++;
		uint64_t currentTime = Performance::GetCurrentPerformanceCount();
		uint64_t timeDiff = currentTime - startFrameTime;
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= amountOfTimeToTakePerFrame)
		{
			if (s_MapStreaming != nullptr && s_MapStreaming->size() > 0)
			{
				MapLoadingStream& stream = s_MapStreaming->at(s_MapStreaming->size() - 1);
				uint64_t agentTimeDiff = currentTime - stream.m_timeStartedLoadingDataType;
				double secondsForAgentsThisFrame = Performance::PerformanceCountToSeconds(
					agentTimeDiff);
				stream.m_amountOfTimeSpentOnAgents += secondsForAgentsThisFrame;
			}
			return false;
		}
	}

	if (s_MapStreaming != nullptr && s_MapStreaming->size() > 0)
	{
		MapLoadingStream& stream = s_MapStreaming->at(s_MapStreaming->size() - 1);
		uint64_t currentTime = Performance::GetCurrentPerformanceCount();
		uint64_t agentTimeDiff = currentTime - stream.m_timeStartedLoadingDataType;
		double secondsForAgentsThisFrame = Performance::PerformanceCountToSeconds(
			agentTimeDiff);
		stream.m_amountOfTimeSpentOnAgents += secondsForAgentsThisFrame;
		DebuggerPrintf("\nSeconds spent loading quest agents this map: %.5f\n",
			(float)stream.m_amountOfTimeSpentOnAgents);

	}

	return true;
}

void MapFiles::HandleSingleQuestAgentDataToLoad(GameEntityToGenDataRelatingToQuest& data, Map* map)
{
	if (map == nullptr)
	{
		return;
	}
	if (data.m_GlobalVariableName != "")
	{
		BaseGameEntity* entity = nullptr;
		Map::GetGlobalVariable(data.m_GlobalVariableName, entity, nullptr);
		Agent* agent = dynamic_cast<Agent*>(entity);
		if (agent != nullptr)
		{
			NPC* npc = dynamic_cast<NPC*>(agent);
			if (npc != nullptr)
			{
				npc->SetQuestCurrentlyClaimedBy(data.m_questTitle);
			}
			if (map != nullptr)
			{
				Quest* quest = map->GetQuestByTitle(data.m_questTitle);
				if (quest != nullptr)
				{
					quest->SetVariable<BaseGameEntity*>(data.m_variableName, entity);
				}
			}
			BaseGameEntity* playerEntity = BaseGameEntity::GetEditablePointerToPlayer();
			Player* player = dynamic_cast<Player*>(playerEntity);
			if (player != nullptr)
			{
				PlayerQuestManager* manager = player->GetEditablePlayerQuestManagerPtr();
				Quest* quest = manager->GetEditableQuestByName(data.m_questTitle);
				if (quest != nullptr)
				{
					quest->SetVariable<BaseGameEntity*>(data.m_variableName, entity);
				}
			}
			return;
		}
		else if (agent == nullptr && entity != nullptr)
		{
			DebuggerPrintf("Warning! Global Variable %s exists, but not as an NPC!", data.m_GlobalVariableName);
		}
	}

	if (data.m_positionSet == false)
	{
		data.m_position = map->GetRandomNonSolidPositionOnMap(true);
	}
	if (!data.m_forceSpawn)
	{
		if (TryGetAlreadyCreatedAgent(data))
		{
			return;
		}
	}

	BaseGameEntity* agent =
		AgentGenerator::SpawnNPCInAgentGeneratorByAgentName(data.m_EntityName.c_str(), data.m_position);
	if (agent == nullptr)
	{
		return;
	}
	NPC* npc = dynamic_cast<NPC*>(agent);
	npc->SetQuestCurrentlyClaimedBy(data.m_questTitle);
	HandleSpecialDataForLoadedAgent(agent, data);
	map->RegisterGameEntity(agent);
	map->RegisterNPC(agent);
}

void MapFiles::PullOutNPCAttributeRequirements(GameEntityToGenDataRelatingToQuest& data,
	std::vector<AgentTemplateNPCAttributesContainer>& npcAttributeValsSearchingFor)
{
	for (int childIdx = 0; childIdx < data.m_node.nChildNode(); childIdx++)
	{
		XMLNode child = data.m_node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "npcattributes")
		{
			std::string npcAttributeName = "";
			std::vector<std::string> whiteList;
			std::vector<std::string> blackList;
			bool useWhiteList = false;
			bool useBlackList = false;
			for (int attributeTypeIdx = 0; attributeTypeIdx < child.nChildNode(); attributeTypeIdx++)
			{
				XMLNode attributeType = child.getChildNode(childIdx);
				std::string attributeTypeName = MakeLower(attributeType.getName());
				if (attributeTypeName == "whitelist")
				{
					useWhiteList = true;
					for (int attributeIdx = 0; attributeIdx < attributeType.nChildNode(); attributeIdx++)
					{
						XMLNode attribute = attributeType.getChildNode(attributeIdx);
						whiteList.push_back(MakeLower(attribute.getName()));
					}
				}
				else if (attributeTypeName == "blacklist")
				{
					useBlackList = true;
					for (int attributeIdx = 0; attributeIdx < attributeType.nChildNode(); attributeIdx++)
					{
						XMLNode attribute = attributeType.getChildNode(attributeIdx);
						blackList.push_back(MakeLower(attribute.getName()));
					}
				}
			}
			AgentTemplateNPCAttributesContainer npcAttributeValSearchingFor
				= AgentTemplateNPCAttributesContainer(npcAttributeName,
					whiteList, blackList, useWhiteList, useBlackList);
			if (npcAttributeName != "")
			{
				npcAttributeValsSearchingFor.push_back(npcAttributeValSearchingFor);
			}
		}
	}
}

bool MapFiles::TryGetAlreadyCreatedAgent(GameEntityToGenDataRelatingToQuest& data)
{
	//Search for applicable replacements
	std::vector<AgentTemplateNPCAttributesContainer> npcAttributeValsSearchingFor;
	PullOutNPCAttributeRequirements(data, npcAttributeValsSearchingFor);

	std::string entityName = MakeLower(data.m_node.getName());
	std::vector<BaseGameEntity*> agentsThatMeetQualifications;
	const std::vector<BaseGameEntity*>* npcs = BaseGameEntity::GetAllNPCsPtr();
	if (npcs != nullptr)
	{
		for (size_t npcIdx = 0; npcIdx < npcs->size(); npcIdx++)
		{
			BaseGameEntity* entity = npcs->at(npcIdx);
			NPC* npc = dynamic_cast<NPC*>(entity);
			if (npc != nullptr
				&& npc->GetQuestCurrentlyClaimedBy() == ""
				&& entityName == npc->GetGeneratorUsed())
			{
				bool npcMeetsAllMyStandards = true;
				const std::vector<NPCAttribute>* attributes = npc->GetNPCAttributes();
				for (size_t npcAttrIdx = 0; npcAttrIdx < npcAttributeValsSearchingFor.size(); npcAttrIdx++)
				{
					for (size_t attributeIdx = 0; attributeIdx < attributes->size(); attributeIdx++)
					{
						if (npcAttributeValsSearchingFor[npcAttrIdx].GetNPCAttributeToCall()
							== attributes->at(attributeIdx).GetNPCAttributeName())
						{
							if (!npcAttributeValsSearchingFor[npcAttrIdx]
								.GetSingleAttributeValueMeetsAllRequirements(
									attributes->at(attributeIdx)))
							{
								npcMeetsAllMyStandards = false;
							}

							break;
						}
					}
					if (!npcMeetsAllMyStandards)
					{
						break;
					}
				}
				if (npcMeetsAllMyStandards)
				{
					agentsThatMeetQualifications.push_back(npc);
				}
			}
		}
	}
	if (agentsThatMeetQualifications.size() != 0)
	{
		int randomAgentToTakeOver = GetRandomIntLessThan(agentsThatMeetQualifications.size());
		BaseGameEntity* agent = agentsThatMeetQualifications[randomAgentToTakeOver];
		HandleSpecialDataForLoadedAgent(agent, data);
		agent->SetPosition(data.m_position);
		return true;
	}
	return false;
}

void MapFiles::HandleSpecialDataForLoadedAgent(BaseGameEntity* entity, 
	GameEntityToGenDataRelatingToQuest& data)
{
	if (entity == nullptr)
	{
		return;
	}
	NPC* npc = dynamic_cast<NPC*>(entity);
	if (npc == nullptr)
	{
		return;
	}

	if (data.m_GlobalVariableName != "")
	{
		BaseGameEntity* entityTest = nullptr;
		Map::GetGlobalVariable(data.m_GlobalVariableName, entityTest, nullptr);
		if (entityTest == nullptr)
		{
			Map::SetGlobalVariable(data.m_GlobalVariableName, entity);
		}
	}

	if (data.m_variableName != "")
	{
		Map* map = Map::GetEditableMap();
		BaseGameEntity* entityPlayer = BaseGameEntity::GetEditablePointerToPlayer();
		Player* player = dynamic_cast<Player*>(entityPlayer);
		bool found = false;
		if (map != nullptr)
		{
			Quest* quest = map->GetQuestByTitle(data.m_questTitle);
			if (quest != nullptr)
			{
				quest->SetVariable<BaseGameEntity*>(data.m_variableName, entity);
				found = true;
				npc->SetQuestCurrentlyClaimedBy(data.m_questTitle);
				if (data.m_overwriteDialogue)
				{
					npc->SetDialogueToOpenIfSpokenTo(data.m_DialogueToSet);
				}
			}
		}
		if (found == false
			&& player != nullptr)
		{
			PlayerQuestManager* manager = player->GetEditablePlayerQuestManagerPtr();
			Quest* quest = manager->GetEditableQuestByName(data.m_questTitle);
			if (quest != nullptr)
			{
				quest->SetVariable<BaseGameEntity*>(data.m_variableName, entity);
				npc->SetQuestCurrentlyClaimedBy(data.m_questTitle);
				if (data.m_overwriteDialogue)
				{
					npc->SetDialogueToOpenIfSpokenTo(data.m_DialogueToSet);
				}
			}
		}
	}

}

void MapFiles::LoadFeaturesAllAtOnce(Map* map)
{
	if (map == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < m_featuresForQuestsWaitingToBeSpawned.size(); i = i)
	{
		GameEntityToGenDataRelatingToQuest& data = m_featuresForQuestsWaitingToBeSpawned[i];
		HandleSingleQuestFeatureDataToLoad(data, map);
		m_featuresForQuestsWaitingToBeSpawned.erase(m_featuresForQuestsWaitingToBeSpawned.begin());
	}
}

bool MapFiles::OnUpdateLoadFeatures(Map* map, int& numberOfActionsTakenThisFrame, double amountOfTimeToTakePerFrame,
	const uint64_t& startFrameTime)
{
	if (map == nullptr
		|| (size_t)m_currentIndexForAgentsAndFeaturesOnUpdate < m_featuresForQuestsWaitingToBeSpawned.size())
	{
		return true;
	}

	if (s_MapStreaming != nullptr && s_MapStreaming->size() > 0)
	{
		MapLoadingStream& stream = s_MapStreaming->at(s_MapStreaming->size() - 1);
		stream.m_timeStartedLoadingDataType = Performance::GetCurrentPerformanceCount();
	}

	while ((size_t)m_currentIndexForAgentsAndFeaturesOnUpdate < m_featuresForQuestsWaitingToBeSpawned.size())
	{
		GameEntityToGenDataRelatingToQuest& data = 
			m_featuresForQuestsWaitingToBeSpawned[0];
		HandleSingleQuestFeatureDataToLoad(data, map);
		m_featuresForQuestsWaitingToBeSpawned.erase(m_featuresForQuestsWaitingToBeSpawned.begin());
		numberOfActionsTakenThisFrame++;
		uint64_t currentTime = Performance::GetCurrentPerformanceCount();
		uint64_t timeDiff = currentTime - startFrameTime;
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= amountOfTimeToTakePerFrame)
		{
			if (s_MapStreaming != nullptr && s_MapStreaming->size() > 0)
			{
				MapLoadingStream& stream = s_MapStreaming->at(s_MapStreaming->size() - 1);
				uint64_t agentTimeDiff = currentTime - stream.m_timeStartedLoadingDataType;
				double secondsForFeaturesThisFrame = Performance::PerformanceCountToSeconds(
					agentTimeDiff);
				stream.m_amountOfTimeSpentOnFeatures += secondsForFeaturesThisFrame;
			}
			return false;
		}
	}

	if (s_MapStreaming != nullptr && s_MapStreaming->size() > 0)
	{
		MapLoadingStream& stream = s_MapStreaming->at(s_MapStreaming->size() - 1);
		uint64_t currentTime = Performance::GetCurrentPerformanceCount();
		uint64_t agentTimeDiff = currentTime - stream.m_timeStartedLoadingDataType;
		double secondsForFeaturesThisFrame = Performance::PerformanceCountToSeconds(
			agentTimeDiff);
		stream.m_amountOfTimeSpentOnFeatures += secondsForFeaturesThisFrame;
		DebuggerPrintf("\nSeconds spent loading quest features this map: %.5f\n",
			(float)stream.m_amountOfTimeSpentOnFeatures);

	}
	return true;
}

void MapFiles::HandleSingleQuestFeatureDataToLoad(GameEntityToGenDataRelatingToQuest& data, Map* map)
{
	if (map == nullptr)
	{
		return;
	}

	if (data.m_GlobalVariableName != "")
	{
		BaseGameEntity* entity = nullptr;
		Map::GetGlobalVariable(data.m_GlobalVariableName, entity, nullptr);
		Feature* feature = dynamic_cast<Feature*>(entity);
		if (feature != nullptr)
		{
			{
				Quest* quest = map->GetQuestByTitle(data.m_questTitle);
				if (quest != nullptr)
				{
					quest->SetVariable<BaseGameEntity*>(data.m_variableName, entity);
				}
			}
			BaseGameEntity* playerEntity = BaseGameEntity::GetEditablePointerToPlayer();
			Player* player = dynamic_cast<Player*>(playerEntity);
			if (player != nullptr)
			{
				PlayerQuestManager* manager = player->GetEditablePlayerQuestManagerPtr();
				Quest* quest = manager->GetEditableQuestByName(data.m_questTitle);
				if (quest != nullptr)
				{
					quest->SetVariable<BaseGameEntity*>(data.m_variableName, entity);
				}
			}
			return;
		}
		else if (entity != nullptr && feature == nullptr)
		{
			DebuggerPrintf("\nWarning! Global variable name for %s already exists, but not as a Feature!", data.m_GlobalVariableName);
		}
	}

	if (data.m_positionSet == false)
	{
		data.m_position = map->GetRandomNonSolidPositionOnMap(true);
	}
	if (!data.m_forceSpawn)
	{
		if (TryGetAlreadyCreatedFeature(data))
		{
			return;
		}
	}

	BaseGameEntity* entity =
		FeatureGenerator::SpawnFeatureByFeatureName(data.m_EntityName.c_str(), data.m_position, map);
	if (entity == nullptr)
	{
		return;
	}
	HandleSpecialDataForLoadedFeature(entity, data);
	map->RegisterGameEntity(entity);
	map->RegisterFeature(entity);
}

bool MapFiles::TryGetAlreadyCreatedFeature(GameEntityToGenDataRelatingToQuest& data)
{
	const std::vector<BaseGameEntity*>* features = BaseGameEntity::GetAllFeaturesPtr();
	if (features == nullptr)
	{
		return false;
	}
	std::string featureGeneratorName = MakeLower(data.m_node.getName());

	std::vector<BaseGameEntity*> featuresThatMeetAllMyStandards;
	for (size_t featureIdx = 0; featureIdx < features->size(); featureIdx++)
	{
		BaseGameEntity* entity = features->at(featureIdx);
		Feature* feature = dynamic_cast<Feature*>(entity);
		if (feature == nullptr)
		{
			continue;
		}
		bool featureMeetsAllMyStandards = true;

		if (feature->GetRelatedQuestTitle() != ""
			|| feature->GetGeneratorUsed() != featureGeneratorName)
		{
			continue;
		}

		if (featureMeetsAllMyStandards)
		{
			featuresThatMeetAllMyStandards.push_back(feature);
		}
	}

	if (featuresThatMeetAllMyStandards.size() == 0)
	{
		return false;
	}
	int randomAgentToTakeOver = GetRandomIntLessThan(featuresThatMeetAllMyStandards.size());
	BaseGameEntity* feature = featuresThatMeetAllMyStandards[randomAgentToTakeOver];
	HandleSpecialDataForLoadedFeature(feature, data);
	feature->SetPosition((data.m_position));
	return true;

}

void MapFiles::HandleSpecialDataForLoadedFeature(BaseGameEntity* entity,
	GameEntityToGenDataRelatingToQuest& data)
{
	if (entity == nullptr)
	{
		return;
	}
	Feature* feature = dynamic_cast<Feature*>(entity);
	if (feature == nullptr)
	{
		return;
	}

	if (data.m_GlobalVariableName != "")
	{
		BaseGameEntity* entityTest = nullptr;
		Map::GetGlobalVariable(data.m_GlobalVariableName, entityTest, nullptr);
		if (entityTest == nullptr)
		{
			Map::SetGlobalVariable(data.m_GlobalVariableName, entity);
		}
	}

	if (data.m_variableName != "")
	{
		Map* map = Map::GetEditableMap();
		BaseGameEntity* entityPlayer = BaseGameEntity::GetEditablePointerToPlayer();
		Player* player = dynamic_cast<Player*>(entityPlayer);
		bool found = false;
		if (map != nullptr)
		{
			Quest* quest = map->GetQuestByTitle(data.m_questTitle);
			if (quest != nullptr)
			{
				quest->SetVariable<BaseGameEntity*>(data.m_variableName, entity);
				found = true;
				feature->SetRelatedQuest(data.m_questTitle);
			}
		}
		if (found == false
			&& player != nullptr)
		{
			PlayerQuestManager* manager = player->GetEditablePlayerQuestManagerPtr();
			Quest* quest = manager->GetEditableQuestByName(data.m_questTitle);
			if (quest != nullptr)
			{
				quest->SetVariable<BaseGameEntity*>(data.m_variableName, entity);
				feature->SetRelatedQuest(data.m_questTitle);
			}
		}
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Getters
const std::string& MapFiles::GetMapDialogueName() const
{
	return m_DialogueMapName;
}

const std::string& MapFiles::GetMapFileName() const
{
	return m_MapFileName;
}

const std::string& MapFiles::GetImageFileLocation() const
{
	return m_ImageFileLoc;
}

const std::string& MapFiles::GetMapDataLocation() const
{
	return m_MapDataLoc;
}

void MapFiles::WriteMapFileToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Map File Name: " + m_MapFileName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Map File Name: " + m_DialogueMapName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Map Image File Path: " + m_ImageFileLoc + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Map Metadata File Path: " + m_MapDataLoc + "\n";

	int nextLevel = indentationAmt + 3;
	if (m_DependenciesAkaOtherMaps.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Map Dependencies: \n";
		for (size_t i = 0; i < m_DependenciesAkaOtherMaps.size(); i++)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel);
			str += m_DialogueMapName[i] + "\n";
		}
	}

	if (m_agentsForQuestsWaitingToBeSpawned.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Number of Agents waiting to be Spawned for Quests: " + 
			std::to_string(m_agentsForQuestsWaitingToBeSpawned.size()) + "\n";
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Agents waiting to be Spawned for Quests: \n";
		for (size_t i = 0; i < m_agentsForQuestsWaitingToBeSpawned.size(); i++)
		{
			const GameEntityToGenDataRelatingToQuest& entityData = m_agentsForQuestsWaitingToBeSpawned[i];
			entityData.WriteToString(str, nextLevel);
		}
	}

	if (m_featuresForQuestsWaitingToBeSpawned.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Number of Features waiting to be spawned for Quests: " +
			std::to_string(m_featuresForQuestsWaitingToBeSpawned.size()) + "\n";
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Features waiting to be Spawned for Quests: \n";
		for (size_t i = 0; i < m_featuresForQuestsWaitingToBeSpawned.size(); i++)
		{
			const GameEntityToGenDataRelatingToQuest& entityData = m_featuresForQuestsWaitingToBeSpawned[i];
			entityData.WriteToString(str, nextLevel);
		}
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Static Functions
STATIC void MapFiles::WriteAllMapFilesToString(std::string& str, int indentationAmt)
{
	if (s_MapFiles == nullptr)
	{
		return;
	}
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of Map Files: " + std::to_string(s_MapFiles->size()) + "\n";

	for (size_t i = 0; i < s_MapFiles->size(); i++)
	{
		const MapFiles& mapFile = s_MapFiles->at(i);
		mapFile.WriteMapFileToString(str, indentationAmt);
	}
}

STATIC bool MapFiles::OnUpdateLoadMapDataAndImage(MapLoadingStream*& stream, Map*& map, MapFiles*& files,
	int& actionTakenThisFrame, uint64_t& currentTime, uint64_t& timeDiff, double& timeDiffDouble)
{
	if (!stream->m_loadedImage)
	{
		if (UpdateMapLoadingImage(files, stream, map, actionTakenThisFrame))
		{
			return false;
		}
	}

	if (!stream->m_loadedXML)
	{
		stream->m_timeStartedLoadingDataType = Performance::GetCurrentPerformanceCount();
		stream->m_loadedXML = map->OnUpdateReadMapSpecialDataFromXML(files->m_MapDataLoc,
			actionTakenThisFrame, stream->m_amountOfTimeToTakeLoadingPerFrame, s_startFrame, nullptr);

		if (stream->m_loadedXML)
		{
			currentTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentTime - stream->m_timeStartedLoadingDataType;
			stream->m_amountOfTimeSpentParsingMapSpecialData += Performance::PerformanceCountToSeconds(timeDiff);
			DebuggerPrintf("\nAmount of time spent parsing map data for this map: %.5f\n",
				(float)stream->m_amountOfTimeSpentParsingMapSpecialData);
		}
		else
		{
			currentTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentTime - stream->m_timeStartedLoadingDataType;
			stream->m_amountOfTimeSpentParsingMapSpecialData += Performance::PerformanceCountToSeconds(timeDiff);
		}

		currentTime = Performance::GetCurrentPerformanceCount();
		timeDiff = currentTime - s_startFrame;
		timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= stream->m_amountOfTimeToTakeLoadingPerFrame)
		{
			return false;
		}
	}
	return true;
}

STATIC bool MapFiles::UpdateMapLoading()
{
	if (s_MapStreaming == nullptr
		|| s_MapStreaming->size() == 0)
	{
		return true;
	}

	s_startFrame = Performance::GetCurrentPerformanceCount();
	MapLoadingStream* stream = &s_MapStreaming->at(s_MapStreaming->size() - 1);
	Map* map = stream->m_mapToLoadOn;
	MapFiles* files = nullptr;
	if (s_MapFiles == nullptr)
	{
		return ClearAMapFileOffTheStack();
	}
	UpdateMapLoadingGetFilesForStream(files, stream);
	if (files == nullptr)
	{
		delete stream->m_mapToLoadOn;
		return ClearAMapFileOffTheStack();
	}

	if (!stream->m_AddedAllOfTheDependencies)
	{
		UpdateMapLoadingDependences(files, stream);
		return false;
	}


	if (map == nullptr)
	{
		stream->m_loadInNewMap = false;
		return ClearAMapFileOffTheStack();
	}

	Map::SetCurrentMap(map, false);
	int actionTakenThisFrame = 0;
	if (!stream->m_loadedBasicData)
	{
		UpdateMapLoadingBasicData(files, stream, map, actionTakenThisFrame);
	}

	uint64_t currentTime = Performance::GetCurrentPerformanceCount();
	uint64_t timeDiff = currentTime - s_startFrame;
	double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
	if (timeDiffDouble >= stream->m_amountOfTimeToTakeLoadingPerFrame)
	{
		return false;
	}

	
	if (!stream->m_MapAlreadyExisted 
		&& !OnUpdateLoadMapDataAndImage(stream, map, files, actionTakenThisFrame,
		currentTime, timeDiff, timeDiffDouble))
	{
		return false;
	}
	

	if (!stream->m_loadedQuestAgents)
	{
		stream->m_loadedQuestAgents =
			files->OnUpdateLoadQuestEntities(map, actionTakenThisFrame,
				stream->m_amountOfTimeToTakeLoadingPerFrame, s_startFrame);

		currentTime = Performance::GetCurrentPerformanceCount();
		timeDiff = currentTime - s_startFrame;
		timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= stream->m_amountOfTimeToTakeLoadingPerFrame)
		{
			return false;
		}
	}

	if (stream->m_registerTileEvents)
	{
		map->PlayThisMapsMusic();
		Map::SetCurrentMap(map);
	}
	else
	{
		map->UnregisterAllTilesFromEvents();
	}
	stream->m_loadInNewMap = false;

	return ClearAMapFileOffTheStack();
}

STATIC bool MapFiles::GetLoadingMap()
{
	return s_MapStreaming != nullptr && s_MapStreaming->size() != 0;
}

STATIC void MapFiles::UpdateMapLoadingGetFilesForStream(MapFiles*& files, MapLoadingStream*& stream)
{
	for (size_t i = 0; i < s_MapFiles->size(); i++)
	{
		if (stream->m_mapFileName == s_MapFiles->at(i).m_MapFileName)
		{
			files = &s_MapFiles->at(i);
			break;
		}
	}
}

STATIC bool MapFiles::CheckDependenciesForIfFileExists(const MapFiles& filesChecking, MapFiles*& files)
{
	for (size_t depenIdx = 0; depenIdx < filesChecking.m_DependenciesAkaOtherMaps.size(); depenIdx++)
	{
		//Then we verify that the file we were pushing forwards dependencies don't match 
		//our current file set we were originally searching.
		if (filesChecking.m_DependenciesAkaOtherMaps[depenIdx] == files->m_MapFileName)
		{
			return true;
		}
	}
	return false;
}

STATIC bool MapFiles::CheckIfFileInfiniteLoopOccurs(MapFiles*& files, const std::string& lowerCaseName)
{
	for (size_t streamIdx = 0; streamIdx < s_MapStreaming->size(); streamIdx++)
	{
		MapLoadingStream& loadingStream = s_MapStreaming->at(streamIdx);
		if (lowerCaseName == loadingStream.m_mapFileName)
		{
			//So the stream previously exists, now need to verify if an infinite loop would occur if we pushed it back.
			bool mapDependencyInfiniteLoopDetected = false;
			for (size_t mapFilesIdx = 0; mapFilesIdx < s_MapFiles->size(); mapFilesIdx++)
			{
				//First we get the files that matches our name that we were hoping to shift forward
				const MapFiles& filesChecking = s_MapFiles->at(mapFilesIdx);
				if (filesChecking.m_MapFileName == lowerCaseName)
				{
					mapDependencyInfiniteLoopDetected = CheckDependenciesForIfFileExists(filesChecking, files);
					break;
				}
			}

			if (mapDependencyInfiniteLoopDetected)
			{
				return true;
			}
			else
			{
				break;
			}
		}
	}

	return false;
}

STATIC bool MapFiles::CheckIfMapAlreadyBuilt(const std::string& lowerCaseName)
{
	for (std::map<std::string, Map*>::iterator it = GameCommons::s_AllLoadedMaps.begin();
		it != GameCommons::s_AllLoadedMaps.end(); it++)
	{
		if (it->first == lowerCaseName)
		{
			return true;
		}
	}
	return false;
}


STATIC bool MapFiles::UpdateMapLoadingDependences(MapFiles*& files, MapLoadingStream*& stream)
{
	if (files == nullptr)
	{
		return true;
	}

	for (size_t dependencyIdx = 0; dependencyIdx < files->m_DependenciesAkaOtherMaps.size(); dependencyIdx++)
	{
		std::string lowerCaseName = MakeLower(files->m_DependenciesAkaOtherMaps.at(dependencyIdx));
		bool alreadyBuilt = CheckIfMapAlreadyBuilt(lowerCaseName);
		if (!alreadyBuilt)
		{
			bool alreadyOnTheList = CheckIfFileInfiniteLoopOccurs(files, lowerCaseName);

			if (!alreadyOnTheList)
			{
				MapLoadingStream newStream;
				newStream.ClearMapLoadingStreamData();
				GameSettings* settings = TheGame::GetGameSettings();
				if (settings != nullptr)
				{
					newStream.m_amountOfTimeToTakeLoadingPerFrame = settings->m_amountOfTimeLoadingMapDataPerFrame;
				}

				newStream.m_mapFileName = lowerCaseName;
				newStream.m_mapToLoadOn = new Map();
				newStream.m_loadInNewMap = true;
				newStream.m_registerTileEvents = false;

				s_MapStreaming->push_back(newStream);
			}
		}
	}
	if (stream != nullptr)
	{
		stream->m_AddedAllOfTheDependencies = true;
	}
	return false;
}

STATIC bool MapFiles::UpdateMapLoadingBasicData(MapFiles*& files, MapLoadingStream*& stream,
	Map*& map, int& actionTakenThisFrame)
{
	std::map<std::string, Map*>::iterator it = GameCommons::s_AllLoadedMaps.find(files->m_MapFileName);
	actionTakenThisFrame++;
	if (it != GameCommons::s_AllLoadedMaps.end())
	{
		delete map;
		map = it->second;
		stream->m_mapToLoadOn = map;
		stream->m_MapAlreadyExisted = true;
		return false;
	}

	stream->m_loadedBasicData = true;
	GameCommons::s_AllLoadedMaps.insert(
		std::pair<std::string, Map*>(files->m_MapFileName, map));
	GameCommons::s_MapNamesInOrderTheyWereLoaded.push_back(files->m_MapFileName);
	map->SetMapName(files->m_MapFileName);
	map->SetMapDialogueName(files->m_DialogueMapName);
	return false;
}

STATIC bool MapFiles::UpdateMapLoadingImage(MapFiles*& files, MapLoadingStream*& stream, Map*& map, 
	int& actionTakenThisFrame)
{

	if (stream)
	{
		stream->m_timeStartedLoadingDataType = Performance::GetCurrentPerformanceCount();
	}
	stream->m_loadedImage = map->OnUpdateReadMapFromImage(files->m_ImageFileLoc,
		actionTakenThisFrame, stream->m_amountOfTimeToTakeLoadingPerFrame, s_startFrame);

	uint64_t currentFrameTime = Performance::GetCurrentPerformanceCount();
	uint64_t timeDiff = currentFrameTime - s_startFrame;
	double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
	if (timeDiffDouble >= stream->m_amountOfTimeToTakeLoadingPerFrame)
	{
		if (stream != nullptr)
		{
			uint64_t currentTime = Performance::GetCurrentPerformanceCount();
			uint64_t agentTimeDiff = currentTime - stream->m_timeStartedLoadingDataType;
			double secondsForMapImageThisFrame = Performance::PerformanceCountToSeconds(
				agentTimeDiff);
			stream->m_amountOfTimeSpentLoadingImageData += secondsForMapImageThisFrame;
		}
		return true;
	}
	if (stream != nullptr)
	{
		uint64_t currentTime = Performance::GetCurrentPerformanceCount();
		uint64_t agentTimeDiff = currentTime - stream->m_timeStartedLoadingDataType;
		double secondsForMapImageThisFrame = Performance::PerformanceCountToSeconds(
			agentTimeDiff);
		stream->m_amountOfTimeSpentLoadingImageData += secondsForMapImageThisFrame;
		DebuggerPrintf("\nSeconds spent loading map image for this map: %.5f\n", 
			(float)stream->m_amountOfTimeSpentLoadingImageData);

	}
	return false;
}

STATIC bool MapFiles::ClearAMapFileOffTheStack()
{
	s_MapStreaming->erase(s_MapStreaming->begin() + s_MapStreaming->size() - 1);

	if (s_MapStreaming->size() == 0)
	{
		return true;
	}

	return false;
}

STATIC void MapFiles::ReadInMapFilesFromCommonXMLFileAllAtOnce()
{
	QuestGenerator::ReadInGeneratorsFromCommonQuestGeneratorsFolderAllAtOnce();

	std::string mapFilesCommonXMLFolder = "Data/XML/MapFiles/";
	std::string mapFilesRootNodeName = "MapFiles";

	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		mapFilesCommonXMLFolder = settings->m_mapFilesCommonXMLFolder;
		mapFilesRootNodeName = settings->m_mapFilesRootNodeName;
	}

	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(mapFilesCommonXMLFolder, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadXMLMapFileFromFileLocation(fileStr, mapFilesRootNodeName);
	}
}

STATIC bool MapFiles::OnUpdateReadInMapFilesFromCommonXMLFile()
{
	if (s_MapFilesWhileLoadingSave == nullptr)
	{
		s_MapFilesWhileLoadingSave = new MapFilesWhileLoadingSave();
	}
	if (s_MapFilesWhileLoadingSave->m_presentlyReadingQuestFolderCommonAgentGenerators)
	{
		if (QuestGenerator::OnUpdateReadInGeneratorsFromCommonQuestGeneratorsFolder())
		{
			s_MapFilesWhileLoadingSave->m_presentlyReadingQuestFolderCommonAgentGenerators = false;
		}
		return false;
	}
	else
	{
		return ActuallyOnUpdateReadInMapFilesFromCommonXMLFile();
	}

}


STATIC void MapFiles::ReadXMLMapFileFromFileLocation(const std::string& fileStr,
	const std::string& mapFilesRootNodeName)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, mapFilesRootNodeName.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		MapFiles file = MapFiles(child);
		if (file.m_MapFileName == ""
			|| (file.m_MapDataLoc == "" && file.m_ImageFileLoc == ""))
		{
			continue;
		}
		if (s_MapFiles == nullptr)
		{
			s_MapFiles = new std::vector<MapFiles>();
		}
		s_MapFiles->push_back(file);
	}
}

STATIC bool MapFiles::ActuallyOnUpdateReadInMapFilesFromCommonXMLFile()
{
	if (s_MapFilesWhileLoadingSave == nullptr)
	{
		//Have no data, can't do anything.
		return true;
	}

	if (!s_MapFilesWhileLoadingSave->m_GotAllXMLMapFiles)
	{
		//Load in data first.
		s_MapFilesWhileLoadingSave->m_GotAllXMLMapFiles = true;
		GameSettings* settings = TheGame::GetGameSettings();
		if (settings != nullptr)
		{
			s_MapFilesWhileLoadingSave->m_mapFilesCommonXMLFolder = settings->m_mapFilesCommonXMLFolder;
			s_MapFilesWhileLoadingSave->m_mapFilesRootNodeName = settings->m_mapFilesRootNodeName;
			s_MapFilesWhileLoadingSave->m_amountOfTimeToTakeToReadInMapFilesPerFrame = settings->m_amountOfTimeToSpendLoadingMapFilesPerFrame;
		}
		s_MapFilesWhileLoadingSave->m_fileLocations = FileUtils::EnumerateFilesInDirectory(
			s_MapFilesWhileLoadingSave->m_mapFilesCommonXMLFolder, "*", true);
		if (s_MapFilesWhileLoadingSave->m_fileLocations.size() == 0)
		{
			return true;
		}

		return false;
	}

	if (s_MapFilesWhileLoadingSave->m_amountOfTimeToTakeToReadInMapFilesPerFrame <= 0)
	{
		//Cant do anything.
		return true;
	}
	int numberReadInThisFrame = 0;
	while ((size_t)s_MapFilesWhileLoadingSave->m_currentIndexOfFileLocations
		< s_MapFilesWhileLoadingSave->m_fileLocations.size())
	{
		std::string fileLocation = s_MapFilesWhileLoadingSave->m_fileLocations[
			s_MapFilesWhileLoadingSave->m_currentIndexOfFileLocations];

		ReadXMLMapFileFromFileLocation(fileLocation, s_MapFilesWhileLoadingSave->m_mapFilesRootNodeName);

		s_MapFilesWhileLoadingSave->m_currentIndexOfFileLocations++;
		numberReadInThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_MapFilesWhileLoadingSave->m_amountOfTimeToTakeToReadInMapFilesPerFrame)
		{
			return false;
		}
	}

	if ((size_t)s_MapFilesWhileLoadingSave->m_currentIndexOfFileLocations 
		>= s_MapFilesWhileLoadingSave->m_fileLocations.size())
	{
		return true;
	}
	return false;

}


STATIC void MapFiles::LoadMapFileDataOntoMapAllAtOnce(Map* map, MapFiles* files)
{
	if (map == nullptr
		|| files == nullptr)
	{
		return;
	}
	files->LoadThisMapOntoMapAllAtOnce(map);
}

STATIC void MapFiles::ClearAllMapFiles()
{
	QuestGenerator::ClearAllGenerators();
	if (s_MapStreaming != nullptr)
	{
		delete s_MapStreaming;
		s_MapStreaming = nullptr;
	}
	if (s_MapFiles != nullptr)
	{
		delete s_MapFiles;
		s_MapFiles = nullptr;
	}
	if (s_MapFilesWhileLoadingSave != nullptr)
	{
		delete s_MapFilesWhileLoadingSave;
		s_MapFilesWhileLoadingSave = nullptr;
	}
}

STATIC void MapFiles::LoadMapByNameAllAtOnce(const std::string& mapFileName)
{
	if (s_MapFiles == nullptr)
	{
		return;
	}

	std::string lowerCaseName = MakeLower(mapFileName);
	bool mapAlreadyLoaded = false;
	for (std::map<std::string, Map*>::iterator it = GameCommons::s_AllLoadedMaps.begin();
		it != GameCommons::s_AllLoadedMaps.end(); it++)
	{
		if (it->first == lowerCaseName)
		{
			Map* theMap = it->second;
			Map::SetCurrentMap(theMap);
			for (size_t mapFileIdx = 0; mapFileIdx < s_MapFiles->size(); mapFileIdx++)
			{
				MapFiles& mapFiles = s_MapFiles->at(mapFileIdx);
				if (mapFiles.m_MapFileName == lowerCaseName)
				{
					mapFiles.LoadEntitiesAllAtOnce(theMap);
				}
			}
			theMap->PlayThisMapsMusic();
			mapAlreadyLoaded = true;
			break;
		}
	}

	for (size_t mapFileIdx = 0; mapFileIdx < s_MapFiles->size(); mapFileIdx++)
	{
		MapFiles& mapFiles = s_MapFiles->at(mapFileIdx);
		if (mapFiles.m_MapFileName == lowerCaseName)
		{
			if (mapAlreadyLoaded == false)
			{
				mapFiles.LoadThisMapAllAtOnce();
			}
			mapFiles.LoadEntitiesAllAtOnce(Map::GetEditableMap());
			return;
		}
	}
}

STATIC void MapFiles::LoadMapByNameOntoMapAllAtOnce(const std::string& mapFileName, Map* map)
{
	if (s_MapFiles == nullptr)
	{
		return;
	}
	std::string lowerCaseName = MakeLower(mapFileName);

	for (size_t mapFileIdx = 0; mapFileIdx < s_MapFiles->size(); mapFileIdx++)
	{
		MapFiles& mapFiles = s_MapFiles->at(mapFileIdx);
		if (mapFiles.m_MapFileName == lowerCaseName)
		{
			mapFiles.LoadThisMapOntoMapAllAtOnce(map);
			return;
		}
	}
}

STATIC void MapFiles::LoadMapByNameRegisterForOnUpdate(const std::string& mapFileName)
{
	if (s_MapFiles == nullptr)
	{
		return;
	}

	if (s_MapStreaming == nullptr)
	{
		s_MapStreaming = new STATIC std::vector<MapLoadingStream>();
	}

	std::string lowerCaseName = MakeLower(mapFileName);

	for (size_t mapFileIdx = 0; mapFileIdx < s_MapFiles->size(); mapFileIdx++)
	{
		MapFiles& mapFiles = s_MapFiles->at(mapFileIdx);
		if (mapFiles.m_MapFileName == lowerCaseName)
		{
			
			MapLoadingStream stream;
			stream.ClearMapLoadingStreamData();
			GameSettings* settings = TheGame::GetGameSettings();
			if (settings != nullptr)
			{
				stream.m_amountOfTimeToTakeLoadingPerFrame = settings->m_amountOfTimeLoadingMapDataPerFrame;
			}
			Map* currentMap = Map::GetEditableMap();
			if (currentMap != nullptr)
			{
				currentMap->UnregisterAllTilesFromEvents();
			}

			stream.m_mapFileName = mapFiles.m_MapFileName;
			stream.m_mapToLoadOn = new Map();
			Map::SetCurrentMap(stream.m_mapToLoadOn);
			stream.m_loadInNewMap = true;
			s_MapStreaming->push_back(stream);
			return;
		}
	}
}

STATIC void MapFiles::LoadJustTheEntitiesFromMapFilesAllAtOnce(Map* map, const std::string& mapFileName)
{
	std::string lowerCaseName = MakeLower(mapFileName);
	for (size_t mapFileIdx = 0; mapFileIdx < s_MapFiles->size(); mapFileIdx++)
	{
		MapFiles& mapFiles = s_MapFiles->at(mapFileIdx);
		if (mapFiles.m_MapFileName == lowerCaseName)
		{
			mapFiles.LoadEntitiesAllAtOnce(map);
		}
	}
}

STATIC void MapFiles::LoadMapByNameOntoMapRegisterForOnUpdate(const std::string& mapFileName, Map* map)
{
	if (s_MapFiles == nullptr)
	{
		return;
	}

	if (s_MapStreaming == nullptr)
	{
		s_MapStreaming = new STATIC std::vector<MapLoadingStream>();
	}

	std::string lowerCaseName = MakeLower(mapFileName);

	for (size_t mapFileIdx = 0; mapFileIdx < s_MapFiles->size(); mapFileIdx++)
	{
		MapFiles& mapFiles = s_MapFiles->at(mapFileIdx);
		if (mapFiles.m_MapFileName == lowerCaseName)
		{
			MapLoadingStream stream;
			stream.ClearMapLoadingStreamData();
			GameSettings* settings = TheGame::GetGameSettings();
			if (settings != nullptr)
			{
				stream.m_amountOfTimeToTakeLoadingPerFrame = settings->m_amountOfTimeLoadingMapDataPerFrame;
			}
			Map* currentMap = Map::GetEditableMap();
			if (currentMap != nullptr)
			{
				currentMap->UnregisterAllTilesFromEvents();
			}
			stream.m_mapFileName = mapFiles.m_MapFileName;
			stream.m_mapToLoadOn = map;
			Map::SetCurrentMap(stream.m_mapToLoadOn);
			stream.m_loadInNewMap = true;
			s_MapStreaming->push_back(stream);

			return;
		}
	}
}

STATIC void MapFiles::LoadAgentNodeFromQuest(const XMLNode& node,
	const XMLNode& mapQuestNode, MapQuestImportantData& data,
	const std::string& questTitle, const std::string& mapFiles,
	MapFiles* files)
{
	if (s_MapFiles == nullptr)
	{
		return;
	}
	std::string mapFile = mapFiles;
	std::string variableName = "";
	std::string globalVariableName = "";
	bool forceSpawnNew = false;
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "mapfile"
			|| attrName == "mapfiles")
		{
			mapFile = std::string(attr.lpszValue);
		}
		else if (attrName == "variablename"
			|| attrName == "variable")
		{
			variableName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "alwaysspawnnew"
			|| attrName == "spawnnew"
			|| attrName == "forcespawnnew")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "true"
				|| val == "t")
			{
				forceSpawnNew = true;
			}
			else if (val == "false"
				|| val == "f")
			{
				forceSpawnNew = false;
			}
		}
		else if (attrName == "globalvariablename"
			|| attrName == "globalvariable")
		{
			globalVariableName = MakeLower(attr.lpszValue);
		}
	}
	if (mapFile == "")
	{
		return;
	}


	if (files != nullptr)
	{
		files->AddAgentToGenWhenNextOpen(node, mapQuestNode, questTitle, data, 
			variableName, forceSpawnNew, globalVariableName);
	}
	else
	{
		std::string lowerCase = MakeLower(mapFile);
		for (size_t i = 0; i < s_MapFiles->size(); i++)
		{
			MapFiles& file = s_MapFiles->at(i);
			if (lowerCase == MakeLower(file.GetMapFileName()))
			{
				file.AddAgentToGenWhenNextOpen(node, mapQuestNode, questTitle, data,
					variableName, forceSpawnNew, globalVariableName);
			}
		}
	}
}

STATIC void MapFiles::LoadFeatureNodeFromQuest(const XMLNode& node,
	const XMLNode& mapQuestNode, MapQuestImportantData& data,
	const std::string& questTitle, const std::string& mapFiles,
	MapFiles* files)
{
	if (s_MapFiles == nullptr)
	{
		return;
	}
	bool forceSpawnNew = true;
	std::string mapFile = mapFiles;
	std::string variableName = "";
	std::string GlobalVariableName = "";
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "mapfile"
			|| attrName == "mapfiles")
		{
			mapFile = std::string(attr.lpszValue);
		}
		else if (attrName == "variablename"
			|| attrName == "variable")
		{
			variableName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "alwaysspawnnew"
			|| attrName == "spawnnew"
			|| attrName == "forcespawnnew")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "true"
				|| val == "t")
			{
				forceSpawnNew = true;
			}
			else if (val == "false"
				|| val == "f")
			{
				forceSpawnNew = false;
			}
		}
		else if (attrName == "globalvariablename"
			|| attrName == "globalvariable")
		{
			GlobalVariableName = MakeLower(attr.lpszValue);
		}
	}
	if (mapFile == "")
	{
		return;
	}

	if (files != nullptr)
	{
		files->AddFeatureToGenWhenNextOpen(node, mapQuestNode,
			questTitle, data, variableName, forceSpawnNew, GlobalVariableName);
	}
	else
	{
		std::string lowerCase = MakeLower(mapFile);
		for (size_t i = 0; i < s_MapFiles->size(); i++)
		{
			MapFiles& file = s_MapFiles->at(i);
			if (lowerCase == MakeLower(file.GetMapFileName()))
			{
				file.AddFeatureToGenWhenNextOpen(node, mapQuestNode,
					questTitle, data, variableName, forceSpawnNew, GlobalVariableName);
			}
		}
	}
}

STATIC const std::string MapFiles::GetRandomMapFile()
{
	if (s_MapFiles == nullptr || s_MapFiles->size() == 0)
	{
		return "";
	}
	int randInt = GetRandomIntLessThan(s_MapFiles->size());
	return s_MapFiles->at(randInt).m_MapFileName;
}