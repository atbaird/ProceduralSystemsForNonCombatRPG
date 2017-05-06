#include "Game/Map/Map.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Map/MapFiles/ExtraDataToLoadOnOpenMapFiles.hpp"
#include "Game/Common/MapCommonVars.hpp"
#include "Game/Collision/SquareCollision.hpp"
#include "Game/Map/TileEvent/TileEventPack.hpp"
#include "Game/Map/MapFiles/MapFiles.hpp"
#include "Game/Common/GameCommonVars.hpp"
#include "Game/Map/MapAgentSpecifications.hpp"
#include "Game/GameEntities/Features/FeatureGenerator.hpp"
#include "Game/GameEntities/Agents/AgentGenerator.hpp"
#include "Game/Quest/QuestGenerator.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/SoundManager/SoundManager.hpp"
#include "Game/SoundManager/ActiveSound.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Game/TheGame.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/Debugging/NamedPropertiesToString.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//static variables
STATIC GlobalVariablesLoadingData* Map::s_GlobalVariablesLoad = nullptr;
STATIC NamedProperties* Map::s_GlobalVariables = nullptr;
STATIC Map* Map::s_CurrentMap = nullptr;
STATIC Map* Map::s_PreviousMap = nullptr;
STATIC std::map<std::string, Map*> Map::s_loadedMaps;
STATIC std::vector<ExtraDataToLoadOnOpenMapFiles>* Map::s_ExtraDataPerMap = nullptr;
STATIC std::vector<Texture*>* Map::s_associatedSpriteSheetsByIdx = nullptr;

STATIC std::vector<SpriteVertWithMultiSprites>* Map::s_BottomModel = nullptr;
STATIC std::vector<SpriteVertWithMultiSprites>* Map::s_Model = nullptr;
STATIC std::vector<SpriteVertWithMultiSprites>* Map::s_TopModel = nullptr;
STATIC std::vector<SpriteVertWithMultiSprites>* Map::s_TopModel2 = nullptr;
STATIC std::vector<unsigned int>* Map::s_BottomIndices = nullptr;
STATIC std::vector<unsigned int>* Map::s_indices = nullptr;
STATIC std::vector<unsigned int>* Map::s_TopIndices = nullptr;
STATIC std::vector<unsigned int>* Map::s_TopIndices2 = nullptr;
STATIC bool Map::s_currentlyLoadingTileDefinitions = true;
STATIC bool Map::s_CurrentlyLoadingMap = false;
STATIC bool Map::s_EnableEnforcingAllTilesAsNotNull = true;

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Constructors
Map::Map()
{
	if (s_associatedSpriteSheetsByIdx == nullptr)
	{
		s_associatedSpriteSheetsByIdx = new std::vector<Texture*>();
	}
	if (s_Model == nullptr)
	{
		s_Model = new std::vector<SpriteVertWithMultiSprites>();
	}
	if (s_TopModel == nullptr)
	{
		s_TopModel = new std::vector<SpriteVertWithMultiSprites>();
	}
	if (s_BottomModel == nullptr)
	{
		s_BottomModel = new std::vector<SpriteVertWithMultiSprites>();
	}
	if (s_TopModel2 == nullptr)
	{
		s_TopModel2 = new std::vector<SpriteVertWithMultiSprites>();
	}
	if (s_indices == nullptr)
	{
		s_indices = new std::vector<unsigned int>();
	}
	if (s_TopIndices == nullptr)
	{
		s_TopIndices = new std::vector<unsigned int>();
	}
	if (s_BottomIndices == nullptr)
	{
		s_BottomIndices = new std::vector<unsigned int>();
	}
	if (s_TopIndices2 == nullptr)
	{
		s_TopIndices2 = new std::vector<unsigned int>();
	}

	unsigned int questPreallocation = 20;
	unsigned int agentsPreallocation = 20;
	unsigned int featuresPreallocation = 20;
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		questPreallocation = settings->m_preallocationAmountQuestsPerMap;
		agentsPreallocation = settings->m_preallocationAmountAgentsPerMap;
		featuresPreallocation = settings->m_preallocationAmountFeaturesPerMap;
	}
	m_BaseGameEntitiesByType.resize(NUM_OF_GAME_ENTITY_TYPES);
	m_BaseGameEntitiesByType[AGENT_ENTITY].reserve(agentsPreallocation);
	m_BaseGameEntitiesByType[FEATURE_ENTITY].reserve(featuresPreallocation);
	m_questAvailableToStart.reserve(questPreallocation);
	m_AllNPCs.reserve(agentsPreallocation);
	m_AllFeatures.reserve(featuresPreallocation);
	m_mesh.CreateVAOID();
	m_mesh.PregenerateVBOAndIBO();
	m_topLayerMesh.CreateVAOID();
	m_topLayerMesh.PregenerateVBOAndIBO();
	m_topLayerMesh2.CreateVAOID();
	m_topLayerMesh2.PregenerateVBOAndIBO();

	std::string vertexShader = std::string(s_CommonMapVertexShader);
	std::string fragmentShader = std::string(s_CommonMapFragmentShader);

	if (settings != nullptr)
	{
		vertexShader = settings->m_defaultMapShaderVertFile;
		fragmentShader = settings->m_defaultMapShaderFragFile;
	}

	ShaderProgram* prog = ShaderProgram::CreateOrGetShaderProgram(
		vertexShader,
		fragmentShader);
	m_material.SetProgram(prog);
}

Map::~Map()
{
	for (size_t i = 0; i < m_questAvailableToStart.size(); i++)
	{
		Quest* quest = m_questAvailableToStart[i];
		if (quest != nullptr)
		{
			delete quest;
		}
	}
	m_questAvailableToStart.clear();
	ClearMap();
}

void Map::ClearMap()
{
	m_tiles.clear();
	m_triggerVolumeTiles.clear();
	m_interactableTiles.clear();
	m_numberOfTiles = 0;
}

void Map::StringTilesTogetherAllAtOnce()
{
	size_t numTiles = (size_t)(m_mapWidthHeight.x * m_mapWidthHeight.y);
	for (size_t tileIdx = 0; tileIdx < numTiles; tileIdx++)
	{
		int x = tileIdx % m_mapWidthHeight.x;
		TILE_COORDS coords(x, (tileIdx - x) / m_mapWidthHeight.x);
		StringSingleTileTogetherWithItsNeighbors(tileIdx, coords);
	}
}


