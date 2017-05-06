#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariableBase.hpp"
#include "Game/Quest/Quest.hpp"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Constructors
QuestRequirementCheckVariableBase::QuestRequirementCheckVariableBase(QuestRequirementCheckVariableType type)
	: QuestRequirement(Check_Variable),
	m_checkVariableType(type)
{

}

QuestRequirementCheckVariableBase::QuestRequirementCheckVariableBase(QuestRequirementCheckVariableType type, 
	const XMLNode& node)
	: QuestRequirement(Check_Variable),
	m_checkVariableType(type)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "operation")
		{
			std::string val = std::string(attr.lpszValue);
			m_specialCaseOperation = GetSpecialCaseOperationForStringContents(val);
		}
		else if (attrName == "check"
			|| attrName == "variable"
			|| attrName == "variablename")
		{
			SetVariableNameToCheck(std::string(attr.lpszValue));
		}
	}
}

QuestRequirementCheckVariableBase::QuestRequirementCheckVariableBase(const QuestRequirementCheckVariableBase& other,
	Quest* owningQuest)
	: QuestRequirement(other, owningQuest),
	m_variableName(other.m_variableName),
	m_checkVariableType(other.m_checkVariableType),
	m_specialCaseOperation(other.m_specialCaseOperation),
	m_lowerCaseVariableName(other.m_lowerCaseVariableName)
{

}

QuestRequirementCheckVariableBase::~QuestRequirementCheckVariableBase()
{
	QuestRequirement::~QuestRequirement();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Setters
void QuestRequirementCheckVariableBase::SetVariableNameToCheck(const std::string& variableName)
{
	m_variableName = variableName;
	m_lowerCaseVariableName = MakeLower(variableName);
}

void QuestRequirementCheckVariableBase::SetCheckVariableType(QuestRequirementCheckVariableType type)
{
	m_checkVariableType = type;
}

void QuestRequirementCheckVariableBase::SetSpecialCaseOperations(SpecialCaseOperations specialCaseOperations)
{
	m_specialCaseOperation = specialCaseOperations;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Getters
const std::string& QuestRequirementCheckVariableBase::GetVariableName() const
{
	return m_variableName;
}


QuestRequirementCheckVariableType QuestRequirementCheckVariableBase::GetCheckVariableType() const
{
	return m_checkVariableType;
}


SpecialCaseOperations QuestRequirementCheckVariableBase::GetSpecialCaseOperations() const
{
	return m_specialCaseOperation;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Static Functions