#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementGlobalCheckVariableBool.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Creation Funcs
QuestRequirement* GlobalCheckVariableBoolCreationFunc(const XMLNode& node)
{
	return new QuestRequirementGlobalCheckVariableBool(node);
}

QuestRequirement* GlobalCheckVariableBoolDefaultCreationFunc()
{
	return new QuestRequirementGlobalCheckVariableBool();
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Static Variables
STATIC QuestRequirementRegistration 
	QuestRequirementGlobalCheckVariableBool::s_GlobalCheckVariableBoolRegistration = 
	QuestRequirementRegistration("GlobalCheckVariableBool",
		GlobalCheckVariableBoolCreationFunc, GlobalCheckVariableBoolDefaultCreationFunc);

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Constructors
QuestRequirementGlobalCheckVariableBool::QuestRequirementGlobalCheckVariableBool()
	: QuestRequirementCheckVariableBase(Global_Bool_Type_Check_Variable)
{
}

QuestRequirementGlobalCheckVariableBool::QuestRequirementGlobalCheckVariableBool(const XMLNode& node)
	: QuestRequirementCheckVariableBase(Global_Bool_Type_Check_Variable, node)
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

QuestRequirementGlobalCheckVariableBool::QuestRequirementGlobalCheckVariableBool(
	const QuestRequirementGlobalCheckVariableBool& other,
	Quest* owningQuest)
	: QuestRequirementCheckVariableBase(other, owningQuest),
	m_value(other.m_value)
{
}

QuestRequirementGlobalCheckVariableBool::~QuestRequirementGlobalCheckVariableBool()
{
	QuestRequirementCheckVariableBase::~QuestRequirementCheckVariableBase();
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Setters
void QuestRequirementGlobalCheckVariableBool::SetVariable(bool var)
{
	m_value = var;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Getters
bool QuestRequirementGlobalCheckVariableBool::GetVariable() const
{
	return m_value;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Operations
void QuestRequirementGlobalCheckVariableBool::WriteQuestRequirementToString(std::string& str,
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Global Check Variable Bool\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_value);

}

const bool QuestRequirementGlobalCheckVariableBool::DeterminePlayerMeetsQuestRequirement() const
{
	if (m_owningQuest == nullptr
		|| m_lowerCaseVariableName == "")
	{
		return false;
	}
	bool val = false;

	Map::GetGlobalVariable(m_lowerCaseVariableName, val, false);
	return val == m_value;
}

QuestRequirement* QuestRequirementGlobalCheckVariableBool::Clone(Quest* owningQuest) const
{
	return new QuestRequirementGlobalCheckVariableBool(*this, owningQuest);
}