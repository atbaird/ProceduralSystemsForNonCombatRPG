#include "Game/Debugging/DebugConsoleCommands.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Console/Console.hpp"
#include "Game/TheGame.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Debugging/DebuggingFileHandler.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Debugging/DebugConsoleCommandsPart2.hpp"

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
STATIC void DebugConsoleCommands::RegisterAllConsoleCommands()
{
	//Commands
	static RegisterCommandHelper PrintGameDataCommandHelper("PrintGameDataDebugLog", 
		PrintGameDataDebugLog, HelpPrintGameDataDebugLog);
	static RegisterCommandHelper PrintOutCurrentGameDataDebugSettingsHelper(
		"PrintOutCurrentGameDataDebugSettings",
		PrintOutCurrentGameDataDebugSettings, HelpPrintOutCurrentGameDataDebugSettings);
	static RegisterCommandHelper PrintOutCurrentGameSettingsContentHelper(
		"PrintOutCurrentGameSettingsContent",
		PrintOutCurrentGameSettingsContent, HelpPrintOutCurrentGameSettingsContent);
	static RegisterCommandHelper PrintOutCurrentGeneratorContentHelper(
		"PrintOutCurrentGeneratorContent",
		PrintOutCurrentGeneratorContent, HelpPrintOutCurrentGeneratorContent);
	static RegisterCommandHelper PrintOutCurrentActiveDataHelper(
		"PrintOutCurrentActiveData",
		PrintOutCurrentActiveData, HelpPrintOutCurrentActiveData);

	static RegisterCommandHelper PrintOutMapActiveDataOnMapHelper(
		"PrintOutMapActiveDataOnMap", PrintOutMapActiveDataOnMap, HelpPrintOutMapActiveDataOnMap);

	RegisterFirst5();
	RegisterSecond5();
	RegisterThird5();
	RegisterFourth5();
	RegisterFifth5();
	RegisterSixth5();
	RegisterLast();
	RegisterExternalConsoleCommands();
}

STATIC void DebugConsoleCommands::RegisterFirst5()
{
	//!first 5 setter Commands
	static RegisterCommandHelper SetShouldIncludePlayerInfoWhenWritingDebugLogHelper(
		"SetShouldIncludePlayerInfoWhenWritingDebugLog",
		SetShouldIncludePlayerInfoWhenWritingDebugLog, HelpSetShouldIncludePlayerInfoWhenWritingDebugLog);
	static RegisterCommandHelper SetShouldIncludeShaderInfoWhenWritingDebugLogHelper(
		"SetShouldIncludeShaderInfoWhenWritingDebugLog",
		SetShouldIncludeShaderInfoWhenWritingDebugLog, HelpSetShouldIncludeShaderInfoWhenWritingDebugLog);
	static RegisterCommandHelper SetShouldIncludeFileDirectoriesInfoWhenWritingDebugLogHelper(
		"SetShouldIncludeFileDirectoriesInfoWhenWritingDebugLog",
		SetShouldIncludeFileDirectoriesInfoWhenWritingDebugLog,
		HelpSetShouldIncludeFileDirectoriesInfoWhenWritingDebugLog);
	static RegisterCommandHelper SetShouldIncludePerFrameDurationInfoWhenWritingDebugLogHelper(
		"SetShouldIncludePerFrameDurationInfoWhenWritingDebugLog",
		SetShouldIncludePerFrameDurationInfoWhenWritingDebugLog,
		HelpSetShouldIncludePerFrameDurationInfoWhenWritingDebugLog);
	static RegisterCommandHelper SetShouldIncludePreallocationsInfoWhenWritingDebugLogHelper(
		"SetShouldIncludePreallocationsInfoWhenWritingDebugLog",
		SetShouldIncludePreallocationsInfoWhenWritingDebugLog,
		HelpSetShouldIncludePreallocationsInfoWhenWritingDebugLog);
}

