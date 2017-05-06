#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/FeatureBehaviorChangeTileSpriteResourcesOnPlacement.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/Map/Map.hpp"

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateChangeTileSpriteResourcesOnPlacementFeatureBehavior()
{
	return new FeatureBehaviorChangeTileSpriteResourcesOnPlacement();
}

STATIC FeatureBehavior* CreateChangeTileSpriteResourcesOnPlacementFeatureBehavior(const XMLNode& node)
{
	return new FeatureBehaviorChangeTileSpriteResourcesOnPlacement(node);
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper 
	FeatureBehaviorChangeTileSpriteResourcesOnPlacement::s_ChangeTileSpriteResourcesOnPlacementBehaviorRegistration
		= FeatureBehaviorRegistrationHelper("ChangeTileSpriteResourcesOnPlacement", 
			CreateChangeTileSpriteResourcesOnPlacementFeatureBehavior,
			DefaultCreateChangeTileSpriteResourcesOnPlacementFeatureBehavior);

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//Constructors
FeatureBehaviorChangeTileSpriteResourcesOnPlacement::FeatureBehaviorChangeTileSpriteResourcesOnPlacement()
	: FeatureBehavior(ON_PLACEMENT_FEATURE_BEHAVIOR)
{
}

FeatureBehaviorChangeTileSpriteResourcesOnPlacement::FeatureBehaviorChangeTileSpriteResourcesOnPlacement(
	const XMLNode& node)
	: FeatureBehavior(node, ON_PLACEMENT_FEATURE_BEHAVIOR)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "tilecoords")
		{
			bool sizeOf1Found = false;
			bool sizeOf2Found = false;
			TILE_COORDS coords = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1Found, sizeOf2Found);
			if (sizeOf1Found)
			{
				m_TileCoords.x = coords.x;
			}
			if (sizeOf2Found)
			{
				m_TileCoords.y = coords.y;
			}
		}
		else if (attrName == "resource"
			|| attrName == "resourcename")
		{
			m_Resource = SpriteResourceHandler::GetSpriteResourceByName(attr.lpszValue);
		}
		else if (attrName == "bottomresource"
			|| attrName == "bottomresourcename")
		{
			m_BottomResource = SpriteResourceHandler::GetSpriteResourceByName(attr.lpszValue);
		}
		else if (attrName == "topresource"
			|| attrName == "topresourcename"
			|| attrName == "top1resource"
			|| attrName == "top1resourcename"
			|| attrName == "topresource1"
			|| attrName == "topresource1name")
		{
			m_Top1Resource = SpriteResourceHandler::GetSpriteResourceByName(attr.lpszValue);
		}
		else if (attrName == "top2resource"
			|| attrName == "top2resourcename"
			|| attrName == "topresource2"
			|| attrName == "topresource2name")
		{
			m_Top2Resource = SpriteResourceHandler::GetSpriteResourceByName(attr.lpszValue);
		}
	}
}

FeatureBehaviorChangeTileSpriteResourcesOnPlacement::FeatureBehaviorChangeTileSpriteResourcesOnPlacement(
	const FeatureBehaviorChangeTileSpriteResourcesOnPlacement& other)
	: FeatureBehavior(other),
	m_TileCoords(other.m_TileCoords),
	m_BottomResource(other.m_BottomResource),
	m_Resource(other.m_Resource),
	m_Top1Resource(other.m_Top1Resource),
	m_Top2Resource(other.m_Top2Resource)
{

}

FeatureBehaviorChangeTileSpriteResourcesOnPlacement::~FeatureBehaviorChangeTileSpriteResourcesOnPlacement()
{
	FeatureBehavior::~FeatureBehavior();
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//Operations
void FeatureBehaviorChangeTileSpriteResourcesOnPlacement::TriggerOnPlacement(const Map* map)
{
	if (map == nullptr
		|| m_owningFeature == nullptr)
	{
		return;
	}
	int tileIdx = map->GetTileIdxForCoords(m_TileCoords);
	if (tileIdx == -1)
	{
		return;
	}

	const Tile* const_tile = map->GetTileAtIdx(tileIdx);
	Tile* tile = const_cast<Tile*>(const_tile);
	if (tile == nullptr)
	{
		return;
	}
	if (m_BottomResource != nullptr)
	{
		tile->ChangeBottomSpriteResource(m_BottomResource);
	}

	if (m_Resource != nullptr)
	{
		tile->ChangeSpriteResource(m_Resource);
	}

	if (m_Top1Resource != nullptr)
	{
		tile->ChangeTopSpriteResource(m_Top1Resource);
	}

	if (m_Top2Resource != nullptr)
	{
		tile->ChangeTopSpriteResource2(m_Top2Resource);
	}
}

FeatureBehavior* FeatureBehaviorChangeTileSpriteResourcesOnPlacement::Clone()
{
	return new FeatureBehaviorChangeTileSpriteResourcesOnPlacement(*this);
}