#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerGiveEntity.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerGiveEntityFunc(const XMLNode& node)
{
	return new QuestTriggerGiveEntity(node);
}

QuestTrigger* QuestTriggerDefaultGiveEntityFunc()
{
	return new QuestTriggerGiveEntity();
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration 
	QuestTriggerGiveEntity::s_TriggerRegistration = QuestTriggerRegistration("GiveEntity", 
		QuestTriggerGiveEntityFunc, QuestTriggerDefaultGiveEntityFunc);

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Constructors
QuestTriggerGiveEntity::QuestTriggerGiveEntity()
	: QuestTrigger()
{

}

QuestTriggerGiveEntity::QuestTriggerGiveEntity(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{
	//GiveEntity
	//	<GiveEntity VariableName = 'Player' Item = 'Package' value = '1.0' />
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
			m_EntityName = std::string(attr.lpszValue);
		}
		else if (attrName == "item"
			|| attrName == "itemname")
		{
			m_ItemName = std::string(attr.lpszValue);
		}
		else if (attrName == "value"
			|| attrName == "amount"
			|| attrName == "amounttogive")
		{
			m_amountToGiveEntity = (float)atof(attr.lpszValue);
		}
	}
}

QuestTriggerGiveEntity::QuestTriggerGiveEntity(const QuestTriggerGiveEntity& other, Quest* owningQuest)
	: QuestTrigger(other, owningQuest)
{

}

QuestTriggerGiveEntity::~QuestTriggerGiveEntity()
{
	QuestTrigger::~QuestTrigger();
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Setters
void QuestTriggerGiveEntity::SetEntityName(const std::string& name)
{
	m_EntityName = name;
}

void QuestTriggerGiveEntity::SetItemName(const std::string& itemName)
{
	m_ItemName = itemName;
}

void QuestTriggerGiveEntity::SetAmountToGiveEntity(float amount)
{
	m_amountToGiveEntity = amount;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Getters
const std::string& QuestTriggerGiveEntity::GetEntityName() const
{
	return m_EntityName;
}

const std::string& QuestTriggerGiveEntity::GetItemName() const
{
	return m_ItemName;
}

float QuestTriggerGiveEntity::GetAmountToGiveEntity() const
{
	return m_amountToGiveEntity;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Operations
bool QuestTriggerGiveEntity::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr)
	{
		return false;
	}
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
	float originalAmt = 0.f;
	agent->GetItemValue<float>(m_ItemName, originalAmt, 0.f);
	agent->SetItemValue(m_ItemName, originalAmt + m_amountToGiveEntity);
	return true;
}

QuestTrigger* QuestTriggerGiveEntity::Clone(Quest* owningQuest) const
{
	QuestTriggerGiveEntity* clone = new QuestTriggerGiveEntity(*this, owningQuest);
	return clone;
}

void QuestTriggerGiveEntity::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Give Entity\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Entity to give Item: " + m_EntityName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Item Name: " + m_ItemName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Amount of Item to Give Entity: " + std::to_string(m_amountToGiveEntity) + "\n";
}