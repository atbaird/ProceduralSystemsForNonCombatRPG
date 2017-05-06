#include "Game/Map/MapFiles/GameEntityToGenDataRelatingToQuest.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Engine/Math/MathToStringUtils.hpp"


void GameEntityToGenDataRelatingToQuest::WriteToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "(Quest Title, Variable Name): " + m_questTitle + "," + m_variableName + "\n"; 
	indentationAmt += 3;

	if (m_GlobalVariableName != "")
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Global Variable Name: " + m_GlobalVariableName + "\n";
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Force Spawn, if entity not already spawned: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_forceSpawn);

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Generator Name to use: " + m_EntityName + "\n";
	
	//Dialogue
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Overwrite Dialogue: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_overwriteDialogue);
	if (m_overwriteDialogue)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Dialogue to Set: " + m_DialogueToSet + "\n";
	}

	//Position
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Position already Generated: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_positionSet);

	if (m_positionSet)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Position to set: " + MathToStringUtils::ToString(m_position);
	}

	/*
	XMLNode m_node;
	XMLNode m_questNodeFromMap;
	*/
}