STATIC void DebugConsoleCommands::RegisterSecond5()
{
	//!second 5 setter Commands
	static RegisterCommandHelper EnableDebugLoggingHelper(
		"EnableDebugLogging",
		EnableDebugLogging, HelpEnableDebugLogging);
	static RegisterCommandHelper AllowWriteGameSettingsContentHelper(
		"AllowWriteGameSettingsContent",
		AllowWriteGameSettingsContent, HelpAllowWriteGameSettingsContent);
	static RegisterCommandHelper AllowWriteGeneratorContentHelper(
		"AllowWriteGeneratorContent",
		AllowWriteGeneratorContent,
		HelpAllowWriteGeneratorContent);
	static RegisterCommandHelper AllowWriteActiveContentHelper(
		"AllowWriteActiveContent",
		AllowWriteActiveContent,
		HelpAllowWriteActiveContent);
	static RegisterCommandHelper AllowWriteAgentGeneratorContentHelper(
		"AllowWriteAgentGeneratorContent",
		AllowWriteAgentGeneratorContent,
		HelpAllowWriteAgentGeneratorContent);
}

STATIC void DebugConsoleCommands::RegisterThird5()
{
	//!third 5 setter Commands
	static RegisterCommandHelper AllowWriteAgentStatsContentHelper(
		"AllowWriteAgentStatsContent",
		AllowWriteAgentStatsContent, HelpAllowWriteAgentStatsContent);
	static RegisterCommandHelper AllowWriteDialogueGroupContentHelper(
		"AllowWriteDialogueGroupContent",
		AllowWriteDialogueGroupContent, HelpAllowWriteDialogueGroupContent);
	static RegisterCommandHelper AllowWriteDialogueSegmentContentHelper(
		"AllowWriteDialogueSegmentContent",
		AllowWriteDialogueSegmentContent,
		AllowWriteDialogueSegmentContent);
	static RegisterCommandHelper AllowWriteFeatureGeneratorContentHelp(
		"AllowWriteFeatureGeneratorContent",
		AllowWriteFeatureGeneratorContent,
		HelpAllowWriteFeatureGeneratorContent);
	static RegisterCommandHelper AllowWriteHairGeneratorContentHelper(
		"AllowWriteAgentGeneratorContent",
		AllowWriteHairGeneratorContent,
		HelpAllowWriteHairGeneratorContent);
}

STATIC void DebugConsoleCommands::RegisterFourth5()
{
	//!fourth 5 setter Commands
	static RegisterCommandHelper AllowWriteItemGeneratorContentHelper(
		"AllowWriteItemGeneratorContent",
		AllowWriteItemGeneratorContent, HelpAllowWriteItemGeneratorContent);
	static RegisterCommandHelper AllowWriteMapFileContentHelper(
		"AllowWriteMapFileContent",
		AllowWriteMapFileContent, HelpAllowWriteMapFileContent);
	static RegisterCommandHelper AllowWriteNameGeneratorContentHelper(
		"AllowWriteNameGeneratorContent",
		AllowWriteNameGeneratorContent,
		HelpAllowWriteNameGeneratorContent);
	static RegisterCommandHelper AllowWriteNPCAttributeContentHelp(
		"AllowWriteNPCAttributeContent",
		AllowWriteNPCAttributeContent,
		HelpAllowWriteNPCAttributeContent);
	static RegisterCommandHelper AllowWriteNPCJobContentHelper(
		"AllowWriteNPCJobContent",
		AllowWriteNPCJobContent,
		HelpAllowWriteNPCJobContent);
}

STATIC void DebugConsoleCommands::RegisterFifth5()
{
	//!fifth 5 setter Commands
	static RegisterCommandHelper AllowWriteQuestGeneratorsContentHelper(
		"AllowWriteQuestGeneratorsContent",
		AllowWriteQuestGeneratorsContent,
		HelpAllowWriteQuestGeneratorsContent);
	static RegisterCommandHelper AllowWriteSoundContentHelper(
		"AllowWriteSoundContent",
		AllowWriteSoundContent,
		HelpAllowWriteSoundContent);
	static RegisterCommandHelper AllowWriteSpeechBubbleContentHelper(
		"AllowWriteSpeechBubbleContent",
		AllowWriteSpeechBubbleContent,
		HelpAllowWriteSpeechBubbleContent);
	static RegisterCommandHelper AllowWriteSpriteAnimationContentHelper(
		"AllowWriteSpriteAnimationContent",
		AllowWriteSpriteAnimationContent,
		HelpAllowWriteSpriteAnimationContent);
	static RegisterCommandHelper AllowWriteSpriteResourceContentHelper(
		"AllowWriteSpriteResourceContent",
		AllowWriteSpriteResourceContent,
		HelpAllowWriteSpriteResourceContent);
}

