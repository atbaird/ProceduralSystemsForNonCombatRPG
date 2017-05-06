#pragma once

#ifndef DEBUGCONSOLECOMMANDSPART2_HPP
#define DEBUGCONSOLECOMMANDSPART2_HPP

#define STATIC
class Command;
class DebugConsoleCommandsPart2
{
private:
protected:
public:
	static void RegisterDebugConsoleCommandsPart2();

	//Help Commands
	static void HelpPrintOutAllGlobalVariables(Command&);
	static void HelpSetGlobalVariableBool(Command&);
	static void HelpSetGlobalVariableFloat(Command&);
	static void HelpSetQuestVariableBool(Command&);
	static void HelpSetQuestVariableFloat(Command&);
	static void HelpPrintGlobalVariableBool(Command&);
	static void HelpPrintGlobalVariableFloat(Command&);
	static void HelpPrintQuestVariableBool(Command&);
	static void HelpPrintQuestVariableFloat(Command&);

	//Commands
	static void PrintOutAllGlobalVariables(Command& other);
	static void SetGlobalVariableBool(Command& input);
	static void SetGlobalVariableFloat(Command& input);
	static void SetQuestVariableBool(Command& input);
	static void SetQuestVariableFloat(Command& input);
	static void PrintGlobalVariableBool(Command& input);
	static void PrintGlobalVariableFloat(Command& input);
	static void PrintQuestVariableBool(Command& input);
	static void PrintQuestVariableFloat(Command& input);

};

#endif