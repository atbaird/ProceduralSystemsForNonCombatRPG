#pragma once

#ifndef TILE_HPP
#define TILE_HPP
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Engine/2D/AnimatedSprite.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/Map/TileNeighborsPack.hpp"
#include "Game/Map/TileDefinition.hpp"
#include "Game/Debugging/DeveloperDebugInfoBox.hpp"

#define TILE_COORDS IntVector2
class SquareCollision;
class Map;

class Tile
{
private:
protected:
	//static variables
	static Vector2 s_tileScale;

public:
	std::vector<SpecialCaseTestResults> m_specialCaseResults;
protected:
	std::vector<BaseGameEntity*> m_entitiesInsideThisTile;
	std::vector<std::vector<SpriteAnimationSequence*>> m_sequences;
	const SpriteResource* m_currentBottomSprite = nullptr;
	const SpriteResource* m_currentSprite = nullptr;
	const SpriteResource* m_currentTopSprite = nullptr;
	const SpriteResource* m_currentTopSprite2 = nullptr;
	Map* m_owningMap = nullptr;

	//variables
	SquareCollision* m_squareCollision = nullptr;
	TILE_COORDS m_blTileCoords;
	Rgba m_tint;
	Vector2 m_blPos = Vector2(0.f, 0.f);
	Vector2 m_position = Vector2(0.f, 0.f);
	unsigned int m_tileType = 0;
	int m_tileMask = 0;
	TileNeighborsPack m_TileNeighborPack;

	const TileDefinition* m_tileDef = nullptr;
	TileEventPack* m_specialTileEventPack = nullptr;
	//TileEventPack* m_tileEventPack = nullptr; //<- bugged out at the moment

	//Bools
	bool m_hasSpecialMapDefinedEvents = false;
	bool m_interactableTile = false;
	bool m_triggerVolumeTile = false; //useful if one to play a particular sound on enter or exit or something.
	bool m_isSolid = false;
	bool m_isLiquid = false;
	bool m_overwrittenIsSolid = false;
	bool m_overwrittenIsLiquid = false;
	bool m_PerformedTestsYet = false;
public:

	//Constructors
	Tile(TILE_COORDS coords = TILE_COORDS::ivec2_zeros, unsigned int tileType = 0, 
		const SpriteResource* resource = nullptr,
		SquareCollision* collision = nullptr, bool isInteractable = false, bool isTriggerVolume = false,
		Map* owningMap = nullptr);
	virtual ~Tile();

	void RunStandardConstructionMethod();

public:

	//Updates and Render
	virtual void Update(const float& dt);

	//Setters
	void SetPerformedTestsYet(bool performedTests) { m_PerformedTestsYet = performedTests; };
	void SetTileCoords(const TILE_COORDS& coords) { m_blTileCoords = coords; };
	void SetTileType(int tileType) { m_tileType = tileType; };
	void SetIsTriggerVolume(bool isTrigger) { m_triggerVolumeTile = isTrigger; };
	void SetCollision(SquareCollision* col) { m_squareCollision = col; };
	void SetBLPosition(const Vector2& pos) { m_blPos = pos; };
	void SetTileCenter(const Vector2& pos) { m_position = pos; };
	virtual void SetOwningMap(Map* map);
	virtual void SetTileDefinition(const TileDefinition* tileDef);
	virtual void SetSpecialTileEventPack(TileEventPack* pack);
	virtual void SetTint(const Rgba& tint);
	virtual void SetPosition(const Vector2& position);
	virtual void ChangeBottomSpriteResource(const SpriteResource* resource);
	virtual void ChangeSpriteResource(const SpriteResource* resource);
	virtual void ChangeTopSpriteResource(const SpriteResource* resource);
	virtual void ChangeTopSpriteResource2(const SpriteResource* resource);
	void SetOverwrittenIsSolid(bool isSolid, bool overwriteIsSolid);
	void SetOverwrittenIsLiquid(bool isLiquid, bool overwriteIsLiquid);
	void SetMask(int mask);
	void SetNorthTile(Tile* tile);
	void SetSouthTile(Tile* tile);
	void SetEastTile(Tile* tile);
	void SetWestTile(Tile* tile);
	void SetNorthEastTile(Tile* tile);
	void SetNorthWestTile(Tile* tile);
	void SetSouthEastTile(Tile* tile);
	void SetSouthWestTile(Tile* tile);
	void SetHasSpecialMapDefinedEvents(bool hasEvents);
	void SetInteractable(bool interactable);

