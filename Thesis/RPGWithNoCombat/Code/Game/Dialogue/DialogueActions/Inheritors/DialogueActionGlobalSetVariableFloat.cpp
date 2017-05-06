#include "Game/Dialogue/DialogueActions/Inheritors/DialogueActionGlobalSetVariableFloat.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Creation Funcs
DialogueTrigger* CreateDialogueActionGlobalSetVariableFloat(const XMLNode& node)
{
	return new DialogueActionGlobalSetVariableFloat(node);
}

DialogueTrigger* CreateDefaultDialogueActionGlobalSetVariableFloat()
{
	return new DialogueActionGlobalSetVariableFloat();
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Static Variables
STATIC const DialogueActionRegistrationHelper s_DialogueActionGlobalSetVariableFloatRegistrationHelper
	= DialogueActionRegistrationHelper("GlobalSetVariableFloat", 
		CreateDialogueActionGlobalSetVariableFloat, 
		CreateDefaultDialogueActionGlobalSetVariableFloat);

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Constructors
DialogueActionGlobalSetVariableFloat::DialogueActionGlobalSetVariableFloat()
	: DialogueTrigger()
{
}

DialogueActionGlobalSetVariableFloat::DialogueActionGlobalSetVariableFloat(const XMLNode& node)
	: DialogueTrigger(node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variable"
			|| attrName == "variableName")
		{
			SetVariableName(std::string(attr.lpszValue));
		}
		else if (attrName == "value")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			{
				m_value = (float)atof(attrVal.c_str());
			}
		}
		else if (attrName == "howtohandle"
			|| attrName == "mode"
			|| attrName == "setmode")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "increment"
				|| attrVal == "increase")
			{
				m_StatOption = INCREMENT;
			}
			else if (attrVal == "set")
			{
				m_StatOption = SET;
			}
		}
	}
}

DialogueActionGlobalSetVariableFloat::DialogueActionGlobalSetVariableFloat(
	const DialogueActionGlobalSetVariableFloat& other)
	: DialogueTrigger(other),
	m_variableName(other.m_variableName),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName),
	m_StatOption(other.m_StatOption),
	m_value(other.m_value)
{
}

DialogueActionGlobalSetVariableFloat::~DialogueActionGlobalSetVariableFloat()
{
	DialogueTrigger::~DialogueTrigger();
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Setters
void DialogueActionGlobalSetVariableFloat::SetVariableName(const std::string& name)
{
	m_variableName = name;
	m_lowerCaseVariableName = MakeLower(name);
}

void DialogueActionGlobalSetVariableFloat::SetWhatToDoWithValue(QuestTriggerWhatToDoWithStatOptions whatToDo)
{
	m_StatOption = whatToDo;
}

void DialogueActionGlobalSetVariableFloat::SetValue(float value)
{
	m_value = value;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Getters
const std::string& DialogueActionGlobalSetVariableFloat::GetVariableName() const
{
	return m_variableName;
}

const std::string& DialogueActionGlobalSetVariableFloat::GetLowerCaseVariableName() const
{
	return m_lowerCaseVariableName;
}

QuestTriggerWhatToDoWithStatOptions DialogueActionGlobalSetVariableFloat::GetWhatToDoWithValue() const
{
	return m_StatOption;
}

float DialogueActionGlobalSetVariableFloat::GetValue() const
{
	return m_value;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Operations
void DialogueActionGlobalSetVariableFloat::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Trigger Name: Global Set Variable Float\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Stat Mode: ";
	switch (m_StatOption)
	{
	case SET:
		str += "SET\n";
		break;
	case INCREMENT:
		str += "INCREMENT\n";
		break;
	default:
		str += "UNKNOWN\n";
		break;
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value: " + std::to_string(m_value) + "\n";
}

void DialogueActionGlobalSetVariableFloat::TriggerAction()
{
	if (m_lowerCaseVariableName == "")
	{
		return;
	}
	float value = m_value;
	if (m_StatOption == INCREMENT)
	{
		float val = 0.f;
		Map::GetGlobalVariable(m_lowerCaseVariableName, val, 0.f);
		value += val;
	}

	Map::SetGlobalVariable(m_lowerCaseVariableName, value);
}

DialogueTrigger* DialogueActionGlobalSetVariableFloat::Clone() const
{
	return new DialogueActionGlobalSetVariableFloat(*this);
}