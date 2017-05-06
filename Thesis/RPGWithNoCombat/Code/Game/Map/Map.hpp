#pragma once

#ifndef MAP_HPP
#define MAP_HPP
#include "Game/Map/Tile.hpp"
#include "Game/Collision/SquareCollision.hpp"
#include "Engine/2D/SpriteVertWithMultiSprites.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseTestResults.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Game/Debugging/DeveloperDebugInfoBox.hpp"
#include <vector>

class SquareCollision;
class TileEventPack;
class MapFiles;
struct MapAgentSpecifications;
struct ExtraDataToLoadOnOpenMapFiles;
struct XMLNode;
struct XMLAttribute;


struct GlobalVariablesLoadingData
{
	uint64_t m_timeStartedLoadingDataType = 0;
	uint64_t m_timeStartedLoadingSingleDataType = 0;
	std::vector<std::string> m_files;
	XMLNode currentNodeReading = XMLNode::emptyNode();
	std::string m_globalVariablesXMLFolderLocation = "Data/XML/GlobalVariables/";
	std::string m_tileDefinitionXMLRootNodeName = "GlobalVariables";
	double m_amountOfTimeToTakeToReadNodesPerFrame = 0.25;
	int m_fileIdx = 0;
	int m_childIdx = 0;
	bool m_receivedFiles = false;
	bool m_readNode = false;
};

class Map
{
private:
protected:
	//static variables
	static GlobalVariablesLoadingData* s_GlobalVariablesLoad;
	static NamedProperties* s_GlobalVariables;
	static Map* s_CurrentMap;
	static Map* s_PreviousMap;
	static std::map<std::string, Map*> s_loadedMaps;
	static std::vector<ExtraDataToLoadOnOpenMapFiles>* s_ExtraDataPerMap;
	static std::vector<Texture*>* s_associatedSpriteSheetsByIdx;
	static std::vector<SpriteVertWithMultiSprites>* s_BottomModel;
	static std::vector<SpriteVertWithMultiSprites>* s_Model;
	static std::vector<SpriteVertWithMultiSprites>* s_TopModel;
	static std::vector<SpriteVertWithMultiSprites>* s_TopModel2;
	static std::vector<unsigned int>* s_BottomIndices;
	static std::vector<unsigned int>* s_indices;
	static std::vector<unsigned int>* s_TopIndices;
	static std::vector<unsigned int>* s_TopIndices2;
	static bool s_currentlyLoadingTileDefinitions;
	static bool s_CurrentlyLoadingMap;
	static bool s_EnableEnforcingAllTilesAsNotNull;
	static constexpr char* s_CommonMapVertexShader
		= "Data/Shaders/SpritePassthroughMultipleTextures.vert";
	static constexpr char* s_CommonMapFragmentShader
		= "Data/Shaders/SpritePassthroughWithColorHandlingMultipleTextures.frag";

