#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementCheckVariableBool.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Creation Funcs
QuestRequirement* CheckVariableBoolCreationFunc(const XMLNode& node)
{
	QuestRequirementCheckVariableBool* val = new QuestRequirementCheckVariableBool(node);
	return val;
}

QuestRequirement* CheckVariableBoolDefaultCreationFunc()
{
	QuestRequirementCheckVariableBool* val = new QuestRequirementCheckVariableBool();
	return val;
}

//Static Variables
STATIC QuestRequirementRegistration 
	QuestRequirementCheckVariableBool::s_CheckVariableBoolRegistration
	= QuestRequirementRegistration("CheckVariableBool", 
		CheckVariableBoolCreationFunc, CheckVariableBoolDefaultCreationFunc);

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Constructors
QuestRequirementCheckVariableBool::QuestRequirementCheckVariableBool()
	: QuestRequirementCheckVariableBase(Bool_Type_Check_Variable)
{

}

QuestRequirementCheckVariableBool::QuestRequirementCheckVariableBool(const XMLNode& node)
	: QuestRequirementCheckVariableBase(Bool_Type_Check_Variable, node)
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
	}
}

QuestRequirementCheckVariableBool::QuestRequirementCheckVariableBool(const QuestRequirementCheckVariableBool& other,
	Quest* owningQuest)
	: QuestRequirementCheckVariableBase(other, owningQuest),
	m_value(other.m_value)
{

}

QuestRequirementCheckVariableBool::~QuestRequirementCheckVariableBool()
{
	QuestRequirementCheckVariableBase::~QuestRequirementCheckVariableBase();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Setters
void QuestRequirementCheckVariableBool::SetVariable(bool var)
{
	m_value = var;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Getters
bool QuestRequirementCheckVariableBool::GetVariable() const
{
	return m_value;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Operations
void QuestRequirementCheckVariableBool::WriteQuestRequirementToString(std::string& str,
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Check Variable Bool\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_value);


}

const bool QuestRequirementCheckVariableBool::DeterminePlayerMeetsQuestRequirement() const
{
	if (m_owningQuest == nullptr
		|| m_lowerCaseVariableName == "")
	{
		return false;
	}
	bool val = false;
	m_owningQuest->GetVariable(m_lowerCaseVariableName, val, false);
	return val == m_value;
}

QuestRequirement* QuestRequirementCheckVariableBool::Clone(Quest* owningQuest) const
{
	QuestRequirementCheckVariableBool* val = new QuestRequirementCheckVariableBool(*this, owningQuest);
	return val;
}