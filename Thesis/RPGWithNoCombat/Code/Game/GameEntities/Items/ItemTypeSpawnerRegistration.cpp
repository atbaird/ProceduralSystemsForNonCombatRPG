#include "Game/GameEntities/Items/ItemTypeSpawnerRegistration.hpp"
#include "Game/GameEntities/Items/Item.hpp"

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Static Variables
STATIC std::map<ItemType, ItemTypeSpawnerRegistration>* ItemTypeSpawnerRegistration::s_ItemTypeSpawnerRegistrations = nullptr;

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Constructors
ItemTypeSpawnerRegistration::ItemTypeSpawnerRegistration(ItemType itemType,
	DefaultBaseItemOfTypeCreationFunc* defaultCreationFunc, BaseItemOfTypeCreationFunc* itemCreationFunc)
	: m_itemType(itemType),
	m_defaultItemCreationFunc(defaultCreationFunc),
	m_itemCreationFunc(itemCreationFunc)
{
	if (m_itemType == INVALID_ITEM_TYPE)
	{
		return;
	}

	if (s_ItemTypeSpawnerRegistrations == nullptr)
	{
		s_ItemTypeSpawnerRegistrations = new std::map<ItemType, ItemTypeSpawnerRegistration>();
	}

	std::map<ItemType, ItemTypeSpawnerRegistration>::iterator it = s_ItemTypeSpawnerRegistrations->find(itemType);
	if (it != s_ItemTypeSpawnerRegistrations->end())
	{
		//SPIT OUT AN ERROR
		return;
	}
	else
	{
		s_ItemTypeSpawnerRegistrations->insert(std::pair<ItemType, ItemTypeSpawnerRegistration>(m_itemType, *this));
	}
}

ItemTypeSpawnerRegistration::~ItemTypeSpawnerRegistration()
{

}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Operations
Item* ItemTypeSpawnerRegistration::SpawnDefaultBaseItem(const SpriteResource* resource, Directions4 defaultDirection)
{
	if (m_defaultItemCreationFunc == nullptr)
	{
		return nullptr;
	}
	return m_defaultItemCreationFunc(resource, defaultDirection);
}

Item* ItemTypeSpawnerRegistration::SpawnBaseItem(const XMLNode& node)
{
	if (m_itemCreationFunc == nullptr)
	{
		return nullptr;
	}
	return m_itemCreationFunc(node);
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Setters

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Getters
ItemType ItemTypeSpawnerRegistration::GetItemType() const
{
	return m_itemType;
}

DefaultBaseItemOfTypeCreationFunc* ItemTypeSpawnerRegistration::GetDefaultItemCreationFunc() const
{
	return m_defaultItemCreationFunc;
}

BaseItemOfTypeCreationFunc* ItemTypeSpawnerRegistration::GetItemCreationFunc() const
{
	return m_itemCreationFunc;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Static Functions
STATIC Item* ItemTypeSpawnerRegistration::CreateDefaultItemTypeSpawnerByItemType(ItemType type,
	const SpriteResource* resource, Directions4 defaultDirection)
{
	if (s_ItemTypeSpawnerRegistrations == nullptr)
	{
		return nullptr;
	}
	std::map<ItemType, ItemTypeSpawnerRegistration>::iterator it = s_ItemTypeSpawnerRegistrations->find(type);
	if (it == s_ItemTypeSpawnerRegistrations->end())
	{
		return nullptr;
	}
	return it->second.SpawnDefaultBaseItem(resource, defaultDirection);
}

Item* ItemTypeSpawnerRegistration::CreateItemTypeSpawnerByItemType(ItemType type, const XMLNode& node)
{
	if (s_ItemTypeSpawnerRegistrations == nullptr)
	{
		return nullptr;
	}
	std::map<ItemType, ItemTypeSpawnerRegistration>::iterator it = s_ItemTypeSpawnerRegistrations->find(type);
	if (it == s_ItemTypeSpawnerRegistrations->end())
	{
		return nullptr;
	}
	return it->second.SpawnBaseItem(node);
}

STATIC bool ItemTypeSpawnerRegistration::DetermineItemTypeSpawnerExists(ItemType type)
{
	if (s_ItemTypeSpawnerRegistrations == nullptr)
	{
		return false;
	}
	std::map<ItemType, ItemTypeSpawnerRegistration>::iterator it = s_ItemTypeSpawnerRegistrations->find(type);
	if (it == s_ItemTypeSpawnerRegistrations->end())
	{
		return false;
	}
	return true;
}

STATIC void ItemTypeSpawnerRegistration::ClearAllItemTypeSpawnerRegistrations()
{
	if (s_ItemTypeSpawnerRegistrations == nullptr)
	{
		return;
	}
	delete s_ItemTypeSpawnerRegistrations;
	s_ItemTypeSpawnerRegistrations = nullptr;
}