void Map::StringSingleTileTogetherWithItsNeighbors(size_t tileIdx, const TILE_COORDS& coords)
{
	if (tileIdx >= m_numberOfTiles)
	{
		return;
	}

	int x = coords.x;
	int y = coords.y;

	//W E S N
	if (x > 0)
	{
		//W
		int idxSetting = tileIdx - (1);
		Tile& tileSetting = m_tiles[idxSetting];
		m_tiles[tileIdx].SetWestTile(tileSetting.GetEditablePtrOfSelf());
	}
	if (x < (m_mapWidthHeight.x - 1))
	{
		//E
		int idxSetting = tileIdx + (1);
		Tile& tileSetting = m_tiles[idxSetting];
		m_tiles[tileIdx].SetEastTile(tileSetting.GetEditablePtrOfSelf());
	}
	if (y > 0)
	{
		//S
		int idxSetting = tileIdx - (m_mapWidthHeight.x);
		Tile& tileSetting = m_tiles[idxSetting];
		m_tiles[tileIdx].SetSouthTile(tileSetting.GetEditablePtrOfSelf());
	}
	if (y < (m_mapWidthHeight.y - 1))
	{
		//N
		int idxSetting = tileIdx + (m_mapWidthHeight.x);
		Tile& tileSetting = m_tiles[idxSetting];
		m_tiles[tileIdx].SetNorthTile(tileSetting.GetEditablePtrOfSelf());
	}

	//SW NW SE NE
	if (x > 0 && y > 0)
	{
		//SW
		int idxSetting = tileIdx - (m_mapWidthHeight.x + 1);
		Tile& tileSetting = m_tiles[idxSetting];
		m_tiles[tileIdx].SetSouthWestTile(tileSetting.GetEditablePtrOfSelf());
	}
	if (x > 0 && y < (m_mapWidthHeight.y - 1))
	{
		//NW
		int idxSetting = tileIdx + (m_mapWidthHeight.x - 1);
		Tile& tileSetting = m_tiles[idxSetting];
		m_tiles[tileIdx].SetNorthWestTile(tileSetting.GetEditablePtrOfSelf());
	}
	if (x < (m_mapWidthHeight.x - 1) && y > 0)
	{
		//SE
		int idxSetting = tileIdx - (m_mapWidthHeight.x - 1);
		Tile& tileSetting = m_tiles[idxSetting];
		m_tiles[tileIdx].SetSouthEastTile(tileSetting.GetEditablePtrOfSelf());
	}
	if (x < (m_mapWidthHeight.x - 1) && y < (m_mapWidthHeight.y - 1))
	{
		//NE
		int idxSetting = tileIdx + (m_mapWidthHeight.x + 1);
		Tile& tileSetting = m_tiles[idxSetting];
		m_tiles[tileIdx].SetNorthEastTile(tileSetting.GetEditablePtrOfSelf());
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Updates and Render
void Map::PlayThisMapsMusic()
{
	if (m_MusicNameForThisMap == "")
	{
		return;
	}
	SoundManager* soundManager = SoundManager::GetSoundManager();
	if (soundManager == nullptr)
	{
		return;
	}
	ActiveSound* currentMusic = soundManager->GetCurrentMusicPlaying();
	std::string LowerCaseMusicName = MakeLower(m_MusicNameForThisMap);
	if (currentMusic == nullptr
		|| currentMusic->GetLowerCaseSoundName() != LowerCaseMusicName)
	{
		soundManager->PlayMusic(m_MusicNameForThisMap, m_musicVolume, true);
	}
}

void Map::Update(float deltaSeconds)
{
	for (size_t i = 0; i < m_numberOfTiles; i++)
	{
		m_tiles[i].Update(deltaSeconds);
	}
	//double time = GetCurrentTimeSeconds();
	RebuildMeshNew();
	//double elapsed = GetCurrentTimeSeconds() - time;
	//DebuggerPrintf("Rebuild Mesh took: %.03f ms\n", elapsed * 1000.0);
	m_modelMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_modelMatrix, 0.f, 0.f, 0.f, Vector3::vec3_zeros);

	UpdateAllEntities(deltaSeconds);
}

void Map::Render() const
{
	m_material.SetModelMatrix(m_modelMatrix);
	ShaderProgram* prog = m_material.GetProgram();
	int numOfTextures = s_associatedSpriteSheetsByIdx->size();
	prog->SetProgramValueInt("gColorChoice", &m_colorSchemeChoice, 1);
	prog->SetProgramValueInt("numOfTextures", &numOfTextures, 1);

	m_bottomLayerMesh.DrawWith(&m_material);
	m_mesh.DrawWith(&m_material);

	RenderAllEntities();

	m_material.SetModelMatrix(m_modelMatrix);


	prog->SetProgramValueInt("gColorChoice", &m_colorSchemeChoice, 1);
	prog->SetProgramValueInt("numOfTextures", &numOfTextures, 1);
	m_topLayerMesh.DrawWith(&m_material);
	m_topLayerMesh2.DrawWith(&m_material);
}

void Map::AdjustCameraPositionBasedOffBounds(Vector2& position)
{
	if (!m_constrainCameraBounds)
	{
		return;
	}
	Vector2 windowSize = g_TheGame->GetWindowSize();
	Vector2 halfSize = windowSize * 0.5f;
	Vector2 blView = position - halfSize;
	Vector2 trView = position + halfSize;

	Vector2 trDifference = m_TRPos - trView;
	Vector2 blDifference = m_BLPos - blView;
	if (trDifference.x < 0.f && blDifference.x > 0.f)
	{
		position.x = m_mapCenter.x;
	}
	else if (trDifference.x < 0.f)
	{
		position.x = position.x + trDifference.x;
	}
	else if (blDifference.x > 0.f)
	{
		position.x = position.x + blDifference.x;
	}

	if (trDifference.y < 0.f && blDifference.y > 0.f)
	{
		position.y = m_mapCenter.y;
	}
	else if (trDifference.y < 0.f)
	{
		position.y = position.y + trDifference.y;
	}
	else if (blDifference.y > 0.f)
	{
		position.y = position.y + blDifference.y;
	}
}


void Map::UnregisterAllTilesFromEvents()
{
	for (size_t tileIdx = 0; tileIdx < m_numberOfTiles; tileIdx++)
	{
		Tile& tile = m_tiles[tileIdx];
		tile.UnsignUpForEvents();
	}
}

void Map::RegisterAllTilesToEventsIfNeed()
{
	for (size_t tileIdx = 0; tileIdx < m_numberOfTiles; tileIdx++)
	{
		Tile& tile = m_tiles[tileIdx];
		tile.SignUpForEvents();
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Setters
void Map::PushBackQuest(Quest* quest)
{
	if (quest == nullptr)
	{
		return;
	}
	m_questAvailableToStart.push_back(quest);
}

void Map::StartQuest(size_t idx)
{
	BaseGameEntity* entity = BaseGameEntity::GetEditablePointerToPlayer();
	if (idx >= m_questAvailableToStart.size()
		|| entity == nullptr)
	{
		return;
	}
	Player* player = dynamic_cast<Player*>(entity);
	if (player == nullptr)
	{
		return;
	}
	Quest* quest = m_questAvailableToStart[idx];
	m_questAvailableToStart.erase(m_questAvailableToStart.begin() + idx);
	if (quest == nullptr)
	{
		return;
	}
	player->AddQuest(quest);
}

void Map::SetTileCollisionSize(const Vector2& collisionSize)
{
	m_perTileCollision.SetBoxSize(collisionSize);
}

void Map::RemoveAllTilesFromEventCalls()
{
	for (size_t tileIdx = 0; tileIdx < m_tiles.size(); tileIdx++)
	{
		Tile& tile = m_tiles[tileIdx];
		tile.SignUpForEvents();
	}
}

void Map::AddAllTilesFromEventCalls()
{
	for (size_t tileIdx = 0; tileIdx < m_tiles.size(); tileIdx++)
	{
		Tile& tile = m_tiles[tileIdx];
		tile.UnsignUpForEvents();
	}
}

void Map::SetMapName(const std::string& mapName)
{
	m_MapName = mapName;
}

void Map::SetMapDialogueName(const std::string mapDialogueName)
{
	m_MapDialogueName = mapDialogueName;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Getters
void Map::AddAllQuestsThatReferenceEntityToAList(const BaseGameEntity* entity, 
	std::vector<Quest*>& quests) const
{
	for (size_t i = 0; i < m_questAvailableToStart.size(); i++)
	{
		Quest* quest = m_questAvailableToStart[i];
		if (quest != nullptr && quest->GetIfQuestReferencesEntity(entity))
		{
			quests.push_back(quest);
		}
	}
}

void Map::GetAllEntitiesAndTileInfoForDeveloperDebug(std::vector<DeveloperDebugInfoBox>& infoBoxes,
	const TILE_COORDS& coords) const
{
	if (coords.x < 0
		|| coords.y < 0
		|| coords.x >= m_mapWidthHeight.x
		|| coords.y >= m_mapWidthHeight.y)
	{
		return;
	}
	int tileIdx = GetTileIdxForCoords(coords);
	m_tiles[tileIdx].GetTileInfoForDeveloperDebug(infoBoxes);

	for (size_t entityType = 0; entityType < m_BaseGameEntitiesByType.size(); entityType++)
	{
		const std::vector<BaseGameEntity*>& entities = m_BaseGameEntitiesByType[entityType];
		for (size_t entityIdx = 0; entityIdx < entities.size(); entityIdx++)
		{
			const BaseGameEntity* entity = entities[entityIdx];
			if (entity != nullptr
				&& entity->GetTileCoords() == coords)
			{
				entity->GetEntityInfoForDeveloperDebug(infoBoxes);
			}
		}
	}
}

const std::vector<Quest*>& Map::GetQuests() const
{
	return m_questAvailableToStart;
}

const std::vector<Quest*>* Map::GetQuestsPtr() const
{
	return &m_questAvailableToStart;
}

std::vector<Quest*>* Map::GetQuestsEditablePtr()
{
	return &m_questAvailableToStart;
}

std::vector<std::vector<BaseGameEntity*>>* Map::GetEntitiesByTypePtr()
{
	return &m_BaseGameEntitiesByType;
}

const std::string& Map::GetMapName() const
{
	return m_MapName;
}

const std::string& Map::GetMapDialogueName() const
{
	return m_MapDialogueName;
}

const Vector2& Map::GetMapCenter() const
{
	return m_mapCenter;
}

const Vector2& Map::GetMapSize() const
{
	return m_mapSize;
}

Vector2 Map::GetHalfMapSize() const
{
	return m_mapSize / 2.f;
}

const Vector2& Map::GetBLMapPos() const
{
	return m_BLPos;
}

const Vector2& Map::GetTRMapPos() const
{
	return m_TRPos;
}

void Map::GetAllUninhabittedTileCenters(std::vector<Vector2>& positions, bool guaranteeNotSolid) const
{
	positions.clear();
	for (size_t tileIdx = 0; tileIdx < m_numberOfTiles; tileIdx++)
	{
		const Tile& tile = m_tiles[tileIdx];
		if (guaranteeNotSolid && tile.GetTileIsSolid())
		{
			continue;
		}
		IntVector2 tileCoords = tile.GetTileCoords();
		bool inhabittedAlready = false;
		for (size_t entityTypeIdx = 0; entityTypeIdx < m_BaseGameEntitiesByType.size(); entityTypeIdx++)
		{
			if (entityTypeIdx != AGENT_ENTITY
				&& entityTypeIdx != FEATURE_ENTITY)
			{
				continue;
			}
			for (size_t entityIdx = 0; entityIdx < m_BaseGameEntitiesByType[entityTypeIdx].size(); entityIdx++)
			{
				const BaseGameEntity* entity = m_BaseGameEntitiesByType[entityTypeIdx][entityIdx];
				if (entity->GetTileCoords() == tileCoords)
				{
					inhabittedAlready = true;
					break;
				}
			}
			if (inhabittedAlready)
			{
				break;
			}
		}
		if (inhabittedAlready)
		{
			continue;
		}
		positions.push_back(GetCenterOfTileAtTileCoords(tileCoords));
	}
}

void Map::GetAllUninhabittedTileCenters(std::vector<TILE_COORDS>& positions, bool guaranteeNotSolid) const
{
	positions.clear();
	for (size_t tileIdx = 0; tileIdx < m_numberOfTiles; tileIdx++)
	{
		const Tile& tile = m_tiles[tileIdx];
		if (guaranteeNotSolid && tile.GetTileIsSolid())
		{
			continue;
		}
		IntVector2 tileCoords = tile.GetTileCoords();
		bool inhabittedAlready = false;
		for (size_t entityTypeIdx = 0; entityTypeIdx < m_BaseGameEntitiesByType.size(); entityTypeIdx++)
		{
			if (entityTypeIdx != AGENT_ENTITY
				&& entityTypeIdx != FEATURE_ENTITY)
			{
				continue;
			}
			for (size_t entityIdx = 0; entityIdx < m_BaseGameEntitiesByType[entityTypeIdx].size(); entityIdx++)
			{
				const BaseGameEntity* entity = m_BaseGameEntitiesByType[entityTypeIdx][entityIdx];
				if (entity->GetTileCoords() == tileCoords)
				{
					inhabittedAlready = true;
					break;
				}
			}
			if (inhabittedAlready)
			{
				break;
			}
		}
		if (inhabittedAlready)
		{
			continue;
		}
		positions.push_back((tileCoords));
	}
}

Vector2 Map::GetRandomPositionOnMap() const
{
	if (m_mapWidthHeight.x == 0 || m_mapWidthHeight.y == 0)
	{
		return Vector2::vec2_zeros;
	}
	TILE_COORDS ranTile = TILE_COORDS((int)((float)m_mapWidthHeight.x * GetRandomFloatZeroToOne()),
		(int)((float)m_mapWidthHeight.y * GetRandomFloatZeroToOne()));
	return GetCenterOfTileAtTileCoords(ranTile);
}

Vector2 Map::GetRandomNonSolidPositionOnMap(bool guaranteeNoEntitiesAreInTileIfPossible) const
{
	if (m_indexsOfAllNonSolidTiles.size() == 0)
	{
		ERROR_AND_DIE(
			"Okay, why did you make a map thats 100% made of solid tiles? And then try to spawn entities on it?");
	}
	std::vector<unsigned int> indexsOfAllNonSolidTiles = m_indexsOfAllNonSolidTiles;
	if (guaranteeNoEntitiesAreInTileIfPossible)
	{
		for (size_t tileIdxIdx = 0; tileIdxIdx < indexsOfAllNonSolidTiles.size(); tileIdxIdx++)
		{
			unsigned int tileIndex = indexsOfAllNonSolidTiles[tileIdxIdx];
			TILE_COORDS coords = m_tiles[tileIndex].GetTileCoords();
			bool hasEntityInIt = false;
			for (size_t entityTypeIdx = 0; entityTypeIdx < m_BaseGameEntitiesByType.size(); entityTypeIdx++)
			{
				if (entityTypeIdx == ITEM_ENTITY)
				{
					continue;
				}
				for (size_t entityIdx = 0; entityIdx < m_BaseGameEntitiesByType[entityTypeIdx].size(); entityIdx++)
				{
					BaseGameEntity* entity = m_BaseGameEntitiesByType[entityTypeIdx][entityIdx];
					if (entityTypeIdx == FEATURE_ENTITY)
					{
						Feature* feature = dynamic_cast<Feature*>(entity);
						if (feature == nullptr
							|| feature->GetSolid() == false)
						{
							continue;
						}
						if (feature->GetTileCoords() == coords)
						{
							indexsOfAllNonSolidTiles.erase(indexsOfAllNonSolidTiles.begin() + tileIdxIdx);
							hasEntityInIt = true;
							break;
						}

					}
					else if (entityTypeIdx == AGENT_ENTITY)
					{
						if (entity != nullptr && entity->GetTileCoords() == coords)
						{
							indexsOfAllNonSolidTiles.erase(indexsOfAllNonSolidTiles.begin() + tileIdxIdx);
							hasEntityInIt = true;
							break;
						}
					}
				}
				if (hasEntityInIt)
				{
					break;
				}
			}
		}

	}

	if (indexsOfAllNonSolidTiles.size() == 0)
	{
		indexsOfAllNonSolidTiles = m_indexsOfAllNonSolidTiles;
	}

	int nonSolidIdx = GetRandomIntLessThan(indexsOfAllNonSolidTiles.size());
	int nonSolidTileIdx = m_indexsOfAllNonSolidTiles[nonSolidIdx];
	const Tile& tile = m_tiles[nonSolidTileIdx];
	return tile.GetPosition();
}

Vector2 Map::GetCenterOfTileAtTileCoords(const TILE_COORDS& coords) const
{
	const Tile* tile = GetTileAtCoords(coords);
	if (tile == nullptr)
	{
		return Vector2::vec2_zeros;
	}
	Vector2 position = tile->GetBLPos() + (Tile::GetTileScale() * 0.5f);
	return position;
}

TILE_COORDS Map::ConvertPositionToTileCoords(const Vector2& pos) const
{
	Vector2 adjustedPos = pos - m_BLPos;
	TILE_COORDS tileCoords = TILE_COORDS((int)floor(adjustedPos.x), (int)floor(adjustedPos.y));
	return tileCoords;
}

const TILE_COORDS& Map::GetMapWidthHeight() const
{
	return m_mapWidthHeight;
}

const Tile* Map::GetTileAtCoords(const TILE_COORDS& coords) const
{	
	int idx = GetTileIdxForCoords(coords);
	if (idx == -1)
	{
		return nullptr;
	}
	return &m_tiles[idx];

}

const Tile* Map::GetTileAtIdx(int idx) const
{
	if (idx < 0 || (size_t)idx >= m_tiles.size())
	{
		return nullptr;
	}
	return &m_tiles[idx];
}


Tile* Map::GetEditableTileAtCoords(const TILE_COORDS& coords)
{
	int idx = GetTileIdxForCoords(coords);
	if (idx == -1)
	{
		return nullptr;
	}
	return &m_tiles[idx];
}

Tile* Map::GetEditableTileAtIdx(int idx)
{
	if (idx < 0 || (size_t)idx >= m_tiles.size())
	{
		return nullptr;
	}
	return &m_tiles[idx];
}

int Map::GetTileIdxForCoords(const TILE_COORDS& coords) const
{
	if (coords.x < 0 || coords.y < 0
		|| coords.x >= m_mapWidthHeight.x || coords.y >= m_mapWidthHeight.y)
	{
		return -1;
	}
	return coords.x + (coords.y * m_mapWidthHeight.x);
}

TILE_COORDS Map::GetTileCoordsForIdx(int idx) const
{
	if (idx < 0 || (size_t)idx >= m_numberOfTiles)
	{
		return TILE_COORDS(-1, -1);
	}
	TILE_COORDS coords(idx % m_mapWidthHeight.x, idx / m_mapWidthHeight.x);
	return coords;
}

Quest* Map::GetQuestByTitle(const std::string& questTitle) const
{
	std::string lowerCase = MakeLower(questTitle);
	for (size_t i = 0; i < m_questAvailableToStart.size(); i++)
	{
		Quest* quest = m_questAvailableToStart[i];
		if (quest != nullptr 
			&& quest->GetLowerCaseQuestTitle() == lowerCase)
		{
			return quest;
		}
	}

	return nullptr;
}

void Map::DestroyAllGameEntities()
{
	for (size_t entityType = 0; entityType < m_BaseGameEntitiesByType.size(); entityType++)
	{
		for (size_t entityIdx = 0; entityIdx < m_BaseGameEntitiesByType[entityType].size(); entityIdx++)
		{
			BaseGameEntity* entity = m_BaseGameEntitiesByType[entityType][entityIdx];
			if (entity != nullptr)
			{
				EventSystem::GlobalUnregisterFromAllEvents(entity);
				delete entity;
			}
		}
		m_BaseGameEntitiesByType[entityType].clear();
	}
	m_BaseGameEntitiesByType.clear();
}

void Map::RebuildMeshNew()
{
	BaseGameEntity::TestIfPlayerIsOnTileWithTransparentTop();
	s_associatedSpriteSheetsByIdx->clear();

	//Vertices
	size_t numberOfTileVertsExpected = m_numberOfTiles * 4;
	s_Model->clear();
	s_BottomModel->clear();
	s_TopModel->clear();
	s_TopModel2->clear();
	s_BottomModel->reserve(numberOfTileVertsExpected);
	s_Model->reserve(numberOfTileVertsExpected);
	s_TopModel->reserve(numberOfTileVertsExpected);
	s_TopModel2->reserve(numberOfTileVertsExpected);

	//Indices
	size_t numberOfTileIndicesExpected = m_numberOfTiles * 6;
	s_indices->clear();
	s_TopIndices->clear();
	s_BottomIndices->clear();
	s_TopIndices2->clear();
	s_indices->reserve(numberOfTileIndicesExpected);
	s_TopIndices->reserve(numberOfTileIndicesExpected);
	s_BottomIndices->reserve(numberOfTileIndicesExpected);
	s_TopIndices2->reserve(numberOfTileIndicesExpected);

	for (size_t tileIdx = 0; tileIdx < m_numberOfTiles; tileIdx++)
	{
		const Tile& tile = m_tiles[tileIdx];
		AddTileVerts(tile);
	}

	m_mesh.setVBO(*s_Model);
	m_mesh.setIBO(*s_indices);
	m_topLayerMesh.setVBO(*s_TopModel);
	m_topLayerMesh.setIBO(*s_TopIndices);
	m_bottomLayerMesh.setVBO(*s_BottomModel);
	m_bottomLayerMesh.setIBO(*s_BottomIndices);
	m_topLayerMesh2.setVBO(*s_TopModel2);
	m_topLayerMesh2.setIBO(*s_TopIndices2);

	for(size_t resIdx = 0; resIdx < s_associatedSpriteSheetsByIdx->size(); resIdx++)
	{
		std::string valToSet = "gDiffuseTex[" + std::to_string(resIdx) + "]";
		if (m_material.SetTexture(s_associatedSpriteSheetsByIdx->at(resIdx), resIdx, valToSet, resIdx) == false)
		{
			m_material.AddTexture(s_associatedSpriteSheetsByIdx->at(resIdx), resIdx, valToSet);
		}
	}
}

void Map::AddTileVerts(const Tile& tile)
{
	SpriteVertWithMultiSprites vert;
	Vector2 blOfTile = tile.GetBLPos();
	Vector2 trOfTile = blOfTile + Tile::GetTileScale();
	Rgba tileTint = tile.GetTint();
	const SpriteResource* res = tile.GetCurrentSpriteResource();
	const SpriteResource* topRes = tile.GetCurrentTopSpriteResource();
	const SpriteResource* topRes2 = tile.GetCurrentTopSpriteResource2();
	const SpriteResource* botRes = tile.GetCurrentBottomSpriteResource();
	const TileDefinition* def = tile.GetTileDefinition();
	
	if (s_EnableEnforcingAllTilesAsNotNull)
	{
		ASSERT_OR_DIE(res != nullptr
			|| topRes != nullptr
			|| topRes2 != nullptr
			|| botRes != nullptr, "All resources for the tile is null!");
	}
	else
	{
		if (res == nullptr && topRes == nullptr
			&& topRes2 == nullptr
			&& botRes == nullptr)
		{
			return;
		}
	}
	
	bool resExists = res != nullptr;
	bool topResExists = topRes != nullptr;
	bool topResExists2 = topRes2 != nullptr;
	bool botResExists = botRes != nullptr;
	AABB2 textureCoords = AABB2::ZERO_TO_ONE;
	AABB2 topResTextureCoords = AABB2::ZERO_TO_ONE;
	AABB2 topResTextureCoords2 = AABB2::ZERO_TO_ONE;
	AABB2 bottomResTextureCoords = AABB2::ZERO_TO_ONE;
	Texture* topResource2 = nullptr;
	Texture* topResource = nullptr;
	Texture* resource = nullptr;
	Texture* bottomResource = nullptr;

	AddTileVertsSetResource(botResExists, bottomResource, bottomResTextureCoords, botRes);
	AddTileVertsSetResource(resExists, resource, textureCoords, res);
	AddTileVertsSetResource(topResExists, topResource, topResTextureCoords, topRes);
	AddTileVertsSetResource(topResExists2, topResource2, topResTextureCoords2, topRes2);

	int bottomResIdx = -1;
	int resIdx = -1;
	int topResIdx = -1;
	int topRes2Idx = -1;

	for (size_t textureIdx = 0; textureIdx < s_associatedSpriteSheetsByIdx->size(); textureIdx++)
	{
		Texture* tex = s_associatedSpriteSheetsByIdx->at(textureIdx);
		if (resource == tex)
		{
			resIdx = textureIdx;
		}
		if (topResource == tex)
		{
			topResIdx = textureIdx;
		}
		if (topResource2 == tex)
		{
			topRes2Idx = textureIdx;
		}
		if (bottomResource == tex)
		{
			bottomResIdx = textureIdx;
		}
		if ((bottomResIdx != -1 || bottomResource == nullptr)
			&& (resIdx != -1 || resource == nullptr)
			&& (topResIdx != -1 || topResource == nullptr)
			&& (topRes2Idx != -1 || topResource2 == nullptr))
		{
			break;
		}
	}

	AddTileVertsAddUnregisteredTextureIfApplicable(bottomResIdx, bottomResource);
	AddTileVertsAddUnregisteredTextureIfApplicable(resIdx, resource);
	AddTileVertsAddUnregisteredTextureIfApplicable(topResIdx, topResource);
	AddTileVertsAddUnregisteredTextureIfApplicable(topRes2Idx, topResource2);

	size_t numOfBottomVerts = s_BottomModel->size();
	size_t numOfVerts = s_Model->size();
	size_t numOfTopVerts = s_TopModel->size();
	size_t numOfTopVerts2 = s_TopModel2->size();

	//BL Position
	vert.m_color = tileTint;
	vert.m_pos = Vector2(blOfTile.x, blOfTile.y);
	bool topLayerDisappears = false;
	bool topLayer2Disappears = false;
	if (def != nullptr)
	{
		topLayerDisappears = def->GetTopLayerDisappearsIfPlayerUnderneath();
		topLayer2Disappears = def->GetTopLayer2DisappearsIfPlayerUnderneath();
	}

	AddTileVertsBasedOnResourceAvailability(vert, resExists, topResExists, topResExists2,
		botResExists, textureCoords, topResTextureCoords, topResTextureCoords2, bottomResTextureCoords,
		false, false, topLayerDisappears, topLayer2Disappears,
		bottomResource, resource, topResource, topResource2,
		bottomResIdx, resIdx, topResIdx, topRes2Idx);

	//BR Position
	vert.m_pos = Vector2(trOfTile.x, blOfTile.y);
	AddTileVertsBasedOnResourceAvailability(vert, resExists, topResExists, topResExists2,
		botResExists, textureCoords, topResTextureCoords, topResTextureCoords2, bottomResTextureCoords,
		true, false, topLayerDisappears, topLayer2Disappears,
		bottomResource, resource, topResource, topResource2,
		bottomResIdx, resIdx, topResIdx, topRes2Idx);

	//TR Position
	vert.m_pos = Vector2(trOfTile.x, trOfTile.y);
	AddTileVertsBasedOnResourceAvailability(vert, resExists, topResExists, topResExists2,
		botResExists, textureCoords, topResTextureCoords, topResTextureCoords2, bottomResTextureCoords,
		true, true, topLayerDisappears, topLayer2Disappears,
		bottomResource, resource, topResource, topResource2,
		bottomResIdx, resIdx, topResIdx, topRes2Idx);

	//TL Position
	vert.m_pos = Vector2(blOfTile.x, trOfTile.y);
	AddTileVertsBasedOnResourceAvailability(vert, resExists, topResExists, topResExists2,
		botResExists, textureCoords, topResTextureCoords, topResTextureCoords2, bottomResTextureCoords,
		false, true, topLayerDisappears, topLayer2Disappears,
		bottomResource, resource, topResource, topResource2,
		bottomResIdx, resIdx, topResIdx, topRes2Idx);

	if (botResExists)
	{
		PushQuadOfVerticesOntoIndiceArray(s_BottomIndices, numOfBottomVerts);
	}

	if (resExists)
	{
		PushQuadOfVerticesOntoIndiceArray(s_indices, numOfVerts);
	}

	if (topResExists)
	{
		PushQuadOfVerticesOntoIndiceArray(s_TopIndices, numOfTopVerts);
	}

	if (topResExists2)
	{
		PushQuadOfVerticesOntoIndiceArray(s_TopIndices2, numOfTopVerts2);
	}
}

void Map::AddTileVertsSetResource(bool setResource, Texture*& resource,
	AABB2& textureCoords, const SpriteResource* res)
{
	if (setResource)
	{
		resource = res->GetSpriteSheet();
		textureCoords = res->GetTextureCoords();
	}
}

void Map::AddTileVertsAddUnregisteredTextureIfApplicable(int& resIdx, Texture* resource)
{
	if (resIdx == -1 && resource != nullptr)
	{
		resIdx = s_associatedSpriteSheetsByIdx->size();
		s_associatedSpriteSheetsByIdx->push_back(resource);
	}
}

void Map::PushQuadOfVerticesOntoIndiceArray(std::vector<unsigned int>*& indices, size_t numOfVerts)
{
	indices->push_back(numOfVerts + 0);
	indices->push_back(numOfVerts + 1);
	indices->push_back(numOfVerts + 2);
	indices->push_back(numOfVerts + 0);
	indices->push_back(numOfVerts + 2);
	indices->push_back(numOfVerts + 3);
}

void Map::AddTileVertsBasedOnResourceAvailability(SpriteVertWithMultiSprites& vert,
	bool resExists, bool topResExists, bool topRes2Exists, bool botResExists,
	const AABB2& textureCoords, const AABB2& topResTextureCoords, const AABB2& topResTextureCoords2,
	const AABB2& bottomResTextureCoords, bool maxX, bool maxY,
	bool topCanVanishIfPlayerUnderneath, bool top2CanVanishIfPlayerUnderneath,
	Texture* bottomResource, Texture* resource, Texture* topResource, Texture* topResource2,
	int bottomResIdx, int resIdx, int topResIdx, int topRes2Idx)
{
	if (botResExists && bottomResource != nullptr)
	{
		vert.m_spriteIdx = bottomResIdx;
		SetVertTextureCoordsToMinMax(vert, maxX, maxY, bottomResTextureCoords);
		s_BottomModel->push_back(vert);
	}

	if (resExists && resource != nullptr)
	{
		vert.m_spriteIdx = resIdx;
		SetVertTextureCoordsToMinMax(vert, maxX, maxY, textureCoords);
		s_Model->push_back(vert);
	}

	if (topResExists && topResource != nullptr)
	{
		vert.m_spriteIdx = topResIdx;
		if (topCanVanishIfPlayerUnderneath && BaseGameEntity::GetIfPlayerIsInTileWithTransparentTop())
		{
			vert.m_color.Alpha = 0.f;
		}
		SetVertTextureCoordsToMinMax(vert, maxX, maxY, topResTextureCoords);
		s_TopModel->push_back(vert);
		vert.m_color.Alpha = 1.f;
	}

	if (topRes2Exists && topResource2 != nullptr)
	{
		vert.m_spriteIdx = topRes2Idx;
		if (top2CanVanishIfPlayerUnderneath && BaseGameEntity::GetIfPlayerIsInTileWithTransparentTop())
		{
			vert.m_color.Alpha = 0.f;
		}
		SetVertTextureCoordsToMinMax(vert, maxX, maxY, topResTextureCoords2);
		s_TopModel2->push_back(vert);
		vert.m_color.Alpha = 1.f;
	}
}

void Map::SetVertTextureCoordsToMinMax(SpriteVertWithMultiSprites& vert, 
	bool maxX, bool maxY, const AABB2& textureCoords) const
{
	if (maxX)
	{
		vert.m_texCoords.x = textureCoords.maxs.x;
	}
	else
	{
		vert.m_texCoords.x = textureCoords.mins.x;
	}
	if (maxY)
	{
		vert.m_texCoords.y = textureCoords.maxs.y;
	}
	else
	{
		vert.m_texCoords.y = textureCoords.mins.y;
	}
}

void Map::WriteOutAllMapActiveDataToString(std::string& str, int indentationAmt,
	bool PushTileEventsPerMapContentToDebugFile,
	bool PushTilesPerMapContentToDebugFile,
	bool PushAgentsPerMapContentToDebugFile,
	bool PushFeaturesPerMapContentToDebugFile,
	bool PushActiveQuestContentToDebugFile) const
{
	WriteOutTileEventDataToString(str, indentationAmt, PushTileEventsPerMapContentToDebugFile);
	WriteOutTilesDataToString(str, indentationAmt, PushTilesPerMapContentToDebugFile);
	WriteOutAgentsDataToString(str, indentationAmt, PushAgentsPerMapContentToDebugFile);
	WriteOutFeaturesDataToString(str, indentationAmt, PushFeaturesPerMapContentToDebugFile);
	WriteOutActiveQuestsDataToString(str, indentationAmt, PushActiveQuestContentToDebugFile);
}


void Map::WriteOutTileEventDataToString(std::string& str, int indentationtAmt,
	bool PushTileEventsPerMapContentToDebugFile) const
{
	if (!PushTileEventsPerMapContentToDebugFile)
	{
		return;
	}

	GameDebuggingCommons::AddIndentation(str, indentationtAmt);
	str += "Number of Tiles with Tile Events: " + std::to_string(m_eventPack.size()) + "\n";

	int nextLevel = indentationtAmt + 3;
	if (m_eventPack.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationtAmt);
		str += "Tile Events this Map: \n";
		for (size_t i = 0; i < m_eventPack.size(); i++)
		{
			const TileEventPack& pack = m_eventPack[i];
			pack.WriteTileEventPackToString(str, nextLevel);
		}
	}
}

void Map::WriteOutTilesDataToString(std::string& str, int indentationtAmt,
	bool PushTilesPerMapContentToDebugFile) const
{
	if (!PushTilesPerMapContentToDebugFile)
	{
		return;
	}

	GameDebuggingCommons::AddIndentation(str, indentationtAmt);
	str += "Number of Tiles on this Map: " + std::to_string(m_tiles.size()) + "\n";

	if (m_tiles.size() > 0)
	{
		int nextLevel = indentationtAmt + 3;
		GameDebuggingCommons::AddIndentation(str, indentationtAmt);
		str += "Tiles on this map: \n";
		for (size_t i = 0; i < m_tiles.size(); i++)
		{
			const Tile& tile = m_tiles[i];
			tile.WriteTileToString(str, nextLevel);
		}
	}
}

void Map::WriteOutAgentsDataToString(std::string& str, int indentationtAmt,
	bool PushAgentsPerMapContentToDebugFile) const
{
	if (!PushAgentsPerMapContentToDebugFile)
	{
		return;
	}

	const std::vector<BaseGameEntity*>& agents = m_BaseGameEntitiesByType[AGENT_ENTITY];

	int nextLevel = indentationtAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationtAmt);
	str += "Number of Agents on this map: " + std::to_string(agents.size()) +"\n";
	if (agents.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationtAmt);
		str += "Agents on this Map: \n";
		for (size_t i = 0; i < agents.size(); i++)
		{
			const BaseGameEntity* agent = agents[i];
			if (agent != nullptr)
			{
				agent->WriteOutEntityImportantDataToStr(str, nextLevel);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL AGENT\n";
			}
		}
	}
}

void Map::WriteOutFeaturesDataToString(std::string& str, int indentationtAmt,
	bool PushFeaturesPerMapContentToDebugFile) const
{
	if (!PushFeaturesPerMapContentToDebugFile)
	{
		return;
	}

	const std::vector<BaseGameEntity*>& features = m_BaseGameEntitiesByType[FEATURE_ENTITY];

	int nextLevel = indentationtAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationtAmt);
	str += "Number of Features on this map: " + std::to_string(features.size()) + "\n";
	if (features.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationtAmt);
		str += "Features on this Map: \n";
		for (size_t i = 0; i < features.size(); i++)
		{
			const BaseGameEntity* feature = features[i];
			if (feature != nullptr)
			{
				feature->WriteOutEntityImportantDataToStr(str, nextLevel);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL FEATURE\n";
			}
		}
	}
}

