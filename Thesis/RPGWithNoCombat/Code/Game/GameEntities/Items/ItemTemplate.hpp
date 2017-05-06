#pragma once

#ifndef ITEMTEMPLATE_HPP
#define ITEMTEMPLATE_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Enums/ItemType.hpp"
#include "Engine/Math/Vector2.hpp"

class Item;
class ItemTemplate
{
private:
protected:
	XMLNode m_itemNode;
	std::string m_itemName = "";
	ItemType m_itemType = INVALID_ITEM_TYPE;
public:
	//Constructors
	ItemTemplate();
	virtual ~ItemTemplate();

	//Operations
	Item* SpawnItem(const Vector2& position = Vector2::vec2_zeros) const;

	//Setters
	void SetAndReadXMLNode(const XMLNode& node);
	void SetItemName(const std::string& name);
	void SetItemType(ItemType type);

	//Getters
	const XMLNode& GetXMLNode() const;
	const std::string& GetItemName() const;
	const ItemType GetItemType() const;
	const std::string GetItemTypeAsString() const;
	void WriteGeneratorToString(std::string& str, int currentIndentationLevel = 0) const;
};
#endif