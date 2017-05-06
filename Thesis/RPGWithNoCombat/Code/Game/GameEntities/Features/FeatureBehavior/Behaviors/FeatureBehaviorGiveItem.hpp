#pragma once

#ifndef FEATUREBEHAVIORGIVEITEM_HPP
#define FEATUREBEHAVIORGIVEITEM_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateFeatureGiveItemSegmentBehavior();
static FeatureBehavior* CreateFeatureGiveItemSegmentBehavior(const XMLNode& node);

class FeatureBehaviorGiveItem : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureGiveItemBehaviorRegistration;

	//Variables
	std::string m_nameOfItemToGive = "";
	float m_amountOfItemToGive = 0.f;
public:
	//Constructors
	FeatureBehaviorGiveItem();
	FeatureBehaviorGiveItem(const XMLNode& node);
	FeatureBehaviorGiveItem(const FeatureBehaviorGiveItem& other);
	virtual ~FeatureBehaviorGiveItem();

	//Operations
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};
#endif