void Map::WriteOutActiveQuestsDataToString(std::string& str, int indentationtAmt,
	bool PushActiveQuestContentToDebugFile) const
{
	if (!PushActiveQuestContentToDebugFile)
	{
		return;
	}
	m_questAvailableToStart;
	int nextLevel = indentationtAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationtAmt);
	str += "Number of Quests unclaimed on this Map: " + std::to_string(m_questAvailableToStart.size()) + "\n";
	if (m_questAvailableToStart.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationtAmt);
		str += "Quests unclaimed on this Map: \n";
		for (size_t i = 0; i < m_questAvailableToStart.size(); i++)
		{
			const Quest* quest = m_questAvailableToStart[i];
			if (quest != nullptr)
			{
				quest->WriteQuestToString(str, nextLevel);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL QUEST\n";
			}
		}
	}
}

STATIC bool Map::WriteActiveQuestFromMapToString(std::string& str, const std::string& questName,
	int indentationAmt) const
{
	std::string lowerCase = MakeLower(questName);
	for (size_t i = 0; i < m_questAvailableToStart.size(); i++)
	{
		const Quest* quest = m_questAvailableToStart[i];
		if (quest != nullptr
			&& lowerCase == quest->GetLowerCaseQuestTitle())
		{
			GameDebuggingCommons::AddIndentation(str, indentationAmt);
			str += "Quest Found in: Map: " + m_MapName + "\n";

			quest->WriteQuestToString(str, indentationAmt + 3);
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//static methods
STATIC bool Map::WriteActiveQuestByNameToStringFromAllMaps(std::string& str, const std::string& questName,
	int indentationAmt)
{
	for (std::map<std::string, Map*>::const_iterator it = GameCommons::s_AllLoadedMaps.begin();
		it != GameCommons::s_AllLoadedMaps.end(); it++)
	{
		Map* map = it->second;
		if (map != nullptr)
		{
			if (map->WriteActiveQuestFromMapToString(str, questName, indentationAmt))
			{
				return true;
			}
		}
	}
	return false;
}

STATIC void Map::WriteAllMapActiveDatasToString(std::string& str, int indentationAmt,
	bool PushTileEventsPerMapContentToDebugFile,
	bool PushTilesPerMapContentToDebugFile,
	bool PushAgentsPerMapContentToDebugFile,
	bool PushFeaturesPerMapContentToDebugFile,
	bool PushActiveQuestContentToDebugFile)
{
	int nextLevel = indentationAmt + 3;

	if (GameCommons::s_MapNamesInOrderTheyWereLoaded.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Order in which all maps were loaded, by name: \n";
		for (size_t i = 0; i < GameCommons::s_MapNamesInOrderTheyWereLoaded.size(); i++)
		{
			GameDebuggingCommons::AddIndentation(str, nextLevel);
			str += GameCommons::s_MapNamesInOrderTheyWereLoaded[i] + "\n";
		}
	}

	if (GameCommons::s_AllLoadedMaps.size() > 0)
	{
		
		for (std::map<std::string, Map*>::const_iterator it = GameCommons::s_AllLoadedMaps.begin();
			it != GameCommons::s_AllLoadedMaps.end(); it++)
		{
			GameDebuggingCommons::AddIndentation(str, indentationAmt);
			str += "Map Name: " + it->first + "\n";
			if (it->second != nullptr)
			{
				it->second->WriteOutAllMapActiveDataToString(str, nextLevel,
					PushTileEventsPerMapContentToDebugFile,
					PushTilesPerMapContentToDebugFile,
					PushAgentsPerMapContentToDebugFile,
					PushFeaturesPerMapContentToDebugFile,
					PushActiveQuestContentToDebugFile);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "MAP IS NULL!\n";
			}
		}
	}


	const BaseGameEntity* playerEntity = BaseGameEntity::GetPointerToPlayer();
	const Player* player = dynamic_cast<const Player*>(playerEntity);
	if (PushActiveQuestContentToDebugFile
		&& player != nullptr)
	{
		player->WriteOutAllQuestsToString(str, indentationAmt);
	}
}

STATIC void Map::WriteSingleMapActiveDatasToString(std::string& str, const std::string& mapLookingFor,
	int indentationAmt,
	bool PushTileEventsPerMapContentToDebugFile,
	bool PushTilesPerMapContentToDebugFile,
	bool PushAgentsPerMapContentToDebugFile,
	bool PushFeaturesPerMapContentToDebugFile,
	bool PushActiveQuestContentToDebugFile)
{
	std::string lowerCase = MakeLower(mapLookingFor);
	std::map<std::string, Map*>::const_iterator it = GameCommons::s_AllLoadedMaps.find(lowerCase);
	if (it == GameCommons::s_AllLoadedMaps.end())
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "--------------\n";
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Unable to find map: " + mapLookingFor + "\n";
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Map may not of been loaded yet\n";
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "--------------\n";
		return;
	}

	int nextLevel = indentationAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Map Name: " + it->first + "\n";
	if (it->second != nullptr)
	{
		it->second->WriteOutAllMapActiveDataToString(str, nextLevel,
			PushTileEventsPerMapContentToDebugFile,
			PushTilesPerMapContentToDebugFile,
			PushAgentsPerMapContentToDebugFile,
			PushFeaturesPerMapContentToDebugFile,
			PushActiveQuestContentToDebugFile);
	}
	else
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += "MAP IS NULL!\n";
	}

	const BaseGameEntity* playerEntity = BaseGameEntity::GetPointerToPlayer();
	const Player* player = dynamic_cast<const Player*>(playerEntity);
	if (PushActiveQuestContentToDebugFile
		&& player != nullptr)
	{
		player->WriteOutAllQuestsToString(str, indentationAmt);
	}
}

STATIC bool Map::UpdateLoadingGlobalVariables()
{
	uint64_t frameStart = Performance::GetCurrentPerformanceCount();
	if (s_GlobalVariablesLoad == nullptr)
	{
		s_GlobalVariablesLoad = new GlobalVariablesLoadingData();
	}

	if (!s_GlobalVariablesLoad->m_receivedFiles)
	{
		GameSettings* settings = TheGame::GetGameSettings();
		if (settings != nullptr)
		{
			s_GlobalVariablesLoad->m_amountOfTimeToTakeToReadNodesPerFrame
				= settings->m_amountOfTimeLoadingGlobalVariablesPerFrame;
			s_GlobalVariablesLoad->m_globalVariablesXMLFolderLocation 
				= settings->m_commonGlobalVariablesFileDirectory;
		}
		s_GlobalVariablesLoad->m_childIdx = 0;
		s_GlobalVariablesLoad->m_readNode = false;

		s_GlobalVariablesLoad->m_files = 
			FileUtils::EnumerateFilesInDirectory(s_GlobalVariablesLoad->m_globalVariablesXMLFolderLocation,
			"*", true);

		uint64_t currentFrame = Performance::GetCurrentPerformanceCount();
		uint64_t FrameDiff = currentFrame - frameStart;
		double frameDiffDouble = Performance::PerformanceCountToSeconds(FrameDiff);
		if (frameDiffDouble >= s_GlobalVariablesLoad->m_amountOfTimeToTakeToReadNodesPerFrame)
		{
			return false;
		}
	}

	while ((size_t)s_GlobalVariablesLoad->m_fileIdx < s_GlobalVariablesLoad->m_files.size())
	{
		std::string file = s_GlobalVariablesLoad->m_files[s_GlobalVariablesLoad->m_fileIdx];

		if (LoadGlobalVariablesFromXMLNodeFileOnUpdate(file, frameStart))
		{
			return false;
		}
		s_GlobalVariablesLoad->m_fileIdx++;
		s_GlobalVariablesLoad->m_childIdx = 0;
		s_GlobalVariablesLoad->m_readNode = false;

		uint64_t currentFrame = Performance::GetCurrentPerformanceCount();
		uint64_t FrameDiff = currentFrame - frameStart;
		double frameDiffDouble = Performance::PerformanceCountToSeconds(FrameDiff);
		if (frameDiffDouble >= s_GlobalVariablesLoad->m_amountOfTimeToTakeToReadNodesPerFrame)
		{
			return false;
		}
	}

	return true;
}

STATIC bool Map::LoadGlobalVariablesFromXMLNodeFileOnUpdate(const std::string& fileLocation,
	uint64_t& frameStart)
{
	if (fileLocation == ""
		|| s_GlobalVariablesLoad == nullptr)
	{
		return false;
	}

	if (!s_GlobalVariablesLoad->m_readNode)
	{
		s_GlobalVariablesLoad->m_readNode = true;
		bool errorOccurred = false;
		s_GlobalVariablesLoad->currentNodeReading = EngineXMLParser::ParseXMLFile(fileLocation,
			s_GlobalVariablesLoad->m_tileDefinitionXMLRootNodeName.c_str(),
			errorOccurred);
		if (errorOccurred)
		{
			return false;
		}
	}

	for (int childIdx = s_GlobalVariablesLoad->m_childIdx; 
		childIdx < s_GlobalVariablesLoad->currentNodeReading.nChildNode();
		childIdx++)
	{
		XMLNode child = s_GlobalVariablesLoad->currentNodeReading.getChildNode(childIdx);
		ReadGlobalVariableFromXMLNode(child);


		uint64_t currentFrame = Performance::GetCurrentPerformanceCount();
		uint64_t FrameDiff = currentFrame - frameStart;
		double frameDiffDouble = Performance::PerformanceCountToSeconds(FrameDiff);
		if (frameDiffDouble >= s_GlobalVariablesLoad->m_amountOfTimeToTakeToReadNodesPerFrame)
		{
			return true;
		}
	}
	return false;
}

STATIC void Map::ReadGlobalVariableFromXMLNode(const XMLNode& node)
{
	std::string variableName = "";
	std::string nodeName = MakeLower(node.getName());
	if (nodeName == "float")
	{
		float value = 0.0f;

		for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = node.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "variable"
				|| attrName == "variablename")
			{
				variableName = MakeLower(attr.lpszValue);
			}
			else if (attrName == "value"
				|| attrName == "defaultvalue")
			{
				value = (float)atof(attr.lpszValue);
			}
		}

		if (variableName == "")
		{
			return;
		}

		SetGlobalVariable(variableName, value);
	}
	else if (nodeName == "bool"
		|| nodeName == "boolean")
	{
		bool value = false;

		for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = node.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "variable"
				|| attrName == "variablename")
			{
				variableName = MakeLower(attr.lpszValue);
			}
			else if (attrName == "value"
				|| attrName == "defaultvalue")
			{
				std::string attrVal = MakeLower(attr.lpszValue);
				if (attrVal == "false"
					|| attrVal == "f")
				{
					value = false;
				}
				else if (attrVal == "true"
					|| attrVal == "t")
				{
					value = true;
				}
			}
		}

		if (variableName == "")
		{
			return;
		}
		SetGlobalVariable(variableName, value);
	}
	else if (nodeName == "text")
	{
		std::string value = "";
		for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = node.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "variable"
				|| attrName == "variablename")
			{
				variableName = MakeLower(attr.lpszValue);
			}
			else if (attrName == "value"
				|| attrName == "defaultvalue")
			{
				value = std::string(attr.lpszValue);
			}
		}
		if (variableName == "")
		{
			return;
		}
		SetGlobalVariable(variableName, value);
	}
	else if (nodeName == "int"
		|| nodeName == "integer")
	{
		int value = 0;
		for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = node.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "variable"
				|| attrName == "variablename")
			{
				variableName = MakeLower(attr.lpszValue);
			}
			else if (attrName == "value"
				|| attrName == "defaultvalue")
			{
				value = atoi(attr.lpszValue);
			}
		}
		if (variableName == "")
		{
			return;
		}
		SetGlobalVariable(variableName, value);
	}
}

