#pragma once

#ifndef TELEPORT_HPP
#define TELEPORT_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"
#include "Engine/Math/IntVector2.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateFeatureTeleportBehavior();
static FeatureBehavior* CreateFeatureTeleportBehavior(const XMLNode& node);

#define TILE_COORDS IntVector2
class Teleport : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureTeleportBehaviorRegistration;

	//Variables
	TILE_COORDS m_tilePositionToTeleportTo = TILE_COORDS::ivec2_zeros;
	bool m_positionToTeleportToSet = false;
public:

	//Constructors
	Teleport();
	Teleport(const XMLNode& node);
	Teleport(const Teleport& other);
	virtual ~Teleport();

	//Operations
	virtual void CheckMapNodeForAttributesOfInterest(const XMLNode& node) override;
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};

#endif