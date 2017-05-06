#pragma once

#ifndef TOGGLETILESOLID_HPP
#define TOGGLETILESOLID_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"
#include "Engine/Math/IntVector2.hpp"


//Spawner Functions
static FeatureBehavior* DefaultCreateToggleTileSolidBehavior();
static FeatureBehavior* CreateToggleTileSolidBehavior(const XMLNode& node);

#define TILE_COORDS IntVector2
class ToggleTileSolid : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureToggleTileSolidBehaviorRegistration;

	//Variables
	bool m_toggleTileOnFeatureOwningTile = true;
	TILE_COORDS m_CustomTileCoords = TILE_COORDS::ivec2_zeros;
public:
	//Constructors
	ToggleTileSolid();
	ToggleTileSolid(const XMLNode& node);
	ToggleTileSolid(const ToggleTileSolid& other);
	virtual ~ToggleTileSolid();

	//Operations
	virtual void CheckMapNodeForAttributesOfInterest(const XMLNode& node) override;
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};
#endif