	//variables
	std::vector<std::vector<BaseGameEntity*>> m_BaseGameEntitiesByType;
	std::vector<BaseGameEntity*> m_BaseGameEntitiesInRenderOrder;
	std::vector<BaseGameEntity*> m_AllNPCs;
	std::vector<BaseGameEntity*> m_AllFeatures;
	std::vector<MapAgentSpecifications> m_mapAgentSpecifications;
	std::vector<Quest*> m_questAvailableToStart;
	std::vector<unsigned int> m_indexsOfAllNonSolidTiles;
	XMLNode m_specialDataRoot;
	Material m_material;
	Mesh m_bottomLayerMesh;
	Mesh m_mesh;
	Mesh m_topLayerMesh;
	Mesh m_topLayerMesh2;
	Matrix44 m_modelMatrix;
	std::string m_MusicNameForThisMap = "";
	std::string m_MapName = "";
	std::string m_MapDialogueName = "";
	const Texture* m_resource = nullptr;
	std::vector<Tile> m_tiles;
	std::vector<Tile*> m_interactableTiles;
	std::vector<Tile*> m_triggerVolumeTiles;
	std::vector<TileEventPack> m_eventPack;
	SquareCollision m_perTileCollision;
	Vector2 m_mapCenter = Vector2::vec2_zeros;
	Vector2 m_mapSize = Vector2::vec2_zeros;
	Vector2 m_BLPos = Vector2::vec2_zeros;
	Vector2 m_TRPos = Vector2::vec2_zeros;
	float m_musicVolume = 1.f;
	TILE_COORDS m_mapWidthHeight = TILE_COORDS::ivec2_zeros;
	size_t m_numberOfTiles = 0;
	int m_colorSchemeChoice = 0;
	int m_numComponentsOnImage = 0;
	int m_currentTileOrXMLIdxAtWhileReading = 0;
	int m_currentGrandChildXMLNodeIdxAt = 0;
	unsigned char* m_imageData = nullptr;
	bool m_constrainCameraBounds = true;
	bool m_InterpretedImageData = false;
	bool m_ReadSpecialData = false;
	bool m_readAttributes = false;
	bool m_readNodes = false;
public:
	//Constructors
	Map();
	virtual ~Map();
	void ClearMap();
	void StringTilesTogetherAllAtOnce();
protected:
	void StringSingleTileTogetherWithItsNeighbors(size_t tileIdx, const TILE_COORDS& coords);
public:

	//Updates and Render
	void PlayThisMapsMusic();
	void Update(float deltaSeconds);
	void Render() const;
	void AdjustCameraPositionBasedOffBounds(Vector2& position);
	void UnregisterAllTilesFromEvents();
	void RegisterAllTilesToEventsIfNeed();

	//Setters
	void PushBackQuest(Quest* quest);
	void StartQuest(size_t idx);
	void SetTileCollisionSize(const Vector2& collisionSize);
	void RemoveAllTilesFromEventCalls();
	void AddAllTilesFromEventCalls();
	void SetMapName(const std::string& mapName);
	void SetMapDialogueName(const std::string mapDialogueName);

	//Getters
	void AddAllQuestsThatReferenceEntityToAList(const BaseGameEntity* entity,
		std::vector<Quest*>& quests) const;
	void GetAllEntitiesAndTileInfoForDeveloperDebug(std::vector<DeveloperDebugInfoBox>& infoBoxes,
		const TILE_COORDS& coords) const;
	const std::vector<Quest*>& GetQuests() const;
	const std::vector<Quest*>* GetQuestsPtr() const;
	std::vector<Quest*>* GetQuestsEditablePtr();
	std::vector<std::vector<BaseGameEntity*>>* GetEntitiesByTypePtr();
	const std::string& GetMapName() const;
	const std::string& GetMapDialogueName() const;
	const Vector2& GetMapCenter() const;
	const Vector2& GetMapSize() const;
	Vector2 GetHalfMapSize() const;
	const Vector2& GetBLMapPos() const;
	const Vector2& GetTRMapPos() const;
	void GetAllUninhabittedTileCenters(std::vector<Vector2>& positions, bool guaranteeNotSolid) const;
	void GetAllUninhabittedTileCenters(std::vector<TILE_COORDS>& positions, bool guaranteeNotSolid) const;
	Vector2 GetRandomPositionOnMap() const;
	Vector2 GetRandomNonSolidPositionOnMap(bool guaranteeNoEntitiesAreInTileIfPossible = false) const;
	Vector2 GetCenterOfTileAtTileCoords(const TILE_COORDS& coords) const;
	TILE_COORDS ConvertPositionToTileCoords(const Vector2& pos) const;
	const TILE_COORDS& GetMapWidthHeight() const;
	const Tile* GetTileAtCoords(const TILE_COORDS& coords) const;
	const Tile* GetTileAtIdx(int idx) const;
	Tile* GetEditableTileAtCoords(const TILE_COORDS& coords);
	Tile* GetEditableTileAtIdx(int idx);
	int GetTileIdxForCoords(const TILE_COORDS& coords) const;
	TILE_COORDS GetTileCoordsForIdx(int idx) const;
	Quest* GetQuestByTitle(const std::string& questTitle) const;

protected:
	void DestroyAllGameEntities();
	void RebuildMeshNew();
	void AddTileVerts(const Tile& tile);
	void AddTileVertsSetResource(bool setResource, Texture*& resource,
		AABB2& textureCoords, const SpriteResource* res);
	void AddTileVertsAddUnregisteredTextureIfApplicable(int& resIdx, Texture* resource);
	void PushQuadOfVerticesOntoIndiceArray(std::vector<unsigned int>*& indices,
		size_t numOfVerts);
	void AddTileVertsBasedOnResourceAvailability(SpriteVertWithMultiSprites& vert,
		bool resExists, bool topResExists, bool topRes2Exists, bool botResExists,
		const AABB2& textureCoords, const AABB2& topResTextureCoords, const AABB2& topResTextureCoords2,
		const AABB2& bottomResTextureCoords, bool maxX, bool maxY,
		bool topCanVanishIfPlayerUnderneath, bool top2CanVanishIfPlayerUnderneath,
		Texture* bottomResource, Texture* resource, Texture* topResource, Texture* topResource2,
		int bottomResIdx, int resIdx, int topResIdx, int topRes2Idx);

