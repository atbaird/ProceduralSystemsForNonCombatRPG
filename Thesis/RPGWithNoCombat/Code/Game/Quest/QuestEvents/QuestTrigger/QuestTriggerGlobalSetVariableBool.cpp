#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerGlobalSetVariableBool.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerGlobalSetVariableBoolFunc(const XMLNode& node)
{
	return new QuestTriggerGlobalSetVariableBool(node);
}

QuestTrigger* QuestTriggerDefaultGlobalSetVariableBoolFunc()
{
	return new QuestTriggerGlobalSetVariableBool();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration 
	QuestTriggerGlobalSetVariableBool::s_QuestTriggerGlobalSetVariableBoolRegistration
		= QuestTriggerRegistration("GlobalSetVariableBool", 
			QuestTriggerGlobalSetVariableBoolFunc, QuestTriggerDefaultGlobalSetVariableBoolFunc);

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Constructors
QuestTriggerGlobalSetVariableBool::QuestTriggerGlobalSetVariableBool()
	: QuestTrigger()
{

}

QuestTriggerGlobalSetVariableBool::QuestTriggerGlobalSetVariableBool(const XMLNode& node, Quest* owningQuest)
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

QuestTriggerGlobalSetVariableBool::QuestTriggerGlobalSetVariableBool(const QuestTriggerGlobalSetVariableBool& other,
	Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_variableName(other.m_variableName),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName),
	m_value(other.m_value)
{

}

QuestTriggerGlobalSetVariableBool::~QuestTriggerGlobalSetVariableBool()
{
	QuestTrigger::~QuestTrigger();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Setters
void QuestTriggerGlobalSetVariableBool::SetValue(bool val)
{
	m_value = val;
}

void QuestTriggerGlobalSetVariableBool::SetVariableName(const std::string variableName)
{
	m_variableName;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Getters
const std::string& QuestTriggerGlobalSetVariableBool::GetVariableName() const
{
	return m_variableName;
}

const std::string& QuestTriggerGlobalSetVariableBool::GetLowerCaseVariableName() const
{
	return m_lowerCaseVariableName;
}

bool QuestTriggerGlobalSetVariableBool::GetValue() const
{
	return m_value;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Operations
bool QuestTriggerGlobalSetVariableBool::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr
		|| m_lowerCaseVariableName == "")
	{
		return false;
	}
	Map::SetGlobalVariable(m_lowerCaseVariableName, m_value);
	return true;
}

QuestTrigger* QuestTriggerGlobalSetVariableBool::Clone(Quest* owningQuest) const
{
	return new QuestTriggerGlobalSetVariableBool(*this, owningQuest);
}

void QuestTriggerGlobalSetVariableBool::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Global Set Variable Bool\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_value);
}