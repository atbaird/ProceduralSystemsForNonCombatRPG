#pragma once

#ifndef GAMESETTINGS_HPP
#define GAMESETTINGS_HPP
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include <string>

#define TILE_COORDS IntVector2

struct GameSettings
{
protected:
	static constexpr int s_BitShifts_IncludePlayerInfo = 0;
	static constexpr int s_BitShifts_IncludeShaderInfo = 1;
	static constexpr int s_BitShifts_IncludeFileDirectoriesInfo = 2;
	static constexpr int s_BitShifts_IncludePerFrameDurationsInfo = 3;
	static constexpr int s_BitShifts_IncludePreallocationsInfo = 4;
	static int s_IncludePlayerInfoMask;
	static int s_IncludeShaderInfoMask;
	static int s_IncludeFileDirectoriesInfoMask;
	static int s_IncludePerFrameDurationsInfoMask;
	static int s_IncludePreallocationsInfoMask;

public:
	Vector2 m_selectionRenderSize = Vector2(2.4f, 0.8f);
	TILE_COORDS m_playerStartingTile = TILE_COORDS(10, 10);
	std::string m_defaultShaderVertFile = "Data/Shaders/SpritePassthrough.vert";
	std::string m_defaultShaderFragFile = "Data/Shaders/SpritePassthroughWithColorHandling.frag";
	std::string m_defaultConsoleShaderVertFile = "Data/Shaders/SpritePassthrough.vert";
	std::string m_defaultConsoleShaderFragFile = "Data/Shaders/SpritePassthrough.frag";
	std::string m_defaultMapShaderVertFile = "Data/Shaders/SpritePassthroughMultipleTextures.vert";
	std::string m_defaultMapShaderFragFile = "Data/Shaders/SpritePassthroughWithColorHandlingMultipleTextures.frag";
	std::string m_agentGeneratorToUseForSpawningPlayer = "Player";
	std::string m_defaultClothesForAllAgents = "Townsfolk 1 clothes";
	std::string m_startingMap = "default";
	std::string m_ConsoleWhiteBoxName = "WhiteBox";

	//Font File info
	std::string m_FontContainingFolder = "Data/Font/";
	std::string m_FontFNTFileName = "DebugFont.fnt";
	std::string m_consoleFontContainingFolder = "Data/Font/";
	std::string m_consoleFontFNTFileName = "DebugFont.fnt";

	//XML Folders
	std::string m_commonGlobalVariablesFileDirectory = "Data/XML/GlobalVariables/";
	std::string m_commonNameGeneratorsFileDirection = "Data/XML/NameGenerators/"; 
	std::string m_commonNPCAttributeContainerFileLocation = "Data/XML/NPCAttributes/";
	std::string m_commonSpeechBubbleXMLFolder = "Data/XML/SpeechBubbles/";
	std::string m_commonDialogueSegmentsXMLFolder = "Data/XML/DialogueSegments/";
	std::string m_commonDialogueGroupingsXMLFolder = "Data/XML/DialogueGroupings/";
	std::string m_commonAgentGeneratorFileDirectory = "Data/XML/AgentGenerators/";
	std::string m_commonFeatureGeneratorFileDirectory = "Data/XML/FeatureGenerators/";
	std::string m_commonHairGeneratorsFolder = "Data/XML/HairGenerators/";
	std::string m_commonItemGeneratorsFolder = "Data/XML/ItemGenerators/";
	std::string m_soundCommonFolder = "Data/XML/Sound/";
	std::string m_commonQuestXMLFolder = "Data/XML/Quests/";
	std::string m_mapFilesCommonXMLFolder = "Data/XML/MapFiles/";
	std::string m_tileDefinitionXMLFolderLocaction = "Data/XML/TileDefinitions/";
	std::string m_CommonAgentStatFolder = "Data/XML/AgentStats/";
	std::string m_CommonNPCJobFileDirectory = "Data/XML/NPCJobs/";
	std::string m_commonSpriteResourcesFileDirectory = "Data/XML/SpriteResources/";
	std::string m_commonSpriteAnimationsFileDirectory = "Data/XML/SpriteAnimations/";

	std::string m_DebugReportFolderLocation = "Data/";
	std::string m_DebugReportFileNameWithoutExtention = "debug";

	//XML Root Node Names
	std::string m_dialogueSegmentsRootXML = "DialogueSegments";
	std::string m_dialogueGroupingsRootXML = "DialogueGroupings";
	std::string m_agentGeneratorsRootNodeName = "AgentGenerators";
	std::string m_featureGeneratorsRootNodeName = "FeatureGenerators";
	std::string m_commonHairRootNodeName = "HairGenerators";
	std::string m_commonItemRootNodeName = "ItemGenerators";
	std::string m_soundCommonRootXMLNode = "Sounds";
	std::string m_commonQuestXMLRoot = "Quests";
	std::string m_mapFilesRootNodeName = "MapFiles";
	std::string m_tileDefinitionsXMLRootName = "TileDefinitions";