	void SetVertTextureCoordsToMinMax(SpriteVertWithMultiSprites& vert, bool maxX,
		bool maxY, const AABB2& textureCoords) const;

public:
	void WriteOutAllMapActiveDataToString(std::string& str, int indentationAmt,
		bool PushTileEventsPerMapContentToDebugFile,
		bool PushTilesPerMapContentToDebugFile,
		bool PushAgentsPerMapContentToDebugFile,
		bool PushFeaturesPerMapContentToDebugFile,
		bool PushActiveQuestContentToDebugFile) const;
	void WriteOutTileEventDataToString(std::string& str, int indentationtAmt,
		bool PushTileEventsPerMapContentToDebugFile = true) const;
	void WriteOutTilesDataToString(std::string& str, int indentationtAmt,
		bool PushTilesPerMapContentToDebugFile = true) const;
	void WriteOutAgentsDataToString(std::string& str, int indentationtAmt,
		bool PushAgentsPerMapContentToDebugFile = true) const;
	void WriteOutFeaturesDataToString(std::string& str, int indentationtAmt,
		bool PushFeaturesPerMapContentToDebugFile = true) const;
	void WriteOutActiveQuestsDataToString(std::string& str, int indentationtAmt,
		bool PushActiveQuestContentToDebugFile = true) const;
public:
	bool WriteActiveQuestFromMapToString(std::string& str, const std::string& questName, 
		int indentationAmt) const;

	//static methods
	static bool WriteActiveQuestByNameToStringFromAllMaps(std::string& str, const std::string& questName,
		int indentationAmt);

	static void WriteAllMapActiveDatasToString(std::string& str, int indentationAmt,
		bool PushTileEventsPerMapContentToDebugFile,
		bool PushTilesPerMapContentToDebugFile,
		bool PushAgentsPerMapContentToDebugFile,
		bool PushFeaturesPerMapContentToDebugFile,
		bool PushActiveQuestContentToDebugFile);
	static void WriteSingleMapActiveDatasToString(std::string& str, const std::string& mapLookingFor,
		int indentationAmt,
		bool PushTileEventsPerMapContentToDebugFile,
		bool PushTilesPerMapContentToDebugFile,
		bool PushAgentsPerMapContentToDebugFile,
		bool PushFeaturesPerMapContentToDebugFile,
		bool PushActiveQuestContentToDebugFile);
	static bool UpdateLoadingGlobalVariables();
	static bool LoadGlobalVariablesFromXMLNodeFileOnUpdate(const std::string& fileLocation,
		uint64_t& frameStart);
	static void ReadGlobalVariableFromXMLNode(const XMLNode& node);
	static bool UpdateLoadingMap();
	static bool GetLoadingMap();
	static bool CreateMap();
	static void RegisterMap(Map* map);
	static Map* GetRegisteredMapIfExists(const std::string& mapName);
	static void ObliterateMaps();
	static void DeleteMaps();
	static const Map* GetMap();
	static Map* GetEditableMap();
	static const Map* GetPreviousMap();
	static Map* GetEditablePreviousMap();
	static bool OnUpdateLoadTileDefinitions();
	static void LoadTileDefinitionsAllAtOnce();
	static void SetCurrentMap(Map* map, bool changePrevMap = true);