STATIC void DebugConsoleCommands::RegisterSixth5()
{
	//!sixth 5 setter Commands
	static RegisterCommandHelper AllowWriteTileDefinitionContentHelper(
		"AllowWriteTileDefinitionContent",
		AllowWriteTileDefinitionContent,
		HelpAllowWriteTileDefinitionContent);
	static RegisterCommandHelper AllowWriteTilesPerMapContentHelper(
		"AllowWriteTilesPerMapContent",
		AllowWriteTilesPerMapContent,
		HelpAllowWriteTilesPerMapContent);
	static RegisterCommandHelper AllowWriteAgentsPerMapContentHelper(
		"AllowWriteAgentsPerMapContent",
		AllowWriteAgentsPerMapContent,
		HelpAllowWriteAgentsPerMapContent);
	static RegisterCommandHelper AllowWriteFeaturesPerMapContentHelper(
		"AllowWriteFeaturesPerMapContent",
		AllowWriteFeaturesPerMapContent,
		HelpAllowWriteFeaturesPerMapContent);
	static RegisterCommandHelper AllowWriteTileEventsPerMapContentHelper(
		"AllowWriteTileEventsPerMapContent",
		AllowWriteTileEventsPerMapContent,
		HelpAllowWriteTileEventsPerMapContent);
}

STATIC void DebugConsoleCommands::RegisterLast()
{
	//!last setter Commands
	static RegisterCommandHelper AllowWriteActiveQuestsPerMapContentHelper(
		"AllowWriteActiveQuestsPerMapContent",
		AllowWriteActiveQuestsPerMapContent,
		HelpAllowWriteActiveQuestsPerMapContent);
	static RegisterCommandHelper AllowWriteGlobalVariablesContentHelper(
		"AllowWriteGlobalVariablesContent",
		AllowWriteGlobalVariablesContent,
		HelpAllowWriteGlobalVariablesContent);
}

STATIC void DebugConsoleCommands::RegisterExternalConsoleCommands()
{
	DebugConsoleCommandsPart2::RegisterDebugConsoleCommandsPart2();
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Help Commands
STATIC void DebugConsoleCommands::HelpPrintGameDataDebugLog(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Prints out all data to a debug log based on user settings.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Will also take two booleans; one to spit it out to the");
	g_Console->ConsolePrintf(UNIMPORTANT, "C++ Command line, one to send it to the Console.");
}

STATIC void DebugConsoleCommands::HelpPrintOutCurrentGameDataDebugSettings(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Prints out the current debug log settings.");
}

STATIC void DebugConsoleCommands::HelpPrintOutCurrentGameSettingsContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Prints out Game Settings data to a debug log based on user settings.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Will also take two booleans; one to spit it out to the");
	g_Console->ConsolePrintf(UNIMPORTANT, "C++ Command line, one to send it to the Console.");
}

STATIC void DebugConsoleCommands::HelpPrintOutCurrentGeneratorContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Prints out Generator data to a debug log based on user settings.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Will also take two booleans; one to spit it out to the");
	g_Console->ConsolePrintf(UNIMPORTANT, "C++ Command line, one to send it to the Console.");
}

STATIC void DebugConsoleCommands::HelpPrintOutCurrentActiveData(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Prints out Active data to a debug log based on user settings.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Will also take two booleans; one to spit it out to the");
	g_Console->ConsolePrintf(UNIMPORTANT, "C++ Command line, one to send it to the Console.");
}

STATIC void DebugConsoleCommands::HelpPrintOutMapActiveDataOnMap(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Given the name of a map, it will print out the active data off of it.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Will also take two booleans; one to spit it out to the");
	g_Console->ConsolePrintf(UNIMPORTANT, "C++ Command line, one to send it to the Console.");
}

