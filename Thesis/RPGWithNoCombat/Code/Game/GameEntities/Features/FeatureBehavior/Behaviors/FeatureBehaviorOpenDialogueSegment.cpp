#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/FeatureBehaviorOpenDialogueSegment.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Dialogue/Dialogue.hpp"

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateFeatureOpenDialogueSegmentBehavior()
{
	return new FeatureBehaviorOpenDialogueSegement();
}

STATIC FeatureBehavior* CreateFeatureOpenDialogueSegmentBehavior(const XMLNode& node)
{
	return new FeatureBehaviorOpenDialogueSegement(node);
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper 
	FeatureBehaviorOpenDialogueSegement::s_FeatureAddToAgentStatBehaviorRegistration
	= FeatureBehaviorRegistrationHelper("OpenDialogue",
		CreateFeatureOpenDialogueSegmentBehavior, DefaultCreateFeatureOpenDialogueSegmentBehavior);

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Constructors
FeatureBehaviorOpenDialogueSegement::FeatureBehaviorOpenDialogueSegement()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

FeatureBehaviorOpenDialogueSegement::FeatureBehaviorOpenDialogueSegement(const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
	for (int childIdx = 0; childIdx < node.nAttribute(); childIdx++)
	{
		XMLAttribute attr = node.getAttribute(childIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "segment"
			|| attrName == "dialogue"
			|| attrName == "dialoguesegment")
		{
			m_dialogueSegmentToOpen = std::string(attr.lpszValue);
		}
	}
}

FeatureBehaviorOpenDialogueSegement::FeatureBehaviorOpenDialogueSegement(
	const FeatureBehaviorOpenDialogueSegement& other)
	: FeatureBehavior(other),
	m_dialogueSegmentToOpen(other.m_dialogueSegmentToOpen)
{

}

FeatureBehaviorOpenDialogueSegement::~FeatureBehaviorOpenDialogueSegement()
{
	FeatureBehavior::~FeatureBehavior();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Operations
void FeatureBehaviorOpenDialogueSegement::CheckMapNodeForAttributesOfInterest(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nAttribute(); childIdx++)
	{
		XMLAttribute attr = node.getAttribute(childIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "segment"
			|| attrName == "dialogue"
			|| attrName == "dialoguesegment")
		{
			m_dialogueSegmentToOpen = std::string(attr.lpszValue);
		}
	}
}

bool FeatureBehaviorOpenDialogueSegement::EntityInteractWithFeature(BaseGameEntity* entity)
{
	if (entity == nullptr
		|| m_owningFeature == nullptr
		|| m_dialogueSegmentToOpen == "")
	{
		return false;
	}
	Player* player = dynamic_cast<Player*>(entity);
	if (player == nullptr)
	{
		return false;
	}
	Dialogue* dialogue = Dialogue::SpawnOrGetCentralDialogue();
	if (dialogue->StartDialogueSegment(m_dialogueSegmentToOpen))
	{
		player->SetIsTalkingToSomeone(true);
		return true;
	}

	return false;
}

FeatureBehavior* FeatureBehaviorOpenDialogueSegement::Clone()
{
	FeatureBehaviorOpenDialogueSegement* clone = new FeatureBehaviorOpenDialogueSegement(*this);
	return clone;
}