#include "Game/GameEntities/Items/ItemTemplate.hpp"
#include "Game/GameEntities/Items/Item.hpp"
#include "Game/GameEntities/Items/ItemTypeSpawnerRegistration.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Constructors
ItemTemplate::ItemTemplate()
{

}

ItemTemplate::~ItemTemplate()
{

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Operations
Item* ItemTemplate::SpawnItem(const Vector2& position) const
{
	Item* item = ItemTypeSpawnerRegistration::CreateItemTypeSpawnerByItemType(m_itemType, m_itemNode);
	item->SetPosition(position);
	return item;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Setters
void ItemTemplate::SetAndReadXMLNode(const XMLNode& node)
{
	m_itemNode = node;
	for (int attrIdx = 0; attrIdx < m_itemNode.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = m_itemNode.getAttribute(attrIdx);
		if (SimpleStrCmpLower(attr.lpszName, "Name"))
		{
			m_itemName = attr.lpszValue;
		}
		else if (SimpleStrCmpLower(attr.lpszName, "ItemType"))
		{
			std::string lowerCaseAttr = MakeLower(attr.lpszValue);
			if (lowerCaseAttr == "clothes"
				|| lowerCaseAttr == "clothing")
			{
				m_itemType = ITEM_TYPE_CLOTHING;
			}
		}
	}
}

void ItemTemplate::SetItemName(const std::string& name)
{
	m_itemName = name;
}

void ItemTemplate::SetItemType(ItemType type)
{
	m_itemType = type;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Getters
const XMLNode& ItemTemplate::GetXMLNode() const
{
	return m_itemNode;
}

const std::string& ItemTemplate::GetItemName() const
{
	return m_itemName;
}

const ItemType ItemTemplate::GetItemType() const
{
	return m_itemType;
}

const std::string ItemTemplate::GetItemTypeAsString() const
{
	switch (m_itemType)
	{
	case INVALID_ITEM_TYPE: return "INVALID";
	case ITEM_TYPE_CLOTHING: return "CLOTHING";
	default: return "";
	}
}

void ItemTemplate::WriteGeneratorToString(std::string& str, int currentIndentationLevel) const
{
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Item Type: " + GetItemTypeAsString() + "\n";

	//May need to think over the items a bit more; since these have to work by having something read the info differently
	//per item type.
}