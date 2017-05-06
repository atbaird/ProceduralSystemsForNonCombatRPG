#pragma once

#ifndef MAPFILES_HPP
#define MAPFILES_HPP
#include <string>
#include <vector>
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Map/MapFiles/GameEntityToGenDataRelatingToQuest.hpp"
#include "Game/Quest/MapQuestImportantData.hpp"

#define STATIC

struct MapFilesWhileLoadingSave
{
	std::vector<std::string> m_fileLocations;
	std::string m_mapFilesCommonXMLFolder = "Data/XML/MapFiles/";
	std::string m_mapFilesRootNodeName = "MapFiles";
	double m_amountOfTimeToTakeToReadInMapFilesPerFrame = 0.25;
	int m_currentIndexOfFileLocations = 0;
	bool m_presentlyReadingQuestFolderCommonAgentGenerators = true;
	bool m_GotAllXMLMapFiles = false;

};

struct AgentTemplateNPCAttributesContainer;
class BaseGameEntity;
class Map;
struct XMLNode;
struct MapLoadingStream;

class MapFiles
{
private:
protected:
	//Static Variables
	static std::vector<MapFiles>* s_MapFiles;
	static MapFilesWhileLoadingSave* s_MapFilesWhileLoadingSave;
	static std::vector<MapLoadingStream>* s_MapStreaming;

	//Variables
	static uint64_t s_startFrame;
	std::vector<GameEntityToGenDataRelatingToQuest> m_agentsForQuestsWaitingToBeSpawned;
	std::vector<GameEntityToGenDataRelatingToQuest> m_featuresForQuestsWaitingToBeSpawned;
	std::vector<std::string> m_DependenciesAkaOtherMaps;
	std::string m_DialogueMapName = "";
	std::string m_MapFileName = "";
	std::string m_ImageFileLoc = "";
	std::string m_MapDataLoc = "";
	int m_currentIndexForAgentsAndFeaturesOnUpdate = 0;
	bool m_fullyLoaded = false;
public:
	//Constructors
	MapFiles();
	MapFiles(const std::string& fileName, const std::string& dialogueMapName,
		const std::string ImageFileLoc, const std::string& mapDataLoc);
	MapFiles(const XMLNode& node);
	virtual ~MapFiles();

	//Setters
protected:
	static void SearchForRandomPosition(GameEntityToGenDataRelatingToQuest& entityData,
	MapQuestImportantData& mapQuestdata,
		const std::string& entityName,
		const std::string& mapNameSearchingFor,
		bool& found);
public:
	void AddAgentToGenWhenNextOpen(const XMLNode& node,
		const XMLNode& mapQuestNode, const std::string& questTitle,
		MapQuestImportantData& mapQuestdata, const std::string& entityName = "",
		bool forceSpawnNew = false, const std::string& globalVariableName = "");
	void AddFeatureToGenWhenNextOpen(const XMLNode& node,
		const XMLNode& mapQuestNode, const std::string& questTitle,
		MapQuestImportantData& mapQuestdata, const std::string& entityName = "",
		bool forceSpawnNew = false, const std::string& globalVariableName = "");
	void SetMapDialogueName(const std::string& dialogueName);
	void SetMapFileName(const std::string& name);
	void SetImageFileLocation(const std::string& loc);
	void SetMapDataLocation(const std::string& loc);
	Map* LoadThisMapAllAtOnce();
	void LoadThisMapOntoMapAllAtOnce(Map* map);
	void LoadEntitiesAllAtOnce(Map* map);
	bool OnUpdateLoadQuestEntities(Map* map, int& numberOfActionsTakenThisFrame, double amountOfTimeToTakePerFrame,
		const uint64_t& startFrameTime);
protected:
	void LoadAgentsAllAtOnce(Map* map);
	bool OnUpdateLoadAgents(Map* map, int& numberOfActionsTakenThisFrame, double amountOfTimeToTakePerFrame,
		const uint64_t& startFrameTime);
	void HandleSingleQuestAgentDataToLoad(GameEntityToGenDataRelatingToQuest& data, Map* map);
	void PullOutNPCAttributeRequirements(GameEntityToGenDataRelatingToQuest& data,
		std::vector<AgentTemplateNPCAttributesContainer>& npcAttributeValsSearchingFor);
	bool TryGetAlreadyCreatedAgent(GameEntityToGenDataRelatingToQuest& data);
	void HandleSpecialDataForLoadedAgent(BaseGameEntity* entity, GameEntityToGenDataRelatingToQuest& data);
	void LoadFeaturesAllAtOnce(Map* map);
	bool OnUpdateLoadFeatures(Map* map, int& numberOfActionsTakenThisFrame, double amountOfTimeToTakePerFrame,
		const uint64_t& startFrameTime);
	void HandleSingleQuestFeatureDataToLoad(GameEntityToGenDataRelatingToQuest& data, Map* map);
	bool TryGetAlreadyCreatedFeature(GameEntityToGenDataRelatingToQuest& data);
	void HandleSpecialDataForLoadedFeature(BaseGameEntity* entity, GameEntityToGenDataRelatingToQuest& data);
public:

