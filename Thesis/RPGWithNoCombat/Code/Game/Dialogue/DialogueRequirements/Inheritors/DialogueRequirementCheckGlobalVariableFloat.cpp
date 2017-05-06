#include "Game/Dialogue/DialogueRequirements/Inheritors/DialogueRequirementCheckGlobalVariableFloat.hpp"
#include "Game/Map/Map.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Spawning Functions
DialogueRequirement* DialogueReqCheckGlobalVariableFloatCreationFunc(const XMLNode& node)
{
	return new DialogueRequirementCheckGlobalVariableFloat(node);
}

DialogueRequirement* DialogueReqCheckGlobalVariableFloatDefaultCreationFunc()
{
	return new DialogueRequirementCheckGlobalVariableFloat();
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Static Variables
STATIC const DialogueRequirementRegistrationHelper 
	DialogueRequirementCheckGlobalVariableFloat::s_CheckGlobalVariableFloatRegHelper
		= DialogueRequirementRegistrationHelper("GlobalCheckVariableFloat", 
			DialogueReqCheckGlobalVariableFloatCreationFunc, 
			DialogueReqCheckGlobalVariableFloatDefaultCreationFunc);

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Constructors
DialogueRequirementCheckGlobalVariableFloat::DialogueRequirementCheckGlobalVariableFloat()
	: DialogueRequirement()
{

}

DialogueRequirementCheckGlobalVariableFloat::DialogueRequirementCheckGlobalVariableFloat(const XMLNode& node)
	: DialogueRequirement(node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variable"
			|| attrName == "variablename")
		{
			SetVariableName(std::string(attr.lpszValue));
		}
		else if (attrName == "value")
		{
			std::string val = std::string(attr.lpszValue);
			QuestRequirement::ParseTextForSpecialCaseOperationsAndValue(val,
				m_specialCaseOperation, m_value);
		}
	}
}

DialogueRequirementCheckGlobalVariableFloat::DialogueRequirementCheckGlobalVariableFloat(
	const DialogueRequirementCheckGlobalVariableFloat& other)
	: DialogueRequirement(other),
	m_variableName(other.m_variableName),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName),
	m_value(other.m_value),
	m_specialCaseOperation(other.m_specialCaseOperation)
{

}

DialogueRequirementCheckGlobalVariableFloat::~DialogueRequirementCheckGlobalVariableFloat()
{
	DialogueRequirement::~DialogueRequirement();
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Setters
void DialogueRequirementCheckGlobalVariableFloat::SetVariableName(const std::string& name)
{
	m_variableName = name;
	m_lowerCaseVariableName = MakeLower(name);
}

void DialogueRequirementCheckGlobalVariableFloat::SetVariable(float variable)
{
	m_value = variable;
}

void DialogueRequirementCheckGlobalVariableFloat::SetSpecialCaseOperation(SpecialCaseOperations specialCase)
{
	m_specialCaseOperation = specialCase;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Getters
const std::string& DialogueRequirementCheckGlobalVariableFloat::GetVariableName() const
{
	return m_variableName;
}

const std::string& DialogueRequirementCheckGlobalVariableFloat::GetLowerCaseVariableName() const
{
	return m_lowerCaseVariableName;
}

float DialogueRequirementCheckGlobalVariableFloat::GetVariable() const
{
	return m_value;
}

SpecialCaseOperations DialogueRequirementCheckGlobalVariableFloat::GetSpecialCaseOperation() const
{
	return m_specialCaseOperation;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Operations
void DialogueRequirementCheckGlobalVariableFloat::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Requirement Name: Check Global Variable Float\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value searching for: ";
	GameDebuggingCommons::WriteSpecialCaseOperatorToString(str, m_specialCaseOperation);
	str	+= std::to_string(m_value) + "\n";
}

bool DialogueRequirementCheckGlobalVariableFloat::CheckIfRequirementIsMet() const
{
	if (m_lowerCaseVariableName == "")
	{
		return false;
	}

	float defaul = 0.f;
	float internalVal = 0.f;
	Map::GetGlobalVariable(m_lowerCaseVariableName, internalVal, defaul);

	switch (m_specialCaseOperation)
	{
	case IS_SAME:						return internalVal == m_value;
	case IS_LESS_THAN:					return internalVal < m_value;
	case IS_GREATER_THAN:				return internalVal > m_value;
	case IS_GREATER_THAN_OR_EQUAL_TO:	return internalVal >= m_value;
	case IS_LESS_THAN_OR_EQUAL_TO:		return internalVal <= m_value;
	case IS_NOT_EQUAL_TO:				return internalVal != m_value;
	default:							return false;
	}
}

DialogueRequirement* DialogueRequirementCheckGlobalVariableFloat::Clone() const
{
	return new DialogueRequirementCheckGlobalVariableFloat(*this);
}