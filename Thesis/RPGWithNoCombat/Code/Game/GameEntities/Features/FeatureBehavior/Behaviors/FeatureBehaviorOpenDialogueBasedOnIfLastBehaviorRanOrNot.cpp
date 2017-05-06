#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Dialogue/Dialogue.hpp"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateOpenDialogueBasedOnIfLastBehaviorRanOrNotBehavior()
{
	return new FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot();
}

STATIC FeatureBehavior* CreateOpenDialogueBasedOnIfLastBehaviorRanOrNotBehavior(const XMLNode& node)
{
	return new FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot(node);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
STATIC const FeatureBehaviorRegistrationHelper
FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot::s_OpenDialogueBasedOnIfLastBehaviorRanOrNotBehaviorRegistration
	= FeatureBehaviorRegistrationHelper("OpenDialogueBasedOnIfLastBehaviorRanOrNot",
		CreateOpenDialogueBasedOnIfLastBehaviorRanOrNotBehavior,
		DefaultCreateOpenDialogueBasedOnIfLastBehaviorRanOrNotBehavior);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Constructors
FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot::FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot::FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot(
	const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "segmentforifran"
			|| attrName == "dialogueforifran"
			|| attrName == "dialoguesegmentforifran")
		{
			m_dialogueSegmentToOpenIfRan = MakeLower(attr.lpszValue);
		}
		else if (attrName == "segmentforifdidnotrun"
			|| attrName == "dialogueforifdidnotrun"
			|| attrName == "dialoguesegmentforifdidnotrun")
		{
			m_dialogueSegmentToOpenIfDidNotRun = MakeLower(attr.lpszValue);
		}
	}
}

FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot::FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot(
	const FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot& other)
	: FeatureBehavior(other),
	m_dialogueSegmentToOpenIfRan(other.m_dialogueSegmentToOpenIfRan),
	m_dialogueSegmentToOpenIfDidNotRun(other.m_dialogueSegmentToOpenIfDidNotRun)
{

}

FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot::~FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot()
{
	FeatureBehavior::~FeatureBehavior();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Operations
void FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot::CheckMapNodeForAttributesOfInterest(
	const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "segmentforifran"
			|| attrName == "dialogueforifran"
			|| attrName == "dialoguesegmentforifran")
		{
			m_dialogueSegmentToOpenIfRan = MakeLower(attr.lpszValue);
		}
		else if (attrName == "segmentforifdidnotrun"
			|| attrName == "dialogueforifdidnotrun"
			|| attrName == "dialoguesegmentforifdidnotrun")
		{
			m_dialogueSegmentToOpenIfDidNotRun = MakeLower(attr.lpszValue);
		}
	}
}

bool FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot::EntityInteractWithFeature(
	BaseGameEntity* entity)
{
	Player* player = dynamic_cast<Player*>(entity);
	if (FeatureBehavior::EntityInteractWithFeature(entity) == false
		|| m_owningFeature == nullptr
		|| player == nullptr)
	{
		return false;
	}

	bool lastBehaviorRan = m_owningFeature->GetLastInteractBehaviorRan();

	Dialogue* dialogue = Dialogue::SpawnOrGetCentralDialogue();

	if (lastBehaviorRan && m_dialogueSegmentToOpenIfRan != "")
	{
		if (dialogue->StartDialogueSegment(m_dialogueSegmentToOpenIfRan))
		{
			player->SetIsTalkingToSomeone(true);
			return true;
		}
	}
	else if(!lastBehaviorRan && m_dialogueSegmentToOpenIfDidNotRun != "")
	{
		if (dialogue->StartDialogueSegment(m_dialogueSegmentToOpenIfDidNotRun))
		{
			player->SetIsTalkingToSomeone(true);
			return true;
		}
	}
	return false;

}

FeatureBehavior* FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot::Clone()
{
	return new FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot(*this);
}