STATIC void DebugConsoleCommands::HelpPrintOutQuestActiveDataOnMap(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Given the name of a quest, it will print out the active data off of it.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Will also take two booleans; one to spit it out to the");
	g_Console->ConsolePrintf(UNIMPORTANT, "C++ Command line, one to send it to the Console.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!first 5 setter Help Commands
STATIC void DebugConsoleCommands::HelpSetShouldIncludePlayerInfoWhenWritingDebugLog(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should print the information used for spawning the player.");
}

STATIC void DebugConsoleCommands::HelpSetShouldIncludeShaderInfoWhenWritingDebugLog(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should print the shader files used.");
}

STATIC void DebugConsoleCommands::HelpSetShouldIncludeFileDirectoriesInfoWhenWritingDebugLog(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should print the file directories searched for data.");
}

STATIC void DebugConsoleCommands::HelpSetShouldIncludePerFrameDurationInfoWhenWritingDebugLog(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "how much time has been allotted per data time per frame to read in data.");
}

STATIC void DebugConsoleCommands::HelpSetShouldIncludePreallocationsInfoWhenWritingDebugLog(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "how many preallocations to set before reading data.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!second 5 setter Help Commands
STATIC void DebugConsoleCommands::HelpEnableDebugLogging(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should print when quit.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteGameSettingsContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write the game settings content.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteGeneratorContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write the generator content.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteActiveContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write the active content.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteAgentGeneratorContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the agent generators.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!third 5 setter Help Commands
STATIC void DebugConsoleCommands::HelpAllowWriteAgentStatsContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the agent stats.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteDialogueGroupContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the dialogue groups.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteDialogueSegmentContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the dialogue segments.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteFeatureGeneratorContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the feature generators.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteHairGeneratorContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the hair generators.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!fourth 5 setter Help Commands
STATIC void DebugConsoleCommands::HelpAllowWriteItemGeneratorContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the item generators.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteMapFileContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the map files.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteNameGeneratorContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the name generators.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteNPCAttributeContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the NPC Attributes.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteNPCJobContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out the NPC Jobs.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!fifth 5 setter Help Commands
STATIC void DebugConsoleCommands::HelpAllowWriteQuestGeneratorsContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out quest generators.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteSoundContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out sounds.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteSpeechBubbleContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out speech bubbles.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteSpriteAnimationContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out sprite animations.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteSpriteResourceContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out sprite resources.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!sixth 5 setter Help Commands
STATIC void DebugConsoleCommands::HelpAllowWriteTileDefinitionContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out tile definitions.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteTilesPerMapContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out tile info per map.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteAgentsPerMapContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out agent info per map.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteFeaturesPerMapContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out feature info per map.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteTileEventsPerMapContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out tile event info per map.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!last setter Help Commands
STATIC void DebugConsoleCommands::HelpAllowWriteActiveQuestsPerMapContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out active quests per map.");
}

STATIC void DebugConsoleCommands::HelpAllowWriteGlobalVariablesContent(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Accepts true or false, and sets whether the debug log");
	g_Console->ConsolePrintf(UNIMPORTANT, "should write out global variables.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Commands
STATIC void DebugConsoleCommands::PrintGameDataDebugLog(Command& input)
{
	if (g_TheGame == nullptr)
		return;
	bool shouldPrintToConsole = false;
	bool shouldPrintToCommandLine = false;
	std::string val0 = MakeLower(input.get_string_by_index(0));
	std::string val1 = MakeLower(input.get_string_by_index(1));
	if (val0 == "true"
		|| val0 == "t")
	{
		shouldPrintToCommandLine = true;
	}

	if (val1 == "true"
		|| val1 == "t")
	{
		shouldPrintToConsole = true;
	}

	g_Console->ConsolePrintf(UNIMPORTANT, "Printing Debug log.");
	g_TheGame->PrintOutDebugContentNextFrame(true, shouldPrintToCommandLine, shouldPrintToConsole);
}

STATIC void DebugConsoleCommands::PrintOutCurrentGameDataDebugSettings(Command&)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string message = "";
	settings->WriteCurrentSettingsToString(message);
	g_Console->ConsolePrintf(UNIMPORTANT, message);
	g_Console->ConsolePrintf(UNIMPORTANT, "---------------------------");
	g_Console->ConsolePrintf(UNIMPORTANT, "Done printing.");
}

STATIC void DebugConsoleCommands::PrintOutCurrentGameSettingsContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	bool shouldPrintToConsole = false;
	bool shouldPrintToCommandLine = false;
	std::string val0 = MakeLower(input.get_string_by_index(0));
	std::string val1 = MakeLower(input.get_string_by_index(1));
	if (val0 == "true"
		|| val0 == "t")
	{
		shouldPrintToCommandLine = true;
	}

	if (val1 == "true"
		|| val1 == "t")
	{
		shouldPrintToConsole = true;
	}
	double durationPlayTime = g_TheGame->WriteOutEndingData();
	std::string str = "";
	settings->WriteDataOutToDebugFile(g_TheGame->m_timeGameStarted, g_TheGame->m_timeGameEnded,
		durationPlayTime, false, str, true, false, false);

	if (shouldPrintToCommandLine)
	{
		DebuggerPrintf(str.c_str());
	}

	if (shouldPrintToConsole)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, str);
	}
	g_Console->ConsolePrintf(UNIMPORTANT, "---------------------------");
	g_Console->ConsolePrintf(UNIMPORTANT, "Done printing.");
}

