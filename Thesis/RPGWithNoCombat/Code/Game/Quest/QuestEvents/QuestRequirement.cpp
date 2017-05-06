#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Quest/Quest.hpp"
#include "Engine/core/EngineXMLParser.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementCheckVariableType.hpp"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Constructors
QuestRequirement::QuestRequirement(QuestRequirementType requirementType)
	: m_questRequirementType(requirementType)
{

}

QuestRequirement::QuestRequirement(QuestRequirementType requirementType, const XMLNode& node)
	: m_questRequirementType(requirementType)
{
	node;
}

QuestRequirement::QuestRequirement(const QuestRequirement& other, Quest* owningQuest)
	: m_questRequirementType(other.m_questRequirementType),
	m_owningQuest(owningQuest)
{

}

QuestRequirement::~QuestRequirement()
{

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Setters
void QuestRequirement::SetQuestOwnedBy(Quest* quest)
{
	m_owningQuest = quest;
}

void QuestRequirement::SetQuestRequirementType(QuestRequirementType type)
{
	m_questRequirementType = type;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Getters
Quest* QuestRequirement::GetQuestOwnedBy() const
{
	return m_owningQuest;
}

QuestRequirementType QuestRequirement::GetQuestRequirementType() const
{
	return m_questRequirementType;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//operations
const bool QuestRequirement::DeterminePlayerMeetsQuestRequirement() const
{
	return false;
}


//Static Functions
STATIC QuestRequirementType QuestRequirement::GetQuestRequirementTypeShouldBe(const XMLNode& node)
{
	std::string nodeName = MakeLower(node.getName());
	if (nodeName == "stats")
	{
		return Check_Player_Stats;
	}
	else if (nodeName == "requirement")
	{
		for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = node.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "value")
			{
				return Check_Variable;
			}
			else if (attrName == "interaction" || attrName == "interact")
			{
				return Player_Interacting_With;
			}
		}
	}
	return INVALID_QUEST_REQUIREMENT_TYPE;
}

STATIC QuestRequirementCheckVariableType QuestRequirement::GetTypeOfCheckVariableShouldBe(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "value")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "f"
				|| val == "t"
				|| val == "false"
				|| val == "true")
			{
				return Bool_Type_Check_Variable;
			}
			else
			{
				return Float_Type_Check_Variable;
			}
		}
	}
	return INVALID_CHECK_VARIABLE_TYPE;
}

STATIC std::string QuestRequirement::GetQuestRequirementTypeAndQuestRequirementCheckVariableType(const XMLNode& node)
{
	std::string nodeName = MakeLower(node.getName());
	std::string typeToCall = "";
	if (nodeName == "stats")
	{
		//lets assume check player stats
		typeToCall = "CheckPlayerStats";
	}
	else if (typeToCall != "requirement")
	{
		typeToCall = nodeName;
	}


	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "requirementtype"
			|| attrName == "type")
		{
			typeToCall = std::string(attr.lpszValue);
			break;
		}
	}
	return typeToCall;
}

SpecialCaseOperations QuestRequirement::GetSpecialCaseOperationForStringContents(
	const std::string& contents)
{
	SpecialCaseOperations operation = IS_SAME;
	if (contents == "==" || contents == "=")
	{
		operation = IS_SAME;
	}
	else if (contents == ">")
	{
		operation = IS_GREATER_THAN;
	}
	else if (contents == "<")
	{
		operation = IS_LESS_THAN;
	}
	else if (contents == ">=")
	{
		operation = IS_GREATER_THAN_OR_EQUAL_TO;
	}
	else if (contents == "<=")
	{
		operation = IS_LESS_THAN_OR_EQUAL_TO;
	}
	else if (contents == "!=")
	{
		operation = IS_NOT_EQUAL_TO;
	}
	return operation;
}

void QuestRequirement::ParseTextForSpecialCaseOperationsAndValue(const std::string& str,
	SpecialCaseOperations& operation, float& value)
{
	size_t numOfSpecialChars = 0;
	for (size_t idx = 0; idx < str.length(); idx++)
	{
		unsigned char car = str[idx];
		if (car == '='
			|| car == '<'
			|| car == '>'
			|| car == '!')
		{
		}
		else
		{
			numOfSpecialChars = idx;
			break;
		}
	}
	if (numOfSpecialChars > 0)
	{
		std::string operatorStr = str.substr(0, numOfSpecialChars);
		std::string numbers = str.substr(numOfSpecialChars, str.length() - numOfSpecialChars);
		value = (float)atof(numbers.c_str());
		operation = GetSpecialCaseOperationForStringContents(operatorStr);
	}
	else
	{
		value = (float)atof(str.c_str());
	}
}

void QuestRequirement::ParseTextForSpecialCaseOperationsAndValue(const std::string& str,
	SpecialCaseOperations& operation, int& value)
{
	size_t numOfSpecialChars = 0;
	for (size_t idx = 0; idx < str.length(); idx++)
	{
		unsigned char car = str[idx];
		if (car == '='
			|| car == '<'
			|| car == '>'
			|| car == '!')
		{
		}
		else
		{
			numOfSpecialChars = idx;
			break;
		}
	}
	if (numOfSpecialChars > 0)
	{
		std::string operatorStr = str.substr(0, numOfSpecialChars);
		std::string numbers = str.substr(numOfSpecialChars, str.length() - numOfSpecialChars);
		value = atoi(numbers.c_str());
		operation = GetSpecialCaseOperationForStringContents(operatorStr);
	}
	else
	{
		value = atoi(str.c_str());
	}
}