STATIC bool Map::UpdateLoadingMap()
{
	s_CurrentlyLoadingMap = MapFiles::GetLoadingMap();
	MapFiles::UpdateMapLoading();
	return s_CurrentlyLoadingMap;
}

STATIC bool Map::GetLoadingMap()
{
	return s_CurrentlyLoadingMap;
}

STATIC bool Map::CreateMap()
{
	if (s_CurrentMap != nullptr)
	{
		return false;
	}
	s_CurrentMap = new Map();
	return true;
}

STATIC void Map::RegisterMap(Map* map)
{
	if (map == nullptr)
	{
		return;
	}
	std::string mapName = MakeLower(map->m_MapName);

	std::map<std::string, Map*>::iterator it = GameCommons::s_AllLoadedMaps.find(mapName);
	if(it != GameCommons::s_AllLoadedMaps.end())
	{
		ERROR_AND_DIE("MAP ALREADY REGISTERED!!");
	}
	else
	{
		GameCommons::s_MapNamesInOrderTheyWereLoaded.push_back(mapName);
		GameCommons::s_AllLoadedMaps.insert(std::pair<std::string, Map*>(mapName, map));
	}
}

STATIC Map* Map::GetRegisteredMapIfExists(const std::string& mapName)
{
	std::string lowerCase = MakeLower(mapName);
	std::map<std::string, Map*>::iterator it = GameCommons::s_AllLoadedMaps.find(lowerCase);
	if (it == GameCommons::s_AllLoadedMaps.end())
	{
		return nullptr;
	}
	return it->second;
}

STATIC void Map::ObliterateMaps()
{
	DeleteMaps();
	if (s_associatedSpriteSheetsByIdx != nullptr)
	{
		delete s_associatedSpriteSheetsByIdx;
		s_associatedSpriteSheetsByIdx = nullptr;
	}
	if (s_Model != nullptr)
	{
		delete s_Model;
		s_Model = nullptr;
	}
	if (s_indices != nullptr)
	{
		delete s_indices;
		s_indices = nullptr;
	}
	if (s_TopModel != nullptr)
	{
		delete s_TopModel;
		s_TopModel = nullptr;
	}
	if (s_TopIndices != nullptr)
	{
		delete s_TopIndices;
		s_TopIndices = nullptr;
	}
	if (s_BottomModel != nullptr)
	{
		delete s_BottomModel;
		s_BottomModel = nullptr;
	}
	if (s_BottomIndices != nullptr)
	{
		delete s_BottomIndices;
		s_BottomIndices = nullptr;
	}
	if (s_TopIndices2 != nullptr)
	{
		delete s_TopIndices2;
		s_TopIndices2 = nullptr;
	}
	if (s_TopModel2 != nullptr)
	{
		delete s_TopModel2;
		s_TopModel2 = nullptr;
	}
}

STATIC void Map::DeleteMaps()
{
	if (s_GlobalVariablesLoad != nullptr)
	{
		delete s_GlobalVariablesLoad;
		s_GlobalVariablesLoad = nullptr;
	}
	for (std::map<std::string, Map*>::iterator it = GameCommons::s_AllLoadedMaps.begin();
		it != GameCommons::s_AllLoadedMaps.end(); it++)
	{
		if (it->second != nullptr)
		{
			it->second->DestroyAllGameEntities();
			delete it->second;
			it->second = nullptr;
		}
	}
	GameCommons::s_AllLoadedMaps.clear();
	GameCommons::s_MapNamesInOrderTheyWereLoaded.clear();
	GameCommons::s_MapNamesInOrderTheyWereLoaded.shrink_to_fit();
}

STATIC const Map* Map::GetMap()
{
	return s_CurrentMap;
}

STATIC Map* Map::GetEditableMap()
{
	return s_CurrentMap;
}

STATIC const Map* Map::GetPreviousMap()
{
	return s_PreviousMap;
}

STATIC Map* Map::GetEditablePreviousMap()
{
	return s_PreviousMap;
}

STATIC bool Map::OnUpdateLoadTileDefinitions()
{
	if (s_currentlyLoadingTileDefinitions)
	{
		if (TileDefinition::OnUpdateLoadTileDefinitionsFromCommonXMLFolder())
		{
			s_currentlyLoadingTileDefinitions = false;
		}
		return false;
	}
	else
	{
		return MapFiles::OnUpdateReadInMapFilesFromCommonXMLFile();
	}
}

STATIC void Map::LoadTileDefinitionsAllAtOnce()
{
	TileDefinition::LoadTileDefinitionsFromCommonXMLFolderAllAtOnce();
	MapFiles::ReadInMapFilesFromCommonXMLFileAllAtOnce();
}