STATIC void DebugConsoleCommands::PrintOutCurrentGeneratorContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return; 
	bool shouldPrintToConsole = false;
	bool shouldPrintToCommandLine = false;
	std::string val0 = MakeLower(input.get_string_by_index(0));
	std::string val1 = MakeLower(input.get_string_by_index(1));
	if (val0 == "true"
		|| val0 == "t")
	{
		shouldPrintToCommandLine = true;
	}

	if (val1 == "true"
		|| val1 == "t")
	{
		shouldPrintToConsole = true;
	}
	double durationPlayTime = g_TheGame->WriteOutEndingData();
	std::string str = "";
	settings->WriteDataOutToDebugFile(g_TheGame->m_timeGameStarted, g_TheGame->m_timeGameEnded,
		durationPlayTime, false, str, false, true, false);

	if (shouldPrintToCommandLine)
	{
		DebuggerPrintf(str.c_str());
	}

	if (shouldPrintToConsole)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, str);
	}
	g_Console->ConsolePrintf(UNIMPORTANT, "---------------------------");
	g_Console->ConsolePrintf(UNIMPORTANT, "Done printing.");
}

STATIC void DebugConsoleCommands::PrintOutCurrentActiveData(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return; 
	bool shouldPrintToConsole = false;
	bool shouldPrintToCommandLine = false;
	std::string val0 = MakeLower(input.get_string_by_index(0));
	std::string val1 = MakeLower(input.get_string_by_index(1));
	if (val0 == "true"
		|| val0 == "t")
	{
		shouldPrintToCommandLine = true;
	}

	if (val1 == "true"
		|| val1 == "t")
	{
		shouldPrintToConsole = true;
	}
	double durationPlayTime = g_TheGame->WriteOutEndingData();
	std::string str = "";
	settings->WriteDataOutToDebugFile(g_TheGame->m_timeGameStarted, g_TheGame->m_timeGameEnded,
		durationPlayTime, false, str, false, false, true);

	if (shouldPrintToCommandLine)
	{
		DebuggerPrintf(str.c_str());
	}

	if (shouldPrintToConsole)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, str);
	}
	g_Console->ConsolePrintf(UNIMPORTANT, "---------------------------");
	g_Console->ConsolePrintf(UNIMPORTANT, "Done printing.");
}

STATIC void DebugConsoleCommands::PrintOutMapActiveDataOnMap(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return; 
	bool shouldPrintToConsole = false;
	bool shouldPrintToCommandLine = false;
	std::string mapName = input.get_string_by_index(0);
	std::string val0 = MakeLower(input.get_string_by_index(1));
	std::string val1 = MakeLower(input.get_string_by_index(2));
	if (val0 == "true"
		|| val0 == "t")
	{
		shouldPrintToCommandLine = true;
	}
	if (val1 == "true"
		|| val1 == "t")
	{
		shouldPrintToConsole = true;
	}
	double durationPlayTime = g_TheGame->WriteOutEndingData();
	std::string str = "";
	settings->WriteDataHeaderOutToString(g_TheGame->m_timeGameStarted, g_TheGame->m_timeGameEnded,
		durationPlayTime, false, str);
	Map::WriteSingleMapActiveDatasToString(str, mapName, 0, settings->m_PushTileEventsPerMapContentToDebugFile,
		settings->m_PushTilesPerMapContentToDebugFile, settings->m_PushAgentsPerMapContentToDebugFile,
		settings->m_PushFeatureGeneratorContentToDebugFile, settings->m_PushActiveQuestContentToDebugFile);

	settings->WriteStringOutToDebugFile(str);

	if (shouldPrintToCommandLine)
	{
		DebuggerPrintf(str.c_str());
	}

	if (shouldPrintToConsole)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, str);
	}
	g_Console->ConsolePrintf(UNIMPORTANT, "---------------------------");
	g_Console->ConsolePrintf(UNIMPORTANT, "Done printing.");
}

