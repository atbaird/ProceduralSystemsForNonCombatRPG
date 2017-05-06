#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/FeatureBehaviorGiveItem.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateFeatureGiveItemSegmentBehavior()
{
	return new FeatureBehaviorGiveItem();
}

STATIC FeatureBehavior* CreateFeatureGiveItemSegmentBehavior(const XMLNode& node)
{
	return new FeatureBehaviorGiveItem(node);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper 
	FeatureBehaviorGiveItem::s_FeatureGiveItemBehaviorRegistration
		= FeatureBehaviorRegistrationHelper("GiveItem", CreateFeatureGiveItemSegmentBehavior,
			DefaultCreateFeatureGiveItemSegmentBehavior);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Constructors
FeatureBehaviorGiveItem::FeatureBehaviorGiveItem()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

FeatureBehaviorGiveItem::FeatureBehaviorGiveItem(const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "itemwillgive"
			|| attrName == "item"
			|| attrName == "itemname")
		{
			m_nameOfItemToGive = MakeLower(attr.lpszValue);
		}
		else if (attrName == "amountofitemwillgive"
			|| attrName == "amounttogive")
		{
			m_amountOfItemToGive = (float)atof(attr.lpszValue);
		}
	}
}

FeatureBehaviorGiveItem::FeatureBehaviorGiveItem(const FeatureBehaviorGiveItem& other)
	: FeatureBehavior(other),
	m_nameOfItemToGive(other.m_nameOfItemToGive),
	m_amountOfItemToGive(other.m_amountOfItemToGive)
{

}

FeatureBehaviorGiveItem::~FeatureBehaviorGiveItem()
{
	FeatureBehavior::~FeatureBehavior();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Operations
bool FeatureBehaviorGiveItem::EntityInteractWithFeature(BaseGameEntity* entity)
{
	Agent* agent = dynamic_cast<Agent*>(entity);
	if (m_nameOfItemToGive == ""
		|| agent == nullptr
		|| FeatureBehavior::EntityInteractWithFeature(entity) == false)
	{
		return false;
	}

	float amountAlreadyHolding = 0.f;
	agent->GetItemValue(m_nameOfItemToGive, amountAlreadyHolding, 0.f);

	amountAlreadyHolding += m_amountOfItemToGive;
	agent->SetItemValue(m_nameOfItemToGive, amountAlreadyHolding);
	return true;
}

FeatureBehavior* FeatureBehaviorGiveItem::Clone()
{
	return new FeatureBehaviorGiveItem(*this);
}