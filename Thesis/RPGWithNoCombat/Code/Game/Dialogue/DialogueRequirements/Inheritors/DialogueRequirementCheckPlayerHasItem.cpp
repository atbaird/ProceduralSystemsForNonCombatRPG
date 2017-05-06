#include "Game/Dialogue/DialogueRequirements/Inheritors/DialogueRequirementCheckPlayerHasItem.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Spawning Functions
DialogueRequirement* DialogueReqCheckPlayerHasItemCreationFunc(const XMLNode& node)
{
	return new DialogueRequirementCheckPlayerHasItem(node);
}

DialogueRequirement* DialogueReqCheckPlayerHasItemDefaultCreationFunc()
{
	return new DialogueRequirementCheckPlayerHasItem();
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Static Variables
STATIC const DialogueRequirementRegistrationHelper 
	DialogueRequirementCheckPlayerHasItem::s_CheckPlayerHasItemRegHelper
		= DialogueRequirementRegistrationHelper("CheckPlayerHasItem", 
			DialogueReqCheckPlayerHasItemCreationFunc,
			DialogueReqCheckPlayerHasItemDefaultCreationFunc);

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Constructors
DialogueRequirementCheckPlayerHasItem::DialogueRequirementCheckPlayerHasItem()
	: DialogueRequirement()
{

}

DialogueRequirementCheckPlayerHasItem::DialogueRequirementCheckPlayerHasItem(const XMLNode& node)
	: DialogueRequirement(node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variable"
			|| attrName == "variablename"
			|| attrName == "item"
			|| attrName == "itemname")
		{
			SetVariableName(std::string(attr.lpszValue));
		}
		else if (attrName == "value")
		{
			std::string val = std::string(attr.lpszValue);
			QuestRequirement::ParseTextForSpecialCaseOperationsAndValue(val,
				m_specialCaseOperation, m_value);
		}
	}
}

DialogueRequirementCheckPlayerHasItem::DialogueRequirementCheckPlayerHasItem(
	const DialogueRequirementCheckPlayerHasItem& other)
	: DialogueRequirement(other),
	m_itemName(other.m_itemName),
	m_lowerCaseItemName(other.m_lowerCaseItemName),
	m_value(other.m_value),
	m_specialCaseOperation(other.m_specialCaseOperation)
{

}

DialogueRequirementCheckPlayerHasItem::~DialogueRequirementCheckPlayerHasItem()
{
	DialogueRequirement::~DialogueRequirement();
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Setters
void DialogueRequirementCheckPlayerHasItem::SetVariableName(const std::string& name)
{
	m_itemName = name;
	m_lowerCaseItemName = MakeLower(name);
}

void DialogueRequirementCheckPlayerHasItem::SetVariable(float variable)
{
	m_value = variable;
}

void DialogueRequirementCheckPlayerHasItem::SetSpecialCaseOperation(SpecialCaseOperations specialCase)
{
	m_specialCaseOperation = specialCase;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Getters
const std::string& DialogueRequirementCheckPlayerHasItem::GetVariableName() const
{
	return m_itemName;
}

const std::string& DialogueRequirementCheckPlayerHasItem::GetLowerCaseVariableName() const
{
	return m_lowerCaseItemName;
}

float DialogueRequirementCheckPlayerHasItem::GetVariable() const
{
	return m_value;
}

SpecialCaseOperations DialogueRequirementCheckPlayerHasItem::GetSpecialCaseOperation() const
{
	return m_specialCaseOperation;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Operations
void DialogueRequirementCheckPlayerHasItem::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Requirement Name: Check Player Has Item\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Item Name: " + m_itemName + "\n";

	str += "Value searching for: ";
	GameDebuggingCommons::WriteSpecialCaseOperatorToString(str, m_specialCaseOperation);
	str += std::to_string(m_value) + "\n";
}

bool DialogueRequirementCheckPlayerHasItem::CheckIfRequirementIsMet() const
{
	BaseGameEntity* playerEntity = BaseGameEntity::GetEditablePointerToPlayer();
	Player* player = dynamic_cast<Player*>(playerEntity);
	if (player == nullptr)
	{
		return false;
	}

	float value = 0.0f;
	player->GetItemValue(m_lowerCaseItemName, value, 0.0f);


	bool equalToCheck = (m_specialCaseOperation == IS_SAME) && value == m_value;
	bool lessThanCheck = (m_specialCaseOperation == IS_LESS_THAN) && value < m_value;
	bool greaterThanCheck = (m_specialCaseOperation == IS_GREATER_THAN) && value > m_value;
	bool greaterThanOrEqualToCheck = (m_specialCaseOperation == IS_GREATER_THAN_OR_EQUAL_TO) && value >= m_value;
	bool lessThanOrEqualToCheck = (m_specialCaseOperation == IS_LESS_THAN_OR_EQUAL_TO) && value <= m_value;
	bool notEqualToCheck = (m_specialCaseOperation == IS_NOT_EQUAL_TO) && value != m_value;

	bool combined = equalToCheck
		|| lessThanCheck
		|| lessThanOrEqualToCheck
		|| greaterThanCheck
		|| greaterThanOrEqualToCheck
		|| notEqualToCheck;
	return combined;
}

DialogueRequirement* DialogueRequirementCheckPlayerHasItem::Clone() const
{
	return new DialogueRequirementCheckPlayerHasItem(*this);
}