	//XML Node Names
	std::string m_agentGeneratorsNodeName = "AgentGenerator";
	std::string m_featureGeneratorsNodeName = "FeatureGenerator";
	std::string m_commonHairNodeName = "HairGenerator";
	std::string m_commonItemNodeName = "ItemGenerator";
	std::string m_soundCommonSoundXMLNode = "sound";
	std::string m_altSoundCommonSoundXMLNode = "soundeffect";
	std::string m_soundCommonMusicXMLNode = "music";
	std::string m_altSoundCommonMusicXMLNode = "soundtrack";
	std::string m_tileDefinitionXMLNodeName = "TileDefinition";


	//Time per frame per nodes to read in.
	double m_amountOfTimeLoadingGlobalVariablesPerFrame = 0.25;
	double m_amountOfTimeToSpendLoadingSpriteResources = 0.25;
	double m_amountOfTimeToSpendLoadingSpriteAnimations = 0.25;
	double m_amountOfTimeToSpendLoadingSoundsPerFrame = 0.25;
	double m_amountOfTimeToSpendLoadingMapFilesPerFrame = 0.25;
	double m_amountOfTimeLoadingQuestsPerFrame = 0.25;
	double m_amountOfTimeLoadingTileDefinitionsPerFrame = 0.25;
	double m_amountOfTimeLoadingSpeechBubblesPerFrame = 0.25;
	double m_amountOfTimeLoadingNPCAttributesPerFrame = 0.25;
	double m_amountOfTimeLoadingHairGeneratorsPerFrame = 0.25;
	double m_amountOfTimeLoadingNameGneratorsPerFrame = 0.25;
	double m_amountOfTimeLoadingFeatureGeneratorsPerFrame = 0.25;
	double m_amountOfTimeLoadingAgentStatsPerFrame = 0.25;
	double m_amountOfTimeLoadingNPCJobsPerFrame = 0.25;
	double m_amountOfTimeLoadingAgentGeneratorsPerFrame = 0.25;
	double m_amountOfTimeLoadingItemGeneratorsPerFrame = 0.25;
	double m_amountOfTimeLoadingMapDataPerFrame = 0.25;
	double m_amountOfTimeLoadingDialogueGroupingsPerFrame = 0.25;
	double m_amountOfTimeLoadingDialogueSegmentsPerFrame = 0.25;

	float m_ConsoleFontSize = 0.005f;

	//Preallocation Amounts
	unsigned int m_GameSeed = 0;
	unsigned int m_maxNumberOfConsoleLines = 15;
	int m_numberOfCharactersPerLine = 35;
	unsigned int m_preallocationAmountSpriteResources = 50;
	unsigned int m_preallocationAmountSpriteAnimations = 50;
	unsigned int m_preallocationAmountAgentGenerators = 50;
	unsigned int m_preallocationAmountFeatureGenerators = 50;
	unsigned int m_preallocationAmountItemGenerators = 50;
	unsigned int m_preallocationAmountHairGenerators = 50;
	unsigned int m_preallocationAmountSoundRegistrations = 50;
	unsigned int m_preallocationAmountSoundEffectRegistrations = 50;
	unsigned int m_preallocationAmountMusicRegistrations = 50;
	unsigned int m_preallocationAmountDialogueSegments = 50;
	unsigned int m_preallocationAmountQuestGenerators = 50;
	unsigned int m_preallocationAmountQuestsPerMap = 20;
	unsigned int m_preallocationAmountTileDefinitions = 20;
	unsigned int m_preallocationAmountAgentsPerMap = 20;
	unsigned int m_preallocationAmountFeaturesPerMap = 20;

	//Masks for Debugging
	int m_GameSettingsDebugFileMask = 
		s_IncludePlayerInfoMask
		| s_IncludeShaderInfoMask
		| s_IncludeFileDirectoriesInfoMask
		| s_IncludePerFrameDurationsInfoMask
		| s_IncludePreallocationsInfoMask;
	bool m_EnableDebugLogging = true;
	bool m_AllowWriteGameSettingsContent = true;
	bool m_AllowWriteGeneratorContent = true;
	bool m_AllowWriteActiveContent = true;

	bool m_PushAgentGeneratorContentToDebugFile = true;
	bool m_PushAgentStatsContentToDebugFile = true;
	bool m_PushDialogueGroupingContentToDebugFile = true;
	bool m_PushDialogueSegmentContentToDebugFile = true;
	bool m_PushFeatureGeneratorContentToDebugFile = true;
	bool m_PushGlobalVariablesContentToDebugFile = true;
	bool m_PushHairGeneratorContentToDebugFile = true;
	bool m_PushItemGeneratorContentToDebugFile = true;
	bool m_PushMapFileContentToDebugFile = true;
	bool m_PushNameGeneratorContentToDebugFile = true;
	bool m_PushNPCAttributeContentToDebugFile = true;
	bool m_PushNPCJobContentToDebugFile = true;
	bool m_PushQuestGeneratorsContentToDebugFile = true;
	bool m_PushSoundContentToDebugFile = true;
	bool m_PushSpeechBubbleContentToDebugFile = true;
	bool m_PushSpriteAnimationContentToDebugFile = true;
	bool m_PushSpriteResourceContentToDebugFile = true;
	bool m_PushTileDefinitionContentToDebugFile = true;
	bool m_PushTilesPerMapContentToDebugFile = true;
	bool m_PushAgentsPerMapContentToDebugFile = true;
	bool m_PushFeaturesPerMapContentToDebugFile = true;
	bool m_PushTileEventsPerMapContentToDebugFile = true;
	bool m_PushActiveQuestContentToDebugFile = true;
	bool m_SpawnConsole = true;
	bool m_EnableDeveloperDebugMode = true;

