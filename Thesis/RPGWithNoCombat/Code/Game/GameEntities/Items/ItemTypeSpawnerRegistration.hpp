#pragma once

#ifndef ITEMTYPESPAWNERREGISTRATION_HPP
#define ITEMTYPESPAWNERREGISTRATION_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Enums/ItemType.hpp"
#include <map>

enum Directions4;
class SpriteResource;
class Item;

typedef Item* (DefaultBaseItemOfTypeCreationFunc)(const SpriteResource* resource, Directions4 defaultDirection);
typedef Item* (BaseItemOfTypeCreationFunc)(const XMLNode& node);

class ItemTypeSpawnerRegistration
{
private:
protected:
	//Static Variables
	static std::map<ItemType, ItemTypeSpawnerRegistration>* s_ItemTypeSpawnerRegistrations;

	ItemType m_itemType = INVALID_ITEM_TYPE;
	DefaultBaseItemOfTypeCreationFunc* m_defaultItemCreationFunc = nullptr;
	BaseItemOfTypeCreationFunc* m_itemCreationFunc = nullptr;
public:
	//Constructors
	ItemTypeSpawnerRegistration(ItemType itemType, DefaultBaseItemOfTypeCreationFunc* defaultCreationFunc,
		BaseItemOfTypeCreationFunc* itemCreationFunc);
	virtual ~ItemTypeSpawnerRegistration();

	//Operations
	Item* SpawnDefaultBaseItem(const SpriteResource* resource, Directions4 defaultDirection);
	Item* SpawnBaseItem(const XMLNode& node);
	
	//Setters

	//Getters
	ItemType GetItemType() const;
	DefaultBaseItemOfTypeCreationFunc* GetDefaultItemCreationFunc() const;
	BaseItemOfTypeCreationFunc* GetItemCreationFunc() const;

	//Static Functions
	static Item* CreateDefaultItemTypeSpawnerByItemType(ItemType type, const SpriteResource* resource,
		Directions4 defaultDirection);
	static Item* CreateItemTypeSpawnerByItemType(ItemType type, const XMLNode& node);
	static bool DetermineItemTypeSpawnerExists(ItemType type);
	static void ClearAllItemTypeSpawnerRegistrations();
};

#endif