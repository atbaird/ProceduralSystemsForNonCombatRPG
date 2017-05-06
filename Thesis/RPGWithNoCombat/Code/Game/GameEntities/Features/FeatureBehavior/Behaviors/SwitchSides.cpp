#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/SwitchSides.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/Map/Map.hpp"

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateFeatureSwitchSidesBehavior()
{
	return new SwitchSides();
}

STATIC FeatureBehavior* CreateFeatureSwitchSidesBehavior(const XMLNode& node)
{
	return new SwitchSides(node);
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper
SwitchSides::s_FeatureSwitchSidesBehaviorRegistration = FeatureBehaviorRegistrationHelper(
	"switchsides", CreateFeatureSwitchSidesBehavior, DefaultCreateFeatureSwitchSidesBehavior);


//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Constructors
SwitchSides::SwitchSides()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

SwitchSides::SwitchSides(const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{

}

SwitchSides::SwitchSides(const SwitchSides& other)
	: FeatureBehavior(other)
{

}

SwitchSides::~SwitchSides()
{
	FeatureBehavior::~FeatureBehavior();
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Operations
bool SwitchSides::EntityInteractWithFeature(BaseGameEntity* entity)
{
	bool canInteract = FeatureBehavior::EntityInteractWithFeature(entity);
	if (canInteract == false)
	{
		return false;
	}
	Map* map = Map::GetEditableMap();
	TILE_COORDS ownerCoords = m_owningFeature->GetTileCoords();
	TILE_COORDS entityCoords = entity->GetTileCoords();
	TILE_COORDS difference = ownerCoords - entityCoords;
	if (map == nullptr || abs(difference.x) > 1 || abs(difference.y) > 1)
	{
		return false;
	}

	TILE_COORDS oppositeCoords = ownerCoords + difference;
	const Tile* tile = map->GetTileAtCoords(oppositeCoords);
	if (tile == nullptr)
	{
		return false;
	}
	bool tileIsSolid = tile->GetTileIsSolid();
	if (tileIsSolid)
	{
		return false;
	}

	Vector2 centerTile = tile->GetPosition();
	entity->SetPosition(centerTile);

	return true;
}

FeatureBehavior* SwitchSides::Clone()
{
	SwitchSides* sides = new SwitchSides(*this);
	return sides;
}