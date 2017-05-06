#pragma once

#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include "Game/GameEntities/Items/Item.hpp"

class Inventory
{
private:
protected:
	std::vector<Item*> m_items;
public:
	//Constructors
	Inventory();
	virtual ~Inventory();

	//Drop Items
	const std::vector<Item*> DropItem(Item* item);
	const std::vector<Item*> DropItem(const std::string& itemName);
	const std::vector<Item*> DropItem(size_t itemIdxInInv);
	const std::vector<Item*> DropNItem(Item* item, int nItems);
	const std::vector<Item*> DropNItem(const std::string& itemName, int nItems);
	const std::vector<Item*> DropNItem(size_t itemIdxInInv, int nItems);

	//Add Items
	bool AddItem(Item* item);
	bool AddItem(const std::string& itemName);
	bool AddNItem(Item* item, int nItems);
	bool AddNItem(const std::string& itemName, int nItems);

	//Getters
	bool ItemExistsInInventory(Item* item);
	bool ItemExistsInInventory(const std::string itemName);
	int IdxItemExistsAtInInventory(Item* item);
	int IdxItemExistsAtInInventory(const std::string& itemName);

};

#endif