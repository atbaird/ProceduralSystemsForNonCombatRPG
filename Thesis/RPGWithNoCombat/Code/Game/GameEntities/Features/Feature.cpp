#include "Game/GameEntities/Features/Feature.hpp"
#include "Engine/Math/MathToStringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Constructors
Feature::Feature(const SpriteResource* resource, Directions4 defaultDirection)
	: BaseGameEntity(FEATURE_ENTITY, resource, defaultDirection)
{
	m_FeatureBehaviors.resize(NUMBER_OF_DEFIND_FEATURE_BEHAVIOR_TYPES);
}

Feature::~Feature()
{
	BaseGameEntity::~BaseGameEntity();

	for (size_t bevType = 0; bevType < m_FeatureBehaviors.size(); bevType++)
	{
		std::vector<FeatureBehavior*>& behaviors = m_FeatureBehaviors[bevType];
		for (size_t bevIdx = 0; bevIdx < behaviors.size(); bevIdx++)
		{
			FeatureBehavior* bev = behaviors[bevIdx];
			if (bev != nullptr)
			{
				delete bev;
			}
		}
		behaviors.clear();
	}
	m_FeatureBehaviors.clear();
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Updates and Render
void Feature::Update(const float& dt)
{
	std::vector<FeatureBehavior*>& updateBevs = m_FeatureBehaviors[ON_UPDATE_FEATURE_BEHAVIOR];

	for (size_t i = 0; i < updateBevs.size(); i++)
	{
		FeatureBehavior* bev = updateBevs[i];
		if (bev != nullptr)
		{
			bev->UpdateFeature(dt);
		}
	}

	BaseGameEntity::Update(dt);
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Setters
void Feature::SetRelatedQuest(const std::string& questTitle)
{
	m_RelatedQuest = questTitle;
}

void Feature::AddFeatureBehaviors(const std::vector<FeatureBehavior*>& behaviors, size_t behaviorType, 
	bool isCopiedAlready)
{
	if (behaviorType >= m_FeatureBehaviors.size())
	{
		m_FeatureBehaviors.resize(behaviorType + 1);
	}
	for (size_t bevIdx = 0; bevIdx < behaviors.size(); bevIdx++)
	{
		FeatureBehavior* bev = behaviors[bevIdx];
		if (bev != nullptr)
		{
			if (INTERACTIVE_FEATURE_BEHAVIOR == behaviorType)
			{
				m_interactable = true;
			}
			if (isCopiedAlready)
			{
				bev->SetOwningFeature(this);
				m_FeatureBehaviors[behaviorType].push_back(bev);
			}
			else
			{
				FeatureBehavior* bevClone = bev->Clone();
				bevClone->SetOwningFeature(this);
				m_FeatureBehaviors[behaviorType].push_back(bevClone);
			}
		}
	}
}

void Feature::AddFeatureBehavior(FeatureBehavior* bev, size_t behaviorType)
{
	if (behaviorType >= m_FeatureBehaviors.size())
	{
		m_FeatureBehaviors.resize(behaviorType + 1);
	}
	bev->SetOwningFeature(this);
	m_FeatureBehaviors[behaviorType].push_back(bev);
	if (INTERACTIVE_FEATURE_BEHAVIOR == behaviorType)
	{
		m_interactable = true;
	}

}

void Feature::AddInteractiveFeatureBehavior(FeatureBehavior* bev)
{
	bev->SetOwningFeature(this);
	m_FeatureBehaviors[INTERACTIVE_FEATURE_BEHAVIOR].push_back(bev);
	m_interactable = true;
}

void Feature::AddOnUpdateFeatureBehavior(FeatureBehavior* bev)
{
	bev->SetOwningFeature(this);
	m_FeatureBehaviors[ON_UPDATE_FEATURE_BEHAVIOR].push_back(bev);
}

void Feature::AddOnPlacementFeatureBehavior(FeatureBehavior* bev)
{
	bev->SetOwningFeature(this);
	m_FeatureBehaviors[ON_PLACEMENT_FEATURE_BEHAVIOR].push_back(bev);
}

void Feature::SetSolid(bool solid, bool overwrite)
{
	m_solid = solid;
	m_overwriteTileOnIsSolid = overwrite;
}

void Feature::SetFeatureMode(unsigned int mode)
{
	m_FeatureMode = mode;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Getters
const std::string& Feature::GetRelatedQuestTitle() const
{
	return m_RelatedQuest;
}

const std::vector<FeatureBehavior*>& Feature::GetInteractiveFeatureBehaviors() const
{
	return m_FeatureBehaviors[INTERACTIVE_FEATURE_BEHAVIOR];
}

std::vector<FeatureBehavior*>* Feature::GetInteractiveFeatureBehaviorsEditablePtr()
{
	return &m_FeatureBehaviors[INTERACTIVE_FEATURE_BEHAVIOR];
}

const std::vector<FeatureBehavior*>* Feature::GetInteractiveFeatureBehaviorsPtr() const
{
	return &m_FeatureBehaviors[INTERACTIVE_FEATURE_BEHAVIOR];
}

bool Feature::GetSolid() const
{
	return m_solid;
}

bool Feature::GetOverwriteTileOnIsSolid() const
{
	return m_overwriteTileOnIsSolid;
}

bool Feature::GetLastInteractBehaviorRan() const
{
	return m_lastInteractRanCorrectly;
}

unsigned int Feature::GetFeatureMode() const
{
	return m_FeatureMode;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Operations
void Feature::WriteDeveloperDebugInfo(DeveloperDebugInfoBox& info) const
{
	DeveloperDebugInfoTextBlurb whatAmIBlurb = DeveloperDebugInfoTextBlurb("Entity type: FEATURE",
		Rgba::s_White);
	info.AddTextBlurb(whatAmIBlurb);

	BaseGameEntity::WriteDeveloperDebugInfo(info);
}

void Feature::EntityInteractWithFeature(BaseGameEntity* entity)
{
	if (entity == nullptr)
	{
		return;
	}

	std::vector<FeatureBehavior*>& interactiveBehaviors = m_FeatureBehaviors[INTERACTIVE_FEATURE_BEHAVIOR];
	for (size_t i = 0; i < interactiveBehaviors.size(); i++)
	{
		FeatureBehavior* bev = interactiveBehaviors[i];
		if (bev != nullptr)
		{
			m_lastInteractRanCorrectly = bev->EntityInteractWithFeature(entity);
		}
	}
}

void Feature::TriggerPlacementActions(const Map* map)
{
	std::vector<FeatureBehavior*>& placementBevs = m_FeatureBehaviors[ON_PLACEMENT_FEATURE_BEHAVIOR];
	for (size_t idx = 0; idx < placementBevs.size(); idx++)
	{
		FeatureBehavior* bev = placementBevs[idx];
		if (bev != nullptr)
		{
			bev->TriggerOnPlacement(map);
		}
	}
}