STATIC void Map::SetCurrentMap(Map* map, bool changePrevMap)
{
	if (changePrevMap)
	{
		s_PreviousMap = s_CurrentMap;
	}
	s_CurrentMap = map;
	if (s_CurrentMap == nullptr)
	{
		BaseGameEntity::SetBaseGameEntitiesPointer(nullptr);
		BaseGameEntity::SetAllNPCsPointer(nullptr);
		BaseGameEntity::SetAllFeaturesPointer(nullptr);
	}
	else
	{
		BaseGameEntity::SetBaseGameEntitiesPointer(&s_CurrentMap->m_BaseGameEntitiesByType);
		BaseGameEntity::SetAllNPCsPointer(&s_CurrentMap->m_AllNPCs);
		BaseGameEntity::SetAllFeaturesPointer(&s_CurrentMap->m_AllFeatures);
	}
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Map Reading Functions
void Map::ReadQuestAgentsToSpawn(const XMLNode& child)
{
	std::string name = child.getName(); //For example: "Villager"

	TILE_COORDS guaranteePosition = TILE_COORDS::ivec2_zeros;
	std::vector<TILE_COORDS> tileCoordsCanSelect;
	bool guaranteePositionBool = false;

	std::string globalVariableName = "";
	int numberToSpawn = 1;
	int maxNumberToSpawn = 1;
	bool spawnLocationRandom = true;
	float chanceToSpawnAll = 1.f;


	ReadMapSpecialDataFromChildGeneralToGenerateNodeAttributes(child,
		guaranteePosition, guaranteePositionBool, numberToSpawn, maxNumberToSpawn, spawnLocationRandom,
		chanceToSpawnAll, globalVariableName);

	if (GetRandomFloatZeroToOne() > chanceToSpawnAll)
	{
		return;
	}
	

	if (maxNumberToSpawn < numberToSpawn)
	{
		int temp = numberToSpawn;
		numberToSpawn = maxNumberToSpawn;
		maxNumberToSpawn = temp;
	}

	numberToSpawn = GetRandomIntInRange(numberToSpawn, maxNumberToSpawn);
	if (numberToSpawn <= 0)
	{
		return;
	}

	ReadMapSpecialDataFromChildGeneralToGenerateNodeChildren(child, tileCoordsCanSelect);
	ReadMapSpecialDataFromChildAgentsToGenerateNodeSpawnNow(name, guaranteePosition, tileCoordsCanSelect,
		guaranteePositionBool, numberToSpawn, spawnLocationRandom, true, globalVariableName);
}

void Map::ReadQuestFeatureToSpawn(const XMLNode& child)
{
	std::string name = child.getName(); //For example: "Door"

	TILE_COORDS guaranteePosition = TILE_COORDS::ivec2_zeros;
	std::vector<TILE_COORDS> tileCoordsCanSelect;
	bool guaranteePositionBool = false;

	int numberToSpawn = 1;
	int maxNumberToSpawn = 1;
	bool spawnLocationRandom = true;
	float chanceToSpawnAll = 1.f;
	std::string globalVariableName = "";

	ReadMapSpecialDataFromChildGeneralToGenerateNodeAttributes(child,
		guaranteePosition, guaranteePositionBool,
		numberToSpawn, maxNumberToSpawn, spawnLocationRandom, chanceToSpawnAll, globalVariableName);

	if (GetRandomFloatZeroToOne() > chanceToSpawnAll)
	{
		return;
	}

	if (maxNumberToSpawn < numberToSpawn)
	{
		int temp = numberToSpawn;
		numberToSpawn = maxNumberToSpawn;
		maxNumberToSpawn = temp;
	}

	numberToSpawn = GetRandomIntInRange(numberToSpawn, maxNumberToSpawn);
	if (numberToSpawn <= 0)
	{
		return;
	}

	ReadMapSpecialDataFromChildGeneralToGenerateNodeChildren(child, tileCoordsCanSelect);
	ReadMapSpecialDataFromChildFeaturesToGenerateNodeSpawnNow(child, name, guaranteePosition, tileCoordsCanSelect,
		guaranteePositionBool, numberToSpawn, spawnLocationRandom, globalVariableName);
}

bool Map::ReadMapSpecialDataFromXMLAllAtOnce(const std::string& fileLoc)
{
	bool errorOccurred = false;
	m_specialDataRoot = EngineXMLParser::ParseXMLFile(fileLoc, CommonMapVars::s_MapXMLRoot, errorOccurred);
	if (errorOccurred)
	{
		return false;
	}


	ReadMapSpecialDataFromXMLAttributes(m_specialDataRoot);
	ReadMapSpecialDataFromXMLChildNodes(m_specialDataRoot, nullptr);
	m_specialDataRoot = XMLNode::emptyNode();
	return true;
}

bool Map::OnUpdateReadMapSpecialDataFromXML(const std::string& fileLoc,
	int& actionsTakenThisFrame, double timeCanTakePerFrame,
	const uint64_t& startFrameTime,
	MapFiles* files)
{

	if (!m_ReadSpecialData)
	{
		m_ReadSpecialData = true;
		bool errorOccurred = false;
		m_specialDataRoot = EngineXMLParser::ParseXMLFile(fileLoc, 
			CommonMapVars::s_MapXMLRoot, errorOccurred);
		if (errorOccurred)
		{
			m_ReadSpecialData = false;
			return true;
		}
		m_currentTileOrXMLIdxAtWhileReading = 0;
		actionsTakenThisFrame++;
	}

	uint64_t currentTime = Performance::GetCurrentPerformanceCount();
	uint64_t timeDiff = currentTime - startFrameTime;
	double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
	if (timeDiffDouble >= timeCanTakePerFrame)
	{
		return false;
	}

	if (!m_readAttributes)
	{
		while (m_currentTileOrXMLIdxAtWhileReading < m_specialDataRoot.nAttribute())
		{
			XMLAttribute attr = m_specialDataRoot.getAttribute(m_currentTileOrXMLIdxAtWhileReading);
			ReadSingleXMLAttributeForXMLData(attr);

			m_currentTileOrXMLIdxAtWhileReading++;
			currentTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentTime - startFrameTime;
			timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			if (timeDiffDouble >= timeCanTakePerFrame)
			{
				return false;
			}
		}
		m_currentTileOrXMLIdxAtWhileReading = 0;
		m_currentGrandChildXMLNodeIdxAt = 0;
		m_readAttributes = true;
		m_readNodes = false;
		currentTime = Performance::GetCurrentPerformanceCount();
		timeDiff = currentTime - startFrameTime;
		timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= timeCanTakePerFrame)
		{
			return false;
		}
	}
	if (!m_readNodes)
	{
		while (m_currentTileOrXMLIdxAtWhileReading < m_specialDataRoot.nChildNode())
		{
			XMLNode child = m_specialDataRoot.getChildNode(m_currentTileOrXMLIdxAtWhileReading);
			bool finishedReadingTheChild = 
				OnUpdateParseXMLNodeSpecialData(child,
					actionsTakenThisFrame, timeCanTakePerFrame, startFrameTime,
					files);

			if (finishedReadingTheChild)
			{
				m_currentTileOrXMLIdxAtWhileReading++;
				m_currentGrandChildXMLNodeIdxAt = 0;
			}

			actionsTakenThisFrame++;
			currentTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentTime - startFrameTime;
			timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			if (timeDiffDouble >= timeCanTakePerFrame)
			{
				return false;
			}
		}
		m_readNodes = true;
		currentTime = Performance::GetCurrentPerformanceCount();
		timeDiff = currentTime - startFrameTime;
		timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= timeCanTakePerFrame)
		{
			return false;
		}
	}

	m_specialDataRoot = XMLNode::emptyNode();
	return true;
}

bool Map::OnUpdateParseXMLNodeSpecialData(const XMLNode& child, 
	int& actionsTakenThisFrame, double amountOfTimeCanTakePerFrame,
	const uint64_t& frameStartTime,
	MapFiles* files)
{
	int numOfGrandChildren = child.nChildNode();
	if (m_currentGrandChildXMLNodeIdxAt >= numOfGrandChildren)
	{
		return true;
	}
	uint64_t currentTime = Performance::GetCurrentPerformanceCount();
	uint64_t timeDiff = currentTime - frameStartTime;
	double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
	if (timeDiffDouble >= amountOfTimeCanTakePerFrame)
	{
		return false;
	}
	std::string childName = MakeLower(child.getName());
	if (childName == "tileevents")
	{
		while(m_currentGrandChildXMLNodeIdxAt < numOfGrandChildren)
		{
			XMLNode tileEventNode = child.getChildNode(m_currentGrandChildXMLNodeIdxAt);
			ReadMapSpecialDataFromChildTileEventNode(tileEventNode);
			m_currentGrandChildXMLNodeIdxAt++;
			actionsTakenThisFrame++;
			currentTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentTime - frameStartTime;
			timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			if (timeDiffDouble >= amountOfTimeCanTakePerFrame)
			{
				return false;
			}
		}

		return true;
	}
	else if (childName == "agentstogenerate")
	{
		while (m_currentGrandChildXMLNodeIdxAt < numOfGrandChildren)
		{
			XMLNode agentToGenerateNode = child.getChildNode(m_currentGrandChildXMLNodeIdxAt);
			ReadMapSpecialDataFromChildAgentsToGenerateNode(agentToGenerateNode);
			m_currentGrandChildXMLNodeIdxAt++;
			actionsTakenThisFrame++;
			currentTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentTime - frameStartTime;
			timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			if (timeDiffDouble >= amountOfTimeCanTakePerFrame)
			{
				return false;
			}
		}
		return true;
	}
	else if (childName == "featurestogenerate")
	{
		while (m_currentGrandChildXMLNodeIdxAt < numOfGrandChildren)
		{
			XMLNode featureToGenerateNode = child.getChildNode(m_currentGrandChildXMLNodeIdxAt);
			ReadMapSpecialDataFromChildFeaturesToGenerateNode(featureToGenerateNode);
			m_currentGrandChildXMLNodeIdxAt++;
			actionsTakenThisFrame++;
			currentTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentTime - frameStartTime;
			timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			if (timeDiffDouble >= amountOfTimeCanTakePerFrame)
			{
				return false;
			}
		}
		return true;
	}
	else if (childName == "quests")
	{
		while (m_currentGrandChildXMLNodeIdxAt < numOfGrandChildren)
		{
			XMLNode questNode = child.getChildNode(m_currentGrandChildXMLNodeIdxAt);
			ReadMapSpecialDataFromXMLQuests(questNode, files);
			m_currentGrandChildXMLNodeIdxAt++;
			actionsTakenThisFrame++;
			currentTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentTime - frameStartTime;
			timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			if (timeDiffDouble >= amountOfTimeCanTakePerFrame)
			{
				return false;
			}
		}
		return true;
	}

	return true;
}

void Map::ReadMapSpecialDataFromXMLAttributes(const XMLNode& root)
{
	for (int attrIdx = 0; attrIdx < root.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = root.getAttribute(attrIdx);
		ReadSingleXMLAttributeForXMLData(attr);
	}
}

void Map::ReadSingleXMLAttributeForXMLData(const XMLAttribute& attr)
{
	std::string attrName = MakeLower(attr.lpszName);
	if (attrName == "clampcamera"
		|| attrName == "constraincamerabounds"
		|| attrName == "constraincamera")
	{
		std::string attrVal = MakeLower(attr.lpszValue);
		if (attrVal == "true"
			|| attrVal == "t")
		{
			m_constrainCameraBounds = true;
		}
		else if (attrVal == "false"
			|| attrVal == "f")
		{
			m_constrainCameraBounds = false;
		}
	}
	else if (attrName == "music")
	{
		m_MusicNameForThisMap = std::string(attr.lpszValue);
	}
	else if (attrName == "musicvolume"
		|| attrName == "musicvol")
	{
		m_musicVolume = (float)atof(attr.lpszValue);
	}
}

void Map::ReadMapSpecialDataFromXMLChildNodes(const XMLNode& root,
	MapFiles* files)
{
	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "tileevents")
		{
			for (int tileEventIdx = 0; tileEventIdx < child.nChildNode(); tileEventIdx++)
			{
				XMLNode tileEventNode = child.getChildNode(tileEventIdx);
				ReadMapSpecialDataFromChildTileEventNode(tileEventNode);
			}
		}
		else if (childName == "agentstogenerate")
		{
			for (int tileEventIdx = 0; tileEventIdx < child.nChildNode(); tileEventIdx++)
			{
				XMLNode agentToGenerateNode = child.getChildNode(tileEventIdx);
				ReadMapSpecialDataFromChildAgentsToGenerateNode(agentToGenerateNode);
			}
		}
		else if (childName == "featurestogenerate")
		{
			for (int tileEventIdx = 0; tileEventIdx < child.nChildNode(); tileEventIdx++)
			{
				XMLNode featureToGenerateNode = child.getChildNode(tileEventIdx);
				ReadMapSpecialDataFromChildFeaturesToGenerateNode(featureToGenerateNode);
			}
		}
		else if (childName == "quests")
		{
			for (int questIdx = 0; questIdx < child.nChildNode(); questIdx++)
			{
				XMLNode questNode = child.getChildNode(questIdx);
				ReadMapSpecialDataFromXMLQuests(questNode, files);
			}
		}
	}
}

void Map::ReadMapSpecialDataFromXMLQuests(const XMLNode& QuestNode,
	MapFiles* files)
{
	std::string questTitle = "";
	float percentChanceToOccur = 1.f;
	for (int attrIdx = 0; attrIdx < QuestNode.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = QuestNode.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "questtitle"
			|| attrName == "title"
			|| attrName == "quest")
		{
			questTitle = std::string(attr.lpszValue);
		}
		else if (attrName == "percentchance"
			|| attrName == "percentchancetooccur"
			|| attrName == "chance"
			|| attrName == "chancetooccur")
		{
			percentChanceToOccur = (float)atof(attr.lpszValue);
		}
	}
	if (percentChanceToOccur <= 0.f)
	{
		return;
	}
	else if (percentChanceToOccur < 1.f)
	{
		float chance = GetRandomFloatZeroToOne();
		if (chance > percentChanceToOccur)
		{
			return;
		}
	}

	if (questTitle != "")
	{
		Quest* quest = QuestGenerator::SpawnQuestByName(questTitle, QuestNode, m_MapName, files);
		if (quest != nullptr)
		{
			m_questAvailableToStart.push_back(quest);
		}
	}
}

void Map::ReadMapSpecialDataFromChildTileEventNode(const XMLNode& child)
{
	TILE_COORDS coordsToEffect = TILE_COORDS::ivec2_zeros;
	bool setTileCoords = ReadMapSpecialDataForTileEventsAttributes(child, coordsToEffect);
	
	if (setTileCoords == false
		|| coordsToEffect.x < 0 || coordsToEffect.x >= m_mapWidthHeight.x
		|| coordsToEffect.y < 0 || coordsToEffect.y >= m_mapWidthHeight.y)
	{
		return;
	}

	TileEvent* onTouch = nullptr;
	TileEvent* onExit = nullptr;
	TileEvent* onEnter = nullptr;
	TileEvent* onInteract = nullptr;
	std::vector<StrXMLAttr> OnExitParams;
	std::vector<StrXMLAttr> OnTouchParams;
	std::vector<StrXMLAttr> OnEnterParams;
	std::vector<StrXMLAttr> OnInteractParams;
	std::string OnInteractName = "";
	std::string OnExitName = "";
	std::string OnEnterName = "";
	std::string OnTouchName = "";
	ReadMapSpecialDataForTileEvents(child, onTouch, onEnter, onExit, onInteract, OnTouchParams,
		OnEnterParams, OnExitParams, OnInteractParams, OnTouchName, OnEnterName, OnExitName, OnInteractName);

	if ((onInteract == nullptr && onEnter == nullptr && onTouch == nullptr && onExit == nullptr))
	{
		return;
	}

	int tileIdx = coordsToEffect.x + (coordsToEffect.y * m_mapWidthHeight.x);
	TileEventPack* eventPack = new TileEventPack(onInteract, onTouch, onEnter, onExit,
		OnInteractParams, OnTouchParams, OnEnterParams, OnExitParams, 
		OnInteractName, OnTouchName, OnEnterName, OnExitName,
		coordsToEffect, tileIdx);
	m_tiles[tileIdx].SetHasSpecialMapDefinedEvents(true);
	m_tiles[tileIdx].SetSpecialTileEventPack(eventPack);
	const TileDefinition* tileDef = m_tiles[tileIdx].GetTileDefinition();

	if (onInteract != nullptr && (tileDef == nullptr || tileDef->GetOnInteractEvent() == nullptr))
	{
		m_tiles[tileIdx].SignUpForOnInteractTest();
	}
	if (onTouch != nullptr && (tileDef == nullptr || tileDef->GetOnTouchEvent() == nullptr))
	{
		m_tiles[tileIdx].SignUpForOnTouchTest();
	}

	if (onEnter != nullptr && (tileDef == nullptr || tileDef->GetOnEnterEvent() == nullptr))
	{
		m_tiles[tileIdx].SignUpForOnEnterTest();
	}
	if (onExit != nullptr && (tileDef == nullptr || tileDef->GetOnExitEvent() == nullptr))
	{
		m_tiles[tileIdx].SignUpForOnExitTest();
	}
}

bool Map::ReadMapSpecialDataForTileEventsAttributes(const XMLNode& child, TILE_COORDS& coordsToEffect)
{
	bool setTileCoords = false;
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		std::string val = std::string(attr.lpszValue);
		if (attrName == "tilecoords"
			|| attrName == "tile")
		{
			setTileCoords = true;
			val = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(val, ' ');
			if (parsed.size() > 0)
			{
				coordsToEffect.x = atoi(parsed[0].c_str());
			}
			if (parsed.size() > 1)
			{
				coordsToEffect.y = atoi(parsed[1].c_str());
			}
		}
	}
	return setTileCoords;
}