STATIC void DebugConsoleCommands::PrintOutQuestActiveDataOnMap(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	bool shouldPrintToConsole = false;
	bool shouldPrintToCommandLine = false;
	std::string questName = input.get_string_by_index(0);
	std::string val0 = MakeLower(input.get_string_by_index(1));
	std::string val1 = MakeLower(input.get_string_by_index(2));
	if (val0 == "true"
		|| val0 == "t")
	{
		shouldPrintToCommandLine = true;
	}
	if (val1 == "true"
		|| val1 == "t")
	{
		shouldPrintToConsole = true;
	}
	double durationPlayTime = g_TheGame->WriteOutEndingData();
	std::string str = "";
	settings->WriteDataHeaderOutToString(g_TheGame->m_timeGameStarted, g_TheGame->m_timeGameEnded,
		durationPlayTime, false, str);
	const BaseGameEntity* playerEntity = BaseGameEntity::GetPointerToPlayer();
	const Player* player = dynamic_cast<const Player*>(playerEntity);
	if (player == nullptr)
	{
		str += "----------------\n";
		str += "Player does not exist\n";
		str += "Will not search for quest info without the player.\n";
		str += "----------------\n";
	}
	else
	{
		if (!player->WriteOutQuestDataByNameToString(str, questName, 0))
		{
			str += "----------------\n";
			str += "Unable to find quest\n";
			str += "Quest might not of been spawned in yet.\n";
			str += "----------------\n";
		}
	}

	settings->WriteStringOutToDebugFile(str);

	if (shouldPrintToCommandLine)
	{
		DebuggerPrintf(str.c_str());
	}

	if (shouldPrintToConsole)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, str);
	}
	g_Console->ConsolePrintf(UNIMPORTANT, "---------------------------");
	g_Console->ConsolePrintf(UNIMPORTANT, "Done printing.");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!first 5 setter Commands
STATIC void DebugConsoleCommands::SetShouldIncludePlayerInfoWhenWritingDebugLog(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->SetShouldIncludePlayerInfoWhenWriting(true);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Player spawning info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->SetShouldIncludePlayerInfoWhenWriting(false);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Player spawning info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}

}

