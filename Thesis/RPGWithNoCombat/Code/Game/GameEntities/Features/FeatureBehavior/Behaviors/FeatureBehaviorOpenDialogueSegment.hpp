#pragma once

#ifndef FEATUREBEHAVIOROPENDIALOGUESEGMENT_HPP
#define FEATUREBEHAVIOROPENDIALOGUESEGMENT_HPP
#include <vector>
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateFeatureOpenDialogueSegmentBehavior();
static FeatureBehavior* CreateFeatureOpenDialogueSegmentBehavior(const XMLNode& node);

class FeatureBehaviorOpenDialogueSegement : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureAddToAgentStatBehaviorRegistration;

	//Variables
	std::string m_dialogueSegmentToOpen = "";
public:
	//Constructors
	FeatureBehaviorOpenDialogueSegement();
	FeatureBehaviorOpenDialogueSegement(const XMLNode& node);
	FeatureBehaviorOpenDialogueSegement(const FeatureBehaviorOpenDialogueSegement& other);
	virtual ~FeatureBehaviorOpenDialogueSegement();

	//Operations
	virtual void CheckMapNodeForAttributesOfInterest(const XMLNode& node) override;
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;

};

#endif