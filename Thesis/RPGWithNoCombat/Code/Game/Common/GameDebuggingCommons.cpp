#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseOperations.hpp"

STATIC void GameDebuggingCommons::AddIndentation(std::string& str, int indentationAmt)
{
	for (int i = 0; i < indentationAmt; i++)
	{
		str += " ";
	}
}

STATIC void GameDebuggingCommons::WriteBoolOntoString(std::string& str, bool theBool, bool addNewLine)
{
	if (theBool)
	{
		str += "TRUE";
	}
	else
	{
		str += "FALSE";
	}

	if (addNewLine)
	{
		str += "\n";
	}
}

STATIC void GameDebuggingCommons::WriteSpecialCaseOperatorToString(std::string& str, SpecialCaseOperations operation,
	bool addNewLine)
{
	switch (operation)
	{
	case IS_SAME:
		str += "==";
		break;
	case IS_GREATER_THAN:
		str += ">";
		break;
	case IS_LESS_THAN:
		str += "<";
		break;
	case IS_LESS_THAN_OR_EQUAL_TO:
		str += "<=";
		break;
	case IS_GREATER_THAN_OR_EQUAL_TO:
		str += ">=";
		break;
	case IS_NOT_EQUAL_TO:
		str += "!=";
		break;
	default:
		str += "UNKNOWN";
		break;
	}

	if (addNewLine)
	{
		str += "\n";
	}
}