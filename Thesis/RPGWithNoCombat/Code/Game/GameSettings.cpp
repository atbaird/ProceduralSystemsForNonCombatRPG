#include "Engine/Math/MathToStringUtils.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Game/GameSettings.hpp"
#include "Game/Debugging/DebuggingFileHandler.hpp"
#include "Game/TheGame.hpp"
#include "Game/GameEntities/Agents/AgentGenerator.hpp"
#include "Game/GameEntities/Hair/HairGenerator.hpp"
#include "Game/GameEntities/Agents/AgentStats/AgentStat.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttributes/NPCAttributeContainer.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCJobs/NPCJob.hpp"
#include "Game/GameEntities/Items/ItemGenerator.hpp"
#include "Game/GameEntities/Features/FeatureGenerator.hpp"
#include "Game/GameEntities/Agents/Names/NameGenerator.hpp"
#include "Game/Dialogue/DialogueSegment.hpp"
#include "Game/SoundManager/SoundRegistration.hpp"
#include "Game/Map/MapFiles/MapFiles.hpp"
#include "Game/Quest/QuestGenerator.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Static Functions
STATIC int GameSettings::s_IncludePlayerInfoMask = 1 << s_BitShifts_IncludePlayerInfo;
STATIC int GameSettings::s_IncludeShaderInfoMask = 1 << s_BitShifts_IncludeShaderInfo;
STATIC int GameSettings::s_IncludeFileDirectoriesInfoMask = 1 << s_BitShifts_IncludeFileDirectoriesInfo;
STATIC int GameSettings::s_IncludePerFrameDurationsInfoMask = 1 << s_BitShifts_IncludePerFrameDurationsInfo;
STATIC int GameSettings::s_IncludePreallocationsInfoMask = 1 << s_BitShifts_IncludePreallocationsInfo;

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Constructors
GameSettings::GameSettings()
{

}

GameSettings::~GameSettings()
{

}


//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Setters
void GameSettings::SetShouldIncludePlayerInfoWhenWriting(bool include)
{
	m_GameSettingsDebugFileMask &= ~s_IncludePlayerInfoMask;
	if (include == true)
	{
		m_GameSettingsDebugFileMask |= s_IncludePlayerInfoMask;
	}
}

void GameSettings::SetShouldIncludeShaderInfoWhenWriting(bool include)
{
	m_GameSettingsDebugFileMask &= ~s_IncludeShaderInfoMask;
	if (include == true)
	{
		m_GameSettingsDebugFileMask |= s_IncludeShaderInfoMask;
	}
}

void GameSettings::SetShouldIncludeFileDirectoriesInfoWhenWriting(bool include)
{
	m_GameSettingsDebugFileMask &= ~s_IncludeFileDirectoriesInfoMask;
	if (include == true)
	{
		m_GameSettingsDebugFileMask |= s_IncludeFileDirectoriesInfoMask;
	}
}

void GameSettings::SetShouldIncludePerFrameDurationInfoWhenWriting(bool include)
{
	m_GameSettingsDebugFileMask &= ~s_IncludePerFrameDurationsInfoMask;
	if (include == true)
	{
		m_GameSettingsDebugFileMask |= s_IncludePerFrameDurationsInfoMask;
	}
}

void GameSettings::SetShouldIncludePreallocationsInfoWhenWriting(bool include)
{
	m_GameSettingsDebugFileMask &= ~s_IncludePreallocationsInfoMask;
	if (include == true)
	{
		m_GameSettingsDebugFileMask |= s_IncludePreallocationsInfoMask;
	}
}


//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Getters
bool GameSettings::GetIfShouldIncludePlayerInfoWhenWriting() const
{
	return (m_GameSettingsDebugFileMask & s_IncludePlayerInfoMask) != 0;
}

bool GameSettings::GetIfShouldIncludeShaderInfoWhenWriting() const
{
	return (m_GameSettingsDebugFileMask & s_IncludeShaderInfoMask) != 0;
}

bool GameSettings::GetIfShouldIncludeFileDirectoriesInfoWhenWriting() const
{
	return (m_GameSettingsDebugFileMask & s_IncludeFileDirectoriesInfoMask) != 0;
}

bool GameSettings::GetIfShouldIncludePerFrameDurationsWhenWriting() const
{
	return (m_GameSettingsDebugFileMask & s_IncludePerFrameDurationsInfoMask) != 0;
}

