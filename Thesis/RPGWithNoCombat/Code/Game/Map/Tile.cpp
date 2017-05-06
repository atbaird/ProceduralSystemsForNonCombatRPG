#include "Game/Map/Tile.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/MapCommonVars.hpp"
#include "Game/Collision/SquareCollision.hpp"
#include "Game/Collision/CircleCollision.hpp"
#include "Game/Collision/CollisionTestFunctions.hpp"
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Engine/Math/MathToStringUtils.hpp"


//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//static variables
STATIC Vector2 Tile::s_tileScale = Vector2(1.f, 1.f);

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Constructors
Tile::Tile(TILE_COORDS coords, unsigned int tileType, const SpriteResource* resource,
	SquareCollision* collision,
	bool isInteractable, bool isTriggerVolume, Map* owningMap)
	: 
	m_currentSprite(resource),
	m_blTileCoords(coords),
	m_blPos(Vector2::vec2_zeros),
	m_tileType(tileType),
	m_tileMask(0),
	m_interactableTile(isInteractable),
	m_triggerVolumeTile(isTriggerVolume),
	m_owningMap(owningMap)
{

	m_squareCollision = collision;
}

Tile::~Tile()
{
	if (m_specialTileEventPack != nullptr)
	{
		delete m_specialTileEventPack;
		m_specialTileEventPack = nullptr;
	}
}

