#include "Game/Map/TileDefinitionSpecialCase/TileDefintionSpecialCaseToSet.hpp"
#include "Game/Map/Tile.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Engine/Math/MathToStringUtils.hpp"

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Constructors
TileDefinitionSpecialCaseToSet::TileDefinitionSpecialCaseToSet()
{

}

TileDefinitionSpecialCaseToSet::~TileDefinitionSpecialCaseToSet()
{

}


//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Setters
void TileDefinitionSpecialCaseToSet::SetBottomResource(const SpriteResource* resource)
{
	m_bottomResource = resource;
	m_overwriteBottomResource = m_bottomResource != nullptr || m_altBottomResource != nullptr;
}

void TileDefinitionSpecialCaseToSet::SetResource(const SpriteResource* resource)
{
	m_resource = resource;
	m_overwriteResource = m_resource != nullptr || m_altResource != nullptr;
}

void TileDefinitionSpecialCaseToSet::SetTopResource2(const SpriteResource* resource)
{
	m_topResource2 = resource;
	m_overwriteTopResource2 = m_topResource2 != nullptr || m_altTopResource2 != nullptr;
}

void TileDefinitionSpecialCaseToSet::SetTopResource(const SpriteResource* resource)
{
	m_topResource = resource;
	m_overwriteTopResource = m_topResource != nullptr || m_altTopResource != nullptr;
}

void TileDefinitionSpecialCaseToSet::SetAltBottomResource(const SpriteResource* resource)
{
	m_altBottomResource = resource;
	m_overwriteBottomResource = m_bottomResource != nullptr || m_altBottomResource != nullptr;
}

void TileDefinitionSpecialCaseToSet::SetAltResource(const SpriteResource* resource)
{
	m_altResource = resource;
	m_overwriteResource = m_resource != nullptr || m_altResource != nullptr;
}

void TileDefinitionSpecialCaseToSet::SetAltTopResource(const SpriteResource* resource)
{
	m_altTopResource = resource;
	m_overwriteTopResource = m_topResource != nullptr || m_altTopResource != nullptr;
}

void TileDefinitionSpecialCaseToSet::SetAltTopResource2(const SpriteResource* resource)
{
	m_altTopResource2 = resource;
	m_overwriteTopResource2 = m_topResource2 != nullptr || m_altTopResource2 != nullptr;
}

void TileDefinitionSpecialCaseToSet::SetIsLiquid(bool overwriteIsLiquid, bool isLiquid)
{
	m_overwriteIsLiquid = overwriteIsLiquid;
	m_isLiquid = isLiquid;
}

void TileDefinitionSpecialCaseToSet::SetIsSolid(bool overwriteIsSolid, bool isSolid)
{
	m_overwriteIsSolid = overwriteIsSolid;
	m_isSolid = isSolid;
}

void TileDefinitionSpecialCaseToSet::WriteToSetToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Debug Name: " + m_debugName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Relative Tile Coords from Test: " + MathToStringUtils::ToString(m_TileCoordsToEditAdjustment) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Overwrite Is Solid: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_overwriteIsSolid);
	
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "If Overwrites tile solidness, what it sets solid to: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_isSolid);


	if (m_bottomResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Bottom Resource Name: " + m_bottomResource->GetSpriteResourceName() + "\n";
	}

	if (m_resource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Resource Name: " + m_resource->GetSpriteResourceName() + "\n";
	}

	if (m_topResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Top Resource Name: " + m_topResource->GetSpriteResourceName() + "\n";
	}

	if (m_topResource2 != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Top Resource 2 Name: " + m_topResource2->GetSpriteResourceName() + "\n";
	}

	if (m_altBottomResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Bottom Resource Name: " + m_altBottomResource->GetSpriteResourceName() + "\n";

		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Bottom Resource Chance: " + std::to_string(m_altBottomResourceChance) + "\n";
	}

	if (m_altResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Resource Name: " + m_altResource->GetSpriteResourceName() + "\n";

		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Resource Chance: " + std::to_string(m_altResourceChance) + "\n";
	}

	if (m_altTopResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Top Resource Name: " + m_altTopResource->GetSpriteResourceName() + "\n";

		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Top Resource Chance: " + std::to_string(m_altTopResourceChance) + "\n";
	}

	if (m_altTopResource2 != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Top Resource 2 Name: " + m_altTopResource2->GetSpriteResourceName() + "\n";

		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Top Resource 2 Chance: " + std::to_string(m_altTopResource2Chance) + "\n";
	}
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Operations
void TileDefinitionSpecialCaseToSet::ApplyContentToTile(Tile& tile) const
{
	if (m_TileCoordsToEditAdjustment != TILE_COORDS::ivec2_zeros)
	{
		TILE_COORDS currentAdjustment = m_TileCoordsToEditAdjustment;
		Tile* neighbor = tile.GetEditablePtrOfSelf();
		ApplyContentToTileBasedOnShift(neighbor, currentAdjustment);
	}
	else
	{
		Tile* tilePtr = tile.GetEditablePtrOfSelf();
		ApplyContentToTilePtr(tilePtr);
	}
}

