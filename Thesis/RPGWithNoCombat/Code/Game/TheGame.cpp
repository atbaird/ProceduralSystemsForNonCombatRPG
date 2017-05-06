#include "Engine/2D/SpriteDatabase.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/2D/SpriteLoader.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/TheGame.hpp"
#include "Game/Common/GameCommonVars.hpp"
#include "Game/GameEntities/Agents/AgentGenerator.hpp"
#include "Game/Map/TileEvent/TileEventFunctions.hpp"
#include "Game/Map/MapFiles/MapFiles.hpp"
#include "Game/GameEntities/Items/ItemGenerator.hpp"
#include "Game/GameEntities/Items/Item.hpp"
#include "Game/Dialogue/Dialogue.hpp"
#include "Game/SoundManager/SoundManager.hpp"
#include "Game/GameEntities/Features/FeatureGenerator.hpp"
#include "Game/GameEntities/Agents/AgentStats/AgentStat.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttributes/NPCAttributeContainer.hpp"
#include "Game/GameEntities/Agents/Names/NameGenerator.hpp"
#include "Game/GameEntities/Hair/HairGenerator.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCJobs/NPCJob.hpp"
#include "Game/Dialogue/DialogueSegment.hpp"
#include "Game/Debugging/DebuggingFileHandler.hpp"
#include "Game/Debugging/DebugConsoleCommands.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/Debugging/DeveloperDebugger.hpp"
#include <time.h>


//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
TheGame* g_TheGame = nullptr;

const IntVector2 TheGame::s_centralMousePosition = IntVector2(800, 700);
const Vector2 TheGame::s_mouseYawPitchSensitivity = Vector2(0.022f, 0.022f);

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC GameSettings* TheGame::s_GameSettings = nullptr;

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//Constructors
TheGame::TheGame()
	: 
	m_quitNow(false),
	m_tileViewRange(10.f, 10.f),
	m_lastPositionPlayerSeenAlive(0.f, 0.f),
	m_map(nullptr)
{
	m_timeGameStarted = DebuggingFileHandler::GetCurrentTimeAndDateAsString();
	m_timeTheGameWasCreated = Performance::GetCurrentPerformanceCount();
	m_generalMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_generalMatrix, 0.f, 0.f, 0.f, Vector2(0.f, 0.f));
	m_secondsTakenPerEachDataPieceLoaded.resize(Amount_of_Data_to_load_before_can_start, 0.f);
	ReadInGameDataFromCommonXMLFolder();
	Map::SpawnGlobalVariables();
	m_halfTileViewRange = Vector2(10.f, 10.f);
}

TheGame::~TheGame()
{
	ClearGameSettings();
	SoundManager::ClearSoundManager();
	m_SoundManager = nullptr;
	Map::ObliterateMaps();
	Map::ClearAllMapsOfAllEntities(true, true);
	Map::DestroyGlobalVariables();
	SpriteLoader::ClearAllSprites();
	if (SpriteDatabase::g_SpriteDatabase != nullptr)
	{
		delete SpriteDatabase::g_SpriteDatabase;
		SpriteDatabase::g_SpriteDatabase = nullptr;
	}
	ShaderProgram::DeletAllShaderPrograms();

	if (GameCommons::s_PlayerController != nullptr)
	{
		delete GameCommons::s_PlayerController;
		GameCommons::s_PlayerController = nullptr;
	}
	if (m_ItemUpForDebug != nullptr)
	{
		delete m_ItemUpForDebug;
		m_ItemUpForDebug = nullptr;
	}
	TileDefinition::ClearTileDefinitions();
	Dialogue::ClearAllDialoguePieces();
	AgentGenerator::ClearAllGenerators();
	ItemGenerator::ClearAllGenerators();
	TileEventFunctions::UnregisterTileEvents();
	Item::AbsoluteClearAllItems();
	MapFiles::ClearAllMapFiles();
	if (DeveloperDebugger::g_DeveloperDebugger != nullptr)
	{
		delete DeveloperDebugger::g_DeveloperDebugger;
		DeveloperDebugger::g_DeveloperDebugger = nullptr;
	}
}

void TheGame::StepsForAfterConstruction()
{
	srand(s_GameSettings->m_GameSeed);
	m_defaultProgram = ShaderProgram::CreateOrGetShaderProgram(
		s_GameSettings->m_defaultShaderVertFile,
		s_GameSettings->m_defaultShaderFragFile);
	ShaderProgram::SetDefaultShaderProgram(m_defaultProgram);
	DialogueSegment::SetDefaultTextShader(m_defaultProgram);

	SpriteDatabase::g_SpriteDatabase = new SpriteDatabase();

	std::string spriteResourcesFolder = std::string(s_CommonSpriteResourcesFolder);
	std::string animationsFolder = std::string(s_CommonSpriteAnimationsFolder);
	unsigned int numberOfSpritesToReserve = 50;
	unsigned int numberOfAnimationsToReserve = 50;
	if (s_GameSettings != nullptr)
	{
		numberOfAnimationsToReserve = s_GameSettings->m_preallocationAmountSpriteAnimations;
		numberOfSpritesToReserve = s_GameSettings->m_preallocationAmountSpriteResources;
		spriteResourcesFolder = s_GameSettings->m_commonSpriteResourcesFileDirectory;
		animationsFolder = s_GameSettings->m_commonSpriteAnimationsFileDirectory;
	}
	SpriteLoader::ReserveSpriteResources(numberOfSpritesToReserve);
	SpriteLoader::ReserveSpriteAnimations(numberOfAnimationsToReserve);
	SpriteLoader::SetUpOnUpdateSaveSprites(spriteResourcesFolder.c_str(), true,
		s_GameSettings->m_amountOfTimeToSpendLoadingSpriteResources);
	SpriteLoader::SetUpOnUpdateSaveAnimations(animationsFolder.c_str(), true,
		s_GameSettings->m_amountOfTimeToSpendLoadingSpriteAnimations);

	TileEventFunctions::RegisterAllTileEventsAtOnce();
	m_dialogue = Dialogue::SpawnOrGetCentralDialogue();

	GameCommons::s_PlayerController = new Controller();
	GameCommons::s_PlayerController->SetLockControls(true);
}