void Map::ReadMapSpecialDataForTileEvents(const XMLNode& child, TileEvent*& onTouch, TileEvent*& onEnter,
	TileEvent*& onExit, TileEvent*& onInteract, std::vector<StrXMLAttr>& OnTouchParams,
	std::vector<StrXMLAttr>& OnEnterParams, std::vector<StrXMLAttr>& OnExitParams,
	std::vector<StrXMLAttr>& OnInteractParams,
	std::string& OnTouchName, std::string& OnEnterName,
	std::string& OnExitName, std::string& OnInteractName)
{
	for (int childIdx = 0; childIdx < child.nChildNode(); childIdx++)
	{
		XMLNode grandChild = child.getChildNode(childIdx);
		std::string childName = MakeLower(grandChild.getName());

		if (childName == "oninteract"
			|| childName == "interact")
		{
			SingleTileEventReadingFunction(grandChild, onInteract, OnInteractParams, OnInteractName);
		}
		else if (childName == "ontouch"
			|| childName == "touch")
		{
			SingleTileEventReadingFunction(grandChild, onTouch, OnTouchParams, OnTouchName);
		}
		else if (childName == "onenter"
			|| childName == "enter")
		{
			SingleTileEventReadingFunction(grandChild, onEnter, OnEnterParams, OnEnterName);
		}
		else if (childName == "onexit"
			|| childName == "exit")
		{
			SingleTileEventReadingFunction(grandChild, onExit, OnExitParams, OnExitName);
		}
	}
}


void Map::SingleTileEventReadingFunction(const XMLNode& grandChild,
	TileEvent*& tileEvent, std::vector<StrXMLAttr>& tileEventParams,
	std::string& name)
{
	for (int attrIdx = 0; attrIdx < grandChild.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = grandChild.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		std::string val = attr.lpszValue;
		if (attrName == "function")
		{
			name = val;
			tileEvent = TileEvent::GetTileEventForName(val);
		}
		else
		{
			tileEventParams.push_back(attr);
		}
	}
}


void Map::ReadMapSpecialDataFromChildAgentsToGenerateNode(const XMLNode& child)
{
	/*
		<AgentsToGenerate>
			<Villager number='1' Job='Smith'/>
			<Villager number='1' position='0,0'/>
			<Villager number='2'>
				<Position position='0,0'/>
			</Villager>
		</AgentsToGenerate>
	*/
	std::string name = child.getName(); //For example: "Villager"

	TILE_COORDS guaranteePosition = TILE_COORDS::ivec2_zeros;
	std::vector<TILE_COORDS> tileCoordsCanSelect;
	bool guaranteePositionBool = false;

	int numberToSpawn = 1;
	int maxNumberToSpawn = 1;
	bool spawnLocationRandom = true;
	float chanceToSpawnAll = 1.f;
	std::string globalVariableName = "";

	ReadMapSpecialDataFromChildGeneralToGenerateNodeAttributes(child,
		guaranteePosition, guaranteePositionBool, numberToSpawn, maxNumberToSpawn, spawnLocationRandom,
		chanceToSpawnAll, globalVariableName);

	if (GetRandomFloatZeroToOne() > chanceToSpawnAll)
	{
		return;
	}

	if (maxNumberToSpawn < numberToSpawn)
	{
		int temp = numberToSpawn;
		numberToSpawn = maxNumberToSpawn;
		maxNumberToSpawn = temp;
	}

	numberToSpawn = GetRandomIntInRange(numberToSpawn, maxNumberToSpawn);
	if (numberToSpawn <= 0)
	{
		return;
	}
	ReadMapSpecialDataFromChildGeneralToGenerateNodeChildren(child, tileCoordsCanSelect);

	ReadMapSpecialDataFromChildAgentsToGenerateNodeSpawnNow(name, guaranteePosition, tileCoordsCanSelect,
		guaranteePositionBool, numberToSpawn, spawnLocationRandom, true, globalVariableName);
}

void Map::ReadMapSpecialDataFromChildFeaturesToGenerateNode(const XMLNode& child)
{
	std::string name = child.getName(); //For example: "Door"

	TILE_COORDS guaranteePosition = TILE_COORDS::ivec2_zeros;
	std::vector<TILE_COORDS> tileCoordsCanSelect;
	bool guaranteePositionBool = false;

	std::string globalVariableName = "";
	int numberToSpawn = 1;
	int maxNumberToSpawn = 1;
	bool spawnLocationRandom = true;
	float chanceToSpawnAll = 1.f;
	ReadMapSpecialDataFromChildGeneralToGenerateNodeAttributes(child,
		guaranteePosition, guaranteePositionBool,
		numberToSpawn, maxNumberToSpawn, spawnLocationRandom,
		chanceToSpawnAll, globalVariableName);

	if (GetRandomFloatZeroToOne() > chanceToSpawnAll)
	{
		return;
	}

	if (maxNumberToSpawn < numberToSpawn)
	{
		int temp = numberToSpawn;
		numberToSpawn = maxNumberToSpawn;
		maxNumberToSpawn = temp;
	}

	numberToSpawn = GetRandomIntInRange(numberToSpawn, maxNumberToSpawn);
	if (numberToSpawn <= 0)
	{
		return;
	}

	ReadMapSpecialDataFromChildGeneralToGenerateNodeChildren(child, tileCoordsCanSelect);
	ReadMapSpecialDataFromChildFeaturesToGenerateNodeSpawnNow(child, name, guaranteePosition, tileCoordsCanSelect,
		guaranteePositionBool, numberToSpawn, spawnLocationRandom, globalVariableName);
	
}


void Map::ReadMapSpecialDataFromChildAgentsToGenerateNodeSpawnNow(const std::string& name,
	const TILE_COORDS& guaranteePosition, std::vector<TILE_COORDS>& tileCoordsCanSelect,
	bool guaranteePositionBool, int numberToSpawn,
	bool spawnLocationRandom, bool guaranteeNonSolidTilesUsedOnly,
	const std::string& globalVariableName)
{
	if (tileCoordsCanSelect.size() > 0)
	{
		EmptyTileCoordsExceptForUninhabitted(tileCoordsCanSelect, guaranteeNonSolidTilesUsedOnly);
	}
	Vector2 tileCenter = Vector2::vec2_zeros;
	if (guaranteePositionBool)
	{
		numberToSpawn--;
		const Tile* tile = GetTileAtCoords(guaranteePosition);
		if (tile != nullptr)
		{
			tileCenter = tile->GetPosition();
			BaseGameEntity* agent = AgentGenerator::SpawnNPCInAgentGeneratorByAgentName(name.c_str(), 
				tileCenter, this);
			RegisterGameEntity(agent);
			RegisterNPC(agent);
			if (globalVariableName != "")
			{
				SetGlobalVariable(globalVariableName, agent);
			}

		}
	}
	while (numberToSpawn > 0 && tileCoordsCanSelect.size() > 0)
	{
		int ranTileCoordIdx = GetRandomIntLessThan(tileCoordsCanSelect.size());
		TILE_COORDS coords = tileCoordsCanSelect[ranTileCoordIdx];
		const Tile* tile = GetTileAtCoords(coords);
		if (tile != nullptr)
		{
			tileCenter = tile->GetPosition();
			BaseGameEntity* agent = AgentGenerator::SpawnNPCInAgentGeneratorByAgentName(name.c_str(), tileCenter, this);
			RegisterGameEntity(agent);
			RegisterNPC(agent);
			if (globalVariableName != "")
			{
				SetGlobalVariable(globalVariableName, agent);
			}
			numberToSpawn--;
		}
		else
		{

		}
		tileCoordsCanSelect.erase(tileCoordsCanSelect.begin() + ranTileCoordIdx);
	}

	if (spawnLocationRandom == false)
	{
		return;
	}

	std::vector<Vector2> allValidPositions;
	GetAllUninhabittedTileCenters(allValidPositions, guaranteeNonSolidTilesUsedOnly);

	if (allValidPositions.size() != 0)
	{
		while (numberToSpawn > 0)
		{
			int index = GetRandomIntLessThan(allValidPositions.size());
			tileCenter = allValidPositions[index];
			BaseGameEntity* agent = AgentGenerator::SpawnNPCInAgentGeneratorByAgentName(name.c_str(), tileCenter, this);
			RegisterGameEntity(agent);
			RegisterNPC(agent);
			if (globalVariableName != "")
			{
				SetGlobalVariable(globalVariableName, agent);
			}
			numberToSpawn--;

			allValidPositions.erase(allValidPositions.begin() + index);

			if (allValidPositions.size() == 0)
			{
				break;
			}
		}
	}
}


void Map::EmptyTileCoordsExceptForUninhabitted(std::vector<TILE_COORDS>& tileCoordsCanSelect,
	bool guaranteeNonSolid)
{
	std::vector<TILE_COORDS> allValidPositions;
	GetAllUninhabittedTileCenters(allValidPositions, guaranteeNonSolid);

	std::vector<bool> foundThem;
	foundThem.resize(tileCoordsCanSelect.size(), false);
	size_t numberFound = 0;

	for (size_t validTileCoordsIdx = 0; validTileCoordsIdx < allValidPositions.size(); validTileCoordsIdx++)
	{
		const TILE_COORDS& validCoords = allValidPositions[validTileCoordsIdx];

		for (size_t tileCoordsIdx = 0; tileCoordsIdx < tileCoordsCanSelect.size(); tileCoordsIdx++)
		{
			TILE_COORDS& ourCoords = tileCoordsCanSelect[tileCoordsIdx];
			if (validCoords == ourCoords)
			{
				foundThem[tileCoordsIdx] = true;
				numberFound++;
				if (numberFound == foundThem.size())
				{
					break;
				}
			}
		}
		if (numberFound == foundThem.size())
		{
			break;
		}

	}
	std::vector<TILE_COORDS> copy;
	copy.reserve(numberFound);

	for (size_t i = 0; i < foundThem.size(); i++)
	{
		if (foundThem[i])
			copy.push_back(tileCoordsCanSelect[i]);
	}
	tileCoordsCanSelect = copy;
}

void Map::ReadMapSpecialDataFromChildFeaturesToGenerateNodeSpawnNow(const XMLNode& child, 
	const std::string& name,
	const TILE_COORDS& guaranteePosition, std::vector<TILE_COORDS>& tileCoordsCanSelect,
	bool guaranteePositionBool, int numberToSpawn,
	bool spawnLocationRandom, const std::string& globalVariableName)
{
	Vector2 tileCenter = Vector2::vec2_zeros;

	if (tileCoordsCanSelect.size() > 0)
	{
		EmptyTileCoordsExceptForUninhabitted(tileCoordsCanSelect, false);
	}

	if (guaranteePositionBool)
	{
		numberToSpawn--;
		const Tile* tile = GetTileAtCoords(guaranteePosition);
		if (tile != nullptr)
		{
			tileCenter = tile->GetPosition();
			BaseGameEntity* feature = FeatureGenerator::SpawnFeatureByFeatureName(name.c_str(), tileCenter, child, this);
			RegisterGameEntity(feature);
			RegisterFeature(feature);
			Feature* featureAsFeature = dynamic_cast<Feature*>(feature);
			if (featureAsFeature != nullptr)
			{
				TILE_COORDS coords = featureAsFeature->GetTileCoords();
				bool isSolid = featureAsFeature->GetSolid();
				bool overwrite = featureAsFeature->GetOverwriteTileOnIsSolid();
				const Tile* const_tile = GetTileAtCoords(coords);
				Tile* tile_test = const_cast<Tile*>(const_tile);
				if (overwrite && tile_test != nullptr)
				{
					tile_test->SetOverwrittenIsSolid(isSolid, true);
				}
				if (globalVariableName != "")
				{
					SetGlobalVariable(globalVariableName, feature);
				}
			}

		}
	}


	while (numberToSpawn > 0 && tileCoordsCanSelect.size() > 0)
	{
		int ranTileCoordIdx = GetRandomIntLessThan(tileCoordsCanSelect.size());
		TILE_COORDS coords = tileCoordsCanSelect[ranTileCoordIdx];
		const Tile* tile = GetTileAtCoords(coords);
		if (tile != nullptr)
		{
			tileCenter = tile->GetPosition();
			BaseGameEntity* feature = FeatureGenerator::SpawnFeatureByFeatureName(name.c_str(), tileCenter, child, this);
			RegisterGameEntity(feature);
			RegisterFeature(feature);
			Feature* featureAsFeature = dynamic_cast<Feature*>(feature);
			if (featureAsFeature != nullptr)
			{
				TILE_COORDS coords_test = featureAsFeature->GetTileCoords();
				bool isSolid = featureAsFeature->GetSolid();
				bool overwrite = featureAsFeature->GetOverwriteTileOnIsSolid();
				const Tile* const_tile = GetTileAtCoords(coords_test);
				Tile* tile_test = const_cast<Tile*>(const_tile);
				if (overwrite && tile_test != nullptr)
				{
					tile_test->SetOverwrittenIsSolid(isSolid, true);
				}
				if (globalVariableName != "")
				{
					SetGlobalVariable(globalVariableName, feature);
				}
			}
			numberToSpawn--;
		}
		else
		{

		}
		tileCoordsCanSelect.erase(tileCoordsCanSelect.begin() + ranTileCoordIdx);
	}

	if (spawnLocationRandom == false)
	{
		return;
	}

	std::vector<Vector2> allValidPositions;
	GetAllUninhabittedTileCenters(allValidPositions, true);
	if (allValidPositions.size() != 0)
	{
		while (numberToSpawn > 0)
		{
			int index = GetRandomIntLessThan(allValidPositions.size());
			tileCenter = allValidPositions[index];
			BaseGameEntity* feature = FeatureGenerator::SpawnFeatureByFeatureName(name.c_str(), tileCenter, child, this);
			RegisterGameEntity(feature);
			RegisterFeature(feature);
			Feature* featureAsFeature = dynamic_cast<Feature*>(feature);
			if (featureAsFeature != nullptr)
			{
				TILE_COORDS coords = featureAsFeature->GetTileCoords();
				bool isSolid = featureAsFeature->GetSolid();
				bool overwrite = featureAsFeature->GetOverwriteTileOnIsSolid();
				const Tile* const_tile = GetTileAtCoords(coords);
				Tile* tile = const_cast<Tile*>(const_tile);
				if (overwrite && tile != nullptr)
				{
					tile->SetOverwrittenIsSolid(isSolid, true);
				}
				if (globalVariableName != "")
				{
					SetGlobalVariable(globalVariableName, feature);
				}
			}
			numberToSpawn--;

			allValidPositions.erase(allValidPositions.begin() + index);
			if (allValidPositions.size() == 0)
			{
				break;
			}
		}
	}

	
}

void Map::ReadMapSpecialDataFromChildGeneralToGenerateNodeAttributes(const XMLNode& child,
	TILE_COORDS& guaranteePosition, 
	bool& guaranteePositionBool,
	int& numberToSpawn, int& maxNumberToSpawn, bool& spawnLocationRandom,
	float& chanceToSpawnAll, std::string& globalVariableName)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "randomposition"
			|| attrName == "randompos")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "false"
				|| val == "f")
			{
				spawnLocationRandom = false;
			}
			else
			{
				spawnLocationRandom = true;
			}
		}
		else if (attrName == "percentagechance"
			|| attrName == "chance"
			|| attrName == "percentagechancetospawn"
			|| attrName == "chancetospawn")
		{
			chanceToSpawnAll = (float)atof(attr.lpszValue);
			if (chanceToSpawnAll < 0.f)
			{
				chanceToSpawnAll = 0.f;
			}
			else if(chanceToSpawnAll > 1.f)
			{
				chanceToSpawnAll = 1.f;
			}
		}
		else if (attrName == "number"
			|| attrName == "numbertospawn")
		{
			bool sizeOf1Found = false;
			bool sizeOf2Found = false;
			IntVector2 numToSpawn = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1Found, sizeOf2Found);
			if (sizeOf1Found)
			{
				numberToSpawn = numToSpawn.x;
				maxNumberToSpawn = numToSpawn.x;
			}
			if (sizeOf2Found)
			{
				maxNumberToSpawn = numToSpawn.y;
			}
		}
		else if (attrName == "position")
		{
			bool sizeOf1Found = false;
			bool sizeOf2Found = false;
			IntVector2 vals = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1Found, sizeOf2Found);
			if (sizeOf1Found)
			{
				guaranteePositionBool = true;
				guaranteePosition.x = vals.x;
			}
			if (sizeOf2Found)
			{
				guaranteePosition.y = vals.y;
			}
		}
		else if (attrName == "globalvariablename"
			|| attrName == "globalvariable")
		{
			globalVariableName = MakeLower(attr.lpszValue);
		}
	}
}

