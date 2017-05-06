#include "Game/Map/TileEvent/TileEventFunctions.hpp"
#include "Game/Map/TileEvent/TileEvent.hpp"
#include "Game/Common/TileEventFunctionCommonVars.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Core/XMLRelated/StrXMLAttr.hpp"
#include "Game/Map/Tile.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Map/MapFiles/MapFiles.hpp"
#include "Game/TheGame.hpp"

STATIC const TileEvent TileEventFunctions::s_ChangeTileImageEvent = 
		TileEvent("ChangeTileImage", TileEventFunctions::EventFuncChangeTileImage);
STATIC const TileEvent TileEventFunctions::s_ChangeMapsEvent =
		TileEvent("ChangeMaps", TileEventFunctions::EventFuncChangeMaps);

STATIC void TileEventFunctions::RegisterAllTileEventsAtOnce()
{
}

STATIC void TileEventFunctions::UnregisterTileEvents()
{
	TileEvent::ClearAllTileEvents();
}

STATIC void TileEventFunctions::EventFuncChangeTileImage(Tile* tile, NamedProperties& params, const std::vector<StrXMLAttr>& attrs)
{
	params;
	if (tile == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < attrs.size(); i++)
	{
		StrXMLAttr attr = attrs[i];
		if (SimpleStrCmpLower(attr.name, TileEventFunctionCommonVars::s_AnimationIdx) == true)
		{
			std::string animIdx(attr.val);
			const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(animIdx);
			if (res != nullptr)
			{
				tile->ChangeSpriteResource(res);
			}
		}
	}
	
}

STATIC void TileEventFunctions::EventFuncChangeMaps(Tile* tile, NamedProperties& params, const std::vector<StrXMLAttr>& attrs)
{
	tile;
	BaseGameEntity* entity = nullptr;
	params.Get("entity", entity, (BaseGameEntity*)nullptr);
	if (entity == nullptr)
	{
		return;
	}
	BaseGameEntity* playerEntity = BaseGameEntity::GetEditablePointerToPlayer();
	if (entity != playerEntity)
	{
		//Might eventually want to have NPCs able to move between maps, but that will take a bit more thought.
		return;
	}

	bool playerPositionSet = false;
	IntVector2 newPlayerPosition = IntVector2::ivec2_zeros;
	std::string mapfileName = "";
	for (size_t attrIdx = 0; attrIdx < attrs.size(); attrIdx++)
	{
		const StrXMLAttr& attr = attrs[attrIdx];
		std::string lowerCaseName = MakeLower(attr.name);
		if (lowerCaseName == "mapfile"
			|| lowerCaseName == "mapfiledata")
		{
			mapfileName = (attr.val);
		}
		else if (lowerCaseName == "playerposition"
			|| lowerCaseName == "playerpos"
			|| lowerCaseName == "player")
		{
			std::string val = ReplaceCharInString(attr.val, ',', ' ');
			std::vector<std::string> parsed = ParseString(val, ' ');
			if (parsed.size() > 0)
			{
				newPlayerPosition.x = atoi(parsed[0].c_str());
			}
			if (parsed.size() > 1)
			{
				newPlayerPosition.y = atoi(parsed[1].c_str());
			}
			playerPositionSet = true;
		}
	}
	if (mapfileName == "")
	{
		return;
	}
	BaseGameEntity* player = BaseGameEntity::GetEditablePointerToPlayer();

	Map* orgMap = Map::GetEditableMap();
	if (orgMap != nullptr)
	{
		orgMap->UnregisterGameEntity(player);
	}
	Map* map = nullptr;
	{
		map = Map::GetRegisteredMapIfExists(mapfileName);
	}
	bool mapWasFound = map != nullptr;

	if (!mapWasFound)
	{
		if (mapfileName != "")
		{
			g_TheGame->SetUpdatePlayerPointer(false);
			MapFiles::LoadMapByNameRegisterForOnUpdate(mapfileName);
			g_TheGame->SetPositionForPlayerOnChangeMap(newPlayerPosition);
		}
		else
		{
			return;
		}
	}
	else
	{
		Map* currentMap = Map::GetEditableMap();
		if (currentMap != nullptr)
		{
			currentMap->UnregisterAllTilesFromEvents();
		}
		MapFiles::LoadJustTheEntitiesFromMapFilesAllAtOnce(map, mapfileName);
		map->PlayThisMapsMusic();
		Map::SetCurrentMap(map);
		map = Map::GetEditableMap();
		map->RegisterGameEntity(player);
		map->RegisterAllTilesToEventsIfNeed();
		Vector2 playerPos = map->GetCenterOfTileAtTileCoords(newPlayerPosition);
		playerEntity->SetPosition(playerPos);
	}

}