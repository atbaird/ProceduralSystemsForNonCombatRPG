#pragma once

#ifndef GAMECOMMONVARS_HPP
#define GAMECOMMONVARS_HPP
#include <map>
#include <vector>
#include <string>
#define STATIC


enum VariousSpriteLayers
{
	Default_SpriteLayer,
	MapTile_SpriteLayer,
	Agent_SpriteLayer,
	NUM_OF_VARIOUS_SPRITE_LAYERS
};

class Controller;
class Map;
class GameCommons
{
public:
	static std::vector<std::string> s_MapNamesInOrderTheyWereLoaded;
	static std::map<std::string, Map*> s_AllLoadedMaps;
	static int s_GameSeed;
	static Controller* s_PlayerController;
};
#endif