void TileDefinitionSpecialCaseToSet::ApplyContentToTileBasedOnShift(Tile* tile, TILE_COORDS& adjustment) const
{
	if (tile == nullptr)
	{
		return;
	}
	while (tile != nullptr
		&& adjustment != TILE_COORDS::ivec2_zeros)
	{
		GetNeighborTile(tile, adjustment);
		if (tile == nullptr)
		{
			break;
		}
		if (adjustment != TILE_COORDS::ivec2_zeros)
		{
			continue;
		}
		ApplyContentToTilePtr(tile);
		break;

	}
}

void TileDefinitionSpecialCaseToSet::GetNeighborTile(Tile*& neighbor, TILE_COORDS& adjustment) const
{
	if (adjustment.y > 0)
	{
		adjustment.y--;
		neighbor = neighbor->GetEditableNorthTile();
	}
	else if (adjustment.y < 0)
	{
		adjustment.y++;
		neighbor = neighbor->GetEditableSouthTile();
	}
	else if (adjustment.x > 0)
	{
		adjustment.x--;
		neighbor = neighbor->GetEditableEastTile();

	}
	else if (adjustment.x < 0)
	{
		adjustment.x++;
		neighbor = neighbor->GetEditableWestTile();
	}
}


void TileDefinitionSpecialCaseToSet::ApplyContentToTilePtr(Tile* tile) const
{
	if (tile == nullptr)
	{
		return;
	}
	if (m_overwriteIsSolid)
	{
		tile->SetOverwrittenIsSolid(m_isSolid, true);
	}
	if (m_overwriteIsLiquid)
	{
		tile->SetOverwrittenIsLiquid(m_isLiquid, true);
	}

	if (m_overwriteBottomResource)
	{
		if (m_bottomResource == nullptr
			|| (m_altBottomResource != nullptr
				&& GetRandomFloatZeroToOne() <= m_altBottomResourceChance))
		{
			tile->ChangeBottomSpriteResource(m_altBottomResource);
		}
		else
		{
			tile->ChangeBottomSpriteResource(m_bottomResource);
		}
	}

	if (m_overwriteResource)
	{
		if (m_resource == nullptr ||
			(m_altResource != nullptr
				&& GetRandomFloatZeroToOne() <= m_altResourceChance))
		{
			tile->ChangeSpriteResource(m_altResource);
		}
		else
		{
			tile->ChangeSpriteResource(m_resource);
		}
	}

	if (m_overwriteTopResource)
	{
		if (m_topResource == nullptr
			|| (m_altTopResource != nullptr
				&& GetRandomFloatZeroToOne() <= m_altTopResourceChance))
		{
			tile->ChangeTopSpriteResource(m_altTopResource);
		}
		else
		{
			tile->ChangeTopSpriteResource(m_topResource);
		}
	}

	if (m_overwriteTopResource2)
	{
		if (m_topResource2 == nullptr
			|| (m_altTopResource2 != nullptr
				&& GetRandomFloatZeroToOne() <= m_altTopResource2Chance))
		{
			tile->ChangeTopSpriteResource2(m_altTopResource2);
		}
		else
		{
			tile->ChangeTopSpriteResource2(m_topResource2);
		}
	}
}