#pragma once

#ifndef SWITCHSIDES_HPP
#define SWITCHSIDES_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateFeatureSwitchSidesBehavior();
static FeatureBehavior* CreateFeatureSwitchSidesBehavior(const XMLNode& node);

class SwitchSides : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureSwitchSidesBehaviorRegistration;

public:
	//Constructors
	SwitchSides();
	SwitchSides(const XMLNode& node);
	SwitchSides(const SwitchSides& other);
	virtual ~SwitchSides();

	//Operations
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};
#endif