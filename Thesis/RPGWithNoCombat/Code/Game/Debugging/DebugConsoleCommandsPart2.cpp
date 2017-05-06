#include "Game/Debugging/DebugConsoleCommandsPart2.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Console/Console.hpp"
#include "Game/TheGame.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Debugging/DebuggingFileHandler.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Debugging/NamedPropertiesToString.hpp"



STATIC void DebugConsoleCommandsPart2::RegisterDebugConsoleCommandsPart2()
{
	static RegisterCommandHelper s_PrintOutAllGlobalVariables("PrintOutAllGlobalVariables",
		PrintOutAllGlobalVariables, HelpPrintOutAllGlobalVariables);

	static RegisterCommandHelper s_SetGlobalVariableBool("SetGlobalVariableBool",
		SetGlobalVariableBool, HelpSetGlobalVariableBool);
	static RegisterCommandHelper s_SetGlobalVariableFloat("SetGlobalVariableFloat",
		SetGlobalVariableFloat, HelpSetGlobalVariableFloat);




}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Help Commands
STATIC void DebugConsoleCommandsPart2::HelpPrintOutAllGlobalVariables(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Prints out all of the global variables directly to console.");
}

STATIC void DebugConsoleCommandsPart2::HelpSetGlobalVariableBool(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Takes the name of a global variable, followed by true/false");
	g_Console->ConsolePrintf(UNIMPORTANT, "and then sets a global variable to that value.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Note that this will even change the original variable type");
	g_Console->ConsolePrintf(UNIMPORTANT, "If it was different.");
}

STATIC void DebugConsoleCommandsPart2::HelpSetGlobalVariableFloat(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Takes the name of a global variable, followed by a float");
	g_Console->ConsolePrintf(UNIMPORTANT, "and then sets a global variable to that value.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Note that this will even change the original variable type");
	g_Console->ConsolePrintf(UNIMPORTANT, "If it was different.");
}

STATIC void DebugConsoleCommandsPart2::HelpSetQuestVariableBool(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Takes the name of the quest, followed by a variable name, then true/false.");
	g_Console->ConsolePrintf(UNIMPORTANT, "If the quest is found, then the variable will be set to that value");
	g_Console->ConsolePrintf(UNIMPORTANT, "Note that this will even change the original variable type");
	g_Console->ConsolePrintf(UNIMPORTANT, "If it was different.");
}

STATIC void DebugConsoleCommandsPart2::HelpSetQuestVariableFloat(Command&)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "Takes the name of the quest, followed by a variable name, then a float.");
	g_Console->ConsolePrintf(UNIMPORTANT, "If the quest is found, then the variable will be set to that value");
	g_Console->ConsolePrintf(UNIMPORTANT, "Note that this will even change the original variable type");
	g_Console->ConsolePrintf(UNIMPORTANT, "If it was different.");
}

STATIC void DebugConsoleCommandsPart2::HelpPrintGlobalVariableBool(Command&)
{

}

STATIC void DebugConsoleCommandsPart2::HelpPrintGlobalVariableFloat(Command&)
{

}

STATIC void DebugConsoleCommandsPart2::HelpPrintQuestVariableBool(Command&)
{

}

STATIC void DebugConsoleCommandsPart2::HelpPrintQuestVariableFloat(Command&)
{

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Commands
STATIC void DebugConsoleCommandsPart2::PrintOutAllGlobalVariables(Command&)
{
	const NamedProperties* props = Map::GetGlobalVariables();

	if (props == nullptr)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, 
			"No global variables found! Global variables container not created yet!");
		return;
	}

	std::string globalVariables = "";
	NamedPropertiesToString::ConvertAllKnownNamedPropertyValuesToString(props, globalVariables, 0);
	g_Console->ConsolePrintf(UNIMPORTANT, "----------------------------");
	g_Console->ConsolePrintf(UNIMPORTANT, globalVariables);
	g_Console->ConsolePrintf(UNIMPORTANT, "----------------------------");
	g_Console->ConsolePrintf(UNIMPORTANT, "Finished printing global variables");
}

STATIC void DebugConsoleCommandsPart2::SetGlobalVariableBool(Command& input)
{
	std::string variableName = input.get_string_by_index(0);
	std::string valueStr = MakeLower(input.get_string_by_index(1));
	bool value = false;

	if (valueStr == "true"
		|| valueStr == "t")
	{
		value = true;
	}
	else if (valueStr == "false"
		|| valueStr == "f")
	{
		value = false;
	}
	else
	{
		g_Console->ConsolePrintf(WARNING, "No valid value found; setting to false");
	}

	std::string valueOut = "";
	if (value == false)
	{
		valueOut = "FALSE";
	}
	else
	{
		valueOut = "TRUE";
	}

	Map::SetGlobalVariable(variableName, value);
	g_Console->ConsolePrintf(UNIMPORTANT, variableName + " set to " + valueOut);
}

STATIC void DebugConsoleCommandsPart2::SetGlobalVariableFloat(Command& input)
{
	std::string variableName = input.get_string_by_index(0);
	std::string valueStr = input.get_string_by_index(1);
	float value = 0.0f;
	value = (float)atof(valueStr.c_str());

	Map::SetGlobalVariable(variableName, value);
	std::string valueOut = std::to_string(value);
	g_Console->ConsolePrintf(UNIMPORTANT, variableName + " set to " + valueOut);

}

STATIC void DebugConsoleCommandsPart2::SetQuestVariableBool(Command& input)
{
	input;
}

STATIC void DebugConsoleCommandsPart2::SetQuestVariableFloat(Command& input)
{
	input;
}

STATIC void DebugConsoleCommandsPart2::PrintGlobalVariableBool(Command& input)
{
	input;
}

STATIC void DebugConsoleCommandsPart2::PrintGlobalVariableFloat(Command& input)
{
	input;
}

STATIC void DebugConsoleCommandsPart2::PrintQuestVariableBool(Command& input)
{
	input;
}

STATIC void DebugConsoleCommandsPart2::PrintQuestVariableFloat(Command& input)
{
	input;
}