#pragma once

#ifndef FEATUREBEHAVIORCHANGETILESPRITERESOURCES_HPP
#define FEATUREBEHAVIORCHANGETILESPRITERESOURCES_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"
#include "Engine/Math/IntVector2.hpp"

#define TILE_COORDS IntVector2

struct XMLNode;
class SpriteResource;
//Spawner Functions
static FeatureBehavior* DefaultCreateChangeTileSpriteResourcesOnPlacementFeatureBehavior();
static FeatureBehavior* CreateChangeTileSpriteResourcesOnPlacementFeatureBehavior(const XMLNode& node);

class FeatureBehaviorChangeTileSpriteResourcesOnPlacement : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_ChangeTileSpriteResourcesOnPlacementBehaviorRegistration;

	//Variables
	TILE_COORDS m_TileCoords = TILE_COORDS::ivec2_zeros;
	const SpriteResource* m_BottomResource = nullptr;
	const SpriteResource* m_Resource = nullptr;
	const SpriteResource* m_Top1Resource = nullptr;
	const SpriteResource* m_Top2Resource = nullptr;
public:
	//Constructors
	FeatureBehaviorChangeTileSpriteResourcesOnPlacement();
	FeatureBehaviorChangeTileSpriteResourcesOnPlacement(const XMLNode& node);
	FeatureBehaviorChangeTileSpriteResourcesOnPlacement(
		const FeatureBehaviorChangeTileSpriteResourcesOnPlacement& other);
	virtual ~FeatureBehaviorChangeTileSpriteResourcesOnPlacement();


	//Operations
	virtual void TriggerOnPlacement(const Map* map) override;
	virtual FeatureBehavior* Clone() override;
};

#endif