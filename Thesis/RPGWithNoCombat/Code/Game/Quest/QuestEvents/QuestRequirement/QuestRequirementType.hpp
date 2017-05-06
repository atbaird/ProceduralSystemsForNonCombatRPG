#pragma once

#ifndef QUESTREQUIREMENTTYPE_HPP
#define QUESTREQUIREMENTTYPE_HPP

enum QuestRequirementType
{
	INVALID_QUEST_REQUIREMENT_TYPE = -1,
	Check_Variable,
	Check_Player_Stats,
	Check_Player_Choice,
	Check_Player_Has_Item,
	Player_Interacting_With,
	Dialogue_Choice_Was,
	Current_Dialogue_Segment_Is,
	NUMBER_OF_QUEST_REQUIREMENT_TYPES
};
#endif