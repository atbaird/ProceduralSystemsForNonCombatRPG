#include "Game/GameEntities/Agents/TileCollisionTestPack.hpp"

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//Constructors
TileCollisionTestPack::TileCollisionTestPack()
{

}

TileCollisionTestPack::~TileCollisionTestPack()
{

}

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//Static Functions
STATIC void TileCollisionTestPack::LoadTileCollisionTestPack(TileCollisionTestPack& collisionPack, const Tile* tile)
{
	collisionPack = TileCollisionTestPack();
	collisionPack.currentTile = tile;
	if (tile != nullptr)
	{
		collisionPack.northTile = tile->GetNorthTile();
		collisionPack.eastTile = tile->GetEastTile();
		collisionPack.southTile = tile->GetSouthTile();
		collisionPack.westTile = tile->GetWestTile();
		collisionPack.currentTileIsSolid = tile->GetTileIsSolid();
		collisionPack.northEastTile = tile->GetNorthEastTile();
		collisionPack.northWestTile = tile->GetNorthWestTile();
		collisionPack.southEastTile = tile->GetSouthEastTile();
		collisionPack.southWestTile = tile->GetSouthWestTile();
		collisionPack.northIsSolid = (collisionPack.northTile != nullptr
			&& collisionPack.northTile->GetTileIsSolid());
		collisionPack.eastIsSolid = collisionPack.eastTile != nullptr
			&& collisionPack.eastTile->GetTileIsSolid();
		collisionPack.westIsSolid = collisionPack.westTile != nullptr
			&& collisionPack.westTile->GetTileIsSolid();
		collisionPack.southIsSolid = collisionPack.southTile != nullptr
			&& collisionPack.southTile->GetTileIsSolid();
		collisionPack.northEastIsSolid = collisionPack.northEastTile != nullptr
			&& collisionPack.northEastTile->GetTileIsSolid();
		collisionPack.northWestIsSolid = collisionPack.northWestTile != nullptr
			&& collisionPack.northWestTile->GetTileIsSolid();
		collisionPack.southEastIsSolid = collisionPack.southEastTile != nullptr
			&& collisionPack.southEastTile->GetTileIsSolid();
		collisionPack.southWestIsSolid = collisionPack.southWestTile != nullptr
			&& collisionPack.southWestTile->GetTileIsSolid();
	}
}