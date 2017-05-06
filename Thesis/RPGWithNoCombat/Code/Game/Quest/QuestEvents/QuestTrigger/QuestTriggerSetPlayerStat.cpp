#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerSetPlayerStat.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerSetPlayerStatFunc(const XMLNode& node)
{
	return new QuestTriggerSetPlayerStat(node);
}

QuestTrigger* QuestTriggerDefaultSetPlayerStatFunc()
{
	return new QuestTriggerSetPlayerStat();
}


//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration 
	QuestTriggerSetPlayerStat::s_SetPlayerStatRegistration 
		= QuestTriggerRegistration("SetPlayerStat",
		QuestTriggerSetPlayerStatFunc,
		QuestTriggerDefaultSetPlayerStatFunc);

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Constructors
QuestTriggerSetPlayerStat::QuestTriggerSetPlayerStat()
	: QuestTrigger()
{

}

QuestTriggerSetPlayerStat::QuestTriggerSetPlayerStat(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variable"
			|| attrName == "variableName")
		{
			SetVariableName(attr.lpszValue, false);
		}
		else if (attrName == "variableAbbreviation"
			|| attrName == "abbreviation")
		{
			SetVariableName(attr.lpszValue, true);
		}
		else if (attrName == "value")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "false"
				|| attrVal == "f")
			{
				m_value = 0;
			}
			else if (attrVal == "true"
				|| attrVal == "t")
			{
				m_value = 1;
			}
			else
			{
				m_value = atoi(attrVal.c_str());
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

QuestTriggerSetPlayerStat::QuestTriggerSetPlayerStat(const QuestTriggerSetPlayerStat& other, Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_value(other.m_value),
	m_variableName(other.m_variableName),
	m_StatOption(other.m_StatOption)
{

}

QuestTriggerSetPlayerStat::~QuestTriggerSetPlayerStat()
{
	QuestTrigger::~QuestTrigger();
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Setters
void QuestTriggerSetPlayerStat::SetValue(int val)
{
	m_value = val;
}
void QuestTriggerSetPlayerStat::SetVariableName(const std::string variableName, 
	bool variableIsAbbreviation)
{
	m_variableName = variableName;
	m_lowerCaseVariableName = MakeLower(variableName);
	m_variableNameIsAbbreviation = variableIsAbbreviation;
}
void QuestTriggerSetPlayerStat::SetWhatToDoWithStatOptions(QuestTriggerWhatToDoWithStatOptions statOption)
{
	m_StatOption = statOption;
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Getters
const std::string& QuestTriggerSetPlayerStat::GetVariableName() const
{
	return m_variableName;
}

int QuestTriggerSetPlayerStat::GetValue() const
{
	return m_value;
}

QuestTriggerWhatToDoWithStatOptions QuestTriggerSetPlayerStat::GetWhatToDoWithStat() const
{
	return m_StatOption;
}

bool QuestTriggerSetPlayerStat::GetVariableNameIsAbbreviation() const
{
	return m_variableNameIsAbbreviation;
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Operations
bool QuestTriggerSetPlayerStat::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr
		|| m_variableName == "")
	{
		return false;
	}
	BaseGameEntity* player = BaseGameEntity::GetEditablePointerToPlayer();
	if (player == nullptr)
	{
		return false;
	}
	Player* playerPtr = dynamic_cast<Player*>(player);
	if (playerPtr == nullptr)
	{
		return false;
	}
	bool validSetterOptionsAreSet = false;
	bool found = false;
	int orgVal = playerPtr->GetValOfStatByName(m_lowerCaseVariableName, found);
	int toSet = 0;
	if (m_StatOption == INCREMENT)
	{
		validSetterOptionsAreSet = true;
		toSet = orgVal + m_value;
	}
	else if (m_StatOption == SET)
	{
		validSetterOptionsAreSet = true;
		toSet = m_value;
	}

	if (validSetterOptionsAreSet)
	{
		if (m_variableNameIsAbbreviation == false)
		{
			playerPtr->SetStatByName(m_lowerCaseVariableName, toSet);
		}
		else
		{
			playerPtr->SetStatByAbbreviation(m_lowerCaseVariableName, toSet);
		}
	}

	return validSetterOptionsAreSet;
}

QuestTrigger* QuestTriggerSetPlayerStat::Clone(Quest* owningQuest) const
{
	QuestTriggerSetPlayerStat* clone = new QuestTriggerSetPlayerStat(*this, owningQuest);
	return clone;
}

void QuestTriggerSetPlayerStat::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Set Player Stat";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Stat Name: " + m_variableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Amount to set to or add: " + std::to_string(m_value) + "\n";

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

}