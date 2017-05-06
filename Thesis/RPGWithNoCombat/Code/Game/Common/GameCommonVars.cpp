#include "Game/Common/GameCommonVars.hpp"

STATIC std::vector<std::string> GameCommons::s_MapNamesInOrderTheyWereLoaded;
STATIC std::map<std::string, Map*> GameCommons::s_AllLoadedMaps;
STATIC int GameCommons::s_GameSeed = 0;
STATIC Controller* GameCommons::s_PlayerController = nullptr;