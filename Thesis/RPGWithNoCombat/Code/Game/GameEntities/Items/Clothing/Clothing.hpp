#pragma once

#ifndef CLOTHING_HPP
#define CLOTHING_HPP
#include "Game/GameEntities/Items/Item.hpp"

static Item* ClothingDefaultSpawner(const SpriteResource* resource, Directions4 defaultDirection);
static Item* ClothingSpawn(const XMLNode& node);

class Clothing : public Item
{
private:
protected:
	static ItemTypeSpawnerRegistration s_ClothingTypeSpawnerRegistration;
public:
	//Constructors
	Clothing(const SpriteResource* resource = nullptr, Directions4 defaultDirection = SOUTH);
	Clothing(const XMLNode& node);
	virtual ~Clothing();
};
#endif