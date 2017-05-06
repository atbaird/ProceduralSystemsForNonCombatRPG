#pragma once

#ifndef GAMEENTITYTOGENDATARELATINGTOQUEST_HPP
#define GAMEENTITYTOGENDATARELATINGTOQUEST_HPP
#include <string>
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Quest/MapQuestImportantData.hpp"

struct GameEntityToGenDataRelatingToQuest
{
	std::string m_questTitle = "";
	std::string m_variableName = "";
	std::string m_EntityName = "";
	std::string m_DialogueToSet = "";
	std::string m_GlobalVariableName = "";
	Vector2 m_position = Vector2::vec2_zeros;
	XMLNode m_node;
	XMLNode m_questNodeFromMap;
	bool m_positionSet = false;
	bool m_overwriteDialogue = false;
	bool m_forceSpawn = true;


	void WriteToString(std::string& str, int indentationAmt) const;
};

#endif