#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckPlayerHasItem.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Creation Funcs
QuestRequirement* CheckPlayerHasItemCreationFunc(const XMLNode& node)
{
	return new QuestRequirementCheckPlayerHasItem(node);
}

QuestRequirement* CheckPlayerHasItemDefaultCreationFunc()
{
	return new QuestRequirementCheckPlayerHasItem();
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Static Variables
STATIC QuestRequirementRegistration 
	QuestRequirementCheckPlayerHasItem::s_CheckPlayerStatsRegistration
		= QuestRequirementRegistration("CheckPlayerHasItem", CheckPlayerHasItemCreationFunc,
			CheckPlayerHasItemDefaultCreationFunc);


//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Constructors
QuestRequirementCheckPlayerHasItem::QuestRequirementCheckPlayerHasItem()
	: QuestRequirement(Check_Player_Has_Item)
{

}

QuestRequirementCheckPlayerHasItem::QuestRequirementCheckPlayerHasItem(const XMLNode& node)
	: QuestRequirement(Check_Player_Has_Item, node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "itemname")
		{
			m_itemName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "value")
		{
			std::string val = std::string(attr.lpszValue);
			ParseTextForSpecialCaseOperationsAndValue(val, m_specialCaseOperation, m_amountWanted);
		}
	}
}

QuestRequirementCheckPlayerHasItem::QuestRequirementCheckPlayerHasItem(
	const QuestRequirementCheckPlayerHasItem& other, Quest* owningQuest)
	: QuestRequirement(other, owningQuest),
	m_itemName(other.m_itemName),
	m_amountWanted(other.m_amountWanted),
	m_specialCaseOperation(other.m_specialCaseOperation)
{

}

QuestRequirementCheckPlayerHasItem::~QuestRequirementCheckPlayerHasItem()
{
	QuestRequirement::~QuestRequirement();
}


//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Operations
void QuestRequirementCheckPlayerHasItem::WriteQuestRequirementToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Check Player Has Item\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Item Name: " + m_itemName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value Desired: ";
	GameDebuggingCommons::WriteSpecialCaseOperatorToString(str, m_specialCaseOperation);
	str += std::to_string(m_amountWanted) + "\n";
}

const bool QuestRequirementCheckPlayerHasItem::DeterminePlayerMeetsQuestRequirement() const
{
	BaseGameEntity* entity = BaseGameEntity::GetEditablePointerToPlayer();
	Player* player = dynamic_cast<Player*>(entity);
	if(m_owningQuest == nullptr
		|| player == nullptr
		|| m_itemName == "")
	{
		return false;
	}
	float defaul = 0.f;
	float internalVal = 0.f;
	player->GetItemValue(m_itemName, internalVal, defaul);

	switch (m_specialCaseOperation)
	{
	case IS_SAME:						return internalVal == m_amountWanted;
	case IS_LESS_THAN:					return internalVal < m_amountWanted;
	case IS_GREATER_THAN:				return internalVal > m_amountWanted;
	case IS_GREATER_THAN_OR_EQUAL_TO:	return internalVal >= m_amountWanted;
	case IS_LESS_THAN_OR_EQUAL_TO:		return internalVal <= m_amountWanted;
	case IS_NOT_EQUAL_TO:				return internalVal != m_amountWanted;
	default:							return false;
	}
}

QuestRequirement* QuestRequirementCheckPlayerHasItem::Clone(Quest* owningQuest) const
{
	return new QuestRequirementCheckPlayerHasItem(*this, owningQuest);
}