#pragma once

#ifndef MAPCOMMONVARS_HPP
#define MAPCOMMONVARS_HPP
#include "Game/Common/GameCommonVars.hpp"

struct CommonMapVars
{
	static const char* s_MapXMLRoot;
	static const char* s_MapXMLTilesChildName;
	static const char* s_MapXMLTileSetChildName;
	static const char* s_MapXMLTileSetPairName;

	static const char* s_MapXMLTileSetPairTileTypeName;
	static const char* s_MapXMLTileSetPairTileCountName;

	static const char* s_debugNameVar;
	static const char* s_tiledefIdxVar;
	static const char* s_MapCodeVar;
	static const char* s_SolidVar;
	static const char* s_liquidVar;
	static const char* s_BottomResourceName;
	static const char* s_ResourceName;
	static const char* s_TopResourceName;
	static const char* s_TopResource2Name;
	static const char* s_testTintVar;
	static const char* s_Function;
	static const char* s_OnTouch;
	static const char* s_OnEnter;
	static const char* s_OnExit;
	static const char* s_OnInteract;
};
#endif