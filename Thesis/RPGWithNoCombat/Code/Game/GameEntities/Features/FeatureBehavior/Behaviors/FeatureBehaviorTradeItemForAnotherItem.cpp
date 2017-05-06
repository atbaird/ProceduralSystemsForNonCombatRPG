#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/FeatureBehaviorTradeItemForAnotherItem.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateFeatureTradeItemForItemBehavior()
{
	return new FeatureBehaviorTradeItemForAnotherItem();
}

STATIC FeatureBehavior* CreateFeatureTradeItemForItemBehavior(const XMLNode& node)
{
	return new FeatureBehaviorTradeItemForAnotherItem(node);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper 
	FeatureBehaviorTradeItemForAnotherItem::s_FeatureTradeItemForItemBehaviorRegistration
		= FeatureBehaviorRegistrationHelper("TradeItemForItem", 
			CreateFeatureTradeItemForItemBehavior, DefaultCreateFeatureTradeItemForItemBehavior);

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Constructors
FeatureBehaviorTradeItemForAnotherItem::FeatureBehaviorTradeItemForAnotherItem()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

FeatureBehaviorTradeItemForAnotherItem::FeatureBehaviorTradeItemForAnotherItem(const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "itemwanted"
			|| attrName == "wanteditem")
		{
			m_itemWantedName = MakeLower(attr.lpszValue);
		}
		else if (attrName == "amountofitemwanted")
		{
			m_amountOfItemNameWanted = (float)atof(attr.lpszValue);
		}
		else if (attrName == "itemwillgive")
		{
			m_itemNameWillGive = MakeLower(attr.lpszValue);
		}
		else if (attrName == "amountofitemwillgive")
		{
			m_amountOfItemNameWillGive = (float)atof(attr.lpszValue);
		}
	}
}

FeatureBehaviorTradeItemForAnotherItem::FeatureBehaviorTradeItemForAnotherItem(
	const FeatureBehaviorTradeItemForAnotherItem& other)
	: FeatureBehavior(other),
	m_amountOfItemNameWanted(other.m_amountOfItemNameWanted),
	m_amountOfItemNameWillGive(other.m_amountOfItemNameWillGive),
	m_itemNameWillGive(other.m_itemNameWillGive),
	m_itemWantedName(other.m_itemWantedName)
{

}

FeatureBehaviorTradeItemForAnotherItem::~FeatureBehaviorTradeItemForAnotherItem()
{
	FeatureBehavior::~FeatureBehavior();
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Operations
bool FeatureBehaviorTradeItemForAnotherItem::EntityInteractWithFeature(BaseGameEntity* entity)
{
	Agent* agent = dynamic_cast<Agent*>(entity);
	bool canInteract = FeatureBehavior::EntityInteractWithFeature(entity);
	if (agent == nullptr
		|| canInteract == false
		|| m_itemNameWillGive == ""
		|| m_itemWantedName == "")
	{
		return false;
	}
	float amountOfItemPresent = 0.f;
	agent->GetItemValue(m_itemWantedName, amountOfItemPresent, 0.f);

	if (amountOfItemPresent < m_amountOfItemNameWanted)
	{
		return false;
	}

	amountOfItemPresent -= m_amountOfItemNameWanted;
	agent->SetItemValue(m_itemWantedName, amountOfItemPresent);

	float amountOfItemGiving = 0.f;
	agent->GetItemValue(m_itemNameWillGive, amountOfItemGiving, 0.f);
	amountOfItemGiving += m_amountOfItemNameWillGive;
	agent->SetItemValue(m_itemNameWillGive, amountOfItemGiving);
	return true;
}

FeatureBehavior* FeatureBehaviorTradeItemForAnotherItem::Clone()
{
	return new FeatureBehaviorTradeItemForAnotherItem(*this);
}