#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/AddToAgentStat.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateFeatureAddToAgentStatBehavior()
{
	return new AddToAgentStat();
}

STATIC FeatureBehavior* CreateFeatureAddToAgentStatBehavior(const XMLNode& node)
{
	return new AddToAgentStat(node);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper AddToAgentStat::s_FeatureAddToAgentStatBehaviorRegistration
= FeatureBehaviorRegistrationHelper("AddToAgentStat",
	CreateFeatureAddToAgentStatBehavior, DefaultCreateFeatureAddToAgentStatBehavior);

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Constructors
AddToAgentStat::AddToAgentStat()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

AddToAgentStat::AddToAgentStat(const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
	ReadXMLAttributes(node);
}


AddToAgentStat::AddToAgentStat(const AddToAgentStat& other)
	: FeatureBehavior(other),
	m_variableNameToSet(other.m_variableNameToSet),
	m_variableAbbreviationToSet(other.m_variableAbbreviationToSet),
	m_valueToSet(other.m_valueToSet),
	m_variableNameToSetIsSet(other.m_variableNameToSetIsSet),
	m_variableAbbreviationToSetIsSet(other.m_variableAbbreviationToSetIsSet),
	m_variableValueToSetIsSet(other.m_variableValueToSetIsSet),
	m_careWhetherItsBeenUsedOnce(other.m_careWhetherItsBeenUsedOnce)
{

}

AddToAgentStat::~AddToAgentStat()
{
	FeatureBehavior::~FeatureBehavior();
}

void AddToAgentStat::ReadXMLAttributes(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "attributeabbreviation"
			|| attrName == "abbreviation")
		{
			m_variableAbbreviationToSet = std::string(attr.lpszValue);
			m_variableAbbreviationToSetIsSet = m_variableAbbreviationToSet != "";
		}
		else if (attrName == "name"
			|| attrName == "attributename")
		{
			m_variableNameToSet = std::string(attr.lpszValue);
			m_variableNameToSetIsSet = m_variableNameToSet != "";
		}
		else if (attrName == "value"
			|| attrName == "attributevalue")
		{
			m_valueToSet = atoi(attr.lpszValue);
			m_variableValueToSetIsSet = true;
		}
		else if (attrName == "onlyuseonce"
			|| attrName == "useonce"
			|| attrName == "carewhetheruseonceornot")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "true"
				|| val == "t")
			{
				m_careWhetherItsBeenUsedOnce = true;
			}
			else if (val == "false"
				|| val == "f")
			{
				m_careWhetherItsBeenUsedOnce = false;
			}
		}
	}
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Operations
bool AddToAgentStat::EntityInteractWithFeature(BaseGameEntity* entity)
{
	bool eitherNameAndAbbreviationWasNotSet = (m_variableNameToSetIsSet == false
		&& m_variableAbbreviationToSetIsSet == false);
	bool alreadyUsedThisSetterOnceAndWeCareAboutIt = (m_careWhetherItsBeenUsedOnce && m_usedOnce);
	if (FeatureBehavior::EntityInteractWithFeature(entity) == false
		|| eitherNameAndAbbreviationWasNotSet
		|| m_variableValueToSetIsSet == false
		|| alreadyUsedThisSetterOnceAndWeCareAboutIt)
	{
		return false;
	}
	Agent* agent = dynamic_cast<Agent*>(entity);
	if (agent == nullptr)
	{
		return false;
	}

	if (m_variableAbbreviationToSetIsSet)
	{
		bool found = false;
		int val = agent->GetValOfStatByAbbreviation(m_variableAbbreviationToSet, found);
		agent->SetStatByAbbreviation(m_variableAbbreviationToSet, val + m_valueToSet);
	}
	else if (m_variableNameToSetIsSet)
	{
		bool found = false;
		int val = agent->GetValOfStatByName(m_variableNameToSet, found);
		agent->SetStatByName(m_variableNameToSet, val + m_valueToSet);
	}

	m_usedOnce = true;
	return true;
}

FeatureBehavior* AddToAgentStat::Clone()
{
	return new AddToAgentStat(*this);
}