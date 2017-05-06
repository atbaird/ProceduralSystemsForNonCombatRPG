#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerSetQuestVariableBool.hpp"
#include "Game/Quest/Quest.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerSetQuestVariableBoolFunc(const XMLNode& node)
{
	return new QuestTriggerSetQuestVariableBool(node);
}

QuestTrigger* QuestTriggerDefaultSetQuestVariableBoolFunc()
{
	return new QuestTriggerSetQuestVariableBool();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration QuestTriggerSetQuestVariableBool::s_QuestTriggerSetQuestStatRegistration
			= QuestTriggerRegistration("SetQuestVariableBool",
				QuestTriggerSetQuestVariableBoolFunc, QuestTriggerDefaultSetQuestVariableBoolFunc);

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Constructors
QuestTriggerSetQuestVariableBool::QuestTriggerSetQuestVariableBool()
	: QuestTrigger()
{

}

QuestTriggerSetQuestVariableBool::QuestTriggerSetQuestVariableBool(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
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
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "false"
				|| attrVal == "f")
			{
				m_value = false;
			}
			else if (attrVal == "true"
				|| attrVal == "t")
			{
				m_value = true;
			}
		}
	}
}

QuestTriggerSetQuestVariableBool::QuestTriggerSetQuestVariableBool(const QuestTriggerSetQuestVariableBool& other,
	Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_value(other.m_value),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName),
	m_variableName(other.m_variableName)
{

}

QuestTriggerSetQuestVariableBool::~QuestTriggerSetQuestVariableBool()
{
	QuestTrigger::~QuestTrigger();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Setters
void QuestTriggerSetQuestVariableBool::SetValue(bool val)
{
	m_value = val;
}

void QuestTriggerSetQuestVariableBool::SetVariableName(const std::string& variableName)
{
	m_variableName = variableName;
	m_lowerCaseVariableName = MakeLower(variableName);
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Getters
const std::string& QuestTriggerSetQuestVariableBool::GetVariableName() const
{
	return m_variableName;
}

const std::string& QuestTriggerSetQuestVariableBool::GetLowerCaseVariableName() const
{
	return m_lowerCaseVariableName;
}

bool QuestTriggerSetQuestVariableBool::GetValue() const
{
	return m_value;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Operations
bool QuestTriggerSetQuestVariableBool::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr
		|| m_lowerCaseVariableName == "")
	{
		return false;
	}

	{
		m_owningQuest->SetVariable(m_lowerCaseVariableName, m_value);
	}

	return true;
}

QuestTrigger* QuestTriggerSetQuestVariableBool::Clone(Quest* owningQuest) const
{
	return new QuestTriggerSetQuestVariableBool(*this, owningQuest);
}

void QuestTriggerSetQuestVariableBool::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Set Quest Variable Bool\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_value);

}