#pragma once

#ifndef TILEDEFINITIONSPECIALCASETOSET_HPP
#define TILEDEFINITIONSPECIALCASETOSET_HPP
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/Math/IntVector2.hpp"

#define TILE_COORDS IntVector2

class Tile;
struct TileDefinitionSpecialCaseToSet
{
public:
	//Variables
	std::string m_debugName = "";
	TILE_COORDS m_TileCoordsToEditAdjustment = TILE_COORDS::ivec2_zeros;
	const SpriteResource* m_bottomResource = nullptr;
	const SpriteResource* m_resource = nullptr;
	const SpriteResource* m_topResource = nullptr;
	const SpriteResource* m_topResource2 = nullptr;
	const SpriteResource* m_altBottomResource = nullptr;
	const SpriteResource* m_altResource = nullptr;
	const SpriteResource* m_altTopResource = nullptr;
	const SpriteResource* m_altTopResource2 = nullptr;

	float m_altBottomResourceChance = 0.f;
	float m_altResourceChance = 0.f;
	float m_altTopResourceChance = 0.f;
	float m_altTopResource2Chance = 0.f;

	bool m_isSolid = false;
	bool m_isLiquid = false;
	bool m_overwriteBottomResource = false;
	bool m_overwriteResource = false;
	bool m_overwriteTopResource = false;
	bool m_overwriteTopResource2 = false;
	bool m_overwriteIsSolid = false;
	bool m_overwriteIsLiquid = false;

	//Constructors
	TileDefinitionSpecialCaseToSet();
	virtual ~TileDefinitionSpecialCaseToSet();

	//Setters
	void SetBottomResource(const SpriteResource* resource);
	void SetResource(const SpriteResource* resource);
	void SetTopResource(const SpriteResource* resource);
	void SetTopResource2(const SpriteResource* resource);
	void SetAltBottomResource(const SpriteResource* resource);
	void SetAltResource(const SpriteResource* resource);
	void SetAltTopResource(const SpriteResource* resource);
	void SetAltTopResource2(const SpriteResource* resource);
	void SetIsLiquid(bool overwriteIsLiquid, bool isLiquid);
	void SetIsSolid(bool overwriteIsSolid, bool isSolid);

	void WriteToSetToString(std::string& str, int indentationAmt) const;

	//Operations
	void ApplyContentToTile(Tile& tile) const;
protected:
	void ApplyContentToTileBasedOnShift(Tile* tile, TILE_COORDS& adjustment) const;
	void GetNeighborTile(Tile*& tile, TILE_COORDS& adjustment) const;
	void ApplyContentToTilePtr(Tile* tile) const;
};

#endif