	//dialogue
	bool m_AddThreeDotsAfterPushFullDialogue = true;
	
public:
	//Constructors
	GameSettings();
	virtual ~GameSettings();

	//Setters
	void SetShouldIncludePlayerInfoWhenWriting(bool include);
	void SetShouldIncludeShaderInfoWhenWriting(bool include);
	void SetShouldIncludeFileDirectoriesInfoWhenWriting(bool include);
	void SetShouldIncludePerFrameDurationInfoWhenWriting(bool include);
	void SetShouldIncludePreallocationsInfoWhenWriting(bool include);

	//Getters
	bool GetIfShouldIncludePlayerInfoWhenWriting() const;
	bool GetIfShouldIncludeShaderInfoWhenWriting() const;
	bool GetIfShouldIncludeFileDirectoriesInfoWhenWriting() const;
	bool GetIfShouldIncludePerFrameDurationsWhenWriting() const;
	bool GetIfShouldIncludePreallocationsInfoWhenWriting() const;

	//Operations
	void WriteCurrentSettingsToString(std::string& str) const;
	void WriteDataOutToDebugFile(const std::string& timeGameStarted, const std::string& timeGameEnded, 
		double durationPlayTime, bool actuallyQuitting);
	void WriteDataOutToDebugFile(const std::string& timeGameStarted, const std::string& timeGameEnded,
		double durationPlayTime, bool actuallyQuitting, std::string& str);
	void WriteDataOutToDebugFile(const std::string& timeGameStarted, const std::string& timeGameEnded,
		double durationPlayTime, bool actuallyQuitting, std::string& str, bool WriteGameSettings,
		bool WriteGeneratorContent, bool WriteActiveContent);

	void WriteStringOutToDebugFile(const std::string& out);

	void WriteDataOutToString(const std::string& timeGameStarted, const std::string& timeGameEnded,
		double durationPlayTime, bool actuallyQuitting, std::string& str);
	void WriteDataOutToString(const std::string& timeGameStarted, const std::string& timeGameEnded,
		double durationPlayTime, bool actuallyQuitting, std::string& str, bool WriteGameSettings,
		bool WriteGeneratorContent, bool WriteActiveContent);
	void WriteDataHeaderOutToString(const std::string& timeGameStarted, const std::string& timeGameEnded,
		double durationPlayTime, bool actuallyQuitting, std::string& str);

	void WriteDataFoldersToString(std::string& str, bool overrideShouldWrite = false);
protected:
	void WritePlayerInformationToString(std::string& str);
	void WriteShaderInformationToString(std::string& str);
	void WriteLoadingPerFrameDurationsToString(std::string& str);
	void WriteFileDirectoriesToString(std::string& str);
	void WritePreallocationAmountsToString(std::string& str);

public:
	void WriteGeneratorContentToString(std::string& str, bool overwrideShouldWrite = false);
protected:
	void WriteAgentGeneratorsToDebugFile(std::string& str);
	void WriteAgentStatsToDebugFile(std::string& str);
	void WriteDialogueGroupingsToDebugFile(std::string& str);
	void WriteDialogueSegmentsToDebugFile(std::string& str);
	void WriteFeatureGeneratorsToDebugFile(std::string& str);
	void WriteGlobalVariablesToDebugFile(std::string& str);
	void WriteHairGeneratorContentToDebugFile(std::string& str);
	void WriteItemGeneratorContentToDebugFile(std::string& str);
	void WriteMapFileContentToDebugFile(std::string& str);
	void WriteNameGeneratorContentToDebugFile(std::string& str);
	void WriteNPCAttributeContentToDebugFile(std::string& str);
	void WriteNPCJobContentToDebugFile(std::string& str);
	void WriteQuestContentToDebugFile(std::string& str);
	void WriteSoundContentToDebugFile(std::string& str);
	void WriteSpeechBubbleContentToDebugFile(std::string& str);
	void WriteSpriteAnimationContentToDebugFile(std::string& str);
	void WriteSpriteResourceContentToDebugFile(std::string& str);
	void WriteTileDefinitionContentToDebugFile(std::string& str);
public:
	void WriteActiveContentToString(std::string& str, bool overrideShouldWrite = false);
protected:
};

#endif