void Tile::RunStandardConstructionMethod()
{
	const Map* map = m_owningMap;
	Vector2 mapCenter = Vector2::vec2_zeros;
	TILE_COORDS mapWidthHeight = TILE_COORDS::ivec2_ones;
	if (map != nullptr)
	{
		mapCenter = map->GetMapCenter();
		mapWidthHeight = map->GetMapWidthHeight();
	}

	Vector2 ActualWidthHeightOfMap = Vector2((float)mapWidthHeight.x * s_tileScale.x, 
		(float)mapWidthHeight.y * s_tileScale.y);
	Vector2 halfWidthHeight = ActualWidthHeightOfMap / 2.f;

	TILE_COORDS blCoordCopy = m_blTileCoords;
	m_blPos = (mapCenter - halfWidthHeight) + Vector2((float)m_blTileCoords.x, (float)m_blTileCoords.y);
	Vector2 pos = m_blPos + (s_tileScale / 2.f);
	SetPosition(pos);
	m_blTileCoords = blCoordCopy;
	//ChangeLayers(MapTile_SpriteLayer);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Updates and Render
void Tile::Update(const float& dt)
{
	dt;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Setters
void Tile::SetOwningMap(Map* map)
{
	m_owningMap = map;
}

void Tile::SetTileDefinition(const TileDefinition* tileDef)
{
	m_tileDef = tileDef;
}

void Tile::SetSpecialTileEventPack(TileEventPack* pack)
{
	m_specialTileEventPack = pack;
}

void Tile::SetTint(const Rgba& tint)
{
	m_tint = tint;
}

void Tile::SetPosition(const Vector2& position)
{
	if (position == m_position)
	{
		return;
	}
	m_position = position;

	const Map* map = Map::GetMap();
	if (map != nullptr)
	{
		m_blTileCoords = map->ConvertPositionToTileCoords(position);
	}
	else
	{
		m_blTileCoords = TILE_COORDS((int)round(position.x), (int)round(position.y)); //Why is the map null?
	}
}

void Tile::ChangeBottomSpriteResource(const SpriteResource* resource)
{
	if(m_currentBottomSprite != resource)
	{
		m_currentBottomSprite = resource;
	}
}

void Tile::ChangeSpriteResource(const SpriteResource* resource)
{
	if (resource != m_currentSprite)
	{
		m_currentSprite = resource;
		//m_UpdateMesh = true;

	}
}

void Tile::ChangeTopSpriteResource(const SpriteResource* resource)
{
	if (resource != m_currentTopSprite)
	{
		m_currentTopSprite = resource;
		//m_UpdateMesh = true;

	}
}

void Tile::ChangeTopSpriteResource2(const SpriteResource* resource)
{
	if (resource != m_currentTopSprite2)
	{
		m_currentTopSprite2 = resource;
	}
}

void Tile::SetOverwrittenIsSolid(bool isSolid, bool overwriteIsSolid)
{
	m_isSolid = isSolid;
	m_overwrittenIsSolid = overwriteIsSolid;
}

void Tile::SetOverwrittenIsLiquid(bool isLiquid, bool overwriteIsLiquid)
{
	m_isLiquid = isLiquid;
	m_overwrittenIsLiquid = overwriteIsLiquid;
}

void Tile::SetMask(int mask)
{
	m_tileMask = mask;
}

void Tile::SetNorthTile(Tile* tile)
{
	m_TileNeighborPack.m_NorthTile = tile;
}

void Tile::SetSouthTile(Tile* tile)
{
	m_TileNeighborPack.m_SouthTile = tile;
}

void Tile::SetEastTile(Tile* tile)
{
	m_TileNeighborPack.m_EastTile = tile;
}

void Tile::SetWestTile(Tile* tile)
{
	m_TileNeighborPack.m_WestTile = tile;
}

void Tile::SetNorthEastTile(Tile* tile)
{
	m_TileNeighborPack.m_NorthEastTile = tile;
}

void Tile::SetNorthWestTile(Tile* tile)
{
	m_TileNeighborPack.m_NorthWestTile = tile;
}

void Tile::SetSouthEastTile(Tile* tile)
{
	m_TileNeighborPack.m_SouthEastTile = tile;
}

void Tile::SetSouthWestTile(Tile* tile)
{
	m_TileNeighborPack.m_SouthWestTile = tile;
}

void Tile::SetHasSpecialMapDefinedEvents(bool hasEvents)
{
	m_hasSpecialMapDefinedEvents = hasEvents;
}

void Tile::SetInteractable(bool interactable)
{
	m_interactableTile = interactable;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Getters
void Tile::GetTileInfoForDeveloperDebug(std::vector<DeveloperDebugInfoBox>& infoBoxes) const
{
	//------------------------------------------------------------------------------
	//Tile Coordinates
	DeveloperDebugInfoBox infoBox;
	infoBox.SetBoxColor(Rgba(0.f, 0.f, 1.f, 0.5f));
	DeveloperDebugInfoTextBlurb CoordinateBlurb(
		"Tile Coordinates: " + MathToStringUtils::ToString(m_blTileCoords),
		Rgba::s_White);
	infoBox.AddTextBlurb(CoordinateBlurb);

	//------------------------------------------------------------------------------
	//Tile Definition
	Rgba tileDefColor = Rgba::s_White;
	std::string tileDefStr = "Tile: ";
	if (m_tileDef != nullptr)
	{
		std::string tileDefName = m_tileDef->GetDebugName();
		tileDefStr += tileDefName;
	}
	else
	{
		tileDefStr += "NULL";
		tileDefColor = Rgba::s_Red;
	}
	DeveloperDebugInfoTextBlurb tileDefBlurb(tileDefStr, tileDefColor);
	infoBox.AddTextBlurb(tileDefBlurb);

	//------------------------------------------------------------------------------
	//Bottom Sprite Resource
	Rgba bottomSpriteColor = Rgba::s_White;
	std::string bottomSpriteText = "Bottom Sprite: ";
	if (m_currentBottomSprite != nullptr)
	{
		std::string spriteResourceName = m_currentBottomSprite->GetSpriteResourceName();
		bottomSpriteText += spriteResourceName;
	}
	else
	{
		bottomSpriteText += "NULL";
	}

	if ((m_tileDef != nullptr
		&& m_currentBottomSprite != m_tileDef->GetBottomSpriteResource()
		&& m_currentBottomSprite != m_tileDef->GetAltBottomSpriteResource())
		|| (m_tileDef == nullptr && m_currentBottomSprite != nullptr))
	{
		bottomSpriteColor = Rgba::s_Red;
	}
	DeveloperDebugInfoTextBlurb bottomSpriteBlurb(bottomSpriteText, bottomSpriteColor);
	infoBox.AddTextBlurb(bottomSpriteBlurb);

	//------------------------------------------------------------------------------
	//Sprite Resource
	Rgba SpriteColor = Rgba::s_White;
	std::string SpriteText = "Sprite: ";
	if (m_currentSprite != nullptr)
	{
		std::string spriteResourceName = m_currentSprite->GetSpriteResourceName();
		SpriteText += spriteResourceName;
	}
	else
	{
		SpriteText += "NULL";
	}

	if ((m_tileDef != nullptr
		&& m_currentSprite != m_tileDef->GetSpriteResource()
		&& m_currentSprite != m_tileDef->GetAltSpriteResource())
		|| (m_tileDef == nullptr && m_currentSprite != nullptr))
	{
		SpriteColor = Rgba::s_Red;
	}
	DeveloperDebugInfoTextBlurb SpriteBlurb(SpriteText, SpriteColor);
	infoBox.AddTextBlurb(SpriteBlurb);

	//------------------------------------------------------------------------------
	//Top1 Sprite Resource
	Rgba top1SpriteColor = Rgba::s_White;
	std::string top1SpriteText = "Top1 Sprite: ";
	if (m_currentTopSprite != nullptr)
	{
		std::string spriteResourceName = m_currentTopSprite->GetSpriteResourceName();
		top1SpriteText += spriteResourceName;
	}
	else
	{
		top1SpriteText += "NULL";
	}

	if ((m_tileDef != nullptr
		&& m_currentTopSprite != m_tileDef->GetTopSpriteResource()
		&& m_currentTopSprite != m_tileDef->GetAltTopSpriteResource())
		|| (m_tileDef == nullptr && m_currentTopSprite != nullptr))
	{
		top1SpriteColor = Rgba::s_Red;
	}
	DeveloperDebugInfoTextBlurb topSpriteBlurb(top1SpriteText, top1SpriteColor);
	infoBox.AddTextBlurb(topSpriteBlurb);

	//------------------------------------------------------------------------------
	//Top2 Sprite Resource
	Rgba top2SpriteColor = Rgba::s_White;
	std::string top2SpriteText = "Top2 Sprite: ";
	if (m_currentTopSprite2 != nullptr)
	{
		std::string spriteResourceName = m_currentTopSprite2->GetSpriteResourceName();
		top2SpriteText += spriteResourceName;
	}
	else
	{
		top2SpriteText += "NULL";
	}

	if ((m_tileDef != nullptr
		&& m_currentTopSprite2 != m_tileDef->GetTopSprite2Resource()
		&& m_currentTopSprite2 != m_tileDef->GetAltTopSprite2Resource())
		|| (m_tileDef == nullptr && m_currentTopSprite2 != nullptr))
	{
		top2SpriteColor = Rgba::s_Red;
	}
	DeveloperDebugInfoTextBlurb top2SpriteBlurb(top2SpriteText, top2SpriteColor);
	infoBox.AddTextBlurb(top2SpriteBlurb);


	//------------------------------------------------------------------------------
	//Is Solid
	std::string solidText = "Is Solid: ";
	if ((m_overwrittenIsSolid && m_isSolid)
		|| (!m_overwrittenIsSolid && m_tileDef != nullptr && m_tileDef->GetIsSolid()))
	{
		solidText += "TRUE";
	}
	else
	{
		solidText += "FALSE";
	}

	Rgba solidColor = Rgba::s_White;
	if (m_overwrittenIsSolid)
	{
		solidColor = Rgba::s_Red;
	}
	DeveloperDebugInfoTextBlurb solidBlurb(solidText, solidColor);
	infoBox.AddTextBlurb(solidBlurb);
	infoBoxes.push_back(infoBox);
}

const Tile* Tile::GetPtrOfSelf() const
{
	return this;
}

Tile* Tile::GetEditablePtrOfSelf()
{
	return this;
}

const Vector2& Tile::GetBLPos() const
{
	return m_blPos;
}

unsigned int Tile::GetTileType() const
{
	return m_tileType;
}

int Tile::GetMask() const
{
	return m_tileMask;
}

const SquareCollision* Tile::GetSquareCollision() const
{
	return m_squareCollision;
}

SquareCollision* Tile::GetEditableSquareCollision() const
{
	return m_squareCollision;
}

bool Tile::GetTileIsSolid() const
{
	bool isSolid = false;
	if (m_overwrittenIsSolid)
	{
		isSolid = m_isSolid;
	}
	else
	{
		isSolid = TileDefinition::GetIsSolidFromMask(m_tileMask);
	}

	if (isSolid == true)
	{
		return isSolid;
	}

	const std::vector<BaseGameEntity*>* allFeatures = BaseGameEntity::GetAllFeaturesPtr();
	if (allFeatures == nullptr)
	{
		return isSolid;
	}

	for (size_t featureIdx = 0; featureIdx < allFeatures->size(); featureIdx++)
	{
		BaseGameEntity* featureEntity = allFeatures->at(featureIdx);
		Feature* feature = dynamic_cast<Feature*>(featureEntity);
		if (feature == nullptr)
		{
			continue;
		}
		TILE_COORDS coords = feature->GetTileCoords();
		if (coords != m_blTileCoords)
		{
			continue;
		}
		bool solid = feature->GetSolid();
		if (solid)
		{
			return true;
		}
	}
	return false;
}

bool Tile::GetTileIsWater() const
{
	if (m_overwrittenIsLiquid)
	{
		return m_isLiquid;
	}
	else
	{
		return TileDefinition::GetIsLiquidFromMask(m_tileMask);
	}
}

void Tile::WriteTileToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tile Coordinates: " + MathToStringUtils::ToString(m_blTileCoords) + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tint: " + MathToStringUtils::ToString(m_tint) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tile Definition: ";
	if (m_tileDef != nullptr)
	{
		str += m_tileDef->GetDebugName() + "\n";
	}
	else
	{
		str += "NULL\n";
	}

	if (m_overwrittenIsSolid)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Solid overwritten to: ";
		GameDebuggingCommons::WriteBoolOntoString(str, m_isSolid);
	}

	if (m_specialTileEventPack != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Tile events: \n";
		m_specialTileEventPack->WriteTileEventPackToString(str, indentationAmt + 3, false);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Operations
void Tile::SignUpForEvents()
{
	if((m_tileDef != nullptr && m_tileDef->GetOnTouchEvent() != nullptr)
		|| (m_specialTileEventPack != nullptr && m_specialTileEventPack->GetOnTouchEvent() != nullptr))
	{
		SignUpForOnTouchTest();
	}
	if ((m_tileDef != nullptr && m_tileDef->GetOnExitEvent() != nullptr)
		|| (m_specialTileEventPack != nullptr && m_specialTileEventPack->GetOnExitEvent() != nullptr))
	{
		SignUpForOnExitTest();
	}
	if ((m_tileDef != nullptr && m_tileDef->GetOnEnterEvent() != nullptr)
		|| (m_specialTileEventPack != nullptr && m_specialTileEventPack->GetOnEnterEvent() != nullptr))
	{
		SignUpForOnEnterTest();
	}

	if ((m_tileDef != nullptr && m_tileDef->GetOnInteractEvent() != nullptr)
		|| (m_specialTileEventPack != nullptr && m_specialTileEventPack->GetOnInteractEvent() != nullptr))
	{
		SignUpForOnInteractTest();
	}
}

void Tile::SignUpForOnTouchTest()
{
	EventSystem::GlobalRegisterEventCallbackMethod("OnTouch", this, &Tile::TestOnTouch);
}

void Tile::SignUpForOnEnterTest()
{
	EventSystem::GlobalRegisterEventCallbackMethod("OnEnter", this, &Tile::TestOnEnter);
}

void Tile::SignUpForOnExitTest()
{
	EventSystem::GlobalRegisterEventCallbackMethod("OnExit", this, &Tile::TestOnExit);
}

void Tile::SignUpForOnInteractTest()
{
	EventSystem::GlobalRegisterEventCallbackMethod("OnInteract", this, &Tile::TestOnInteract);
}

void Tile::UnsignUpForEvents()
{
	EventSystem::GlobalUnregisterFromAllEvents(this);
}


void Tile::TestOnTouch(NamedProperties& params)
{
	if (m_owningMap != nullptr && Map::GetMap() != m_owningMap)
	{
		return;
	}
	BaseGameEntity* entity = nullptr;

	params.Get("entity", entity, (BaseGameEntity*)nullptr);
	if (GetIfShouldPayAttentionToBaseGameEntity(entity, params, true, this) == false)
	{
		return;
	}
	for (size_t i = 0; i < m_entitiesInsideThisTile.size(); i++)
	{
		if (m_entitiesInsideThisTile[i] == entity)
		{
			m_entitiesInsideThisTile.erase(m_entitiesInsideThisTile.begin() + i);
			return;
		}
	}
	m_entitiesInsideThisTile.push_back(entity);
	if (m_tileDef != nullptr)
	{
		TileEvent* event = m_tileDef->GetOnTouchEvent();
		if (event != nullptr)
		{
			std::vector<StrXMLAttr> OnTouchParams = m_tileDef->GetOnTouchParams();
			event->CallFunction(this, params, OnTouchParams);
		}
	}
	if (m_specialTileEventPack != nullptr)
	{
		TileEvent* specialEvent = m_specialTileEventPack->GetEditableOnTouchEvent();
		if (specialEvent != nullptr)
		{
			std::vector<StrXMLAttr> OnTouchParams = m_specialTileEventPack->GetOnTouchParams();
			specialEvent->CallFunction(this, params, OnTouchParams);
		}
	}

}


void Tile::TestOnEnter(NamedProperties& params)
{
	if (m_owningMap != nullptr && Map::GetMap() != m_owningMap)
	{
		return;
	}
	BaseGameEntity* entity = nullptr;
	TILE_COORDS entityCoordsOriginal = TILE_COORDS(-1, -1);

	params.Get("entity", entity, (BaseGameEntity*)nullptr);
	params.Get("originalTileCoords", entityCoordsOriginal, TILE_COORDS(-1,-1));
	if (entity == nullptr)
	{
		return;
	}
	TILE_COORDS entityCoords = entity->GetTileCoords();
	if (entityCoords != m_blTileCoords)
	{
		return;
	}
	if (m_tileDef != nullptr)
	{
		TileEvent* event = m_tileDef->GetOnEnterEvent();
		if (event != nullptr)
		{
			std::vector<StrXMLAttr> OnEnterParams = m_tileDef->GetOnEnterParams();
			m_tileDef->GetOnEnterEvent()->CallFunction(this, params, OnEnterParams);
		}
	}
	if (m_specialTileEventPack != nullptr)
	{
		TileEvent* specialEvent = m_specialTileEventPack->GetEditableOnEnterEvent();
		if (specialEvent != nullptr)
		{
			std::vector<StrXMLAttr> OnEnterParams = m_specialTileEventPack->GetOnEnterParams();
			specialEvent->CallFunction(this, params, OnEnterParams);
		}
	}
}

void Tile::TestOnExit(NamedProperties& params)
{
	if (m_owningMap != nullptr && Map::GetMap() != m_owningMap)
	{
		return;
	}
	BaseGameEntity* entity = nullptr;

	params.Get("entity", entity, (BaseGameEntity*)nullptr);
	if (GetIfShouldPayAttentionToBaseGameEntity(entity, params, false, this) == false)
	{
		return;
	}
	for (size_t i = 0; i < m_entitiesInsideThisTile.size(); i++)
	{
		if (m_entitiesInsideThisTile[i] == entity)
		{
			m_entitiesInsideThisTile.erase(m_entitiesInsideThisTile.begin() + i);
			break;
		}
	}
	if (m_tileDef != nullptr)
	{
		TileEvent* event = m_tileDef->GetOnExitEvent();
		if (event != nullptr)
		{
			std::vector<StrXMLAttr> OnExitParams = m_tileDef->GetOnExitParams();
			event->CallFunction(this, params, OnExitParams);
		}
	}
	if (m_specialTileEventPack != nullptr)
	{
		TileEvent* specialEvent = m_specialTileEventPack->GetEditableOnExitEvent();
		if (specialEvent != nullptr)
		{
			std::vector<StrXMLAttr> OnExitParams = m_specialTileEventPack->GetOnExitParams();
			m_specialTileEventPack->GetEditableOnExitEvent()->CallFunction(this, params, OnExitParams);
		}
	}
}

void Tile::TestOnInteract(NamedProperties& params)
{
	if (m_owningMap != nullptr && Map::GetMap() != m_owningMap)
	{
		return;
	}
	BaseGameEntity* entity = nullptr;

	params.Get("entity", entity, (BaseGameEntity*)nullptr);
	if (GetIfShouldPayAttentionToBaseGameEntity(entity, params, true, this) == false)
	{
		return;
	}
	if (m_tileDef != nullptr)
	{
		TileEvent* event = m_tileDef->GetOnInteractEvent();
		if (event != nullptr)
		{
			std::vector<StrXMLAttr> OnInteractParams = m_tileDef->GetOnInteractParams();
			event->CallFunction(this, params, OnInteractParams);
		}
	}
	if (m_specialTileEventPack != nullptr)
	{
		TileEvent* specialEvent = m_specialTileEventPack->GetEditableOnInteractEvent();
		if ( specialEvent != nullptr)
		{
			std::vector<StrXMLAttr> OnInteractParams = m_specialTileEventPack->GetOnInteractParams();
			m_specialTileEventPack->GetEditableOnInteractEvent()->CallFunction(this, params, OnInteractParams);
		}
	}
}

bool Tile::GetIfShouldPayAttentionToBaseGameEntity(const BaseGameEntity* entity, NamedProperties& params, bool onEnter, Tile* myself)
{
	if (entity == nullptr
		|| myself == nullptr)
	{
		return false;
	}
	TILE_COORDS myTileCoords = myself->GetTileCoords();
	Collision* collision = entity->GetCollision();
	if (collision == nullptr)
	{
		return false;
	}
	CollisionType colType = collision->GetCollisionType();
	Vector2 orgPosition = entity->GetPosition();
	TILE_COORDS orgCoords = entity->GetTileCoords();
	if (onEnter == false)
	{
		//OnExit test
		PropertyGetResult paramResult = params.Get("originalPosition", orgPosition);
		if (paramResult != PROPERTY_FETCH_SUCCESS)
		{
			return false;
		}
		paramResult = params.Get("originalTileCoords", orgCoords);
		if (paramResult != PROPERTY_FETCH_SUCCESS)
		{
			return false;
		}
	}
	//TILE_COORDS diff = orgCoords - myTileCoords;
	//float distance = diff.getLength();
	//if (distance > 3.f)
	//{
	//	return false;
	//}
	const Map* map = Map::GetMap();
	if (map == nullptr)
	{
		return false;
	}
	const Tile* tile = map->GetTileAtCoords(orgCoords);
	if (tile == nullptr)
	{
		return false;
	}

	if (colType == INVALID_COLLISION)
	{
		return false;
	}
	else if (colType == CIRCLE_COLLISION)
	{
		return Tile::GetIfShouldPayAttentionToBaseGameEntityCircleCollision(entity, collision, orgPosition, orgCoords, this, tile, onEnter);
	}
	else if (colType == SQUARE_COLLISION)
	{
	}

	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//static methods
STATIC void Tile::SetTileScale(const Vector2& tileScale)
{
	s_tileScale = tileScale;
	Map* map = Map::GetEditableMap();
	if (map != nullptr)
	{
		map->SetTileCollisionSize(tileScale);
	}
}

STATIC const Vector2& Tile::GetTileScale()
{
	return s_tileScale;
}

STATIC bool Tile::GetIfShouldPayAttentionToBaseGameEntityCircleCollision(const BaseGameEntity* entity, Collision* collision,
	const Vector2& orgPosition,	const TILE_COORDS& orgCoords, Tile* self, const Tile* tile, bool onEnter)
{
	orgCoords;
	entity;
	bool read = false;
	bool diag = false;
	bool xDirNeg = false;
	bool yDirNeg = false;
	bool expectOnX = false;
	bool expectOnY = false;
	if (tile == self)
	{
		read = true;
	}
	else
	{
		TILE_COORDS coordsTest = self->GetTileCoords() - tile->GetTileCoords();
		if (tile->GetNorthEastTile() == self
			|| tile->GetNorthWestTile() == self
			|| tile->GetSouthWestTile() == self
			|| tile->GetSouthEastTile() == self)
		{
			diag = true;
			read = true;
		}
		else if (tile->GetNorthTile() == self
			|| tile->GetWestTile() == self
			|| tile->GetSouthTile() == self
			|| tile->GetEastTile() == self)
		{
			read = true;
		}
		if (coordsTest.x != 0)
		{
			expectOnX = true;
			if (coordsTest.x < 0)
			{
				xDirNeg = true;
			}
		}
		if (coordsTest.y != 0)
		{
			expectOnY = true;
			if (coordsTest.y < 0)
			{
				yDirNeg = true;
			}
		}
	}
	if (read == false)
	{
		return false;
	}
	CircleCollision* circCol = dynamic_cast<CircleCollision*>(collision);
	CollisionTestResult result;
	if (diag)
	{
		result = CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionDiag(self->m_squareCollision,
			self->m_blTileCoords, circCol, orgPosition, xDirNeg, yDirNeg);
	}
	else
	{
		result = CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionNESW(self->m_squareCollision, self->m_blPos, circCol,
			orgPosition, expectOnX, (expectOnX && xDirNeg) || (expectOnX == false && yDirNeg));
	}
	return result.GetOverlapDetected() == onEnter;
}