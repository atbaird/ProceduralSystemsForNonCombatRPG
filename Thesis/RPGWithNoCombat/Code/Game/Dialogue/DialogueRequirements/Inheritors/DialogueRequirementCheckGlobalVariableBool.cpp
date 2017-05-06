#include "Game/Dialogue/DialogueRequirements/Inheritors/DialogueRequirementCheckGlobalVariableBool.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Spawning Functions
DialogueRequirement* DialogueReqCheckGlobalVariableBoolCreationFunc(const XMLNode& node)
{
	return new DialogueRequirementCheckGlobalVariableBool(node);
}

DialogueRequirement* DialogueReqCheckGlobalVariableBoolDefaultCreationFunc()
{
	return new DialogueRequirementCheckGlobalVariableBool();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Static Variables
STATIC const DialogueRequirementRegistrationHelper 
	DialogueRequirementCheckGlobalVariableBool::s_CheckGlobalVariableBoolRegHelper
		= DialogueRequirementRegistrationHelper("GlobalCheckVariableBool", 
			DialogueReqCheckGlobalVariableBoolCreationFunc,
			DialogueReqCheckGlobalVariableBoolDefaultCreationFunc);

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Constructors
DialogueRequirementCheckGlobalVariableBool::DialogueRequirementCheckGlobalVariableBool()
	: DialogueRequirement()
{

}

DialogueRequirementCheckGlobalVariableBool::DialogueRequirementCheckGlobalVariableBool(const XMLNode& node)
	: DialogueRequirement(node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "value")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "true"
				|| val == "t"
				|| val == "=true"
				|| val == "==true"
				|| val == "=t"
				|| val == "==t"
				|| val == "!=false"
				|| val == "!=f")
			{
				m_value = true;
			}
			else if (val == "false"
				|| val == "f"
				|| val == "==false"
				|| val == "=false"
				|| val == "==f"
				|| val == "=f"
				|| val == "!=true"
				|| val == "!=t")
			{
				m_value = false;
			}
		}
		else if (attrName == "variable"
			|| attrName == "variablename")
		{
			SetVariableName(attr.lpszValue);
		}
	}
}


DialogueRequirementCheckGlobalVariableBool::DialogueRequirementCheckGlobalVariableBool(
	const DialogueRequirementCheckGlobalVariableBool& other)
	: DialogueRequirement(other),
	m_variableName(other.m_variableName),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName),
	m_value(other.m_value)
{

}

DialogueRequirementCheckGlobalVariableBool::~DialogueRequirementCheckGlobalVariableBool()
{
	DialogueRequirement::~DialogueRequirement();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Setters
void DialogueRequirementCheckGlobalVariableBool::SetVariableName(const std::string& name)
{
	m_variableName = name;
	m_lowerCaseVariableName = MakeLower(name);
}

void DialogueRequirementCheckGlobalVariableBool::SetValue(bool value)
{
	m_value = value;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Getters
const std::string& DialogueRequirementCheckGlobalVariableBool::GetVariableName() const
{
	return m_variableName;
}

const std::string& DialogueRequirementCheckGlobalVariableBool::GetLowerCaseVariableName() const
{
	return m_lowerCaseVariableName;
}

bool DialogueRequirementCheckGlobalVariableBool::GetValue() const
{
	return m_value;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Operations
void DialogueRequirementCheckGlobalVariableBool::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Requirement Name: Check Global Variable Bool\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value desired: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_value);
}

bool DialogueRequirementCheckGlobalVariableBool::CheckIfRequirementIsMet() const
{
	if (m_lowerCaseVariableName == "")
	{
		return false;
	}
	bool val = false;
	Map::GetGlobalVariable(m_lowerCaseVariableName, val, false);
	return val == m_value;
}

DialogueRequirement* DialogueRequirementCheckGlobalVariableBool::Clone() const
{
	return new DialogueRequirementCheckGlobalVariableBool(*this);
}