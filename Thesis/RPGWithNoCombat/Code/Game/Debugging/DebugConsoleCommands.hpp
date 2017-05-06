#pragma once

#ifndef DEBUGCONSOLECOMMANDS_HPP
#define DEBUGCONSOLECOMMANDS_HPP

class Command;
class DebugConsoleCommands
{
private:
protected:
public:
	//------------------------------------------------------------------------------------------
	static void RegisterAllConsoleCommands();
protected:
	static void RegisterFirst5();
	static void RegisterSecond5();
	static void RegisterThird5();
	static void RegisterFourth5();
	static void RegisterFifth5();
	static void RegisterSixth5();
	static void RegisterLast();
	static void RegisterExternalConsoleCommands();
public:

	//------------------------------------------------------------------------------------------
	//Help Commands
	static void HelpPrintGameDataDebugLog(Command& input);
	static void HelpPrintOutCurrentGameDataDebugSettings(Command& input);
	static void HelpPrintOutCurrentGameSettingsContent(Command& input);
	static void HelpPrintOutCurrentGeneratorContent(Command& input);
	static void HelpPrintOutCurrentActiveData(Command& input);
	static void HelpPrintOutMapActiveDataOnMap(Command& input);
	static void HelpPrintOutQuestActiveDataOnMap(Command& input);

	//!first 5 setter Help Commands
	static void HelpSetShouldIncludePlayerInfoWhenWritingDebugLog(Command& input);
	static void HelpSetShouldIncludeShaderInfoWhenWritingDebugLog(Command& input);
	static void HelpSetShouldIncludeFileDirectoriesInfoWhenWritingDebugLog(Command& input);
	static void HelpSetShouldIncludePerFrameDurationInfoWhenWritingDebugLog(Command& input);
	static void HelpSetShouldIncludePreallocationsInfoWhenWritingDebugLog(Command& input);

	//!second 5 setter Help Commands
	static void HelpEnableDebugLogging(Command&);
	static void HelpAllowWriteGameSettingsContent(Command&);
	static void HelpAllowWriteGeneratorContent(Command&);
	static void HelpAllowWriteActiveContent(Command&);
	static void HelpAllowWriteAgentGeneratorContent(Command&);

	//!third 5 setter Help Commands
	static void HelpAllowWriteAgentStatsContent(Command& input);
	static void HelpAllowWriteDialogueGroupContent(Command& input);
	static void HelpAllowWriteDialogueSegmentContent(Command& input);
	static void HelpAllowWriteFeatureGeneratorContent(Command& input);
	static void HelpAllowWriteHairGeneratorContent(Command& input);

	//!fourth 5 setter Help Commands
	static void HelpAllowWriteItemGeneratorContent(Command& input);
	static void HelpAllowWriteMapFileContent(Command& input);
	static void HelpAllowWriteNameGeneratorContent(Command& input);
	static void HelpAllowWriteNPCAttributeContent(Command& input);
	static void HelpAllowWriteNPCJobContent(Command& input);

	//!fifth 5 setter Help Commands
	static void HelpAllowWriteQuestGeneratorsContent(Command& input);
	static void HelpAllowWriteSoundContent(Command& input);
	static void HelpAllowWriteSpeechBubbleContent(Command& input);
	static void HelpAllowWriteSpriteAnimationContent(Command& input);
	static void HelpAllowWriteSpriteResourceContent(Command& input);

	//!sixth 5 setter Help Commands
	static void HelpAllowWriteTileDefinitionContent(Command& input);
	static void HelpAllowWriteTilesPerMapContent(Command& input);
	static void HelpAllowWriteAgentsPerMapContent(Command& input);
	static void HelpAllowWriteFeaturesPerMapContent(Command& input);
	static void HelpAllowWriteTileEventsPerMapContent(Command& input);

	//!last setter Help Commands
	static void HelpAllowWriteActiveQuestsPerMapContent(Command& input);
	static void HelpAllowWriteGlobalVariablesContent(Command& input);

	//------------------------------------------------------------------------------------------
	//Commands
	static void PrintGameDataDebugLog(Command& input);
	static void PrintOutCurrentGameDataDebugSettings(Command& input);
	static void PrintOutCurrentGameSettingsContent(Command& input);
	static void PrintOutCurrentGeneratorContent(Command& input);
	static void PrintOutCurrentActiveData(Command& input);
	static void PrintOutMapActiveDataOnMap(Command& input);
	static void PrintOutQuestActiveDataOnMap(Command& input);

	//!first 5 setter Commands
	static void SetShouldIncludePlayerInfoWhenWritingDebugLog(Command& input);
	static void SetShouldIncludeShaderInfoWhenWritingDebugLog(Command& input);
	static void SetShouldIncludeFileDirectoriesInfoWhenWritingDebugLog(Command& input);
	static void SetShouldIncludePerFrameDurationInfoWhenWritingDebugLog(Command& input);
	static void SetShouldIncludePreallocationsInfoWhenWritingDebugLog(Command& input);

	//!second 5 setter Commands
	static void EnableDebugLogging(Command& input);
	static void AllowWriteGameSettingsContent(Command& input);
	static void AllowWriteGeneratorContent(Command& input);
	static void AllowWriteActiveContent(Command& input);
	static void AllowWriteAgentGeneratorContent(Command& input);

	//!third 5 setter Commands
	static void AllowWriteAgentStatsContent(Command& input);
	static void AllowWriteDialogueGroupContent(Command& input);
	static void AllowWriteDialogueSegmentContent(Command& input);
	static void AllowWriteFeatureGeneratorContent(Command& input);
	static void AllowWriteHairGeneratorContent(Command& input);

	//!fourth 5 setter Commands
	static void AllowWriteItemGeneratorContent(Command& input);
	static void AllowWriteMapFileContent(Command& input);
	static void AllowWriteNameGeneratorContent(Command& input);
	static void AllowWriteNPCAttributeContent(Command& input);
	static void AllowWriteNPCJobContent(Command& input);

	//!fifth 5 setter Commands
	static void AllowWriteQuestGeneratorsContent(Command& input);
	static void AllowWriteSoundContent(Command& input);
	static void AllowWriteSpeechBubbleContent(Command& input);
	static void AllowWriteSpriteAnimationContent(Command& input);
	static void AllowWriteSpriteResourceContent(Command& input);

	//!sixth 5 setter Commands
	static void AllowWriteTileDefinitionContent(Command& input);
	static void AllowWriteTilesPerMapContent(Command& input);
	static void AllowWriteAgentsPerMapContent(Command& input);
	static void AllowWriteFeaturesPerMapContent(Command& input);
	static void AllowWriteTileEventsPerMapContent(Command& input);

	//!last setter Commands
	static void AllowWriteActiveQuestsPerMapContent(Command& input);
	static void AllowWriteGlobalVariablesContent(Command& input);

};

#endif