#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/ChangeFeatureAnimations.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Features/Feature.hpp"

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateChangeFeatureAnimationsBehavior()
{
	return new ChangeFeatureAnimations();
}

STATIC FeatureBehavior* CreateChangeFeatureAnimationsBehavior(const XMLNode& node)
{
	return new ChangeFeatureAnimations(node);
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper
	ChangeFeatureAnimations::s_ChangeFeatureAnimationsBehaviorRegistration
		= FeatureBehaviorRegistrationHelper("ChangeFeatureAnimations",
			CreateChangeFeatureAnimationsBehavior, 
			DefaultCreateChangeFeatureAnimationsBehavior);

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Constructors
ChangeFeatureAnimations::ChangeFeatureAnimations()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

ChangeFeatureAnimations::ChangeFeatureAnimations(const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "toggle")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "false"
				|| val == "f") 
			{
				m_enableToggling = false;
			}
			else if (val == "true"
				|| val == "t")
			{
				m_enableToggling = true;
			}
		}
		else if (attrName == "direction")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			if (attrVal == "south")
			{
				m_direction = SOUTH;
				m_originalDirection = SOUTH;
			}
			else if (attrVal == "east")
			{
				m_direction = EAST;
				m_originalDirection = EAST;
			}
			else if (attrVal == "north")
			{
				m_direction = NORTH;
				m_originalDirection = NORTH;
			}
			else if (attrVal == "west")
			{
				m_direction = WEST;
				m_originalDirection = WEST;
			}
		}
		else if (attrName == "animidx")
		{
			int val = atoi(attr.lpszValue);
			if (val != 0 || (val == 0 && SimpleStrCmp(attr.lpszValue, "0")))
			{
				m_animationIdx = val;
			}
			else
			{
				if (SimpleStrCmpLower(attr.lpszValue, "walk"))
				{
					m_animationIdx = WALK;
				}
			}
			m_originalAnimationIdx = m_animationIdx;
		}
	}
}

ChangeFeatureAnimations::ChangeFeatureAnimations(const ChangeFeatureAnimations& other)
	: FeatureBehavior(other),
	m_direction(other.m_direction),
	m_animationIdx(other.m_animationIdx),
	m_enableToggling(other.m_enableToggling),
	m_originalAnimationIdx(other.m_originalAnimationIdx),
	m_originalDirection(other.m_originalDirection)
{

}

ChangeFeatureAnimations::~ChangeFeatureAnimations()
{
	FeatureBehavior::~FeatureBehavior();
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Operations
void ChangeFeatureAnimations::CheckMapNodeForAttributesOfInterest(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "toggle")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "false"
				|| val == "f")
			{
				m_enableToggling = false;
			}
			else if (val == "true"
				|| val == "t")
			{
				m_enableToggling = true;
			}
		}
	}
}

bool ChangeFeatureAnimations::EntityInteractWithFeature(BaseGameEntity* entity)
{
	if (entity == nullptr
 		|| m_owningFeature == nullptr)
	{
		return false;
	}
	
	Directions4 dir = m_direction;
	int animationIdx = m_animationIdx;

	if (m_enableToggling)
	{
		int curAnimIdx = m_owningFeature->GetCurrentAnimationIdx();
		Directions4 curDir = m_owningFeature->GetCurrentAnimationsIdx();
		if (curDir == m_originalDirection
			&& curAnimIdx == m_originalAnimationIdx)
		{
			dir = m_direction;
			animationIdx = m_animationIdx;
			m_direction = m_originalDirection;
			m_animationIdx = m_originalAnimationIdx;
		}
		else
		{
			dir = m_originalDirection;
			animationIdx = m_originalAnimationIdx;
			m_direction = curDir;
			m_animationIdx = curAnimIdx;
		}
	}

	m_owningFeature->ChangeCurrentAnimationPlaying(dir, animationIdx);
	return true;
}

FeatureBehavior* ChangeFeatureAnimations::Clone()
{
	return new ChangeFeatureAnimations(*this);
}