STATIC void DebugConsoleCommands::SetShouldIncludeShaderInfoWhenWritingDebugLog(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->SetShouldIncludeShaderInfoWhenWriting(true);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include shader info.");
	}
	else if(val == "false"
		|| val == "f")
	{
		settings->SetShouldIncludeShaderInfoWhenWriting(false);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude shader info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::SetShouldIncludeFileDirectoriesInfoWhenWritingDebugLog(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->SetShouldIncludeFileDirectoriesInfoWhenWriting(true);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include data file directories.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->SetShouldIncludeFileDirectoriesInfoWhenWriting(false);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude data file directories.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::SetShouldIncludePerFrameDurationInfoWhenWritingDebugLog(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->SetShouldIncludePerFrameDurationInfoWhenWriting(true);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include the per frame data reading durations.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->SetShouldIncludePerFrameDurationInfoWhenWriting(false);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude the per frame data reading durations.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::SetShouldIncludePreallocationsInfoWhenWritingDebugLog(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->SetShouldIncludePreallocationsInfoWhenWriting(true);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include the preallocation info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->SetShouldIncludePreallocationsInfoWhenWriting(false);
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude the preallocation info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!second 5 setter Commands
STATIC void DebugConsoleCommands::EnableDebugLogging(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_EnableDebugLogging = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now print at the end of the game.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_EnableDebugLogging = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now not print at the end of the game.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteGameSettingsContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_AllowWriteGameSettingsContent = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Game Settings content.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_AllowWriteGameSettingsContent = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Game Settings content.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteGeneratorContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_AllowWriteGeneratorContent = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Generator content.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_AllowWriteGeneratorContent = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Generator contet.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteActiveContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_AllowWriteActiveContent = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Active content.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_AllowWriteActiveContent = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Active content.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteAgentGeneratorContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushAgentGeneratorContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Agent Generators info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushAgentGeneratorContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Agent Generators info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!third 5 setter Commands
STATIC void DebugConsoleCommands::AllowWriteAgentStatsContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushAgentStatsContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Agent Stat info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushAgentStatsContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Agent Stat info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteDialogueGroupContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushDialogueGroupingContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Dialogue Group info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushDialogueGroupingContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Dialogue Group info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteDialogueSegmentContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushDialogueSegmentContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Dialogue Segment info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushDialogueSegmentContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Dialogue Segment info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteFeatureGeneratorContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushFeatureGeneratorContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Feature Generator info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushFeatureGeneratorContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Feature Generator info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteHairGeneratorContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushHairGeneratorContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Hair Generator info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushHairGeneratorContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Hair Generator info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!fourth 5 setter Commands
STATIC void DebugConsoleCommands::AllowWriteItemGeneratorContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushItemGeneratorContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Item Generator info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushItemGeneratorContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Item Generator info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteMapFileContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushMapFileContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Map File info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushMapFileContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Map File info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteNameGeneratorContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushNameGeneratorContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include Name Generator info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushNameGeneratorContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude Name Generator info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteNPCAttributeContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushNPCAttributeContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include NPC Attribute info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushNPCAttributeContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude NPC Attribute info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteNPCJobContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushNPCJobContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include NPC Job info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushNPCJobContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude NPC Job info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!fifth 5 setter Commands
STATIC void DebugConsoleCommands::AllowWriteQuestGeneratorsContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushQuestGeneratorsContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include quest generator info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushQuestGeneratorsContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude quest generator info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteSoundContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushSoundContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include sound info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushSoundContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude sound info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteSpeechBubbleContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushSpeechBubbleContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include speech bubble info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushSpeechBubbleContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude speech bubble info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteSpriteAnimationContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushSpriteAnimationContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include sprite animation info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushSpriteAnimationContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude sprite animation info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteSpriteResourceContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushSpriteResourceContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include sprite resource info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushSpriteResourceContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude sprite resource info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!sixth 5 setter Commands
STATIC void DebugConsoleCommands::AllowWriteTileDefinitionContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushTileDefinitionContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include tile definition info.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushTileDefinitionContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude tile definition info.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteTilesPerMapContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushTilesPerMapContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include tile info per map.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushTilesPerMapContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude tile info per map.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteAgentsPerMapContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushAgentsPerMapContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include agent info per map.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushAgentsPerMapContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude agent info per map.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteFeaturesPerMapContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushFeaturesPerMapContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include feature info per map.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushFeatureGeneratorContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude feature info per map.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteTileEventsPerMapContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushTileEventsPerMapContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include tile event info per map.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushTileEventsPerMapContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude tile event info per map.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//!last setter Commands
STATIC void DebugConsoleCommands::AllowWriteActiveQuestsPerMapContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushActiveQuestContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include active quest info per map.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushActiveQuestContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude active quest info per map.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

STATIC void DebugConsoleCommands::AllowWriteGlobalVariablesContent(Command& input)
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings == nullptr)
		return;
	std::string val = MakeLower(input.get_string_by_index(0));
	if (val == "true"
		|| val == "t")
	{
		settings->m_PushGlobalVariablesContentToDebugFile = true;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include global variables.");
	}
	else if (val == "false"
		|| val == "f")
	{
		settings->m_PushGlobalVariablesContentToDebugFile = false;
		g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude global variables.");
	}
	else
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
	}
}

//GameSettings* settings = TheGame::GetGameSettings();
//if (settings == nullptr)
//return;
//std::string val = MakeLower(input.get_string_by_index(0));
//if (val == "true"
//	|| val == "t")
//{
//	
//	g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now include the preallocation info.");
//}
//else if (val == "false"
//	|| val == "f")
//{
//	
//	g_Console->ConsolePrintf(UNIMPORTANT, "Debug log will now exclude the preallocation info.");
//}
//else
//{
//	g_Console->ConsolePrintf(UNIMPORTANT, "Invalid input.");
//}