bool GameSettings::GetIfShouldIncludePreallocationsInfoWhenWriting() const
{
	return (m_GameSettingsDebugFileMask & s_IncludePreallocationsInfoMask) != 0;
}

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Operations
void GameSettings::WriteCurrentSettingsToString(std::string& str) const
{
	str += "-----------------------\n";
	str += "Debug Log Settings: \n";
	str += "-----------------------\n";
	str += "Enable Debug Logging when Quit: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_EnableDebugLogging);
	str += "Allow Game Settings Content to be written to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_AllowWriteGameSettingsContent);
	str += "Allow Generator Data to be written to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_AllowWriteGeneratorContent);
	str += "Allow Write Active Content to be written to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_AllowWriteActiveContent);
	str += "-------\n";
	str += "Game Settings Content.\n";
	str += "Include Player Spawn Info: ";
	GameDebuggingCommons::WriteBoolOntoString(str, GetIfShouldIncludePlayerInfoWhenWriting());
	str += "Include Shader Info: ";
	GameDebuggingCommons::WriteBoolOntoString(str, GetIfShouldIncludeShaderInfoWhenWriting());
	str += "Include File Directory Info: ";
	GameDebuggingCommons::WriteBoolOntoString(str, GetIfShouldIncludeFileDirectoriesInfoWhenWriting());
	str += "Include Per Frame Durations for Loading: ";
	GameDebuggingCommons::WriteBoolOntoString(str, GetIfShouldIncludePerFrameDurationsWhenWriting());
	str += "Include Preallocation Info: ";
	GameDebuggingCommons::WriteBoolOntoString(str, GetIfShouldIncludePreallocationsInfoWhenWriting());
	str += "-------\n";
	str += "Generator Content.\n";
	str += "Write Agent Generator Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushAgentGeneratorContentToDebugFile);
	str += "Write Agent Stat Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushAgentStatsContentToDebugFile);
	str += "Write Dialogue Group Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushDialogueGroupingContentToDebugFile);
	str += "Write Dialogue Segment Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushDialogueSegmentContentToDebugFile);
	str += "Write Feature Generator Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushFeatureGeneratorContentToDebugFile);
	str += "Write Hair Generator Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushHairGeneratorContentToDebugFile);
	str += "Write Item Generator Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushItemGeneratorContentToDebugFile);
	str += "Write Map File Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushMapFileContentToDebugFile);
	str += "Write Name Generator Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushNameGeneratorContentToDebugFile);
	str += "Write NPC Attribute Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushNPCAttributeContentToDebugFile);
	str += "Write NPC Job Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushNPCJobContentToDebugFile);
	str += "Write Quest Generator Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushQuestGeneratorsContentToDebugFile);
	str += "Write Sound Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushSoundContentToDebugFile);
	str += "Write Speech Bubble Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushSpeechBubbleContentToDebugFile);
	str += "Write Sprite Animation Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushSpriteAnimationContentToDebugFile);
	str += "Write Sprite Resource Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushSpriteResourceContentToDebugFile);
	str += "Write Tile Definition Content to Debug Log: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushTileDefinitionContentToDebugFile);
	str += "-------\n";
	str += "Active Content.\n";
	str += "Write Global Variables: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushGlobalVariablesContentToDebugFile);
	str += "Write each Tile Event from each Map: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushTileEventsPerMapContentToDebugFile);
	str += "Write each Tile from each Map: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushTilesPerMapContentToDebugFile);
	str += "Write each Agent from each Map: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushAgentsPerMapContentToDebugFile);
	str += "Write each Feature from each Map: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushFeaturesPerMapContentToDebugFile);
	str += "Write each Active Quest from each Map and the Player: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_PushActiveQuestContentToDebugFile);
	str += "-----------------------\n";

}

void GameSettings::WriteDataOutToDebugFile(const std::string& timeGameStarted, const std::string& timeGameEnded,
	double durationPlayTime, bool actuallyQuitting)
{
	std::string str = "";
	WriteDataOutToDebugFile(timeGameStarted, timeGameEnded,
		durationPlayTime, actuallyQuitting, str);
}

void GameSettings::WriteDataOutToDebugFile(const std::string& timeGameStarted,
	const std::string& timeGameEnded,
	double durationPlayTime, bool actuallyQuitting, std::string& str)
{
	WriteDataOutToString(timeGameStarted, timeGameEnded, durationPlayTime, actuallyQuitting, str);

	DebuggingFileHandler fileHandler;
	fileHandler.Open(m_DebugReportFolderLocation.c_str(),
		m_DebugReportFileNameWithoutExtention.c_str());

	fileHandler.Write(str);
	fileHandler.Close();
}

