#pragma once

#ifndef TILECOLLISIONTESTPACK_HPP
#define TILECOLLISIONTESTPACK_HPP
#include "Game/Map/Tile.hpp"

struct TileCollisionTestPack
{
public:
	//Variables
	const Tile* currentTile = nullptr;
	const Tile* northTile = nullptr;
	const Tile* eastTile = nullptr;
	const Tile* southTile = nullptr;
	const Tile* westTile = nullptr;
	const Tile* northEastTile = nullptr;
	const Tile* northWestTile = nullptr;
	const Tile* southWestTile = nullptr;
	const Tile* southEastTile = nullptr;
	bool currentTileIsSolid = false;
	bool northIsSolid = false;
	bool eastIsSolid = false;
	bool southIsSolid = false;
	bool westIsSolid = false;
	bool northEastIsSolid = false;
	bool northWestIsSolid = false;
	bool southWestIsSolid = false;
	bool southEastIsSolid = false;

public:
	//Constructors
	TileCollisionTestPack();
	virtual ~TileCollisionTestPack();

	//Static Functions
	static void LoadTileCollisionTestPack(TileCollisionTestPack& collisionPack, const Tile* tile);
};


#endif