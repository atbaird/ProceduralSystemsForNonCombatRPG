#include "Game/GameEntities/Items/Inventory/Inventory.hpp"
#include "Game/GameEntities/Items/ItemGenerator.hpp"

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Constructors
Inventory::Inventory()
{

}

Inventory::~Inventory()
{

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Drop Items
const std::vector<Item*> Inventory::DropItem(Item* item)
{
	if (item == nullptr)
	{
		return std::vector<Item*>();
	}
	return DropNItem(item->GetName(), 1);
}


const std::vector<Item*> Inventory::DropItem(const std::string& itemName)
{
	return DropNItem(itemName, 1);
}

const std::vector<Item*> Inventory::DropItem(size_t itemIdxInInv)
{
	return DropNItem(itemIdxInInv, 1);
}

const std::vector<Item*> Inventory::DropNItem(Item* item, int nItems)
{
	if (item == nullptr)
	{
		return std::vector<Item*>();
	}
	return DropNItem(item->GetName(), nItems);
}


const std::vector<Item*> Inventory::DropNItem(const std::string& itemName, int nItems)
{
	std::vector<Item*> itemsOut;
	size_t lastIdxInItemsOut = 0;

	for (size_t i = 0; i < m_items.size(); i++)
	{
		Item* it = m_items[i];
		if (it == nullptr)
		{
			m_items.erase(m_items.begin() + i);
			i--;
			continue;
		}

		if (it->GetName() == itemName)
		{
			int cnt = it->GetItemCount();
			if (cnt < nItems)
			{
				nItems -= cnt;
				if (itemsOut.size() > 0)
				{
					itemsOut[lastIdxInItemsOut]->SetItemCount(itemsOut[lastIdxInItemsOut]->GetItemCount() + cnt);
					delete it;
				}
				else
				{
					itemsOut.push_back(it);
					it->SetOnGround(true);
					it->SetEquipped(false);
				}
				m_items.erase(m_items.begin() + i);
				i--;
				continue;
			}
			else if (cnt == nItems)
			{
				if (itemsOut.size() > 0)
				{
					itemsOut[lastIdxInItemsOut]->SetItemCount(itemsOut[lastIdxInItemsOut]->GetItemCount() + nItems);
					delete it;
				}
				else
				{
					itemsOut.push_back(it);
					it->SetOnGround(true);
					it->SetEquipped(false);
				}
				m_items.erase(m_items.begin() + i);
				return itemsOut;
			}
			else //cnt > nItems
			{
				it->SetItemCount(cnt - nItems);
				if (itemsOut.size() > 0)
				{
					itemsOut[lastIdxInItemsOut]->SetItemCount(itemsOut[lastIdxInItemsOut]->GetItemCount() + nItems);
				}
				else
				{
					BaseGameEntity* entity = ItemGenerator::SpawnItemByItemName(it->GetName().c_str(), Vector2::vec2_zeros);
					Item* itSpawned = dynamic_cast<Item*>(entity);
					itSpawned->SetItemCount(nItems);
					itemsOut.push_back(itSpawned);
					itSpawned->SetOnGround(true);
					itSpawned->SetEquipped(false);
				}
				return itemsOut;
			}
		}
	}
	return itemsOut;
}

const std::vector<Item*> Inventory::DropNItem(size_t itemIdxInInv, int nItems)
{
	std::vector<Item*> itemsOut;
	if (itemIdxInInv >= m_items.size()
		|| m_items[itemIdxInInv] == nullptr)
	{
		return itemsOut;
	}


	Item* item = m_items[itemIdxInInv];
	int cnt = item->GetItemCount();
	if (cnt <= nItems)
	{
		m_items.erase(m_items.begin() + itemIdxInInv);
		itemsOut.push_back(item);
		item->SetOnGround(true);
		item->SetEquipped(false);

	}
	else
	{
		item->SetItemCount(item->GetItemCount() - nItems);
		BaseGameEntity* entity = ItemGenerator::SpawnItemByItemName(item->GetName().c_str(), Vector2::vec2_zeros);
		Item* itSpawned = dynamic_cast<Item*>(entity);
		itSpawned->SetItemCount(nItems);
		itSpawned->SetOnGround(true);
		itSpawned->SetEquipped(false);
		itemsOut.push_back(itSpawned);
	}
	return itemsOut;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Add Items
bool Inventory::AddItem(Item* item)
{
	return AddNItem(item, 1);
}

bool Inventory::AddItem(const std::string& itemName)
{
	return AddNItem(itemName, 1);
}

bool Inventory::AddNItem(Item* item, int nItems)
{
	if (item == nullptr
		|| item->GetItemCount() == 0)
	{
		return false;
	}
	int idx = IdxItemExistsAtInInventory(item);

	if (nItems > item->GetItemCount())
	{
		nItems = item->GetItemCount();
	}

	if (idx == -1)
	{
		if (item->GetItemCount() > nItems)
		{
			item->SetItemCount(item->GetItemCount() - nItems);
			BaseGameEntity* entity = ItemGenerator::SpawnItemByItemName(item->GetName().c_str(), Vector2::vec2_zeros);
			Item* itSpawned = dynamic_cast<Item*>(entity);
			item = itSpawned;
			item->SetItemCount(nItems);
		}
		item->SetOnGround(false);
		item->SetEquipped(false);
		m_items.push_back(item);
	}
	else
	{

		if (item->GetItemCount() > nItems)
		{
			m_items[idx]->SetItemCount(m_items[idx]->GetItemCount() + nItems);
			item->SetItemCount(item->GetItemCount() - nItems);
		}
		else
		{
			m_items[idx]->SetItemCount(m_items[idx]->GetItemCount() + item->GetItemCount());
			delete item;
			return true;
		}
	}
	return false;
}

bool Inventory::AddNItem(const std::string& itemName, int nItems)
{
	BaseGameEntity* entity = ItemGenerator::SpawnItemByItemName(itemName.c_str(), Vector2::vec2_zeros);
	Item* itSpawned = dynamic_cast<Item*>(entity);
	itSpawned->SetItemCount(nItems);
	return AddNItem(itSpawned, nItems);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Getters
bool Inventory::ItemExistsInInventory(Item* item)
{
	if (item == nullptr)
	{
		return false;
	}
	for (size_t i = 0; i < m_items.size(); i++)
	{
		Item* it = m_items[i];
		if (it != nullptr && it->GetName() == item->GetName())
		{
			return true;
		}
	}
	return false;
}

bool Inventory::ItemExistsInInventory(const std::string itemName)
{
	for (size_t i = 0; i < m_items.size(); i++)
	{
		Item* it = m_items[i];
		if (it != nullptr && it->GetName() == itemName)
		{
			return true;
		}
	}
	return false;
}

int Inventory::IdxItemExistsAtInInventory(Item* item)
{
	if (item == nullptr)
	{
		return -1;
	}

	for (size_t i = 0; i < m_items.size(); i++)
	{
		Item* it = m_items[i];
		if (it != nullptr && it->GetName() == item->GetName())
		{
			return (int)i;
		}
	}
	return -1;
}

int Inventory::IdxItemExistsAtInInventory(const std::string& itemName)
{
	for (size_t i = 0; i < m_items.size(); i++)
	{
		Item* it = m_items[i];
		if (it != nullptr && it->GetName() == itemName)
		{
			return (int)i;
		}
	}
	return -1;
}