void GameSettings::WriteDataOutToDebugFile(const std::string& timeGameStarted, 
	const std::string& timeGameEnded, double durationPlayTime, bool actuallyQuitting, std::string& str, 
	bool WriteGameSettings,	bool WriteGeneratorContent, bool WriteActiveContent)
{
	WriteDataOutToString(timeGameStarted, timeGameEnded, durationPlayTime, actuallyQuitting, str,
		WriteGameSettings, WriteGeneratorContent, WriteActiveContent);
	WriteStringOutToDebugFile(str);
}

void GameSettings::WriteStringOutToDebugFile(const std::string& out)
{
	DebuggingFileHandler fileHandler;
	fileHandler.Open(m_DebugReportFolderLocation.c_str(),
		m_DebugReportFileNameWithoutExtention.c_str());

	fileHandler.Write(out);
	fileHandler.Close();
}

void GameSettings::WriteDataOutToString(const std::string& timeGameStarted, const std::string& timeGameEnded,
	double durationPlayTime, bool actuallyQuitting, std::string& str)
{
	WriteDataHeaderOutToString(timeGameStarted, timeGameEnded, durationPlayTime, actuallyQuitting, str);
	WriteDataFoldersToString(str);
	WriteGeneratorContentToString(str);
	WriteActiveContentToString(str);
}

void GameSettings::WriteDataOutToString(const std::string& timeGameStarted, const std::string& timeGameEnded,
	double durationPlayTime, bool actuallyQuitting, std::string& str, bool WriteGameSettings,
	bool WriteGeneratorContent, bool WriteActiveContent)
{
	WriteDataHeaderOutToString(timeGameStarted, timeGameEnded, durationPlayTime, actuallyQuitting, str);
	if (WriteGameSettings)
	{
		WriteDataFoldersToString(str, WriteGameSettings);
	}
	if (WriteGeneratorContent)
	{
		WriteGeneratorContentToString(str, WriteGeneratorContent);
	}
	if (WriteActiveContent)
	{
		WriteActiveContentToString(str, WriteActiveContent);
	}
}

void GameSettings::WriteDataHeaderOutToString(const std::string& timeGameStarted, 
	const std::string& timeGameEnded,
	double durationPlayTime, bool actuallyQuitting, std::string& str)
{
	str = "-------------------------------------";
	str += "\nGame Start Time: " + timeGameStarted;
	if (actuallyQuitting)
	{
		str += "\nGame End Time: ";
	}
	else
	{
		str += "\nCurrent Game Time: ";
	}
	str += timeGameEnded;
	str += "\nGame Duration: " + std::to_string(durationPlayTime);
	str += "\nGame Seed: " + std::to_string(m_GameSeed);
}


void GameSettings::WriteDataFoldersToString(std::string& str, bool overrideShouldWrite)
{
	if (!overrideShouldWrite && !m_AllowWriteGameSettingsContent)
	{
		return;
	}

	str += "\n-------------------------------------";
	str += "\nGame Settings";
	str += "\n----------------------";
	if (GetIfShouldIncludePlayerInfoWhenWriting())
	{
		str += "\n------------------";
		WritePlayerInformationToString(str);
	}
	if (GetIfShouldIncludeShaderInfoWhenWriting())
	{
		str += "\n------------------";
		WriteShaderInformationToString(str);
	}
	if (GetIfShouldIncludeFileDirectoriesInfoWhenWriting())
	{
		str += "\n------------------";
		WriteFileDirectoriesToString(str);
	}
	if (GetIfShouldIncludePerFrameDurationsWhenWriting())
	{
		str += "\n------------------";
		WriteLoadingPerFrameDurationsToString(str);
	}
	if (GetIfShouldIncludePreallocationsInfoWhenWriting())
	{
		str += "\n------------------";
		WritePreallocationAmountsToString(str);
	}
	str += "\n----------------------";
}


void GameSettings::WritePlayerInformationToString(std::string& str)
{
	str += "\nStarting Game Player information";
	str += "\n--------";
	str += "Player Agent Generator: " + (m_agentGeneratorToUseForSpawningPlayer) + "\n";
	str += "Starting Tile position: " + MathToStringUtils::ToString(m_playerStartingTile) + "\n";
	str += "Starting Map Name: " + m_startingMap + "\n";
	str += "Default Clothing for Agents: " + m_defaultClothesForAllAgents + "\n";
	str += "\n--------";
}