void Map::ReadMapSpecialDataFromChildGeneralToGenerateNodeChildren(const XMLNode& child,
	std::vector<TILE_COORDS>& tileCoordsCanSelect)
{
	for (int childIdx = 0; childIdx < child.nChildNode(); childIdx++)
	{
		XMLNode grandChild = child.getChildNode(childIdx);
		std::string grandChildName = MakeLower(grandChild.getName());
		if (grandChildName == "position")
		{
			for (int attrIdx = 0; attrIdx < grandChild.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = grandChild.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "position"
					|| attrName == "coords")
				{
					bool sizeOf1Found = false;
					TILE_COORDS coords = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1Found);
					if (sizeOf1Found)
					{
						tileCoordsCanSelect.push_back(coords);
					}
				}
				else if (attrName == "positionrange")
				{
					TILE_COORDS min;
					TILE_COORDS max;
					std::vector<std::string> parsed = ParseString(attr.lpszValue,',');
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
						if (max.x < min.x)
						{
							int temp = min.x;
							min.x = max.x;
							max.x = temp;
						}
						if (max.y < min.y)
						{
							int temp = min.y;
							min.y = max.y;
							max.y = temp;
						}

						for (int x = min.x; x <= max.x; x++)
						{
							for (int y = min.y; y <= max.y; y++)
							{
								TILE_COORDS coords(x, y);
								tileCoordsCanSelect.push_back(coords);
							}
						}
					}
				}
			}
		}
	}
}


bool Map::ReadMapFromImageAllAtOnce(const std::string& fileLoc)
{
	ClearMap(); 
	m_numComponentsOnImage = 0;
	GetImageDataFromFileLocation(fileLoc, m_numComponentsOnImage);
	for (size_t tileIdx = 0; tileIdx < m_numberOfTiles; tileIdx++)
	{
		int redByteIndex = tileIdx * m_numComponentsOnImage;
		int greenByteIndex = redByteIndex + 1;
		int blueByteIndex = redByteIndex + 2;
		unsigned char RedByte = m_imageData[redByteIndex]; //int from [0, 255]
		unsigned char GreenByte = m_imageData[greenByteIndex];
		unsigned char BlueByte = m_imageData[blueByteIndex];

		Vector3 tileDefBits = Vector3((float)RedByte, (float)GreenByte, (float)BlueByte);
		ReadMapFromImageTileSetter(tileIdx, tileDefBits);
	}
	m_InterpretedImageData = true;
	stbi_image_free(m_imageData);
	m_imageData = nullptr;



	for (size_t tileIdx = 0; tileIdx < m_numberOfTiles; tileIdx++)
	{
		ReadSpecialTileDefinitionCases(tileIdx);
		Tile& tile = m_tiles[tileIdx];
		if (tile.GetTileIsSolid() == false)
		{
			m_indexsOfAllNonSolidTiles.push_back(tileIdx);
		}
	}

	return true;
}

bool Map::OnUpdateReadMapFromImage(const std::string& fileLoc,
	int& currentNumberOfActionsTaken, double amountOfTimeToTakePerFrame,
	const uint64_t& frameStartTime)
{
	if (s_GlobalVariablesLoad == nullptr)
	{
		s_GlobalVariablesLoad = new STATIC GlobalVariablesLoadingData();
	}
	s_GlobalVariablesLoad->m_timeStartedLoadingDataType = Performance::GetCurrentPerformanceCount();
	if (m_imageData == nullptr)
	{
		m_numComponentsOnImage = 0;
		m_currentTileOrXMLIdxAtWhileReading = 0;
		m_InterpretedImageData = false;
		GetImageDataFromFileLocation(fileLoc, m_numComponentsOnImage);
		currentNumberOfActionsTaken++;
	}
	uint64_t currentFrameTime = Performance::GetCurrentPerformanceCount();
	uint64_t timeDiff = currentFrameTime - frameStartTime;
	double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
	if (timeDiffDouble >= amountOfTimeToTakePerFrame)
	{
		return false;
	}
	if (!m_InterpretedImageData)
	{
		while ((size_t)m_currentTileOrXMLIdxAtWhileReading < m_numberOfTiles)
		{
			s_GlobalVariablesLoad->m_timeStartedLoadingSingleDataType = Performance::GetCurrentPerformanceCount();
			int redByteIndex = m_currentTileOrXMLIdxAtWhileReading * m_numComponentsOnImage;
			int greenByteIndex = redByteIndex + 1;
			int blueByteIndex = redByteIndex + 2;
			unsigned char RedByte = m_imageData[redByteIndex]; //int from [0, 255]
			unsigned char GreenByte = m_imageData[greenByteIndex];
			unsigned char BlueByte = m_imageData[blueByteIndex];

			Vector3 tileDefBits = Vector3((float)RedByte, (float)GreenByte, (float)BlueByte);
			ReadMapFromImageTileSetter(m_currentTileOrXMLIdxAtWhileReading, tileDefBits);

			m_currentTileOrXMLIdxAtWhileReading++;
			currentNumberOfActionsTaken++;

			currentFrameTime = Performance::GetCurrentPerformanceCount();
			timeDiff = currentFrameTime - frameStartTime;
			timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			uint64_t singleEntityTimeDiff = currentFrameTime - 
				s_GlobalVariablesLoad->m_timeStartedLoadingSingleDataType;
			double singleItemTimeDiff = Performance::PerformanceCountToSeconds(singleEntityTimeDiff);
			DebuggerPrintf("\nTime spent parsing single pixel for image data: %.5f",
				(float)singleItemTimeDiff);
			
			if (timeDiffDouble >= amountOfTimeToTakePerFrame)
			{
				return false;
			}
		}

		m_currentTileOrXMLIdxAtWhileReading = 0;
		m_InterpretedImageData = true;

		Vector2 tileScale = Tile::GetTileScale();
		m_mapSize = Vector2(tileScale.x * ((float)m_mapWidthHeight.x), 
			tileScale.y * ((float)m_mapWidthHeight.y));
		Vector2 halfMapSize = GetHalfMapSize();
		m_BLPos = GetMapCenter() - halfMapSize;
		m_TRPos = GetMapCenter() + halfMapSize;

		currentFrameTime = Performance::GetCurrentPerformanceCount();
		timeDiff = currentFrameTime - frameStartTime;
		timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= amountOfTimeToTakePerFrame)
		{
			return false;
		}
	}

	while ((size_t)m_currentTileOrXMLIdxAtWhileReading < m_numberOfTiles)
	{
		s_GlobalVariablesLoad->m_timeStartedLoadingSingleDataType = Performance::GetCurrentPerformanceCount();
		ReadSpecialTileDefinitionCases(m_currentTileOrXMLIdxAtWhileReading);
		Tile& tile = m_tiles[m_currentTileOrXMLIdxAtWhileReading];
		if (tile.GetTileIsSolid() == false)
		{
			m_indexsOfAllNonSolidTiles.push_back(m_currentTileOrXMLIdxAtWhileReading);
		}
		m_currentTileOrXMLIdxAtWhileReading++;
		currentNumberOfActionsTaken++;
		currentFrameTime = Performance::GetCurrentPerformanceCount();
		timeDiff = currentFrameTime - frameStartTime;
		timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		uint64_t singleEntityTimeDiff = currentFrameTime -
			s_GlobalVariablesLoad->m_timeStartedLoadingSingleDataType;
		double singleItemTimeDiff = Performance::PerformanceCountToSeconds(singleEntityTimeDiff);
		DebuggerPrintf("\nTime spent parsing Special case data for single tile: %.5f",
			(float)singleItemTimeDiff);
		if (timeDiffDouble >= amountOfTimeToTakePerFrame)
		{
			return false;
		}
	}
	stbi_image_free(m_imageData);
	m_imageData = nullptr;
	m_InterpretedImageData = true;
	return true;

}

void Map::GetImageDataFromFileLocation(const std::string& fileLocation, int& numComponents)
{
	IntVector2 tilesWideHigh;
	numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	int numComponentsRequested = 0; // don't care; we support 3 (RGB) or 4 (RGBA)
	m_imageData = stbi_load(fileLocation.c_str(),
		&tilesWideHigh.x, &tilesWideHigh.y,
		&numComponents, numComponentsRequested);
	numComponents;
	m_mapWidthHeight = tilesWideHigh;
	m_numberOfTiles = m_mapWidthHeight.x * m_mapWidthHeight.y;
	m_tiles.resize(m_numberOfTiles);

	Vector2 tileScale = Tile::GetTileScale();
	m_mapSize = Vector2(tileScale.x * ((float)m_mapWidthHeight.x),
		tileScale.y * ((float)m_mapWidthHeight.y));
	Vector2 halfMapSize = GetHalfMapSize();
	m_BLPos = GetMapCenter() - halfMapSize;
	m_TRPos = GetMapCenter() + halfMapSize;
}

void Map::ReadMapFromImageTileSetter(size_t tileIdx, const Vector3& tileDefBits)
{
	if (tileIdx >= m_numberOfTiles)
	{
		return;
	}
	int x = tileIdx % m_mapWidthHeight.x;
	int y = (tileIdx - x) / m_mapWidthHeight.x;
	int trueTileIdx = x + ((m_mapWidthHeight.y - (y + 1)) * m_mapWidthHeight.x);
	TILE_COORDS coords(x, (m_mapWidthHeight.y - (y + 1)));
	const Vector2& tileScale = Tile::GetTileScale();
	Vector2 blPosition = m_BLPos + Vector2(tileScale.x * coords.x, tileScale.y * coords.y);
	Vector2 trPosition = blPosition + tileScale;
	Vector2 centerPosition = blPosition + (tileScale * 0.5f);

	bool found = false;
	TileDefinition* def = TileDefinition::GetTileDefinitionForImageCode(tileDefBits, found);
	if (def == nullptr)
	{
		return;
	}

	Tile& tileInMap = m_tiles[trueTileIdx];
	tileInMap.SetOwningMap(this);
	tileInMap.SetTileCoords(coords);
	tileInMap.SetCollision(&m_perTileCollision);
	tileInMap.SetBLPosition(blPosition);
	tileInMap.SetTileCenter(centerPosition);
	tileInMap.SetTileDefinition(def);
	if (found)
	{
		tileInMap.SetTileType(def->GetTileDefIdx());
		tileInMap.ChangeSpriteResource(def->GetRandSpriteResource());
		tileInMap.SetInteractable(def->GetIsInteractable());
		tileInMap.SetIsTriggerVolume(def->GetIsTriggerVolume());
		tileInMap.ChangeBottomSpriteResource(def->GetRandBottomSpriteResource());
		tileInMap.ChangeTopSpriteResource(def->GetRandTopSpriteResource());
		tileInMap.ChangeTopSpriteResource2(def->GetRandTopSpriteResource2());
		tileInMap.SetTint(def->GetTint());
		tileInMap.SetMask(def->GetMask());
	}
	else
	{
		tileInMap.SetTileType(0);
		tileInMap.ChangeSpriteResource(nullptr);
	}
	//tileInMap.RunStandardConstructionMethod();
	if (def->GetIsTriggerVolume())
	{
		m_triggerVolumeTiles.push_back(&tileInMap);
	}
	if (def->GetIsInteractable())
	{
		m_interactableTiles.push_back(&tileInMap);
	}
	tileInMap.SignUpForEvents();

	StringSingleTileTogetherWithItsNeighbors(trueTileIdx, coords);
}


void Map::ReadSpecialTileDefinitionCases(size_t tileIdx)
{
	if (tileIdx >= m_numberOfTiles)
	{
		return;
	}
	Tile& tileInMap = m_tiles[tileIdx];
	const TileDefinition* def = tileInMap.GetTileDefinition();

	if (def == nullptr)
	{
		return;
	}

	const std::vector<TileDefinitionSpecialCase>& specialCases = def->GetSpecialCasesPtr();

	if (specialCases.size() == 0)
	{
		return;
	}

	TILE_COORDS currentTileCoords = tileInMap.GetTileCoords();
	

	for (size_t specialCaseIdx = 0; specialCaseIdx < specialCases.size(); specialCaseIdx++)
	{
		const TileDefinitionSpecialCase& specialCase = specialCases[specialCaseIdx];
		if (specialCase.m_ChangesToPerform.size() == 0
			|| (specialCase.GetMeetsAllStandardsOfCase(currentTileCoords, this,
				tileInMap.m_specialCaseResults) == false)
			)
		{
			continue;
		}

		for (size_t caseToApplyIdx = 0; caseToApplyIdx < specialCase.m_ChangesToPerform.size(); caseToApplyIdx++)
		{
			const TileDefinitionSpecialCaseToSet& toSet = specialCase.m_ChangesToPerform[caseToApplyIdx];
			TILE_COORDS coordsToEffect = currentTileCoords;
			if (coordsToEffect.x < 0 || coordsToEffect.y < 0
				|| coordsToEffect.x >= m_mapWidthHeight.x || coordsToEffect.y >= m_mapWidthHeight.y)
			{
				continue;
			}
			int currentTileIdx = coordsToEffect.x + (coordsToEffect.y * m_mapWidthHeight.x);
			Tile& tile = m_tiles[currentTileIdx];
			toSet.ApplyContentToTile(tile);
		}
	}
	tileInMap.SetPerformedTestsYet(true);

}

SpecialCaseTestResults Map::CheckNumberOfTilesSameInDirection(size_t& tileCount, const TILE_COORDS& startCoords,
	const TILE_COORDS& direction, const TileDefinition* def, int& numberSteppedInDirection) const
{
	SpecialCaseTestResults results;
	results.m_def = def;
	results.m_direction = direction;
	results.m_startCoords = startCoords;
	if (def == nullptr)
	{
		return results;
	}
	TILE_COORDS currentCoords = startCoords;
	while (currentCoords.x >= 0
		&& currentCoords.y >= 0
		&& currentCoords.x < m_mapWidthHeight.x
		&& currentCoords.y < m_mapWidthHeight.y)
	{
		currentCoords += direction;
		if (currentCoords.x < 0 || currentCoords.y < 0
			|| currentCoords.y >= m_mapWidthHeight.y
			|| currentCoords.x >= m_mapWidthHeight.x)
		{
			break;
		}
		int currentTileIdx = currentCoords.x + (currentCoords.y * m_mapWidthHeight.x);
		const Tile& tile = m_tiles[currentTileIdx];
		const TileDefinition* currentDef = tile.GetTileDefinition();
		if (currentDef == def)
		{
			tileCount++;

			//for (size_t caseIdx = 0; caseIdx < tile.m_specialCaseResults.size(); caseIdx++)
			//{
			//	const SpecialCaseTestResults& result = tile.m_specialCaseResults[caseIdx];
			//	if (result.m_def == def
			//		&& result.m_direction == direction)
			//	{
			//		results.m_resultsFound = tileCount + result.m_resultsFound;
			//		tileCount = result.m_resultsFound;
			//		return results;
			//	}
			//}
			numberSteppedInDirection++;
		}
		else
		{
			if (currentDef == nullptr)
			{
				break;
			}
			std::string tileDefName = (currentDef->GetDebugName());
			if (def->IsTileDefinitionNameOneToIgnore(tileDefName))
			{
				tileCount++;
				//for (size_t caseIdx = 0; caseIdx < tile.m_specialCaseResults.size(); caseIdx++)
				//{
				//	const SpecialCaseTestResults& result = tile.m_specialCaseResults[caseIdx];
				//	if (result.m_def == def
				//		&& result.m_direction == direction)
				//	{
				//		results.m_resultsFound = tileCount + result.m_resultsFound;
				//		tileCount = result.m_resultsFound;
				//		return results;
				//	}
				//}
				numberSteppedInDirection++;
			}
			else
			{
				break;
			}
		}
	}
	results.m_resultsFound = tileCount;
	return results;
}


