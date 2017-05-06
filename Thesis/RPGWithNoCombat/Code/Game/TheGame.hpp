#pragma once
#include "Engine/Core/Time.hpp"
#include "Engine/Core/Camera3D.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Debug/DebugRenderring.hpp"
#include "Engine/Font/Font.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Controllers/Controller.hpp"
#include "Game/GameSettings.hpp"

#ifndef THEGAME_HPP
#define THEGAME_HPP
//-----------------------------------------------------
class TheGame;

//-----------------------------------------------------
extern TheGame* g_TheGame;

class SoundManager;
class BaseGameEntity;
class Dialogue;
struct XMLNode;
//-----------------------------------------------
//Pos Y is Down Dir!

enum DataPresentlyLoading
{
	SoundData,
	SpritesAndAnimations,
	MapFileContents,
	DialoguePieces,
	NPCAttributes,
	HairGenerators,
	Names,
	Features,
	AgentStats,
	NPCJobs,
	Agents,
	Items,
	GlobalVariables,
	Maps,
	Amount_of_Data_to_load_before_can_start
};

class TheGame
{
protected:
	//Static Variables
	static constexpr char* s_CommonGameSettingsFolder = "Data/XML/GameSettings/";
	static constexpr char* s_CommonGameSettingsRoot = "GameSettings";
	static constexpr char* s_CommonSpriteResourcesFolder = "Data/XML/SpriteResources/";
	static constexpr char* s_CommonSpriteAnimationsFolder = "Data/XML/SpriteAnimations/";
	static GameSettings* s_GameSettings;

	//Variables
public:
	uint64_t m_timeTheGameWasCreated = 0;
protected:
	uint64_t m_timeJustBeforeTheGameWasDestroyed = 0;
	uint64_t m_frameStartTime = 0;
	Vector2 WindowSize = Vector2(16.f, 9.f);
	static const IntVector2 s_centralMousePosition;
	static const Vector2 s_mouseYawPitchSensitivity;
	std::map<std::string, Map*> m_allMaps;
	std::vector<std::vector<Vertex_PCT>> m_loadingMessageVertices;
	std::vector<std::vector<unsigned int>> m_loadingMessageIndices;
	std::vector<Material> m_loadingMessageTextMats;
	std::vector<Mesh> m_loadingMessageTextMeshes;
	std::vector<float> m_secondsTakenPerEachDataPieceLoaded;
	std::string m_LoadingMessageToScreen = "Reading Game Settings";
	std::string m_messageLastFrame = "";
public:
	std::string m_timeGameStarted = "";
	std::string m_timeGameEnded = "";
protected:
	Matrix44 m_Projection;
	Matrix44 m_View;
	Matrix44 m_DialogueProjection;
	Matrix44 m_DialogueView;
	Matrix44 m_generalMatrix;
	Vector2 m_tileViewRange;
	Vector2 m_halfTileViewRange;
	Vector2 m_lastPositionPlayerSeenAlive;
	float m_loadingMessageDialogueSize = 0.005f;
	float m_secondsSinceLastIncrementedPeriods = 0.f;
	float m_secondsBeforeIncrementPeriods = 0.5f;
	IntVector2 m_positionToSetThePlayerTo = IntVector2::ivec2_zeros;
	ShaderProgram* m_defaultProgram = nullptr;
	Dialogue* m_dialogue = nullptr;
	Map* m_map = nullptr;
	Font* m_font = nullptr;
	BaseGameEntity* m_ItemUpForDebug = nullptr;
	BaseGameEntity* m_PlayerPointer = nullptr;
	SoundManager* m_SoundManager = nullptr;
	DataPresentlyLoading m_dataLoadingMode = (DataPresentlyLoading)0;
	size_t m_maxNumberOfPeriods = 3;
	size_t m_numberOfPeriodsForLoadingMessage = 1;
	bool m_F1Down = false;
	bool m_updatePlayerPointer = true;
	bool m_quitNow;
	bool m_DebugIKeyIsDown = false;
	bool m_DebugKKeyIsDown = false;
	bool m_DataFullyLoaded = false;
	bool m_wasLoadingMapLastFrame = false;
	bool m_singleFramePassedSinceFinishedLoading = false;
	bool m_PrintOutContentNextFrame = false;
	bool m_PrintOutContentNextFrameFramePassedFirst = false;
	bool m_PrintOutToConsole = false;
	bool m_PrintOutToCommandLine = false;
public:
	//Constructors
	TheGame();
	~TheGame();
	void StepsForAfterConstruction();
protected:
	void OldStepsForAfterConstruction();
public:

	//Updates and Render
	void Update(const float& deltaSeconds, const bool& focus);
protected:
	void RegularUpdate(const float& deltaSeconds, const bool& focus);
	void LoadingUpdate(const float& deltaSeconds, const bool& focus);
	void UpdateLoadingMessage(const float& deltaSeconds);
	void HandleClosingDebugLogging(bool actuallyQuitting = true);
public:
	double WriteOutEndingData();
protected:
	void HandleClosingDebugLogging(std::string& str, bool actuallyQuitting = true);

	void PrepareViewAndProjectionMatrix();
public:
	void Render() const;
protected:
	void RegularRender() const;
	void LoadingRender() const;
	void ReadDataOffPlayer();
	void SpawnNPCAtRandomPosition(const std::string& npcEntityName);
	void SpawnRandomNPCAtRandomPosition();
	Vector2 GetRandomPositionOnMap() const;

public:
	//Utils
	void SetUp2DView() const;
	void DrawDebugAxisLines() const;

	//Setters
	void PrintOutDebugContentNextFrame(bool print, bool shouldPrintToCommandLine,
		bool shouldPrintToConsole);
	void SetUpdatePlayerPointer(bool updatePlayerPointer);
	void SetFont(Font* font);
	void SetPositionForPlayerOnChangeMap(const IntVector2& position);

	//Getters
	bool isQuitting() const;
	bool GetWasLoadingMapLastFrame() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;
	const Vector2& GetWindowSize() const;
	uint64_t GetFrameStartTime() const;

	//Static Functions
	static void SpawnGameSettings();
	static void ClearGameSettings();
	static void ReadInGameDataFromCommonXMLFolder();
protected:
	static void ReadGameDataOffOfXMLNodeMap(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeShader(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeMapShader(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodePlayer(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeClothes(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeFont(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeDialogueSegments(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeDialogueGroupings(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeAgentGenerators(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeFeatureGenerators(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeHairGenerators(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeNPCAttributes(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeAgentStats(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeNPCJobs(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeSpeechBubbles(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeNameGenerators(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeItemGenerators(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeSoundGenerators(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeQuestGenerators(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeMapFiles(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeSpriteResources(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeSpriteAnimations(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeTileDefinitions(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeGlobalVariables(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodePreallocations(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeLoadPerFrame(const XMLNode& child);
	static void ReadTheGameDataOffOfXMLNodeDebugging(const XMLNode& child);
public:
	static void ReadGameDataOffOfXMLNode(const XMLNode& node);
protected:
	static void ReadGameDataAttributesOffOfXMLNode(const XMLNode& node);
	static void ReadGameDataNodesOffOfXMLNode(const XMLNode& node);
public:
	static GameSettings* GetGameSettings();
};

#endif //THEGAME_HPP