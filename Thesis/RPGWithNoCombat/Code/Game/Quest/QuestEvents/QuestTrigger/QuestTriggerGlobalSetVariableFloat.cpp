#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerGlobalSetVariableFloat.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerGlobalSetVariableFloatFunc(const XMLNode& node)
{
	return new QuestTriggerGlobalSetVariableFloat(node);
}

QuestTrigger* QuestTriggerDefaultGlobalSetVariableFloatFunc()
{
	return new QuestTriggerGlobalSetVariableFloat();
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration 
	QuestTriggerGlobalSetVariableFloat::s_QuestTriggerGlobalSetVariableFloatRegistration
		= QuestTriggerRegistration("GlobalSetVariableFloat", QuestTriggerGlobalSetVariableFloatFunc,
			QuestTriggerDefaultGlobalSetVariableFloatFunc);

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Constructors
QuestTriggerGlobalSetVariableFloat::QuestTriggerGlobalSetVariableFloat()
	: QuestTrigger()
{

}

QuestTriggerGlobalSetVariableFloat::QuestTriggerGlobalSetVariableFloat(
	const XMLNode& node, Quest* owningQuest)
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

QuestTriggerGlobalSetVariableFloat::QuestTriggerGlobalSetVariableFloat(
	const QuestTriggerGlobalSetVariableFloat& other, Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_variableName(other.m_variableName),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName),
	m_StatOption(other.m_StatOption),
	m_value(other.m_value)
{

}

QuestTriggerGlobalSetVariableFloat::~QuestTriggerGlobalSetVariableFloat()
{
	QuestTrigger::~QuestTrigger();
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Setters
void QuestTriggerGlobalSetVariableFloat::SetValue(float val)
{
	m_value = val;
}

void QuestTriggerGlobalSetVariableFloat::SetVariableName(const std::string variableName)
{
	m_variableName = variableName;
	m_lowerCaseVariableName = MakeLower(variableName);
}

void QuestTriggerGlobalSetVariableFloat::SetWhatToDoWithStatOptions(QuestTriggerWhatToDoWithStatOptions statOption)
{
	m_StatOption = statOption;
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Getters
const std::string& QuestTriggerGlobalSetVariableFloat::GetVariableName() const
{
	return m_variableName;
}

const std::string& QuestTriggerGlobalSetVariableFloat::GetLowerCaseVariableName() const
{
	return m_lowerCaseVariableName;
}

float QuestTriggerGlobalSetVariableFloat::GetValue() const
{
	return m_value;
}

QuestTriggerWhatToDoWithStatOptions QuestTriggerGlobalSetVariableFloat::GetWhatToDoWithStat() const
{
	return m_StatOption;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Operations
bool QuestTriggerGlobalSetVariableFloat::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr
		|| m_lowerCaseVariableName == "")
	{
		return false;
	}
	float valueToSet = m_value;
	if (m_StatOption == INCREMENT)
	{
		float val = 0.f;
		Map::GetGlobalVariable(m_lowerCaseVariableName, val, 0.f);
		valueToSet += val;
	}

	Map::SetGlobalVariable(m_lowerCaseVariableName, valueToSet);

	return true;
}

QuestTrigger* QuestTriggerGlobalSetVariableFloat::Clone(Quest* owningQuest) const
{
	return new QuestTriggerGlobalSetVariableFloat(*this, owningQuest);
}

void QuestTriggerGlobalSetVariableFloat::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Global Set Variable Float\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Mode: ";
	switch (m_StatOption)
	{
	case INCREMENT:
		str += "INCREMENT\n";
	case SET:
		str += "SET\n";
	default:
		str += "UNKNOWN\n";
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value: " + std::to_string(m_value) + "\n";
}