	//Getters
	const std::string& GetMapDialogueName() const;
	const std::string& GetMapFileName() const;
	const std::string& GetImageFileLocation() const;
	const std::string& GetMapDataLocation() const;
	void WriteMapFileToString(std::string& str, int indentationAmt) const;


	//Static Functions
	static void WriteAllMapFilesToString(std::string& str, int indentationAmt);
protected:
	static bool OnUpdateLoadMapDataAndImage(MapLoadingStream*& stream, Map*& map, MapFiles*& files, int& actionTakenThisFrame,
		uint64_t& currentTime, uint64_t& timeDiff, double& timeDiffDouble);
public:
	static bool UpdateMapLoading();
	static bool GetLoadingMap();
protected:
	static void UpdateMapLoadingGetFilesForStream(MapFiles*& files, MapLoadingStream*& stream);
	static bool CheckDependenciesForIfFileExists(const MapFiles& filesChecking, MapFiles*& files);
	static bool CheckIfFileInfiniteLoopOccurs(MapFiles*& files, const std::string& lowerCaseName);
	static bool CheckIfMapAlreadyBuilt(const std::string& lowerCaseName);
	static bool UpdateMapLoadingDependences(MapFiles*& files, MapLoadingStream*& stream);
	static bool UpdateMapLoadingBasicData(MapFiles*& files, MapLoadingStream*& stream, Map*& map, int& actionTakenThisFrame);
	static bool UpdateMapLoadingImage(MapFiles*& files, MapLoadingStream*& stream, Map*& map, int& actionTakenThisFrame);
	static bool ClearAMapFileOffTheStack();

public:
	static void ReadInMapFilesFromCommonXMLFileAllAtOnce();
	static bool OnUpdateReadInMapFilesFromCommonXMLFile();
protected:
	static void ReadXMLMapFileFromFileLocation(const std::string& fileStr,
		const std::string& mapFilesRootNodeName);
	static bool ActuallyOnUpdateReadInMapFilesFromCommonXMLFile();
public:

	static void LoadMapFileDataOntoMapAllAtOnce(Map* map, MapFiles* files);
	static void ClearAllMapFiles();
	static void LoadMapByNameAllAtOnce(const std::string& mapFileName);
	static void LoadMapByNameOntoMapAllAtOnce(const std::string& mapFileName, Map* map);
	static void LoadMapByNameRegisterForOnUpdate(const std::string& mapFileName);
	static void LoadJustTheEntitiesFromMapFilesAllAtOnce(Map* map, const std::string& mapFileName);
	static void LoadMapByNameOntoMapRegisterForOnUpdate(const std::string& mapFileName, Map* map);
	static void LoadAgentNodeFromQuest(const XMLNode& node,
		const XMLNode& mapQuestNode, MapQuestImportantData& data,
		const std::string& questTitle, const std::string& mapFiles = "",
		MapFiles* files = nullptr);
	static void LoadFeatureNodeFromQuest(const XMLNode& node,
		const XMLNode& mapQuestNode, MapQuestImportantData& data,
		const std::string& questTitle, const std::string& mapFiles = "",
		MapFiles* files = nullptr);
	static const std::string GetRandomMapFile();
};

struct MapLoadingStream
{
	//Variables
	uint64_t m_timeStartedLoadingDataType = 0;
	XMLNode m_rootNode;
	Map* m_mapToLoadOn = nullptr;
	std::string m_mapFileName = "";
	double m_amountOfTimeToTakeLoadingPerFrame = 0.25;
	double m_amountOfTimeSpentLoadingImageData = 0.0;
	double m_amountOfTimeSpentOnAgents = 0.0;
	double m_amountOfTimeSpentOnFeatures = 0.0;
	double m_amountOfTimeSpentParsingMapSpecialData = 0.0;
	int m_nodeIndex = 0;
	bool m_loadInNewMap = false;
	bool m_loadedImage = false;
	bool m_loadedXML = false;
	bool m_loadedBasicData = false;
	bool m_loadedQuestAgents = false;
	bool m_AddedAllOfTheDependencies = false;
	bool m_registerTileEvents = true;
	bool m_MapAlreadyExisted = false;

	MapLoadingStream() {};
	//Operations
	void ClearMapLoadingStreamData();
};

#endif