#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerConvertLocalVariableToText.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerConvertLocalVaraibleToTextFunc(const XMLNode& node)
{
	return new QuestTriggerConvertLocalVariableToText(node);
}

QuestTrigger* QuestTriggerDefaultConvertLocalVaraibleToTextFunc()
{
	return new QuestTriggerConvertLocalVariableToText();
}

//Static Variables
STATIC QuestTriggerRegistration 
	QuestTriggerConvertLocalVariableToText::s_ConvertLocalVariableToLocalTextRegistration
		= QuestTriggerRegistration("ConvertLocalVariableToText",
			QuestTriggerConvertLocalVaraibleToTextFunc, 
			QuestTriggerDefaultConvertLocalVaraibleToTextFunc);

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Constructors
QuestTriggerConvertLocalVariableToText::QuestTriggerConvertLocalVariableToText()
	: QuestTrigger()
{

}

QuestTriggerConvertLocalVariableToText::QuestTriggerConvertLocalVariableToText(
	const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "textvariablename"
			|| attrName == "textvariable")
		{
			m_TextVariableName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "variablename"
			|| attrName == "variablenameconverting")
		{
			m_VariableNameConverting = MakeLower(attr.lpszValue);
		}
		else if (attrName == "floatdecimallimit"
			|| attrName == "iffloatthenusethisdecimallimit"
			|| attrName == "decimallimit")
		{
			m_IfFloatThenUseThisDecimalLimit = atoi(attr.lpszValue);
		}
		else if (attrName == "globaltext"
			|| attrName == "globaltextvariable"
			|| attrName == "setasglobaltext"
			|| attrName == "setasglobaltextvariable")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "true"
				|| attrVal == "t")
			{
				m_IsGlobalText = true;
			}
			else if(attrVal == "false"
				|| attrVal == "f")
			{
				m_IsGlobalText = false;
			}
		}
	}
}

QuestTriggerConvertLocalVariableToText::QuestTriggerConvertLocalVariableToText(
	const QuestTriggerConvertLocalVariableToText& other,
	Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_TextVariableName(other.m_TextVariableName),
	m_VariableNameConverting(other.m_VariableNameConverting),
	m_IsGlobalText(other.m_IsGlobalText),
	m_IfFloatThenUseThisDecimalLimit(other.m_IfFloatThenUseThisDecimalLimit)
{

}

QuestTriggerConvertLocalVariableToText::~QuestTriggerConvertLocalVariableToText()
{
	QuestTrigger::~QuestTrigger();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Setters
void QuestTriggerConvertLocalVariableToText::SetTextVariableName(const std::string& textVariableName)
{
	m_TextVariableName = textVariableName;
}

void QuestTriggerConvertLocalVariableToText::SetVariableNameConverting(
	const std::string& variableNameConverting)
{
	m_VariableNameConverting = variableNameConverting;
}

void QuestTriggerConvertLocalVariableToText::SetFloatLimit(int limit)
{
	m_IfFloatThenUseThisDecimalLimit = limit;
}

void QuestTriggerConvertLocalVariableToText::SetIsGlobalTextOut(bool globalTextVariable)
{
	m_IsGlobalText = globalTextVariable;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Getters
const std::string& QuestTriggerConvertLocalVariableToText::GetTextVariableName() const
{
	return m_TextVariableName;
}

const std::string& QuestTriggerConvertLocalVariableToText::GetVariableNameConverting() const
{
	return m_VariableNameConverting;
}

int QuestTriggerConvertLocalVariableToText::GetFloatDecimalLimit() const
{
	return m_IfFloatThenUseThisDecimalLimit;
}

bool QuestTriggerConvertLocalVariableToText::GetIsGlobalTextVariable() const
{
	return m_IsGlobalText;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Operations
bool QuestTriggerConvertLocalVariableToText::PerformQuestTrigger()
{
	if (m_TextVariableName == ""
		|| m_VariableNameConverting == ""
		|| m_owningQuest == nullptr)
	{
		return false;
	}

	float varAttempt1 = 0.0f;
	PropertyGetResult result = m_owningQuest->GetVariable(m_VariableNameConverting, varAttempt1, 0.0f);

	if (result == PROPERTY_FETCH_SUCCESS)
	{
		std::string stringToSet = std::to_string(varAttempt1);
		ParseStringForFloatHandling(stringToSet);
		SetStringToVariableName(stringToSet);

		return true;
	}

	bool varAttempt3 = false;
	result = m_owningQuest->GetVariable(m_VariableNameConverting, varAttempt3, false);
	if (result == PROPERTY_FETCH_SUCCESS)
	{
		std::string stringToSet = "";
		if (varAttempt3 == false)
		{
			stringToSet = "false";
		}
		else
		{
			stringToSet = "true";
		}
		SetStringToVariableName(stringToSet);


		return true;
	}

	int varAttempt2 = 0;
	result = m_owningQuest->GetVariable(m_VariableNameConverting, varAttempt2, 0);
	if (result == PROPERTY_FETCH_SUCCESS)
	{
		std::string stringToSet = std::to_string(varAttempt2);
		SetStringToVariableName(stringToSet);

		return true;
	}


	return false;
}

QuestTrigger* QuestTriggerConvertLocalVariableToText::Clone(Quest* owningQuest) const
{
	return new QuestTriggerConvertLocalVariableToText(*this, owningQuest);
}

void QuestTriggerConvertLocalVariableToText::SetStringToVariableName(const std::string& toSet)
{
	if (m_IsGlobalText)
	{
		Map::SetGlobalVariable(m_TextVariableName, toSet);
	}
	else
	{
		m_owningQuest->SetVariable(m_TextVariableName, toSet);
	}
}

void QuestTriggerConvertLocalVariableToText::ParseStringForFloatHandling(std::string& string)
{
	bool decimalEncountered = false;
	int numberOfCharsPassedAfterDecimal = 0;
	int idxAfterBreak = -1;
	for (size_t idx = 0; idx < string.length(); idx++)
	{
		std::string car = std::string(string.at(idx), 1);
		if (!decimalEncountered && car == ".")
		{
			decimalEncountered = true;
		}
		else if (decimalEncountered)
		{
			numberOfCharsPassedAfterDecimal++;
			if (numberOfCharsPassedAfterDecimal >= m_IfFloatThenUseThisDecimalLimit)
			{
				idxAfterBreak = (int)idx;
				break;
			}
		}
	}
	if (idxAfterBreak == -1)
	{
		return;
	}
	string = string.substr(0, idxAfterBreak);
}

void QuestTriggerConvertLocalVariableToText::WriteQuestTriggerToString(std::string& str, 
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Convert Local Variable to Text\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Name of Variable to convert: " + m_VariableNameConverting + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Text Variable Name: " + m_TextVariableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Text should be global variable: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_IsGlobalText);

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of decimal places to limit, if variable is float: "
		+ std::to_string(m_IfFloatThenUseThisDecimalLimit) + "\n";
}