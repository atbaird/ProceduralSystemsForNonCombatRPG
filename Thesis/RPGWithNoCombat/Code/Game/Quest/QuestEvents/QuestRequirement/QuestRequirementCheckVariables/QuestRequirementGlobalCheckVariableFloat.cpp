#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementGlobalCheckVariableFloat.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Creation Funcs
QuestRequirement* GlobalCheckVariableFloatCreationFunc(const XMLNode& node)
{
	return new QuestRequirementGlobalCheckVariableFloat(node);
}

QuestRequirement* GlobalCheckVariableFloatDefaultCreationFunc()
{
	return new QuestRequirementGlobalCheckVariableFloat();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Static Variables
STATIC QuestRequirementRegistration QuestRequirementGlobalCheckVariableFloat::s_GlobalCheckVariableFloatRegistration
	= QuestRequirementRegistration("GlobalCheckVariableFloat", 
		GlobalCheckVariableFloatCreationFunc, GlobalCheckVariableFloatDefaultCreationFunc);

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Constructors
QuestRequirementGlobalCheckVariableFloat::QuestRequirementGlobalCheckVariableFloat()
	: QuestRequirementCheckVariableBase(Global_Float_Type_Check_Variable)
{

}

QuestRequirementGlobalCheckVariableFloat::QuestRequirementGlobalCheckVariableFloat(const XMLNode& node)
	: QuestRequirementCheckVariableBase(Global_Float_Type_Check_Variable, node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "value")
		{
			std::string val = std::string(attr.lpszValue);
			ParseTextForSpecialCaseOperationsAndValue(val, m_specialCaseOperation, m_value);
		}
	}
}

QuestRequirementGlobalCheckVariableFloat::QuestRequirementGlobalCheckVariableFloat(
	const QuestRequirementGlobalCheckVariableFloat& other,
	Quest* owningQuest)
	: QuestRequirementCheckVariableBase(other, owningQuest),
	m_value(other.m_value)
{
}

QuestRequirementGlobalCheckVariableFloat::~QuestRequirementGlobalCheckVariableFloat()
{
	QuestRequirementCheckVariableBase::~QuestRequirementCheckVariableBase();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Setters
void QuestRequirementGlobalCheckVariableFloat::SetVariable(float var)
{
	m_value = var;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Getters
float QuestRequirementGlobalCheckVariableFloat::GetVariable() const
{
	return m_value;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Operations
void QuestRequirementGlobalCheckVariableFloat::WriteQuestRequirementToString(std::string& str,
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Global Check Variable Float\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value Desired: ";
	GameDebuggingCommons::WriteSpecialCaseOperatorToString(str, m_specialCaseOperation);
	str += std::to_string(m_value) + "\n";
}

const bool QuestRequirementGlobalCheckVariableFloat::DeterminePlayerMeetsQuestRequirement() const
{
	if (m_owningQuest == nullptr
		|| m_lowerCaseVariableName == "")
	{
		return false;
	}
	float defaul = 0.f;
	float internalVal = 0.f;

	//need to use internal val instead to pull out of a global set of variables.
	//m_owningQuest->GetVariable<float>(m_variableName, internalVal, defaul);
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

QuestRequirement* QuestRequirementGlobalCheckVariableFloat::Clone(Quest* owningQuest) const
{
	return new QuestRequirementGlobalCheckVariableFloat(*this, owningQuest);
}