	//Getters
	void GetTileInfoForDeveloperDebug(std::vector<DeveloperDebugInfoBox>& infoBoxes) const;
protected:
public:
	const Tile* GetPtrOfSelf() const;
	Tile* GetEditablePtrOfSelf();
	const Vector2& GetBLPos() const;
	unsigned int GetTileType() const;
	int GetMask() const;
	const SquareCollision* GetSquareCollision() const;
	SquareCollision* GetEditableSquareCollision() const;
	bool GetTileIsSolid() const;
	bool GetTileIsWater() const;
	void WriteTileToString(std::string& str, int indentationAmt) const;
	
	const Tile* GetNorthTile() const { return m_TileNeighborPack.m_NorthTile; };
	const Tile* GetSouthTile() const { return m_TileNeighborPack.m_SouthTile; };
	const Tile* GetEastTile() const { return m_TileNeighborPack.m_EastTile; };
	const Tile* GetWestTile() const { return m_TileNeighborPack.m_WestTile; };
	const Tile* GetNorthEastTile() const { return m_TileNeighborPack.m_NorthEastTile; };
	const Tile* GetNorthWestTile() const { return m_TileNeighborPack.m_NorthWestTile; };
	const Tile* GetSouthEastTile() const { return m_TileNeighborPack.m_SouthEastTile; };
	const Tile* GetSouthWestTile() const { return m_TileNeighborPack.m_SouthWestTile; };
	Tile* GetEditableNorthTile() const { return m_TileNeighborPack.m_NorthTile; };
	Tile* GetEditableSouthTile() const { return m_TileNeighborPack.m_SouthTile; };
	Tile* GetEditableEastTile() const { return m_TileNeighborPack.m_EastTile; };
	Tile* GetEditableWestTile() const { return m_TileNeighborPack.m_WestTile; };
	Tile* GetEditableNorthEastTile() const { return m_TileNeighborPack.m_NorthEastTile; };
	Tile* GetEditableNorthWestTile() const { return m_TileNeighborPack.m_NorthWestTile; };
	Tile* GetEditableSouthEastTile() const { return m_TileNeighborPack.m_SouthEastTile; };
	Tile* GetEditableSouthWestTile() const { return m_TileNeighborPack.m_SouthWestTile; };
	const Vector2& GetPosition() const { return m_position; };
	const Rgba& GetTint() const { return m_tint; };
	virtual const TILE_COORDS& GetTileCoords() const { return m_blTileCoords; };
	const SpriteResource* GetCurrentBottomSpriteResource() const { return m_currentBottomSprite; };
	const SpriteResource* GetCurrentSpriteResource() const { return m_currentSprite; };
	const SpriteResource* GetCurrentTopSpriteResource() const { return m_currentTopSprite; };
	const SpriteResource* GetCurrentTopSpriteResource2() const { return m_currentTopSprite2; };
	const bool GetHasSpecialMapDefinedEvents() const { return m_hasSpecialMapDefinedEvents; };
	const TileDefinition* GetTileDefinition() const { return m_tileDef; };

	//Operations
	virtual void SignUpForEvents();
	virtual void SignUpForOnTouchTest();
	virtual void SignUpForOnEnterTest();
	virtual void SignUpForOnExitTest();
	virtual void SignUpForOnInteractTest();
	virtual void UnsignUpForEvents();

	void TestOnTouch(NamedProperties& params);
	void TestOnEnter(NamedProperties& params);
	void TestOnExit(NamedProperties& params);
	void TestOnInteract(NamedProperties& params);

	bool GetIfShouldPayAttentionToBaseGameEntity(const BaseGameEntity* entity, NamedProperties& params,
		bool onTouch = true, 
		Tile* myself = nullptr);

	//static methods
	static void SetTileScale(const Vector2& tileScale);
	static const Vector2& GetTileScale();
protected:
	static bool GetIfShouldPayAttentionToBaseGameEntityCircleCollision(const BaseGameEntity* entity, Collision* collision,
		const Vector2& orgPosition, const TILE_COORDS& orgCoords, Tile* self, const Tile* tile, bool onTouch);
};
#endif