	//Map Reading Functions
	void ReadQuestAgentsToSpawn(const XMLNode& child);
	void ReadQuestFeatureToSpawn(const XMLNode& child);
	bool ReadMapSpecialDataFromXMLAllAtOnce(const std::string& fileLoc);
	bool OnUpdateReadMapSpecialDataFromXML(const std::string& fileLoc,
		int& actionsTakenThisFrame, double timeCanTakePerFrame,
		const uint64_t& startFrameTime,
		MapFiles* files);
protected:
	bool OnUpdateParseXMLNodeSpecialData(const XMLNode& child,
		int& actionsTakenThisFrame, double amountOfTimeCanTakePerFrame,
		const uint64_t& frameStartTime,
		MapFiles* files);
	void ReadMapSpecialDataFromXMLAttributes(const XMLNode& root);
	void ReadSingleXMLAttributeForXMLData(const XMLAttribute& attr);
	void ReadMapSpecialDataFromXMLChildNodes(const XMLNode& root,
		MapFiles* files);
	void ReadMapSpecialDataFromXMLQuests(const XMLNode& child,
		MapFiles* files);
	void ReadMapSpecialDataFromChildTileEventNode(const XMLNode& child);
	bool ReadMapSpecialDataForTileEventsAttributes(const XMLNode& child, TILE_COORDS& coordsToEffect);
	void ReadMapSpecialDataForTileEvents(const XMLNode& child, TileEvent*& onTouch, TileEvent*& onEnter, 
		TileEvent*& onExit,	TileEvent*& onInteract, std::vector<StrXMLAttr>& OnTouchParams,
		std::vector<StrXMLAttr>& OnEnterParams,	std::vector<StrXMLAttr>& OnExitParams,
		std::vector<StrXMLAttr>& OnInteractParams,
		std::string& OnTouchName, std::string& OnEnterName, 
		std::string& OnExitName, std::string& OnInteractName);
	void SingleTileEventReadingFunction(const XMLNode& grandChild, 
		TileEvent*& tileEvent, std::vector<StrXMLAttr>& tileEventParams,
		std::string& name);
	void ReadMapSpecialDataFromChildAgentsToGenerateNode(const XMLNode& child);
	void ReadMapSpecialDataFromChildFeaturesToGenerateNode(const XMLNode& child);
	void ReadMapSpecialDataFromChildAgentsToGenerateNodeSpawnNow(
		const std::string& name,
		const TILE_COORDS& guaranteePosition, std::vector<TILE_COORDS>& tileCoordsCanSelect,
		bool guaranteePositionBool, int numberToSpawn,
		bool spawnLocationRandom, bool guaranteeNonSolidTilesUsedOnly,
		const std::string& globalVariableName);
	void EmptyTileCoordsExceptForUninhabitted(std::vector<TILE_COORDS>& tileCoordsCanSelect,
		bool guaranteeNonSolid);


	void ReadMapSpecialDataFromChildFeaturesToGenerateNodeSpawnNow(
		const XMLNode& child, const std::string& name,
		const TILE_COORDS& guaranteePosition, std::vector<TILE_COORDS>& tileCoordsCanSelect,
		bool guaranteePositionBool, int numberToSpawn,
		bool spawnLocationRandom, const std::string& globalVariableName);

