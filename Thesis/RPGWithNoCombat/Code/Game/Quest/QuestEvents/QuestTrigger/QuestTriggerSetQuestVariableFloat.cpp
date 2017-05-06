#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerSetQuestVariableFloat.hpp"
#include "Game/Quest/Quest.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerSetQuestStatFunc(const XMLNode& node)
{
	return new QuestTriggerSetQuestVariableFloat(node);
}

QuestTrigger* QuestTriggerDefaultSetQuestStatFunc()
{
	return new QuestTriggerSetQuestVariableFloat();
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration
	QuestTriggerSetQuestVariableFloat::s_QuestTriggerSetQuestStatRegistration
		= QuestTriggerRegistration("SetQuestVariableFloat",
			QuestTriggerSetQuestStatFunc,
			QuestTriggerDefaultSetQuestStatFunc);

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Constructors
QuestTriggerSetQuestVariableFloat::QuestTriggerSetQuestVariableFloat()
	: QuestTrigger()
{

}

QuestTriggerSetQuestVariableFloat::QuestTriggerSetQuestVariableFloat(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{
	//<SetQuestVariable Variable='QuestComplete' value='2.0'/>
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
				m_value = 0.0f;
			}
			else if (attrVal == "true"
				|| attrVal == "t")
			{
				m_value = 1.0f;
			}
			else
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

QuestTriggerSetQuestVariableFloat::QuestTriggerSetQuestVariableFloat(const QuestTriggerSetQuestVariableFloat& other,
	Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_StatOption(other.m_StatOption),
	m_value(other.m_value),
	m_variableName(other.m_variableName),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName)
{

}

QuestTriggerSetQuestVariableFloat::~QuestTriggerSetQuestVariableFloat()
{
	QuestTrigger::~QuestTrigger();
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Setters
void QuestTriggerSetQuestVariableFloat::SetValue(float val)
{
	m_value = val;
}

void QuestTriggerSetQuestVariableFloat::SetVariableName(const std::string& variableName)
{
	m_variableName = variableName;
	m_lowerCaseVariableName = MakeLower(variableName);
}

void QuestTriggerSetQuestVariableFloat::SetWhatToDoWithStatOptions(QuestTriggerWhatToDoWithStatOptions statOption)
{
	m_StatOption = statOption;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Getters
const std::string& QuestTriggerSetQuestVariableFloat::GetVariableName() const
{
	return m_variableName;
}

const std::string& QuestTriggerSetQuestVariableFloat::GetLowerCaseVariableName() const
{
	return m_lowerCaseVariableName;
}

float QuestTriggerSetQuestVariableFloat::GetValue() const
{
	return m_value;
}

QuestTriggerWhatToDoWithStatOptions QuestTriggerSetQuestVariableFloat::GetWhatToDoWithStat() const
{
	return m_StatOption;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Operations
bool QuestTriggerSetQuestVariableFloat::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr
		|| m_lowerCaseVariableName == "")
	{
		return false;
	}
	bool validSetterOptionsAreSet = false;
	float variable = 0.f;
	m_owningQuest->GetVariable(m_lowerCaseVariableName, variable, 0.f);
	float toSet = 0.f;
	if (m_StatOption == INCREMENT)
	{
		validSetterOptionsAreSet = true;
		toSet = variable + m_value;
	}
	else if (m_StatOption == SET)
	{
		validSetterOptionsAreSet = true;
		toSet = m_value;
	}

	if (validSetterOptionsAreSet)
	{
		m_owningQuest->SetVariable(m_lowerCaseVariableName, toSet);
	}

	return validSetterOptionsAreSet;
}

QuestTrigger* QuestTriggerSetQuestVariableFloat::Clone(Quest* owningQuest) const
{
	QuestTriggerSetQuestVariableFloat* clone = new QuestTriggerSetQuestVariableFloat(*this, owningQuest);
	return clone;
}

void QuestTriggerSetQuestVariableFloat::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Set Quest Variable Float\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Variable Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Mode: ";

	switch (m_StatOption)
	{
	case SET:
		str += "SET\n";
		break;
	case INCREMENT:
		str += "INCREMENT\n";
		break;
	default:
		str += "UNKNOWN\n";
		break;
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value: " + std::to_string(m_value) + "\n";
}