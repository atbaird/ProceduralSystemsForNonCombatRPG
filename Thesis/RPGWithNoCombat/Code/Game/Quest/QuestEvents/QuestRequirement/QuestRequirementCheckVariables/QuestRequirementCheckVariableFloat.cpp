#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementCheckVariableFloat.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Creation Funcs
QuestRequirement* CheckVariableFloatCreationFunc(const XMLNode& node)
{
	QuestRequirementCheckVariableFloat* var = new QuestRequirementCheckVariableFloat(node);
	return var;
}

QuestRequirement* CheckVariableFloatDefaultCreationFunc()
{
	QuestRequirementCheckVariableFloat* var = new QuestRequirementCheckVariableFloat();
	return var;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Static Variables
STATIC QuestRequirementRegistration QuestRequirementCheckVariableFloat::s_CheckVariableFloatRegistration
	= QuestRequirementRegistration("CheckVariableFloat", 
		CheckVariableFloatCreationFunc, CheckVariableFloatDefaultCreationFunc);

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Constructors
QuestRequirementCheckVariableFloat::QuestRequirementCheckVariableFloat()
	: QuestRequirementCheckVariableBase(Float_Type_Check_Variable)
{
}

QuestRequirementCheckVariableFloat::QuestRequirementCheckVariableFloat(const XMLNode& node)
	: QuestRequirementCheckVariableBase(Float_Type_Check_Variable, node)
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

QuestRequirementCheckVariableFloat::QuestRequirementCheckVariableFloat(const QuestRequirementCheckVariableFloat& other,
	Quest* owningQuest)
	: QuestRequirementCheckVariableBase(other, owningQuest),
	m_value(other.m_value)
{

}

QuestRequirementCheckVariableFloat::~QuestRequirementCheckVariableFloat()
{
	QuestRequirementCheckVariableBase::~QuestRequirementCheckVariableBase();
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Setters
void QuestRequirementCheckVariableFloat::SetVariable(float var)
{
	m_value = var;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Getters
float QuestRequirementCheckVariableFloat::GetVariable() const
{
	return m_value;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Operations
void QuestRequirementCheckVariableFloat::WriteQuestRequirementToString(std::string& str, 
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Check Variable Float\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value Desired: ";
	GameDebuggingCommons::WriteSpecialCaseOperatorToString(str, m_specialCaseOperation);
	str += std::to_string(m_value) + "\n";
}

const bool QuestRequirementCheckVariableFloat::DeterminePlayerMeetsQuestRequirement() const
{
	if (m_owningQuest == nullptr
		|| m_lowerCaseVariableName == "")
	{
		return false;
	}
	float defaul = 0.f;
	float internalVal = 0.f;
	m_owningQuest->GetVariable<float>(m_lowerCaseVariableName, internalVal, defaul);

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

QuestRequirement* QuestRequirementCheckVariableFloat::Clone(Quest* owningQuest) const
{
	QuestRequirementCheckVariableFloat* clone = new QuestRequirementCheckVariableFloat(*this, owningQuest);
	return clone;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Static Functions