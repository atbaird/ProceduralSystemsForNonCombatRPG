#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementCheckEntityHasVariable.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Creation Funcs
QuestRequirement* CheckEntityHasVariableCreationFunc(const XMLNode& node)
{
	return new QuestRequirementCheckEntityHasVariable(node);
}

QuestRequirement* CheckEntityHasVariableDefaultCreationFunc()
{
	return new QuestRequirementCheckEntityHasVariable();
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Static Variables
STATIC QuestRequirementRegistration 
	QuestRequirementCheckEntityHasVariable::s_CheckEntityHasRegistration 
	= QuestRequirementRegistration("CheckEntityHas", CheckEntityHasVariableCreationFunc,
		CheckEntityHasVariableDefaultCreationFunc);

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Constructors
QuestRequirementCheckEntityHasVariable::QuestRequirementCheckEntityHasVariable()
	: QuestRequirementCheckVariableBase(Entity_Has_Type_Check_Variable)
{

}

QuestRequirementCheckEntityHasVariable::QuestRequirementCheckEntityHasVariable(const XMLNode& node)
	: QuestRequirementCheckVariableBase(Entity_Has_Type_Check_Variable, node)
{
	//<Requirement HaveItem = 'Staff' Entity = 'player' Value = '>=1.0' / >
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "value")
		{
			std::string val = std::string(attr.lpszValue);
			ParseTextForSpecialCaseOperationsAndValue(val, m_specialCaseOperation, m_amountToHave);
		}
		else if (attrName == "hasitem"
			|| attrName == "item"
			|| attrName == "haveitem")
		{
			m_itemName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "checkentity")
		{
			m_entityName = std::string(attr.lpszValue);
		}
	}
}

QuestRequirementCheckEntityHasVariable::QuestRequirementCheckEntityHasVariable(
	const QuestRequirementCheckEntityHasVariable& other, Quest* owningQuest)
	: QuestRequirementCheckVariableBase(other, owningQuest),
	m_entityName(other.m_entityName),
	m_itemName(other.m_itemName),
	m_amountToHave(other.m_amountToHave)
{

}

QuestRequirementCheckEntityHasVariable::~QuestRequirementCheckEntityHasVariable()
{
	QuestRequirementCheckVariableBase::~QuestRequirementCheckVariableBase();
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Setters
void QuestRequirementCheckEntityHasVariable::SetEntityName(const std::string& entityName)
{
	m_entityName = entityName;
}

void QuestRequirementCheckEntityHasVariable::SetItemName(const std::string& itemName)
{
	m_itemName = itemName;
}

void QuestRequirementCheckEntityHasVariable::SetAmountToHave(float amtToHave)
{
	m_amountToHave = amtToHave;
}


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Getters
const std::string& QuestRequirementCheckEntityHasVariable::GetEntityName() const
{
	return m_entityName;
}

const std::string& QuestRequirementCheckEntityHasVariable::GetItemName() const
{
	return m_itemName;
}

float QuestRequirementCheckEntityHasVariable::GetAmountToHave() const
{
	return m_amountToHave;
}


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Operations
void QuestRequirementCheckEntityHasVariable::WriteQuestRequirementToString(std::string& str,
	int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Requirement Name: Check Entity Has\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Entity Name: " + m_entityName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Item Name: " + m_itemName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Amount of Item desired: " + std::to_string(m_amountToHave) + "\n";

}

const bool QuestRequirementCheckEntityHasVariable::DeterminePlayerMeetsQuestRequirement() const
{
	if (m_owningQuest == nullptr)
	{
		return false;
	}
	BaseGameEntity* entity = nullptr;
	m_owningQuest->GetVariable<BaseGameEntity*>(m_entityName, entity, nullptr);
	if (entity == nullptr)
	{
		return false;
	}
	Agent* agent = dynamic_cast<Agent*>(entity);
	if (agent == nullptr)
	{
		return false;
	}

	float amt = 0.f;
	agent->GetItemValue(m_itemName, amt, 0.f);
	
	bool equalToCheck = (m_specialCaseOperation == IS_SAME) && amt == m_amountToHave;
	bool lessThanCheck = (m_specialCaseOperation == IS_LESS_THAN) && amt < m_amountToHave;
	bool greaterThanCheck = (m_specialCaseOperation == IS_GREATER_THAN) && amt > m_amountToHave;
	bool greaterThanOrEqualToCheck = (m_specialCaseOperation == IS_GREATER_THAN_OR_EQUAL_TO) && amt >= m_amountToHave;
	bool lessThanOrEqualToCheck = (m_specialCaseOperation == IS_LESS_THAN_OR_EQUAL_TO) && amt <= m_amountToHave;

	bool combined = equalToCheck
		|| lessThanCheck
		|| lessThanOrEqualToCheck
		|| greaterThanCheck
		|| greaterThanOrEqualToCheck;

	return combined;
}

QuestRequirement* QuestRequirementCheckEntityHasVariable::Clone(Quest* owningQuest) const
{
	return new QuestRequirementCheckEntityHasVariable(*this, owningQuest);
}