	void ReadMapSpecialDataFromChildGeneralToGenerateNodeAttributes(const XMLNode& child,
		TILE_COORDS& guaranteePosition, bool& guaranteePositionBool,
		int& numberToSpawn, int& maxNumberToSpawn, bool& spawnLocationRandom,
		float& chanceToSpawnAll, std::string& globalVariableName);
	void ReadMapSpecialDataFromChildGeneralToGenerateNodeChildren(const XMLNode& child,
		std::vector<TILE_COORDS>& tileCoordsCanSelect);
public:
	bool ReadMapFromImageAllAtOnce(const std::string& fileLoc);
	bool OnUpdateReadMapFromImage(const std::string& fileLoc, 
		int& currentNumberOfActionsTaken, double amountOfTimeToTakePerFrame,
		const uint64_t& frameStartTime);
protected:
	void GetImageDataFromFileLocation(const std::string& fileLocation, int& numComponents);
	void ReadMapFromImageTileSetter(size_t tileIdx, const Vector3& tileDefBits);
	void ReadSpecialTileDefinitionCases(size_t tileIdx);
public:
	SpecialCaseTestResults CheckNumberOfTilesSameInDirection(size_t& tileCount, const TILE_COORDS& startCoords,
		const TILE_COORDS& direction, const TileDefinition* def, int& numberSteppedInDirection) const;
	void PassResultInDirection(const TILE_COORDS& startCoords, const TILE_COORDS& direction,
		int countFound, int numberToMoveTowards, const TileDefinition* def);
	void NotifyTilesInDirectionOfTestResults(const SpecialCaseTestResults& results);
public:

	//Entity Related
	void UpdateAllEntities(const float& dt);
	void RenderAllEntities() const;
	void RemoveGameEntityRegistration(BaseGameEntity* entity);
	void DestroyGameEntityAndRegistration(BaseGameEntity* entity, bool destroy = true);
	void ClearAllGameEntitiesOfType(GameEntityType type);
	void ClearAllGameEntities(bool allowPlayerKill = false, bool totalClear = false);
	void RegisterGameEntity(BaseGameEntity* entity);
	void UnregisterGameEntity(BaseGameEntity* entity);
	void RegisterNPC(BaseGameEntity* npc);
	void UnregisterNPC(BaseGameEntity* npc);
	void DestroyNPC(BaseGameEntity* npc);
	void RegisterFeature(BaseGameEntity* npc);
	void UnregisterFeature(BaseGameEntity* npc);
	void DestroyFeature(BaseGameEntity* npc);

	static void ClearAllMapsOfAllEntities(bool allowPlayerKill, bool totalClear);

	//Global Variables related
	static void WriteGlobalVariablesToString(std::string& str, int indentationAmount);
	static void SpawnGlobalVariables();
	static void DestroyGlobalVariables();
	static const NamedProperties* GetGlobalVariables();
	static NamedProperties* GetEditableGlobalVariables();
	static void SetEnforceNoEmptySpriteResourcesAllowedOnMap(bool enforce);

	static void SetGlobalVariable(const std::string& variableName, const float& variable);
	static void SetGlobalVariable(const std::string& variableName, const bool& variable);
	static void SetGlobalVariable(const std::string& variableName, BaseGameEntity* variable);
	static void SetGlobalVariable(const std::string& variableName, const std::string& variable);
	static void SetGlobalVariable(const std::string& variableName, const int& variable);

	static PropertyGetResult GetGlobalVariable(const std::string& variableName, float& variable);
	static PropertyGetResult GetGlobalVariable(const std::string& variableName, bool& variable);
	static PropertyGetResult GetGlobalVariable(const std::string& variableName, BaseGameEntity*& variable);
	static PropertyGetResult GetGlobalVariable(const std::string& variableName, std::string& variable);
	static PropertyGetResult GetGlobalVariable(const std::string& variableName, int& variable);

	static PropertyGetResult GetGlobalVariable(const std::string& variableName, float& variable, 
		const float& defaul);
	static PropertyGetResult GetGlobalVariable(const std::string& variableName, bool& variable, 
		const bool& defaul);
	static PropertyGetResult GetGlobalVariable(const std::string& variableName, BaseGameEntity*& variable,
		BaseGameEntity* defaul);
	static PropertyGetResult GetGlobalVariable(const std::string& variableName, std::string& variable,
		const std::string& defaul);
	static PropertyGetResult GetGlobalVariable(const std::string& variableName, int& variable,
		const int& defaul);
};

#endif