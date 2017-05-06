#pragma once

#ifndef QUESTREQUIREMENTCHECKVARIABLETYPE_HPP
#define QUESTREQUIREMENTCHECKVARIABLETYPE_HPP

enum QuestRequirementCheckVariableType
{
	INVALID_CHECK_VARIABLE_TYPE = -1,
	Float_Type_Check_Variable,
	Bool_Type_Check_Variable,
	Entity_Has_Type_Check_Variable,
	Global_Float_Type_Check_Variable,
	Global_Bool_Type_Check_Variable,
	NUMBER_OF_QUEST_REQUIREMENT_CHECK_VARIABLE_TYPES
};
#endif