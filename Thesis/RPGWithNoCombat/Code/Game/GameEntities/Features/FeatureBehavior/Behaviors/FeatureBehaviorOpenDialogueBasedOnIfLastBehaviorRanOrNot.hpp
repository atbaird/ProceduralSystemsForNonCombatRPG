#pragma once

#ifndef FEATUREBEHAVIOROPENDIALOGUEBASEDONIFLASTBEHAVIORRANORNOT_HPP
#define FEATUREBEHAVIOROPENDIALOGUEBASEDONIFLASTBEHAVIORRANORNOT_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehaviorRegistrationHelper.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateOpenDialogueBasedOnIfLastBehaviorRanOrNotBehavior();
static FeatureBehavior* CreateOpenDialogueBasedOnIfLastBehaviorRanOrNotBehavior(const XMLNode& node);

class FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper 
		s_OpenDialogueBasedOnIfLastBehaviorRanOrNotBehaviorRegistration;

	//Variables
	std::string m_dialogueSegmentToOpenIfRan = "";
	std::string m_dialogueSegmentToOpenIfDidNotRun = "";
public:
	//Constructors
	FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot();
	FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot(const XMLNode& node);
	FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot(
		const FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot& other);
	virtual ~FeatureBehaviorOpenDialogueBasedOnIfLastBehaviorRanOrNot();

	//Operations
	virtual void CheckMapNodeForAttributesOfInterest(const XMLNode& node) override;
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};

#endif // !FEATUREBEHAVIOROPENDIALOGUEBASEDONIFLASTBEHAVIORRANORNOT_HPP