void Map::PassResultInDirection(const TILE_COORDS& startTile, const TILE_COORDS& direction,
	int numberInDirection, int numberToMoveTowards, const TileDefinition* def)
{
	if (direction == TILE_COORDS::ivec2_zeros
		|| numberToMoveTowards <= 0
		|| numberInDirection <= 0)
	{
		return;
	}

	SpecialCaseTestResults results;
	results.m_def = def;
	results.m_direction = direction;
	TILE_COORDS currentCoords = startTile;
	while (currentCoords.x >= 0
		&& currentCoords.y >= 0
		&& currentCoords.x < m_mapWidthHeight.x
		&& currentCoords.y < m_mapWidthHeight.y)
	{
		numberInDirection--;
		numberToMoveTowards--;
		currentCoords += direction;
		if (numberToMoveTowards <= 0 || numberInDirection <= 0
			|| currentCoords.x < 0 || currentCoords.y < 0
			|| currentCoords.y >= m_mapWidthHeight.y
			|| currentCoords.x >= m_mapWidthHeight.x)
		{
			break;
		}
		results.m_resultsFound = numberInDirection;
		results.m_startCoords = currentCoords;
		int currentTileIdx = currentCoords.x + (currentCoords.y * m_mapWidthHeight.x);
		Tile& tile = m_tiles[currentTileIdx];

		tile.m_specialCaseResults.push_back(results);

	}
}


void Map::NotifyTilesInDirectionOfTestResults(const SpecialCaseTestResults& results)
{
	const TileDefinition* def = results.m_def;
	TILE_COORDS startTileCoords = results.m_startCoords;
	int startTileIdx = GetTileIdxForCoords(startTileCoords);
	TILE_COORDS direction = results.m_direction;
	TILE_COORDS currentTileCoords = startTileCoords - direction;
	int amountToSubtractFromIdx = 0;
	if (direction.x < 0 && direction.y == 0)
	{
		amountToSubtractFromIdx = 1;
	}
	else if (direction.x > 0 && direction.y == 0)
	{
		amountToSubtractFromIdx = -1;
	}
	else if (direction.x == 0 && direction.y < 0)
	{
		amountToSubtractFromIdx = m_mapWidthHeight.x;
	}
	else if (direction.x == 0 && direction.y > 0)
	{
		amountToSubtractFromIdx = -m_mapWidthHeight.x;
	}
	int currentTileIdx = startTileIdx - amountToSubtractFromIdx;
	size_t number = results.m_resultsFound - 1;
	number;

	while (currentTileCoords.x < m_mapWidthHeight.x
		&& currentTileCoords.x >= 0
		&& currentTileCoords.y < m_mapWidthHeight.y
		&& currentTileCoords.y >= 0)
	{
		Tile& tile = m_tiles[currentTileIdx];
		bool found = false;
		for (size_t tileResultsIdx = 0; tileResultsIdx < tile.m_specialCaseResults.size(); tileResultsIdx++)
		{
			const SpecialCaseTestResults& result = tile.m_specialCaseResults[tileResultsIdx];
			if (result.m_direction == direction
				&& result.m_def == def)
			{
				found = true;
				break;
			}
		}


		currentTileIdx = currentTileIdx - amountToSubtractFromIdx;
		currentTileCoords = currentTileCoords - direction;
	}

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Entity Related

void Map::UpdateAllEntities(const float& dt)
{
	for (size_t typeIdx = 0; typeIdx < m_BaseGameEntitiesByType.size(); typeIdx++)
	{
		std::vector<BaseGameEntity*>& entitiesOfType = m_BaseGameEntitiesByType.at(typeIdx);
		for (size_t entityIdx = 0; entityIdx < entitiesOfType.size(); entityIdx++)
		{
			BaseGameEntity* entity = entitiesOfType[entityIdx];
			if (entity == nullptr)
			{
				continue;
			}
			entity->Update(dt);
		}
	}
	m_BaseGameEntitiesInRenderOrder.clear();

	for (size_t typeIdx = 0; typeIdx < m_BaseGameEntitiesByType.size(); typeIdx++)
	{
		std::vector<BaseGameEntity*>& entitySetCopy = m_BaseGameEntitiesByType.at(typeIdx);
		m_BaseGameEntitiesInRenderOrder.reserve(m_BaseGameEntitiesInRenderOrder.size() + entitySetCopy.size());
		for (size_t entityIdx = 0; entityIdx < entitySetCopy.size(); entityIdx++)
		{
			BaseGameEntity* entity = entitySetCopy[entityIdx];
			if (entity == nullptr)
			{
				continue;
			}
			Vector2 entityPosition = entity->GetPosition();
			bool entityInserted = false;
			//int idxToInsert = -1;
			for (size_t renderOrderedEntityIdx = 0; 
				renderOrderedEntityIdx < m_BaseGameEntitiesInRenderOrder.size(); renderOrderedEntityIdx++)
			{
				BaseGameEntity* alreadyRegisteredEntity = m_BaseGameEntitiesInRenderOrder[renderOrderedEntityIdx];
				Vector2 regPosition = alreadyRegisteredEntity->GetPosition();
				if (entityPosition.y > (regPosition.y))
				{
					entityInserted = true;
					m_BaseGameEntitiesInRenderOrder.insert(m_BaseGameEntitiesInRenderOrder.begin() 
						+ renderOrderedEntityIdx, entity);
					break;
					//idxToInsert = i;
					//break;
				}
			}

			if (entityInserted == false)
			{
				m_BaseGameEntitiesInRenderOrder.push_back(entity);
			}
		}
	}
}

void Map::RenderAllEntities() const
{
	{
		for (size_t entityIdx = 0; entityIdx < m_BaseGameEntitiesInRenderOrder.size(); entityIdx++)
		{
			BaseGameEntity* entity = m_BaseGameEntitiesInRenderOrder[entityIdx];
			if (entity == nullptr)
			{
				continue;
			}
			entity->Render();
		}
	}
}

void Map::RemoveGameEntityRegistration(BaseGameEntity* entity)
{
	if ( entity == nullptr)
	{
		return;
	}

	GameEntityType type = entity->GetEntityType();

	for (size_t entityIdx = 0; entityIdx < m_BaseGameEntitiesByType.at(type).size(); entityIdx++)
	{
		if (m_BaseGameEntitiesByType[type].at(entityIdx) == entity)
		{
			m_BaseGameEntitiesByType[type].erase(m_BaseGameEntitiesByType[type].begin() + entityIdx);
			return;
		}
	}
}

void Map::DestroyGameEntityAndRegistration(BaseGameEntity* entity, bool destroy)
{
	if (entity != nullptr)
	{
		RemoveGameEntityRegistration(entity);
		entity->UnsignUpForEvents();
		if (destroy)
		{
			delete entity;
		}
	}
}

void Map::ClearAllGameEntitiesOfType(GameEntityType type)
{
	if (type < 0 || (size_t)type >= m_BaseGameEntitiesByType.size())
	{
		return;
	}

	if (m_BaseGameEntitiesByType[type].size() == 0)
	{
		return;
	}
	for (size_t entityIdx = 0; entityIdx < m_BaseGameEntitiesByType[type].size(); entityIdx++)
	{
		if (m_BaseGameEntitiesByType[type][entityIdx] != nullptr)
		{
			delete m_BaseGameEntitiesByType[type][entityIdx];
		}
	}
	m_BaseGameEntitiesByType.clear();
	m_BaseGameEntitiesInRenderOrder.clear();
}

void Map::ClearAllGameEntities(bool allowPlayerKill, bool totalClear)
{
	totalClear;
	{
		BaseGameEntity* playerPtr = BaseGameEntity::GetEditablePointerToPlayer();
		if (allowPlayerKill == false)
		{
			UnregisterGameEntity(playerPtr);
		}
		for (size_t typeIdx = 0; typeIdx < m_BaseGameEntitiesByType.size(); typeIdx++)
		{
			ClearAllGameEntitiesOfType((GameEntityType)typeIdx);
		}

		if (allowPlayerKill == false)
		{
			RegisterGameEntity(playerPtr);
		}
	}
}

void Map::RegisterGameEntity(BaseGameEntity* entity)
{
	if (entity == nullptr)
	{
		return;
	}
	GameEntityType type = entity->GetEntityType();
	if (type == INVALID_ENTITY)
	{
		return;
	}

	if ((size_t)type >= m_BaseGameEntitiesByType.size())
	{
		m_BaseGameEntitiesByType.resize(type + 1);
	}

	m_BaseGameEntitiesByType[type].push_back(entity);
}

void Map::UnregisterGameEntity(BaseGameEntity* entity)
{
	if (entity == nullptr)
	{
		return;
	}
	GameEntityType type = entity->GetEntityType();
	if (type == INVALID_ENTITY)
	{
		return;
	}
	for (size_t entityIdx = 0; entityIdx < m_BaseGameEntitiesByType[type].size(); entityIdx++)
	{
		BaseGameEntity* testEntity = m_BaseGameEntitiesByType[type][entityIdx];
		if (testEntity == entity)
		{
			m_BaseGameEntitiesByType[type].erase(m_BaseGameEntitiesByType[type].begin() + entityIdx);
			return;
		}
	}
}


void Map::RegisterNPC(BaseGameEntity* npc)
{
	if (npc == nullptr)
	{
		return;
	}
	for (size_t npcIdx = 0; npcIdx < m_AllNPCs.size(); npcIdx++)
	{
		BaseGameEntity* npcTest = m_AllNPCs[npcIdx];
		if (npcTest == npc)
		{
			return;
		}
	}

	m_AllNPCs.push_back(npc);
}

void Map::UnregisterNPC(BaseGameEntity* npc)
{
	if (npc == nullptr)
	{
		return;
	}
	for (size_t npcIdx = 0; npcIdx < m_AllNPCs.size(); npcIdx++)
	{
		BaseGameEntity* npcTest = m_AllNPCs[npcIdx];
		if (npcTest == npc)
		{
			m_AllNPCs.erase(m_AllNPCs.begin() + npcIdx);
			return;
		}
	}
}

void Map::DestroyNPC(BaseGameEntity* npc)
{
	if (npc == nullptr)
	{
		return;
	}
	UnregisterNPC(npc);
	UnregisterGameEntity(npc);
	delete npc;
}


void Map::RegisterFeature(BaseGameEntity* feature)
{
	if (feature == nullptr)
	{
		return;
	}
	for (size_t featureIdx = 0; featureIdx < m_AllFeatures.size(); featureIdx++)
	{
		BaseGameEntity* featureTest = m_AllFeatures[featureIdx];
		if (featureTest == feature)
		{
			return;
		}
	}

	m_AllFeatures.push_back(feature);

}

void Map::UnregisterFeature(BaseGameEntity* feature)
{
	if (feature == nullptr)
	{
		return;
	}
	for (size_t featureIdx = 0; featureIdx < m_AllFeatures.size(); featureIdx++)
	{
		BaseGameEntity* featureTest = m_AllFeatures[featureIdx];
		if (featureTest == feature)
		{
			m_AllFeatures.erase(m_AllFeatures.begin() + featureIdx);
			return;
		}
	}
}

void Map::DestroyFeature(BaseGameEntity* feature)
{
	if (feature == nullptr)
	{
		return;
	}
	UnregisterFeature(feature);
	UnregisterGameEntity(feature);
	delete feature;
}


STATIC void Map::ClearAllMapsOfAllEntities(bool allowPlayerKill, bool totalClear)
{
	for (std::map<std::string, Map*>::iterator it = GameCommons::s_AllLoadedMaps.begin();
		it != GameCommons::s_AllLoadedMaps.end(); it++)
	{
		Map* map = it->second;
		if (map != nullptr)
		{
			map->ClearAllGameEntities(allowPlayerKill, totalClear);
		}
	}

	if(totalClear)
	{
		BaseGameEntity::TotalClearGameEntities();
	}
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Global Variables related
STATIC void Map::WriteGlobalVariablesToString(std::string& str, int indentationAmount)
{
	if (s_GlobalVariables == nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmount);
		str += "No global variables defined.\n";
		return;
	}
	indentationAmount += 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmount);
	str += "Number of global variables: " + std::to_string(s_GlobalVariables->Size()) + "\n";

	indentationAmount += 3;
	NamedPropertiesToString::ConvertAllKnownNamedPropertyValuesToString(s_GlobalVariables, str, indentationAmount);
}

STATIC void Map::SpawnGlobalVariables()
{
	if (s_GlobalVariables == nullptr)
	{
		s_GlobalVariables = new NamedProperties();
	}
}

STATIC void Map::DestroyGlobalVariables()
{
	if (s_GlobalVariables != nullptr)
	{
		delete s_GlobalVariables;
		s_GlobalVariables = nullptr;
	}
}

STATIC const NamedProperties* Map::GetGlobalVariables()
{
	return s_GlobalVariables;
}

STATIC NamedProperties* Map::GetEditableGlobalVariables()
{
	return s_GlobalVariables;
}

STATIC void Map::SetEnforceNoEmptySpriteResourcesAllowedOnMap(bool enforce)
{
	s_EnableEnforcingAllTilesAsNotNull = enforce;
}

STATIC void Map::SetGlobalVariable(const std::string& variableName, const float& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return;
	}
	s_GlobalVariables->Set<float>(variableName, variable);
}

STATIC void Map::SetGlobalVariable(const std::string& variableName, const bool& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return;
	}
	s_GlobalVariables->Set<bool>(variableName, variable);
}

STATIC void Map::SetGlobalVariable(const std::string& variableName, BaseGameEntity* variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return;
	}
	s_GlobalVariables->Set<BaseGameEntity*>(variableName, variable);
}

STATIC void Map::SetGlobalVariable(const std::string& variableName, const std::string& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return;
	}
	s_GlobalVariables->Set<std::string>(variableName, variable);
}

STATIC void Map::SetGlobalVariable(const std::string& variableName, const int& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return;
	}
	s_GlobalVariables->Set<int>(variableName, variable);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, float& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<float>(variableName, variable);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, bool& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<bool>(variableName, variable);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, BaseGameEntity*& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<BaseGameEntity*>(variableName, variable);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, std::string& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<std::string>(variableName, variable);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, int& variable)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<int>(variableName, variable);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, float& variable, const float& defaul)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<float>(variableName, variable, defaul);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, bool& variable, const bool& defaul)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<bool>(variableName, variable, defaul);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, BaseGameEntity*& variable,
	BaseGameEntity* defaul)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<BaseGameEntity*>(variableName, variable, defaul);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, std::string& variable,
	const std::string& defaul)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<std::string>(variableName, variable, defaul);
}

STATIC PropertyGetResult Map::GetGlobalVariable(const std::string& variableName, int& variable,
	const int& defaul)
{
	if (s_GlobalVariables == nullptr)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	return s_GlobalVariables->Get<int>(variableName, variable, defaul);
}