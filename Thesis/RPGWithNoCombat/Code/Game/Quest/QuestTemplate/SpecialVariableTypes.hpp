#pragma once

#ifndef SPECIALVARIABLETYPES_HPP
#define SPECIALVARIABLETYPES_HPP
#include "Game/Quest/QuestTemplate/SpecialVariableTypeEnum.hpp"
#include <string>


struct SpecialVariableTypes
{
public:
	//Variables
	SpecialVariableTypeEnum m_specialVariableType;
	std::string m_variableName = "";

public:
	//Constructors
	SpecialVariableTypes();
	virtual ~SpecialVariableTypes();

	//Setters
	void SetSpecialVariableTypeEnum(SpecialVariableTypeEnum var);
	void SetVariableName(const std::string& varName);

	//Getters
	SpecialVariableTypeEnum GetSpecialVariableType() const;
	const std::string& GetVariableName() const;
};

#endif