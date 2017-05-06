#pragma once

#ifndef CHANGEFEATUREANIMATIONS_HPP
#define CHANGEFEATUREANIMATIONS_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"
#include "Game/Enums/Directions4.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateChangeFeatureAnimationsBehavior();
static FeatureBehavior* CreateChangeFeatureAnimationsBehavior(const XMLNode& node);

class ChangeFeatureAnimations : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_ChangeFeatureAnimationsBehaviorRegistration;
	
	//Variables
	Directions4 m_direction = SOUTH;
	Directions4 m_originalDirection = SOUTH;
	int m_animationIdx = 0;
	int m_originalAnimationIdx = 0;
	bool m_enableToggling = false;
public:
	//Constructors
	ChangeFeatureAnimations();
	ChangeFeatureAnimations(const XMLNode& node);
	ChangeFeatureAnimations(const ChangeFeatureAnimations& other);
	virtual ~ChangeFeatureAnimations();

	//Operations
	virtual void CheckMapNodeForAttributesOfInterest(const XMLNode& node) override;
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};
#endif