void TheGame::OldStepsForAfterConstruction()
{
	SoundManager::ReadInAllSoundsFromXMLAllAtOnce();
	m_SoundManager = SoundManager::CreateOrGetSoundManager();

	SpriteLoader::LoadAllSpritesResourcesFromFolderAsNew(s_CommonSpriteResourcesFolder, true);
	SpriteLoader::LoadAllSpriteAnimationsFromFolderAsNew(s_CommonSpriteAnimationsFolder, true);


	AgentGenerator::ReadInGeneratorsFromCommonAgentGeneratorsFolderAllAtOnce();
	ItemGenerator::ReadInGeneratorsFromCommonItemGeneratorsFolderAllAtOnce();
	BaseGameEntity* clothes = ItemGenerator::SpawnItemByItemName(
		s_GameSettings->m_defaultClothesForAllAgents.c_str(),
		Vector2::vec2_zeros);
	Item* clothesItem = dynamic_cast<Item*>(clothes);
	Item::SetDefaultMaleClothes(clothesItem);
	Item::SetDefaultFemaleClothes(clothesItem);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//Updates and Render
void TheGame::Update(const float& deltaSeconds, const bool& focus)
{
	m_frameStartTime = Performance::GetCurrentPerformanceCount();
	if (m_DataFullyLoaded)
	{
		RegularUpdate(deltaSeconds, focus);
	}
	else
	{
		LoadingUpdate(deltaSeconds, focus);
	}
}

void TheGame::RegularUpdate(const float& deltaSeconds, const bool& focus)
{
	if (m_SoundManager != nullptr)
	{
		m_SoundManager->Update(deltaSeconds);
	}
	g_input->Update();
	if (GameCommons::s_PlayerController != nullptr)
	{
		GameCommons::s_PlayerController->ControllerUpdate();
	}
	if (focus)
	{
		if (g_input->IsKeyDown(VK_ESCAPE) == true)
		{
			if (s_GameSettings->m_EnableDebugLogging)
			{
				HandleClosingDebugLogging();
			}
			m_quitNow = true;
		}
	}

	if (m_ItemUpForDebug != nullptr)
	{
		m_ItemUpForDebug->Update(0.f);
		if (g_input->IsKeyDown('I') && m_DebugIKeyIsDown == false)
		{
			m_DebugIKeyIsDown = true;
			m_ItemUpForDebug->DebugCycleThroughWalkingDirections();
		}
		if (g_input->IsKeyDown('K') && m_DebugKKeyIsDown == false)
		{
			m_DebugKKeyIsDown = true;
			m_ItemUpForDebug->DebugCycleThroughSpriteIdxInAnimation();
		}
	}
	if (g_input->IsKeyDown('I') == false && m_DebugIKeyIsDown == true)
	{
		m_DebugIKeyIsDown = false;
	}
	if (g_input->IsKeyDown('K') == false && m_DebugKKeyIsDown == true)
	{
		m_DebugKKeyIsDown = false;
	}
	if (g_input->IsKeyDown(VK_F1) == true && m_F1Down == false)
	{
		m_F1Down = true;
		if (DeveloperDebugger::g_DeveloperDebugger != nullptr)
		{
			DeveloperDebugger::g_DeveloperDebugger->ToggleRenderAllowed();
		}
	}


	if (g_input->IsKeyDown(VK_F1) == false && m_F1Down == true)
	{
		m_F1Down = false;
	}

	bool currentlyLoadingMap = 
		Map::UpdateLoadingMap();
	m_map = Map::GetEditableMap();
	if (DeveloperDebugger::g_DeveloperDebugger != nullptr)
	{
		DeveloperDebugger::g_DeveloperDebugger->Update(m_map, currentlyLoadingMap, 
			m_lastPositionPlayerSeenAlive);
	}

	if (currentlyLoadingMap)
	{
		if (GameCommons::s_PlayerController != nullptr)
		{
			GameCommons::s_PlayerController->SetLockControls(true);
		}
		m_LoadingMessageToScreen = "Loading map";
		UpdateLoadingMessage(deltaSeconds);
		m_wasLoadingMapLastFrame = true;
		return;
	}

	if (m_wasLoadingMapLastFrame)
	{
		if (GameCommons::s_PlayerController != nullptr)
		{
			GameCommons::s_PlayerController->SetLockControls(false);
		}
		m_map->RegisterGameEntity(m_PlayerPointer);
		Vector2 playerPos = m_map->GetCenterOfTileAtTileCoords(m_positionToSetThePlayerTo);
		m_PlayerPointer->SetPosition(playerPos);
		m_lastPositionPlayerSeenAlive = playerPos;
		g_TheGame->SetUpdatePlayerPointer(true);
	}

	if (m_updatePlayerPointer)
	{
		m_PlayerPointer = BaseGameEntity::GetEditablePointerToPlayer();
	}

	m_wasLoadingMapLastFrame = false;

	if (m_map != nullptr)
	{
		m_map->Update(deltaSeconds);
	}

	ReadDataOffPlayer();
	PrepareViewAndProjectionMatrix();

	if (m_PrintOutContentNextFrame)
	{
		if (m_PrintOutContentNextFrameFramePassedFirst)
		{
			m_PrintOutContentNextFrame = false;
			m_PrintOutContentNextFrameFramePassedFirst = false;
			std::string str = "";
			HandleClosingDebugLogging(str, false);
			g_Console->ConsolePrintf(UNIMPORTANT, "Debug Log Printed.");
			if (m_PrintOutToCommandLine)
			{
				m_PrintOutToCommandLine = false;
				DebuggerPrintf(str.c_str());
			}

			if (m_PrintOutToConsole)
			{
				g_Console->ConsolePrintf(UNIMPORTANT, str);
				m_PrintOutToConsole = false;
			}
		}
		else
		{
			m_PrintOutContentNextFrameFramePassedFirst = true;
		}
	}
	else
	{
		m_PrintOutContentNextFrameFramePassedFirst = false;
	}

}

void TheGame::LoadingUpdate(const float& deltaSeconds, const bool& focus)
{
	if (focus)
	{
		if (g_input->IsKeyDown(VK_ESCAPE) == true)
		{
			if (s_GameSettings->m_EnableDebugLogging)
			{
				HandleClosingDebugLogging();
			}
			m_quitNow = true;
		}
	}
	deltaSeconds;

	PrepareViewAndProjectionMatrix();

	m_messageLastFrame = m_LoadingMessageToScreen;
	bool finishedLoadingCurrentMode = false;

	uint64_t timeBeforeRunFrameLoad = Performance::GetCurrentPerformanceCount();
	switch (m_dataLoadingMode)
	{
	case SoundData:
		m_LoadingMessageToScreen = "Game Loading: Loading Sounds";
		finishedLoadingCurrentMode = SoundManager::OnUpdateReadInAllSoundsFromXML();
		if (finishedLoadingCurrentMode)
		{
			m_SoundManager = SoundManager::CreateOrGetSoundManager();
		}
		break;
	case SpritesAndAnimations:
		m_LoadingMessageToScreen = "Game Loading: Loading Sprites and Animations";
		finishedLoadingCurrentMode = SpriteLoader::OnUpdateLoadSpriteResourcesAndAnimations();
		if(finishedLoadingCurrentMode)
		{
			if (s_GameSettings->m_SpawnConsole)
			{
				Font* consoleFont = Font::CreateOrGetFont(s_GameSettings->m_consoleFontContainingFolder,
					s_GameSettings->m_consoleFontFNTFileName);
				ShaderProgram* consoleShader = ShaderProgram::CreateOrGetShaderProgram(
					s_GameSettings->m_defaultConsoleShaderVertFile, 
					s_GameSettings->m_defaultConsoleShaderFragFile);
				Texture* consoleWhiteBoxTexture = nullptr;
				const SpriteResource* res = 
					SpriteResourceHandler::GetSpriteResourceByName(s_GameSettings->m_ConsoleWhiteBoxName);
				if (res != nullptr)
				{
					consoleWhiteBoxTexture = res->GetSpriteSheet();
				}


				g_Console = new Console(consoleFont, false, consoleShader, consoleWhiteBoxTexture,
					s_GameSettings->m_ConsoleFontSize, s_GameSettings->m_maxNumberOfConsoleLines);
				g_Console->SetFontsize(s_GameSettings->m_ConsoleFontSize);
				DebugConsoleCommands::RegisterAllConsoleCommands();
			}

			if (s_GameSettings->m_EnableDeveloperDebugMode)
			{
				Font* consoleFont = Font::CreateOrGetFont(s_GameSettings->m_consoleFontContainingFolder,
					s_GameSettings->m_consoleFontFNTFileName);
				ShaderProgram* consoleShader = ShaderProgram::CreateOrGetShaderProgram(
					s_GameSettings->m_defaultConsoleShaderVertFile,
					s_GameSettings->m_defaultConsoleShaderFragFile);
				const SpriteResource* res =
					SpriteResourceHandler::GetSpriteResourceByName(s_GameSettings->m_ConsoleWhiteBoxName);

				DeveloperDebugger::g_DeveloperDebugger = new DeveloperDebugger(
					s_GameSettings->m_ConsoleFontSize, consoleFont, consoleShader,
					res);
			}
		}
		break;
	case MapFileContents:
		m_LoadingMessageToScreen = "Game Loading: Loading Tile Definitions";
		finishedLoadingCurrentMode = Map::OnUpdateLoadTileDefinitions();
		break;
	case DialoguePieces:
		m_LoadingMessageToScreen = "Game Loading: Loading Dialogue Segments and Groupings";
		finishedLoadingCurrentMode = Dialogue::OnUpdateLoadDialoguePieces();
		break;
	case NPCAttributes:
		m_LoadingMessageToScreen = "Game Loading: Loading NPC Attributes";
		finishedLoadingCurrentMode = NPCAttributeContainer::OnUpdateReadInNPCAttributesFromCommonFolder();
		break;
	case HairGenerators:
		m_LoadingMessageToScreen = "Game Loading: Loading Hair Generators";
		finishedLoadingCurrentMode = HairGenerator::OnUpdateReadInGeneratorsFromCommonHairGeneratorsFolder();
		break;
	case Names:
		m_LoadingMessageToScreen = "Game Loading: Loading NPC Attributes";
		finishedLoadingCurrentMode = NameGenerator::OnUpdateReadInGeneratorsFromCommonNameGeneratorsFolder();
		break;
	case Features:
		m_LoadingMessageToScreen = "Game Loading: Loading Feature Generators";
		finishedLoadingCurrentMode = FeatureGenerator::OnUpdateLoadAllFeaturesFromXML();
		break;
	case AgentStats:
		m_LoadingMessageToScreen = "Game Loading: Loading Agent Stats";
		finishedLoadingCurrentMode = AgentStat::OnUpdateReadInAllAgentStatsFromCommonFolder();
		break;
	case NPCJobs:
		m_LoadingMessageToScreen = "Game Loading: Loading NPC Jobs";
		finishedLoadingCurrentMode = NPCJob::OnUpdateRegisterAllJobsFromXMLFile();
		break;
	case Agents:
		m_LoadingMessageToScreen = "Game Loading: Loading Agent Generators";
		finishedLoadingCurrentMode = AgentGenerator::OnUpdateReadInGeneratorsFromCommonAgentGeneratorsFolder();
		break;
	case Items:
		m_LoadingMessageToScreen = "Game Loading: Loading Item Generators";
		finishedLoadingCurrentMode = ItemGenerator::OnUpdateReadInGeneratorsFromCommonItemGeneratorsFolder();
		if (finishedLoadingCurrentMode)
		{
			BaseGameEntity* clothes = ItemGenerator::SpawnItemByItemName(
				s_GameSettings->m_defaultClothesForAllAgents.c_str(),
				Vector2::vec2_zeros);
			Item* clothesItem = dynamic_cast<Item*>(clothes);
			Item::SetDefaultMaleClothes(clothesItem);
			Item::SetDefaultFemaleClothes(clothesItem);
			MapFiles::LoadMapByNameRegisterForOnUpdate(s_GameSettings->m_startingMap);
			//Map::LoadTileDefinitionsAllAtOnce();
		}
		break;
	case GlobalVariables:
		m_LoadingMessageToScreen = "Game Loading: Loading Global Variables";
		finishedLoadingCurrentMode = Map::UpdateLoadingGlobalVariables();
		break;
	case Maps:
		m_LoadingMessageToScreen = "Game Loading: Loading first map";
		finishedLoadingCurrentMode = !Map::UpdateLoadingMap();
		break;
	case Amount_of_Data_to_load_before_can_start:
		m_LoadingMessageToScreen = "Game Loading: Loading first map";
		if(m_singleFramePassedSinceFinishedLoading)
		{
			m_DataFullyLoaded = true;
			//MapFiles::LoadMapByNameAllAtOnce(s_GameSettings->m_startingMap);
			m_map = Map::GetEditableMap();

			Vector2 playerPosition = m_map->GetCenterOfTileAtTileCoords(
				s_GameSettings->m_playerStartingTile);

			m_PlayerPointer = AgentGenerator::SpawnPlayerInAgentGeneratorByAgentName(
				s_GameSettings->m_agentGeneratorToUseForSpawningPlayer.c_str(),
				playerPosition);
			m_lastPositionPlayerSeenAlive = playerPosition;
			BaseGameEntity::SetPointerToPlayer(m_PlayerPointer);
			m_map->RegisterGameEntity(m_PlayerPointer);

			GameCommons::s_PlayerController->SetLockControls(false);
		}
		else
		{
			m_singleFramePassedSinceFinishedLoading = true;
		}

		break;
	default:
		//What happened?
		break;
	}
	uint64_t timeAfterFrameRuns = Performance::GetCurrentPerformanceCount();

	if (m_dataLoadingMode != Amount_of_Data_to_load_before_can_start)
	{
		uint64_t deltaTime = timeAfterFrameRuns - timeBeforeRunFrameLoad;
		m_secondsTakenPerEachDataPieceLoaded[m_dataLoadingMode] +=
			(float)Performance::PerformanceCountToSeconds(deltaTime);
	}

	if (finishedLoadingCurrentMode)
	{
		m_dataLoadingMode = (DataPresentlyLoading)(m_dataLoadingMode + 1);
	}
	UpdateLoadingMessage(deltaSeconds);

}

void TheGame::UpdateLoadingMessage(const float& deltaSeconds)
{
	m_secondsSinceLastIncrementedPeriods += deltaSeconds;
	if (m_secondsSinceLastIncrementedPeriods >= m_secondsBeforeIncrementPeriods)
	{
		m_secondsSinceLastIncrementedPeriods = 0.f;
		m_numberOfPeriodsForLoadingMessage++;
		if (m_numberOfPeriodsForLoadingMessage > m_maxNumberOfPeriods)
		{
			m_numberOfPeriodsForLoadingMessage = 1;
		}
	}

	m_LoadingMessageToScreen += "\n";
	for (size_t numOfPeriods = 0; numOfPeriods < m_numberOfPeriodsForLoadingMessage; numOfPeriods++)
	{
		m_LoadingMessageToScreen += ".";
	}


	if (m_messageLastFrame == m_LoadingMessageToScreen
		|| m_font == nullptr)
	{
		return;
	}

	m_font->GetTextToRender(m_loadingMessageVertices, m_loadingMessageIndices,
		Vector2(0.f, -0.3f), m_LoadingMessageToScreen,
		m_loadingMessageDialogueSize,
		Vector3(1.f, 0.f), Vector3(0.f, 1.f),
		Rgba::s_White, Font_Center_Aligned, true, 0.5f);

	size_t pageCount = m_font->GetPageCount();
	m_loadingMessageTextMeshes.resize(pageCount);
	m_loadingMessageTextMats.resize(pageCount);
	for (int pageIdx = 0; pageIdx < (int)pageCount; pageIdx++)
	{
		m_loadingMessageTextMeshes[pageIdx].setVBO(m_loadingMessageVertices[pageIdx]);
		m_loadingMessageTextMeshes[pageIdx].setIBO(m_loadingMessageIndices[pageIdx]);
		m_loadingMessageTextMats[pageIdx].SetProgram(ShaderProgram::GetDefaultShaderProgram());
		const Texture* fontPage = m_font->GetPageForIdx(pageIdx);
		m_loadingMessageTextMats[pageIdx].SetOrAddTexture(fontPage, 0, "gTextureDiffuse", 0);
	}
}


void TheGame::HandleClosingDebugLogging(bool actuallyQuitting)
{
	std::string str = "";
	HandleClosingDebugLogging(str, actuallyQuitting);
}

double TheGame::WriteOutEndingData()
{
	m_timeGameEnded = DebuggingFileHandler::GetCurrentTimeAndDateAsString();
	m_timeJustBeforeTheGameWasDestroyed = Performance::GetCurrentPerformanceCount();
	uint64_t durationOfPlayTime = m_timeJustBeforeTheGameWasDestroyed - m_timeTheGameWasCreated;
	return Performance::PerformanceCountToSeconds(durationOfPlayTime);
}

void TheGame::HandleClosingDebugLogging(std::string& str, bool actuallyQuitting)
{
	double durationPlayTime = WriteOutEndingData();
	s_GameSettings->WriteDataOutToDebugFile(m_timeGameStarted, m_timeGameEnded,
		durationPlayTime, actuallyQuitting, str);
}

void TheGame::PrepareViewAndProjectionMatrix()
{
	m_Projection.MatrixMakeIdentity();
	m_View.MatrixMakeIdentity();
	m_DialogueProjection.MatrixMakeIdentity();
	m_DialogueView.MatrixMakeIdentity();

	Matrix44::MakeEulerRotation(m_View, DegreesToRadians(0.f), DegreesToRadians(-0.f),
		DegreesToRadians(0.f), Vector3(m_lastPositionPlayerSeenAlive, 0.f)); //
	Matrix44::MatrixInvertOrthogonal(m_View);
	Matrix44::MatrixMakeOrthogonal(m_Projection, WindowSize, -1.f, 1.f);

	Matrix44::MakeEulerRotation(m_DialogueView, DegreesToRadians(0.f), DegreesToRadians(-0.f),
		DegreesToRadians(0.f), Vector3::vec3_zeros); //
	Matrix44::MatrixInvertOrthogonal(m_DialogueView);
	Matrix44::MatrixMakeOrthogonal(m_DialogueProjection, WindowSize, -1.f, 1.f);
}

void TheGame::Render() const
{
	if (m_DataFullyLoaded)
	{
		RegularRender();
	}
	else
	{
		LoadingRender();
	}
}

void TheGame::RegularRender() const
{
	g_Renderer->DepthTestingTrueFalse(false);
	//g_Renderer->FramebufferBind(current_surface);
	SetUp2DView();
	ShaderProgram::SetProjectionAndViewAll(m_Projection, m_View);

	bool loadingMap = Map::GetLoadingMap();
	if (!loadingMap && m_map != nullptr)
	{
		m_map->Render();
	}
	else if (loadingMap)
	{
		Map::GetPreviousMap()->Render();
	}

	if (m_ItemUpForDebug != nullptr)
	{
		m_ItemUpForDebug->Render();
	}

	//Begin dialogue rendering.
	SetUp2DView();
	ShaderProgram::SetProjectionAndViewAll(m_DialogueProjection, m_DialogueView);

	if (m_dialogue != nullptr)
	{
		m_dialogue->Render();
	}
;
	if(loadingMap)
	{
		if (m_font != nullptr)
		{
			for (int pageIdx = 0; pageIdx < m_font->GetPageCount(); pageIdx++)
			{
				m_loadingMessageTextMats[pageIdx].SetModelMatrix(m_generalMatrix);
				m_loadingMessageTextMeshes[pageIdx].DrawWith(&m_loadingMessageTextMats[pageIdx]);
			}
		}
	}

	if (DeveloperDebugger::g_DeveloperDebugger != nullptr)
	{
		DeveloperDebugger::g_DeveloperDebugger->Render();
	}
}

void TheGame::LoadingRender() const
{
	SetUp2DView();
	ShaderProgram::SetProjectionAndViewAll(m_DialogueProjection, m_DialogueView);

	if (m_font != nullptr)
	{
		for (int pageIdx = 0; pageIdx < m_font->GetPageCount(); pageIdx++)
		{
			m_loadingMessageTextMats[pageIdx].SetModelMatrix(m_generalMatrix);
			m_loadingMessageTextMeshes[pageIdx].DrawWith(&m_loadingMessageTextMats[pageIdx]);
		}
	}
}

void TheGame::ReadDataOffPlayer()
{
	if (m_updatePlayerPointer)
	{
		m_PlayerPointer = BaseGameEntity::GetEditablePointerToPlayer();
	}
	if (m_PlayerPointer != nullptr)
	{
		m_lastPositionPlayerSeenAlive = m_PlayerPointer->GetPosition();
		if (m_map != nullptr)
		{
			m_map->AdjustCameraPositionBasedOffBounds(m_lastPositionPlayerSeenAlive);
		}
	}
}

void TheGame::SpawnNPCAtRandomPosition(const std::string& npcEntityName)
{
	if (m_map == nullptr
		|| npcEntityName == "")
	{
		return;
	}
	Vector2 ranPosition = GetRandomPositionOnMap();
	BaseGameEntity* npc = AgentGenerator::SpawnNPCInAgentGeneratorByAgentName(
		npcEntityName.c_str(), ranPosition);
	Map* map = Map::GetEditableMap();
	map->RegisterGameEntity(npc);
	map->RegisterNPC(npc);

}

void TheGame::SpawnRandomNPCAtRandomPosition()
{
	if (m_map == nullptr)
	{
		return;
	}
	Vector2 ranPosition = GetRandomPositionOnMap();
	BaseGameEntity* npc = AgentGenerator::SpawnNPCInAgentGeneratorRandom(ranPosition);
	Map* map = Map::GetEditableMap();
	map->RegisterGameEntity(npc);
	map->RegisterNPC(npc);
}

Vector2 TheGame::GetRandomPositionOnMap() const
{
	if (m_map == nullptr)
	{
		return Vector2::vec2_zeros;
	}
	return m_map->GetRandomPositionOnMap();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//Utils
void TheGame::SetUp2DView() const
{
	g_Renderer->SetOrtho(Vector2(0.f, 0.f), WindowSize);
}

void TheGame::DrawDebugAxisLines() const
{
	g_Renderer->DepthTestingTrueFalse(true);
	g_Renderer->DrawGridLinesNEU(2.f, 2.f, 2.f, 3.f);
	g_Renderer->DepthTestingTrueFalse(false);
	g_Renderer->DrawGridLinesNEU(2.f, 2.f, 2.f, 1.f);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//Setters
void TheGame::PrintOutDebugContentNextFrame(bool print, bool shouldPrintToCommandLine,
	bool shouldPrintToConsole)
{
	m_PrintOutContentNextFrame = print;
	m_PrintOutToCommandLine = shouldPrintToCommandLine;
	m_PrintOutToConsole = shouldPrintToConsole;
}

void TheGame::SetUpdatePlayerPointer(bool updatePlayerPointer)
{
	m_updatePlayerPointer = updatePlayerPointer;
}

void TheGame::SetFont(Font* font)
{
	m_font = font;
	Dialogue::SetDefaultFont(font);
}

void TheGame::SetPositionForPlayerOnChangeMap(const IntVector2& position)
{
	m_positionToSetThePlayerTo = position;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//Getters
bool TheGame::isQuitting() const
{
	return m_quitNow;
}

bool TheGame::GetWasLoadingMapLastFrame() const
{
	return m_wasLoadingMapLastFrame;
}

float TheGame::GetWindowWidth() const
{
	return WindowSize.x;
}

float TheGame::GetWindowHeight() const
{
	return WindowSize.y;
}

const Vector2& TheGame::GetWindowSize() const
{
	return WindowSize;
}

uint64_t TheGame::GetFrameStartTime() const
{
	return m_frameStartTime;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
//Static Functions
STATIC void TheGame::SpawnGameSettings()
{
	if (s_GameSettings == nullptr)
	{
		s_GameSettings = new GameSettings();
	}
}

STATIC void TheGame::ClearGameSettings()
{
	if (s_GameSettings != nullptr)
	{
		delete s_GameSettings;
		s_GameSettings = nullptr;
	}
}

STATIC void TheGame::ReadInGameDataFromCommonXMLFolder()
{
	SpawnGameSettings();
	s_GameSettings->m_GameSeed = (unsigned int)time(0);
	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(s_CommonGameSettingsFolder, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
		if (SimpleStrCmpLower(last3Letters, "xml") == false)
		{
			continue;
		}

		bool errorOccurred = false;
		XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, s_CommonGameSettingsRoot, errorOccurred);
		if (errorOccurred == true)
		{
			continue;
		}

		ReadGameDataOffOfXMLNode(root);
	}
}

STATIC void TheGame::ReadGameDataOffOfXMLNodeMap(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "mapname"
			|| attrName == "map")
		{
			s_GameSettings->m_startingMap = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeShader(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "vertexfile"
			|| attrName == "vertfile"
			|| attrName == "vertex"
			|| attrName == "vert")
		{
			s_GameSettings->m_defaultShaderVertFile = std::string(attr.lpszValue);
		}
		else if (attrName == "fragmentfile"
			|| attrName == "fragfile"
			|| attrName == "fragment"
			|| attrName == "frag")
		{
			s_GameSettings->m_defaultShaderFragFile = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeMapShader(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "vertexfile"
			|| attrName == "vertfile"
			|| attrName == "vertex"
			|| attrName == "vert")
		{
			s_GameSettings->m_defaultMapShaderVertFile = std::string(attr.lpszValue);
		}
		else if (attrName == "fragmentfile"
			|| attrName == "fragfile"
			|| attrName == "fragment"
			|| attrName == "frag")
		{
			s_GameSettings->m_defaultMapShaderFragFile = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodePlayer(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "agentgenerator"
			|| attrName == "agent"
			|| attrName == "player")
		{
			s_GameSettings->m_agentGeneratorToUseForSpawningPlayer = std::string(attr.lpszValue);
		}
		else if (attrName == "tilecoords"
			|| attrName == "position")
		{
			bool sizeOf1 = false;
			TILE_COORDS coords = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1);
			if (sizeOf1)
			{
				s_GameSettings->m_playerStartingTile = coords;
			}
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeClothes(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "itemgenerator"
			|| attrName == "clothinggenerator"
			|| attrName == "item"
			|| attrName == "clothing")
		{
			s_GameSettings->m_defaultClothesForAllAgents = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeFont(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_FontContainingFolder = std::string(attr.lpszValue);
		}
		else if (attrName == "fntfile"
			|| attrName == "fntfilename")
		{
			s_GameSettings->m_FontFNTFileName = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeDialogueSegments(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonDialogueSegmentsXMLFolder = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeDialogueGroupings(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonDialogueGroupingsXMLFolder = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeAgentGenerators(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonAgentGeneratorFileDirectory = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeFeatureGenerators(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonFeatureGeneratorFileDirectory = std::string(attr.lpszValue);
		}
	}
}


STATIC void TheGame::ReadTheGameDataOffOfXMLNodeHairGenerators(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonHairGeneratorsFolder = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeNPCAttributes(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonNPCAttributeContainerFileLocation = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeAgentStats(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_CommonAgentStatFolder = std::string(attr.lpszValue);
		}
	}
}
STATIC void TheGame::ReadTheGameDataOffOfXMLNodeNPCJobs(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_CommonNPCJobFileDirectory = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeSpeechBubbles(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonSpeechBubbleXMLFolder = std::string(attr.lpszValue);
		}
	}

}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeNameGenerators(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonNameGeneratorsFileDirection = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeItemGenerators(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonItemGeneratorsFolder = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeSoundGenerators(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_soundCommonFolder = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeQuestGenerators(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonQuestXMLFolder = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeMapFiles(const XMLNode& child)
{/*
	<TileDefinitions ContainingFolder='Data/XML/TileDefinitions/'/>
	*/
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_mapFilesCommonXMLFolder = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeSpriteResources(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonSpriteResourcesFileDirectory = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeSpriteAnimations(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonSpriteAnimationsFileDirectory = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeTileDefinitions(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_tileDefinitionXMLFolderLocaction = std::string(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeGlobalVariables(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "folder"
			|| attrName == "containingfolder")
		{
			s_GameSettings->m_commonGlobalVariablesFileDirectory = std::string(attr.lpszValue);
		}
	}
}


STATIC void TheGame::ReadTheGameDataOffOfXMLNodePreallocations(const XMLNode& child)
{
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);

		if (attrName == "agentgenerators"
			|| attrName == "agentgenerator")
		{
			//unsigned int m_preallocationAmountAgentGenerators = 50;
			s_GameSettings->m_preallocationAmountAgentGenerators = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "agentspermap"
			|| attrName == "agents")
		{
			//unsigned int m_preallocationAmountAgentsPerMap = 20;
			s_GameSettings->m_preallocationAmountAgentsPerMap = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "featuregenerators"
			|| attrName == "featuregenerator")
		{
			//unsigned int m_preallocationAmountFeatureGenerators = 50;
			s_GameSettings->m_preallocationAmountFeatureGenerators = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "featurespermap"
			|| attrName == "features")
		{
			//unsigned int m_preallocationAmountFeaturesPerMap = 20;
			s_GameSettings->m_preallocationAmountFeaturesPerMap = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "hairgenerators"
			|| attrName == "hairgenerator")
		{
			//unsigned int m_preallocationAmountHairGenerators = 50;
			s_GameSettings->m_preallocationAmountHairGenerators = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "itemgenerators"
			|| attrName == "itemgenerator")
		{
			//unsigned int m_preallocationAmountItemGenerators = 50;
			s_GameSettings->m_preallocationAmountItemGenerators = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "soundregistrations"
			|| attrName == "soundregistration"
			|| attrName =="allsoundregistrations"
			|| attrName == "allsoundregistration")
		{
			//unsigned int m_preallocationAmountSoundRegistrations = 50;
			s_GameSettings->m_preallocationAmountSoundRegistrations = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "soundeffectregistrations"
			|| attrName == "soundeffectregistration")
		{
			//unsigned int m_preallocationAmountSoundEffectRegistrations = 50;
			s_GameSettings->m_preallocationAmountSoundEffectRegistrations = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "musicregistrations"
			|| attrName == "musicregistration")
		{
			//unsigned int m_preallocationAmountMusicRegistrations = 50;
			s_GameSettings->m_preallocationAmountMusicRegistrations = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "tiledefinitions"
			|| attrName == "tiledefinition")
		{
			//unsigned int m_preallocationAmountTileDefinitions = 20;
			s_GameSettings->m_preallocationAmountTileDefinitions = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "dialoguesegments"
			|| attrName == "dialoguesegment")
		{
			//unsigned int m_preallocationAmountDialogueSegments = 50;
			s_GameSettings->m_preallocationAmountDialogueSegments = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "questgenerators"
			|| attrName == "questgenerator")
		{
			//unsigned int m_preallocationAmountQuestGenerators = 50;
			s_GameSettings->m_preallocationAmountQuestGenerators = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "quests"
			|| attrName == "questspermap")
		{
			//unsigned int m_preallocationAmountQuestsPerMap = 20;
			s_GameSettings->m_preallocationAmountQuestsPerMap = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "spriteresources"
			|| attrName == "spriteresource")
		{
			//unsigned int m_preallocationAmountSpriteResources = 50;
			s_GameSettings->m_preallocationAmountSpriteResources = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "spriteanimations"
			|| attrName == "spriteanimation")
		{
			//unsigned int m_preallocationAmountSpriteAnimations = 50;
			s_GameSettings->m_preallocationAmountSpriteAnimations = (unsigned int)atoi(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeLoadPerFrame(const XMLNode& child)
{

	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "spriteresources"
			|| attrName == "spriteresource")
		{
			s_GameSettings->m_amountOfTimeToSpendLoadingSpriteResources = atof(attr.lpszValue);
		}
		else if (attrName == "spriteanimations"
			|| attrName == "spriteanimation")
		{
			s_GameSettings->m_amountOfTimeToSpendLoadingSpriteAnimations = atof(attr.lpszValue);
		}
		else if (attrName == "allsounds"
			|| attrName == "sounds"
			|| attrName == "soundeffects"
			|| attrName == "music")
		{
			s_GameSettings->m_amountOfTimeToSpendLoadingSoundsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "mapfiles")
		{
			s_GameSettings->m_amountOfTimeToSpendLoadingMapFilesPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "quests"
			|| attrName == "questgenerators"
			|| attrName == "quest"
			|| attrName == "questgenerator")
		{
			s_GameSettings->m_amountOfTimeLoadingQuestsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "tiledefinitions"
			|| attrName == "tiledefinition")
		{
			s_GameSettings->m_amountOfTimeLoadingTileDefinitionsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "speechbubbles"
			|| attrName == "speechbubble")
		{
			s_GameSettings->m_amountOfTimeLoadingSpeechBubblesPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "attributes"
			|| attrName == "npcattributes"
			|| attrName == "attribute"
			|| attrName == "npcattribute")
		{
			s_GameSettings->m_amountOfTimeLoadingNPCAttributesPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "hairgenerators"
			|| attrName == "hairgenerator"
			|| attrName == "hair")
		{
			s_GameSettings->m_amountOfTimeLoadingHairGeneratorsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "namegenerators"
			|| attrName == "namegenerator"
			|| attrName == "names"
			|| attrName == "name")
		{
			s_GameSettings->m_amountOfTimeLoadingNameGneratorsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "features"
			|| attrName == "feature"
			|| attrName == "featuregenerators"
			|| attrName == "featuregenerator")
		{
			s_GameSettings->m_amountOfTimeLoadingFeatureGeneratorsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "agentstats"
			|| attrName == "agentstat")
		{
			s_GameSettings->m_amountOfTimeLoadingAgentStatsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "npcjobs"
			|| attrName == "npcjob")
		{
			s_GameSettings->m_amountOfTimeLoadingNPCJobsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "agentgenerators"
			|| attrName == "agentgenerator"
			|| attrName == "agents"
			|| attrName == "agent")
		{
			s_GameSettings->m_amountOfTimeLoadingAgentGeneratorsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "itemgenerators"
			|| attrName == "itemgenerator"
			|| attrName == "items"
			|| attrName == "item")
		{
			s_GameSettings->m_amountOfTimeLoadingItemGeneratorsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "mapdata"
			|| attrName == "mapdatanodes"
			|| attrName == "mapdatanode"
			|| attrName == "mapdataandpixels"
			|| attrName == "mapdatanadpixel"
			|| attrName == "map")
		{
			s_GameSettings->m_amountOfTimeLoadingMapDataPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "dialoguesegment"
			|| attrName == "dialoguesegments")
		{
			s_GameSettings->m_amountOfTimeLoadingDialogueSegmentsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "dialoguegroupings"
			|| attrName == "dialoguegrouping")
		{
			s_GameSettings->m_amountOfTimeLoadingDialogueGroupingsPerFrame = atof(attr.lpszValue);
		}
		else if (attrName == "globalvariables")
		{
			s_GameSettings->m_amountOfTimeLoadingGlobalVariablesPerFrame = atof(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadTheGameDataOffOfXMLNodeDebugging(const XMLNode& child)
{
	bool overwriteSeed = false;
	unsigned int seed = 0;
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "overwriteseed")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "false"
				|| attrVal == "f")
			{
				overwriteSeed = false;
			}
			else if (attrVal == "true"
				|| attrVal == "t")
			{
				overwriteSeed = true;
			}
		}
		else if (attrName == "consolefontsize")
		{
			s_GameSettings->m_ConsoleFontSize = (float)atof(attr.lpszValue);
		}
		else if (attrName == "maxnumberofconsolelines"
			|| attrName == "maxconsolelines")
		{
			s_GameSettings->m_maxNumberOfConsoleLines = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "enableconsole"
			|| attrName == "spawnconsole")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_SpawnConsole = false;
			}
			else if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_SpawnConsole = true;
			}
		}
		else if (attrName == "enabledeveloperdebug"
			|| attrName == "enabledevdebug")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_EnableDeveloperDebugMode = true;
			}
			else if(attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_EnableDeveloperDebugMode = false;
			}
		}
		else if (attrName == "consolefontfolder")
		{
			s_GameSettings->m_consoleFontContainingFolder = std::string(attr.lpszValue);
		}
		else if (attrName == "consolefontfntfile")
		{
			s_GameSettings->m_consoleFontFNTFileName = std::string(attr.lpszValue);
		}
		else if (attrName == "consoleshadervertfile"
			|| attrName == "consoleshadervert")
		{
			s_GameSettings->m_defaultConsoleShaderVertFile = std::string(attr.lpszValue);
		}
		else if (attrName == "consoleshaderfragfile"
			|| attrName == "consoleshaderfrag")
		{
			s_GameSettings->m_defaultConsoleShaderFragFile = std::string(attr.lpszValue);
		}
		else if (attrName == "consolewhiteboxspritename")
		{
			s_GameSettings->m_ConsoleWhiteBoxName = std::string(attr.lpszValue);
		}
		else if (attrName == "seed")
		{
			seed = (unsigned int)atoi(attr.lpszValue);
		}
		else if (attrName == "outputfolder"
			|| attrName == "folder")
		{
			s_GameSettings->m_DebugReportFolderLocation = MakeLower(attr.lpszValue);
		}
		else if (attrName == "file"
			|| attrName == "debugfile"
			|| attrName == "debugfilewoutextnetion"
			|| attrName == "debugfilewithoutextention")
		{
			s_GameSettings->m_DebugReportFileNameWithoutExtention = MakeLower(attr.lpszValue);
		}
		else if (attrName == "enabledebuglogging"
			|| attrName == "enablelogging")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_EnableDebugLogging = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_EnableDebugLogging = false;
			}
		}
		else if (attrName == "writegamesettingscontent")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_AllowWriteGameSettingsContent = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_AllowWriteGameSettingsContent = false;
			}
		}
		else if (attrName == "writegeneratorcontent")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_AllowWriteGeneratorContent = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_AllowWriteGeneratorContent = false;
			}
		}
		else if (attrName == "writeactivecontent")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_AllowWriteActiveContent = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_AllowWriteActiveContent = false;
			}
		}
		else if (attrName == "gamesettingsincludeplayerinfo")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->SetShouldIncludePlayerInfoWhenWriting(true);
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->SetShouldIncludePlayerInfoWhenWriting(false);
			}
		}
		else if (attrName == "gamesettingsincludeshaderinfo")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->SetShouldIncludeShaderInfoWhenWriting(true);
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->SetShouldIncludeShaderInfoWhenWriting(false);
			}
		}
		else if (attrName == "gamesettingsincludefiledirectoriesinfo")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->SetShouldIncludeFileDirectoriesInfoWhenWriting(true);
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->SetShouldIncludeFileDirectoriesInfoWhenWriting(false);
			}
		}
		else if (attrName == "gamesettingsincludeperframedurationsinfo"
			|| attrName == "gamesettingsincludeperframedurationinfo")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->SetShouldIncludePerFrameDurationInfoWhenWriting(true);
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->SetShouldIncludePerFrameDurationInfoWhenWriting(false);
			}
		}
		else if (attrName == "gamesettingsincludepreallocationsinfo")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->SetShouldIncludePreallocationsInfoWhenWriting(true);
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->SetShouldIncludePreallocationsInfoWhenWriting(false);
			}
		}
		else if (attrName == "writeagentgeneratorstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushAgentGeneratorContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushAgentGeneratorContentToDebugFile = false;
			}
		}
		else if (attrName == "writeagentstatstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushAgentStatsContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushAgentStatsContentToDebugFile = false;
			}
		}
		else if (attrName == "writedialoguegroupstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushDialogueGroupingContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushDialogueGroupingContentToDebugFile = false;
			}
		}
		else if (attrName == "writedialoguesegmentstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushDialogueSegmentContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushDialogueSegmentContentToDebugFile = false;
			}
		}
		else if (attrName == "writefeaturegeneratorstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushFeatureGeneratorContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushFeatureGeneratorContentToDebugFile = false;
			}
		}
		else if (attrName == "writeglobalvariablestodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushGlobalVariablesContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushGlobalVariablesContentToDebugFile = false;
			}
		}
		else if (attrName == "writehairgeneratorstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushHairGeneratorContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushHairGeneratorContentToDebugFile = false;
			}
		}
		else if (attrName == "writeitemgeneratorstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushItemGeneratorContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushItemGeneratorContentToDebugFile = false;
			}
		}
		else if (attrName == "writemapfilestodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushMapFileContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushMapFileContentToDebugFile = false;
			}
		}
		else if (attrName == "writenamegeneratorstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushNameGeneratorContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushNameGeneratorContentToDebugFile = false;
			}
		}
		else if (attrName == "writenpcattributestodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushNPCAttributeContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushNPCAttributeContentToDebugFile = false;
			}
		}
		else if (attrName == "writenpcjobstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushNPCJobContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushNPCJobContentToDebugFile = false;
			}
		}
		else if (attrName == "writequestgeneratorstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushQuestGeneratorsContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushQuestGeneratorsContentToDebugFile = false;
			}
		}
		else if (attrName == "writesoundstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushSoundContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushSoundContentToDebugFile = false;
			}
		}
		else if (attrName == "writespeechbubblestodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushSpeechBubbleContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushSpeechBubbleContentToDebugFile = false;
			}
		}
		else if (attrName == "writespriteanimationstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushSpriteAnimationContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushSpriteAnimationContentToDebugFile = false;
			}
		}
		else if (attrName == "writespriteresourcestodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushSpriteResourceContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushSpriteResourceContentToDebugFile = false;
			}
		}
		else if (attrName == "writetiledefinitionstodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushTileDefinitionContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushTileDefinitionContentToDebugFile = false;
			}
		}
		else if (attrName == "writeactivetilespermaptodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushTilesPerMapContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushTilesPerMapContentToDebugFile = false;
			}
		}
		else if (attrName == "writeactiveagentspermaptodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushAgentsPerMapContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushAgentsPerMapContentToDebugFile = false;
			}
		}
		else if (attrName == "writeactivefeaturespermaptodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushFeaturesPerMapContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushFeaturesPerMapContentToDebugFile = false;
			}
		}
		else if (attrName == "writeactivetileeventspermaptodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushTileEventsPerMapContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushTileEventsPerMapContentToDebugFile = false;
			}
		}
		else if (attrName == "writeactivequestspermaptodebugfile")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_PushActiveQuestContentToDebugFile = true;
			}
			else if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_PushActiveQuestContentToDebugFile = false;
			}
		}
	}

	if (overwriteSeed)
	{
		s_GameSettings->m_GameSeed = seed;
	}
}


