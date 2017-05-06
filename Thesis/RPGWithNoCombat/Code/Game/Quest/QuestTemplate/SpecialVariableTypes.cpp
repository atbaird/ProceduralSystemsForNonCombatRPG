#include "Game/Quest/QuestTemplate/SpecialVariableTypes.hpp"

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Constructors
SpecialVariableTypes::SpecialVariableTypes()
{

}

SpecialVariableTypes::~SpecialVariableTypes()
{

}

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Setters
void SpecialVariableTypes::SetSpecialVariableTypeEnum(SpecialVariableTypeEnum var)
{
	m_specialVariableType = var;
}

void SpecialVariableTypes::SetVariableName(const std::string& varName)
{
	m_variableName = varName;
}

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Getters
SpecialVariableTypeEnum SpecialVariableTypes::GetSpecialVariableType() const
{
	return m_specialVariableType;
}

const std::string& SpecialVariableTypes::GetVariableName() const
{
	return m_variableName;
}