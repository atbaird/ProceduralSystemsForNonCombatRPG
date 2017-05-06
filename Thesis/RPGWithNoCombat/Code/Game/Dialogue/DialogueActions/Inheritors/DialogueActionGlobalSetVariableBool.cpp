#include "Game/Dialogue/DialogueActions/Inheritors/DialogueActionGlobalSetVariableBool.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Creation Funcs
DialogueTrigger* CreateDialogueActionGlobalSetVariableBool(const XMLNode& node)
{
	return new DialogueActionGlobalSetVariableBool(node);
}

DialogueTrigger* CreateDefaultDialogueActionGlobalSetVariableBool()
{
	return new DialogueActionGlobalSetVariableBool();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Static Variables
STATIC const DialogueActionRegistrationHelper 
	DialogueActionGlobalSetVariableBool::s_DialogueActionGlobalSetVariableBoolRegistrationHelper
		= DialogueActionRegistrationHelper("GlobalSetVariableBool",
			CreateDialogueActionGlobalSetVariableBool, 
			CreateDefaultDialogueActionGlobalSetVariableBool);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Constructors
DialogueActionGlobalSetVariableBool::DialogueActionGlobalSetVariableBool()
	: DialogueTrigger()
{

}

DialogueActionGlobalSetVariableBool::DialogueActionGlobalSetVariableBool(const XMLNode& node)
	: DialogueTrigger(node)
{

}

DialogueActionGlobalSetVariableBool::DialogueActionGlobalSetVariableBool(
	const DialogueActionGlobalSetVariableBool& other)
	: DialogueTrigger(other),
	m_variableName(other.m_variableName),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName),
	m_value(other.m_value)
{

}

DialogueActionGlobalSetVariableBool::~DialogueActionGlobalSetVariableBool()
{
	DialogueTrigger::~DialogueTrigger();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Setters
void DialogueActionGlobalSetVariableBool::SetVariableName(const std::string& name)
{
	m_variableName = name;
	m_lowerCaseVariableName = MakeLower(name);
}

void DialogueActionGlobalSetVariableBool::SetValue(bool value)
{
	m_value = value;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Getters
const std::string& DialogueActionGlobalSetVariableBool::GetVariableName() const
{
	return m_variableName;
}

const std::string& DialogueActionGlobalSetVariableBool::GetLowerCaseVariableName() const
{
	return m_lowerCaseVariableName;
}

bool DialogueActionGlobalSetVariableBool::GetValue() const
{
	return m_value;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Operations
void DialogueActionGlobalSetVariableBool::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Trigger Name: Global Set Variable Bool\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name to Set: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value to set it to: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_value);

}

void DialogueActionGlobalSetVariableBool::TriggerAction()
{
	if (m_lowerCaseVariableName == "")
	{
		return;
	}
	Map::SetGlobalVariable(m_lowerCaseVariableName, m_value);
}

DialogueTrigger* DialogueActionGlobalSetVariableBool::Clone() const
{
	return new DialogueActionGlobalSetVariableBool(*this);
}