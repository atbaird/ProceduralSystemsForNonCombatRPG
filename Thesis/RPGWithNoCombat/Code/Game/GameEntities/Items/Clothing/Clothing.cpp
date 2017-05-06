#include "Game/GameEntities/Items/Clothing/Clothing.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

STATIC Item* ClothingDefaultSpawner(const SpriteResource* resource, Directions4 defaultDirection)
{
	return new Clothing(resource, defaultDirection);
}

STATIC Item* ClothingSpawn(const XMLNode& node)
{
	return new Clothing(node);
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Static Variables
STATIC ItemTypeSpawnerRegistration Clothing::s_ClothingTypeSpawnerRegistration 
	= ItemTypeSpawnerRegistration(ITEM_TYPE_CLOTHING, ClothingDefaultSpawner, ClothingSpawn);

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Constructors
Clothing::Clothing(const SpriteResource* resource, Directions4 defaultDirection)
	: Item(ITEM_TYPE_CLOTHING, resource, defaultDirection)
{
	m_ableToBeEquipped = true;
}

Clothing::Clothing(const XMLNode& node)
	: Item(ITEM_TYPE_CLOTHING, node)
{

}

Clothing::~Clothing()
{
	Item::~Item();
}