void GameSettings::WriteShaderInformationToString(std::string& str)
{
	str += "\nShader information";
	str += "\n--------";
	str += "\nShader Vert File: " + m_defaultShaderVertFile;
	str += "\nShader Frag File: " + m_defaultShaderFragFile;
	str += "\nMap Shader Vert File: " + m_defaultMapShaderVertFile;
	str += "\nMap Shader Frag File: " + m_defaultMapShaderFragFile;
	str += "\nFont folder: " + m_FontContainingFolder;
	str += "\nFont FNT File: " + m_FontFNTFileName;
	str += "\n--------";
}

void GameSettings::WriteLoadingPerFrameDurationsToString(std::string& str)
{
	str += "\nDuration per frame used for loading";
	str += "\n--------";
	str += "\nSeconds To Allow per Frame For Loading Agent Generators: " + 
		std::to_string(m_amountOfTimeLoadingAgentGeneratorsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Agent Stats: " +
		std::to_string(m_amountOfTimeLoadingAgentStatsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Dialogue Groupings: " +
		std::to_string(m_amountOfTimeLoadingDialogueGroupingsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Dialogue Segments: " +
		std::to_string(m_amountOfTimeLoadingDialogueSegmentsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Feature Generators: " +
		std::to_string(m_amountOfTimeLoadingFeatureGeneratorsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Global Variables: " +
		std::to_string(m_amountOfTimeLoadingGlobalVariablesPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Hair Generators: " +
		std::to_string(m_amountOfTimeLoadingHairGeneratorsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Item Generators: " +
		std::to_string(m_amountOfTimeLoadingItemGeneratorsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Map Files: " +
		std::to_string(m_amountOfTimeToSpendLoadingMapFilesPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Map Data: " +
		std::to_string(m_amountOfTimeLoadingMapDataPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Name Generators: " +
		std::to_string(m_amountOfTimeLoadingNameGneratorsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading NPC Attributes: " +
		std::to_string(m_amountOfTimeLoadingNPCAttributesPerFrame);
	str += "\nSeconds To Allow per Frame For Loading NPC Jobs: " +
		std::to_string(m_amountOfTimeLoadingNPCJobsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Quests: " +
		std::to_string(m_amountOfTimeLoadingQuestsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Speech Bubbles: " +
		std::to_string(m_amountOfTimeLoadingSpeechBubblesPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Sounds: " +
		std::to_string(m_amountOfTimeToSpendLoadingSoundsPerFrame);
	str += "\nSeconds To Allow per Frame For Loading Sprite Animations: " +
		std::to_string(m_amountOfTimeToSpendLoadingSpriteAnimations);
	str += "\nSeconds To Allow per Frame For Loading Sprite Resources: " +
		std::to_string(m_amountOfTimeToSpendLoadingSpriteResources);
	str += "\nSeconds To Allow per Frame For Loading Tile Definitions: " +
		std::to_string(m_amountOfTimeLoadingTileDefinitionsPerFrame);
	str += "\n--------";
}

void GameSettings::WriteFileDirectoriesToString(std::string& str)
{
	str += "\nData file directories";
	str += "\n--------";
	str += "\nAgent Generators Data Folder: " + m_commonAgentGeneratorFileDirectory;
	str += "\nAgent Stats Data Folder: " + m_CommonAgentStatFolder;
	str += "\nDialogue Groupings Data Folder: " + m_commonDialogueGroupingsXMLFolder;
	str += "\nDialogue Segments Data Folder: " + m_commonDialogueSegmentsXMLFolder;
	str += "\nFeature Generators Data Folder: " + m_commonFeatureGeneratorFileDirectory;
	str += "\nGlobal Variables Data Folder: " + m_commonGlobalVariablesFileDirectory;
	str += "\nHair Generators Data Folder: " + m_commonHairGeneratorsFolder;
	str += "\nItem Generators Data Folder: " + m_commonItemGeneratorsFolder;
	str += "\nMap Files Data Folder: " + m_mapFilesCommonXMLFolder;
	str += "\nName Generators Data Folder: " + m_commonNameGeneratorsFileDirection;
	str += "\nNPC Attributes Data Folder: " + m_commonNPCAttributeContainerFileLocation;
	str += "\nNPC Jobs Data Folder: " + m_CommonNPCJobFileDirectory;
	str += "\nQuest Data Folder: " + m_commonQuestXMLFolder;
	str += "\nSound Data Folder: " + m_soundCommonFolder;
	str += "\nSpeech Bubbles Data Folder: " + m_commonSpeechBubbleXMLFolder;
	str += "\nSprite Animations Data Folder: " + m_commonSpriteAnimationsFileDirectory;
	str += "\nSprite Resources Data Folder: " + m_commonSpriteResourcesFileDirectory;
	str += "\nTile Definitions Data Folder: " + m_tileDefinitionXMLFolderLocaction;
	str += "\n--------";
}

void GameSettings::WritePreallocationAmountsToString(std::string& str)
{
	str += "\nPreallocations";
	str += "\n--------";
	str += "\nPrealloactions for Agent Generators: " + std::to_string(m_preallocationAmountAgentGenerators);
	str += "\nPrealloactions for Agents per Map: " + std::to_string(m_preallocationAmountAgentsPerMap);
	str += "\nPrealloactions for Dialogue Segments: " + std::to_string(m_preallocationAmountDialogueSegments);
	str += "\nPrealloactions for Feature Generators: " + std::to_string(m_preallocationAmountFeatureGenerators);
	str += "\nPrealloactions for Features per Map: " + std::to_string(m_preallocationAmountFeaturesPerMap);
	str += "\nPrealloactions for Hair Generators: " + std::to_string(m_preallocationAmountHairGenerators);
	str += "\nPrealloactions for Item Generators: " + std::to_string(m_preallocationAmountItemGenerators);
	str += "\nPrealloactions for Music: " + std::to_string(m_preallocationAmountMusicRegistrations);
	str += "\nPrealloactions for Quests: " + std::to_string(m_preallocationAmountQuestGenerators);
	str += "\nPrealloactions for Quests per Map: " + std::to_string(m_preallocationAmountQuestsPerMap);
	str += "\nPrealloactions for Sounds: " + std::to_string(m_preallocationAmountSoundRegistrations);
	str += "\nPrealloactions for Sound Effects: " + std::to_string(m_preallocationAmountSoundEffectRegistrations);
	str += "\nPrealloactions for Sprite Animations: " + std::to_string(m_preallocationAmountSpriteAnimations);
	str += "\nPrealloactions for Sprite Resources: " + std::to_string(m_preallocationAmountSpriteResources);
	str += "\nPrealloactions for Tile Definitions: " + std::to_string(m_preallocationAmountTileDefinitions);

	str += "\n--------";
}


void GameSettings::WriteGeneratorContentToString(std::string& str, bool overrideShouldWrite)
{
	if (!overrideShouldWrite && !m_AllowWriteGeneratorContent)
	{
		return;
	}
	str += "\n-------------------------------------";
	str += "\nGenerator and Resource Content";
	str += "\n----------------------";
	WriteAgentGeneratorsToDebugFile(str);
	WriteAgentStatsToDebugFile(str);
	WriteDialogueGroupingsToDebugFile(str);
	WriteDialogueSegmentsToDebugFile(str);
	WriteFeatureGeneratorsToDebugFile(str);
	WriteHairGeneratorContentToDebugFile(str);
	WriteItemGeneratorContentToDebugFile(str);
	WriteMapFileContentToDebugFile(str);
	WriteNameGeneratorContentToDebugFile(str);
	WriteNPCAttributeContentToDebugFile(str);
	WriteNPCJobContentToDebugFile(str);
	WriteQuestContentToDebugFile(str);
	WriteSoundContentToDebugFile(str);
	WriteSpeechBubbleContentToDebugFile(str);
	WriteSpriteAnimationContentToDebugFile(str);
	WriteSpriteResourceContentToDebugFile(str);
	WriteTileDefinitionContentToDebugFile(str);
	str += "\n----------------------";
}



void GameSettings::WriteAgentGeneratorsToDebugFile(std::string& str)
{
	if (!m_PushAgentGeneratorContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nAgent Generator Data";
	str += "\n--------";
	AgentGenerator::WriteAllAgentGeneratorsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteAgentStatsToDebugFile(std::string& str)
{
	if (!m_PushAgentStatsContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nAgent Stat Data";
	str += "\n--------";
	AgentStat::WriteAllAgentStatsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteDialogueGroupingsToDebugFile(std::string& str)
{
	if (!m_PushDialogueGroupingContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nDialogue Groups";
	str += "\n--------";
	DialogueSegment::WriteAllDialogueGroupsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteDialogueSegmentsToDebugFile(std::string& str)
{
	if (!m_PushDialogueSegmentContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nDialogue Segments";
	str += "\n--------";
	DialogueSegment::WriteAllDialogueSegmentsToString(str, 0);
	str += "\n--------";
}


void GameSettings::WriteFeatureGeneratorsToDebugFile(std::string& str)
{
	if (!m_PushFeatureGeneratorContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nFeature Generators";
	str += "\n--------";
	FeatureGenerator::WriteAllFeatureGeneratorsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteGlobalVariablesToDebugFile(std::string& str)
{
	if (!m_PushGlobalVariablesContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nGlobal Variables";
	str += "\n--------";
	Map::WriteGlobalVariablesToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteHairGeneratorContentToDebugFile(std::string& str)
{
	if (!m_PushHairGeneratorContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nHair Generators";
	str += "\n--------";
	HairGenerator::WriteAllHairGeneratorsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteItemGeneratorContentToDebugFile(std::string& str)
{
	if (!m_PushItemGeneratorContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nItem Generators";
	str += "\n--------";
	ItemGenerator::WriteAllItemGeneratorsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteMapFileContentToDebugFile(std::string& str)
{
	if (!m_PushMapFileContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nMap Files";
	str += "\n--------";
	MapFiles::WriteAllMapFilesToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteNameGeneratorContentToDebugFile(std::string& str)
{
	if (!m_PushNameGeneratorContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nName Generators";
	str += "\n--------";
	NameGenerator::WriteAllNameGeneratorsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteNPCAttributeContentToDebugFile(std::string& str)
{
	if (!m_PushNPCAttributeContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nNPC Attributes";
	str += "\n--------";
	NPCAttributeContainer::WriteAllNPCAttributeContainersToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteNPCJobContentToDebugFile(std::string& str)
{
	if (!m_PushNPCJobContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nNPC Jobs";
	str += "\n--------";
	NPCJob::WriteAllNPCJobsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteQuestContentToDebugFile(std::string& str)
{
	if (!m_PushQuestGeneratorsContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nQuest Generators";
	str += "\n--------";
	QuestGenerator::WriteAllQuestGenerators(str, 0);
	str += "\n--------";
}

void GameSettings::WriteSoundContentToDebugFile(std::string& str)
{
	if (!m_PushSoundContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nSounds";
	str += "\n--------";
	SoundRegistration::WriteAllSoundRegistrationsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteSpeechBubbleContentToDebugFile(std::string& str)
{
	if (!m_PushSpeechBubbleContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nSpeech Bubbles";
	str += "\n--------";
	SpeechBubble::WriteAllSpeechBubblesToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteSpriteAnimationContentToDebugFile(std::string& str)
{
	if (!m_PushSpriteAnimationContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nSprite Animations";
	str += "\n--------";
	SpriteResourceHandler::WriteSpriteAnimationsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteSpriteResourceContentToDebugFile(std::string& str)
{
	if (!m_PushSpriteResourceContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nSprite Resources";
	str += "\n--------";
	SpriteResourceHandler::WriteSpriteResourcesToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteTileDefinitionContentToDebugFile(std::string& str)
{
	if (!m_PushTileDefinitionContentToDebugFile)
	{
		return;
	}
	str += "\n--------";
	str += "\nTile Definitions";
	str += "\n--------";
	TileDefinition::WriteAllTileDefinitionsToString(str, 0);
	str += "\n--------";
}

void GameSettings::WriteActiveContentToString(std::string& str, bool overrideShouldWrite)
{
	if (!overrideShouldWrite && !m_AllowWriteActiveContent)
	{
		return;
	}
	str += "\n-------------------------------------";
	str += "\nActive Data";
	str += "\n----------------------";
	WriteGlobalVariablesToDebugFile(str);
	Map::WriteAllMapActiveDatasToString(str, 0,
		m_PushTileEventsPerMapContentToDebugFile,
		m_PushTilesPerMapContentToDebugFile,
		m_PushAgentsPerMapContentToDebugFile,
		m_PushFeaturesPerMapContentToDebugFile,
		m_PushActiveQuestContentToDebugFile);


	str += "\n----------------------";
}