#pragma once

#ifndef TILENEIGHBORPACK_HPP
#define TILENEIGHBORPACK_HPP
class Tile;

struct TileNeighborsPack
{
private:
protected:
public:
	//Neighbor Pointers
	Tile* m_NorthTile = nullptr;
	Tile* m_SouthTile = nullptr;
	Tile* m_EastTile = nullptr;
	Tile* m_WestTile = nullptr;
	Tile* m_NorthEastTile = nullptr;
	Tile* m_NorthWestTile = nullptr;
	Tile* m_SouthEastTile = nullptr;
	Tile* m_SouthWestTile = nullptr;

	//Constructors
	TileNeighborsPack();
	virtual ~TileNeighborsPack();
};

#endif