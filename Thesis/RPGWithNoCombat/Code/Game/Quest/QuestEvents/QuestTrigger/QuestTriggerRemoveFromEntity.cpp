#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRemoveFromEntity.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerRemoveFromEntityFunc(const XMLNode& node)
{
	QuestTriggerRemoveFromEntity* trigger = new QuestTriggerRemoveFromEntity(node);
	return trigger;
}

QuestTrigger* QuestTriggerDefaultRemoveFromEntityFunc()
{
	return new QuestTriggerRemoveFromEntity();
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration QuestTriggerRemoveFromEntity::s_RemoveFromEntityRegistration
	= QuestTriggerRegistration("RemoveFromEntity", 
		QuestTriggerRemoveFromEntityFunc, QuestTriggerDefaultRemoveFromEntityFunc);

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Constructors
QuestTriggerRemoveFromEntity::QuestTriggerRemoveFromEntity()
	: QuestTrigger()
{

}

QuestTriggerRemoveFromEntity::QuestTriggerRemoveFromEntity(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variablename"
			|| attrName == "variable"
			|| attrName == "name"
			|| attrName == "entityname"
			|| attrName == "entity")
		{
			m_EntityName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "item"
			|| attrName == "itemname")
		{
			m_ItemName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "value"
			|| attrName == "amount"
			|| attrName == "amounttoremove")
		{
			m_quantityToRemove = (float)atof(attr.lpszValue);
		}
	}
}

QuestTriggerRemoveFromEntity::QuestTriggerRemoveFromEntity(const QuestTriggerRemoveFromEntity& other,
	Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_ItemName(other.m_ItemName),
	m_quantityToRemove(other.m_quantityToRemove)
{

}

QuestTriggerRemoveFromEntity::~QuestTriggerRemoveFromEntity()
{
	QuestTrigger::~QuestTrigger();
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Setters
void QuestTriggerRemoveFromEntity::SetEntityName(const std::string& entityName)
{
	m_EntityName = entityName;
}

void QuestTriggerRemoveFromEntity::SetItemName(const std::string itemName)
{
	m_ItemName = itemName;
}

void QuestTriggerRemoveFromEntity::SetQuantityToRemove(float quantityToRemove)
{
	m_quantityToRemove = quantityToRemove;
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Getters
const std::string& QuestTriggerRemoveFromEntity::GetEntityName() const
{
	return m_EntityName;
}

const std::string& QuestTriggerRemoveFromEntity::GetItemName() const
{
	return m_ItemName;
}

float QuestTriggerRemoveFromEntity::GetQuantityToRemove() const
{
	return m_quantityToRemove;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Operations
bool QuestTriggerRemoveFromEntity::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr)
	{
		return false;
	}
	float quantity = 0.f;
	BaseGameEntity* entity = nullptr;
	m_owningQuest->GetVariable<BaseGameEntity*>(m_EntityName, entity, nullptr);
	if (entity == nullptr)
	{
		return false;
	}
	Agent* agent = dynamic_cast<Agent*>(entity);
	if (agent == nullptr)
	{
		return false;
	}
	agent->GetItemValue<float>(m_ItemName, quantity, 0.f);
	agent->SetItemValue<float>(m_ItemName, (quantity - m_quantityToRemove));
	return true;
}

QuestTrigger* QuestTriggerRemoveFromEntity::Clone(Quest* owningQuest) const
{
	QuestTriggerRemoveFromEntity* clone = new QuestTriggerRemoveFromEntity(*this, owningQuest);
	return clone;
}

void QuestTriggerRemoveFromEntity::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Remove From Entity\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Entity to remove Item from: " + m_EntityName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Item Name: " + m_ItemName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Amount to remove: " + std::to_string(m_quantityToRemove) + "\n";
}