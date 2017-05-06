#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/ToggleTileSolid.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/Map/Map.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateToggleTileSolidBehavior()
{
	return new ToggleTileSolid();
}

STATIC FeatureBehavior* CreateToggleTileSolidBehavior(const XMLNode& node)
{
	return new ToggleTileSolid(node);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper 
	ToggleTileSolid::s_FeatureToggleTileSolidBehaviorRegistration
		= FeatureBehaviorRegistrationHelper("ToggleTileSolid", 
			CreateToggleTileSolidBehavior, DefaultCreateToggleTileSolidBehavior);

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Constructors
ToggleTileSolid::ToggleTileSolid()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

ToggleTileSolid::ToggleTileSolid(const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
}

ToggleTileSolid::ToggleTileSolid(const ToggleTileSolid& other)
	: FeatureBehavior(other),
	m_toggleTileOnFeatureOwningTile(other.m_toggleTileOnFeatureOwningTile),
	m_CustomTileCoords(other.m_CustomTileCoords)
{

}

ToggleTileSolid::~ToggleTileSolid()
{
	FeatureBehavior::~FeatureBehavior();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Operations
void ToggleTileSolid::CheckMapNodeForAttributesOfInterest(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "tilecoordstotoggle"
			|| attrName == "tile"
			|| attrName == "tilecoords"
			|| attrName == "position"
			|| attrName == "coords")
		{
			bool sizeOf1Found = false;
			m_CustomTileCoords = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1Found);
			if (sizeOf1Found)
			{
				m_toggleTileOnFeatureOwningTile = false;
			}
			else
			{
				m_toggleTileOnFeatureOwningTile = true;
			}
		}
	}
}

bool ToggleTileSolid::EntityInteractWithFeature(BaseGameEntity* entity)
{
	Map* map = Map::GetEditableMap();
	if (entity == nullptr
		|| m_owningFeature == nullptr
		|| map == nullptr)
	{
		return false;
	}
	TILE_COORDS coords = TILE_COORDS::ivec2_zeros;
	if (m_toggleTileOnFeatureOwningTile)
	{
		coords = m_owningFeature->GetTileCoords();
	}
	else
	{
		coords = m_CustomTileCoords;
	}
	const Tile* const_tile = map->GetTileAtCoords(coords);
	if (const_tile == nullptr)
	{
		return false;
	}
	Tile* tile = const_cast<Tile*>(const_tile);
	if (tile == nullptr)
	{
		return false;
	}
	bool solid = tile->GetTileIsSolid();
	tile->SetOverwrittenIsSolid(!solid, true);
	return true;
}

FeatureBehavior* ToggleTileSolid::Clone()
{
	return new ToggleTileSolid(*this);
}