STATIC void TheGame::ReadGameDataOffOfXMLNode(const XMLNode& node)
{
	ReadGameDataAttributesOffOfXMLNode(node);
	ReadGameDataNodesOffOfXMLNode(node);
}


STATIC void TheGame::ReadGameDataAttributesOffOfXMLNode(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "enforcemapsmusthavenotileswithnospriteresources")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "false"
				|| attrVal == "f")
			{
				Map::SetEnforceNoEmptySpriteResourcesAllowedOnMap(false);
			}
			else if (attrVal == "true"
				|| attrVal == "t")
			{
				Map::SetEnforceNoEmptySpriteResourcesAllowedOnMap(true);
			}
		}
		else if (attrName == "addthreedotsifthereismoretodialoguesegment")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "false"
				|| attrVal == "f")
			{
				s_GameSettings->m_AddThreeDotsAfterPushFullDialogue = false;
			}
			else if (attrVal == "true"
				|| attrVal == "t")
			{
				s_GameSettings->m_AddThreeDotsAfterPushFullDialogue = true;
			}
		}
		else if (attrName == "numberofcharactersperline")
		{
			s_GameSettings->m_numberOfCharactersPerLine = atoi(attr.lpszValue);
		}
	}
}

STATIC void TheGame::ReadGameDataNodesOffOfXMLNode(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "startingmap"
			|| childName == "map")
		{
			ReadGameDataOffOfXMLNodeMap(child);
		}
		else if (childName == "defaultshader"
			|| childName == "shader")
		{
			ReadTheGameDataOffOfXMLNodeShader(child);
		}
		else if (childName == "defaultmapshader"
			|| childName == "mapshader")
		{
			ReadTheGameDataOffOfXMLNodeMapShader(child);
		}
		else if (childName == "player")
		{
			ReadTheGameDataOffOfXMLNodePlayer(child);
		}
		else if (childName == "defaultclothes"
			|| childName == "clothes")
		{
			ReadTheGameDataOffOfXMLNodeClothes(child);
		}
		else if (childName == "font")
		{
			ReadTheGameDataOffOfXMLNodeFont(child);
		}
		else if (childName == "dialoguesegments"
			|| childName == "dialoguesegment")
		{
			ReadTheGameDataOffOfXMLNodeDialogueSegments(child);
		}
		else if (childName == "dialoguegroupings"
			|| childName == "dialoguegrouping")
		{
			ReadTheGameDataOffOfXMLNodeDialogueGroupings(child);
		}
		else if (childName == "agents"
			|| childName == "agentgenerators"
			|| childName == "characters"
			|| childName == "charactergenerators")
		{
			ReadTheGameDataOffOfXMLNodeAgentGenerators(child);
		}
		else if (childName == "features"
			|| childName == "featuregenerators")
		{
			ReadTheGameDataOffOfXMLNodeFeatureGenerators(child);
		}
		else if (childName == "agentstats")
		{
			ReadTheGameDataOffOfXMLNodeAgentStats(child);
		}
		else if (childName == "npcattributes")
		{
			ReadTheGameDataOffOfXMLNodeNPCAttributes(child);
		}
		else if (childName == "npcjobs")
		{
			ReadTheGameDataOffOfXMLNodeNPCJobs(child);
		}
		else if (childName == "speechbubble"
			|| childName == "speechbubbles")
		{
			ReadTheGameDataOffOfXMLNodeSpeechBubbles(child);
		}
		else if (childName == "hairgenerators")
		{
			ReadTheGameDataOffOfXMLNodeHairGenerators(child);
		}
		else if (childName == "namegenerators"
			|| childName == "namegenerator")
		{
			ReadTheGameDataOffOfXMLNodeNameGenerators(child);
		}
		else if (childName == "itemgenerators"
			|| childName == "items")
		{
			ReadTheGameDataOffOfXMLNodeItemGenerators(child);
		}
		else if (childName == "soundgenerators"
			|| childName == "soundregistrations"
			|| childName == "sounds")
		{
			ReadTheGameDataOffOfXMLNodeSoundGenerators(child);
		}
		else if (childName == "questregistrations"
			|| childName == "questgenerators"
			|| childName == "quests")
		{
			ReadTheGameDataOffOfXMLNodeQuestGenerators(child);
		}
		else if (childName == "mapfiles")
		{
			ReadTheGameDataOffOfXMLNodeMapFiles(child);
		}
		else if (childName == "spriteresources"
			|| childName == "spriteresource")
		{
			ReadTheGameDataOffOfXMLNodeSpriteResources(child);
		}
		else if (childName == "spriteanimations"
			|| childName == "spriteanimation")
		{
			ReadTheGameDataOffOfXMLNodeSpriteAnimations(child);
		}
		else if (childName == "tiledefinitions")
		{
			ReadTheGameDataOffOfXMLNodeTileDefinitions(child);
		}
		else if (childName == "globalvariables")
		{
			ReadTheGameDataOffOfXMLNodeGlobalVariables(child);
		}
		else if (childName == "preallocations")
		{
			ReadTheGameDataOffOfXMLNodePreallocations(child);
		}
		else if (childName == "amountoftimetotakeforloadingperframe"//AmountOfTimeToTakeForLoadingPerFrame
			|| childName == "loadperframe"
			|| childName == "timeperframe")
		{
			ReadTheGameDataOffOfXMLNodeLoadPerFrame(child);
		}
		else if (childName == "debug"
			|| childName == "debugging")
		{
			ReadTheGameDataOffOfXMLNodeDebugging(child);
		}
	}
}

STATIC GameSettings* TheGame::GetGameSettings()
{
	return s_GameSettings;
}