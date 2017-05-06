#pragma once

#ifndef QUESTREQUIREMENTCHECKVARIABLEBASE_HPP
#define QUESTREQUIREMENTCHECKVARIABLEBASE_HPP
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementCheckVariableType.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

class QuestRequirementCheckVariableBase : public QuestRequirement
{
private:
protected:
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	QuestRequirementCheckVariableType m_checkVariableType = INVALID_CHECK_VARIABLE_TYPE;
	SpecialCaseOperations m_specialCaseOperation = IS_SAME;
public:
	//Constructors
	QuestRequirementCheckVariableBase(QuestRequirementCheckVariableType type);
	QuestRequirementCheckVariableBase(QuestRequirementCheckVariableType type, const XMLNode& node);
	QuestRequirementCheckVariableBase(const QuestRequirementCheckVariableBase& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckVariableBase();

	//Setters
	void SetVariableNameToCheck(const std::string& variableName);
	void SetCheckVariableType(QuestRequirementCheckVariableType type);
	void SetSpecialCaseOperations(SpecialCaseOperations specialCaseOperations);

	//Getters
	const std::string& GetVariableName() const;
	QuestRequirementCheckVariableType GetCheckVariableType() const;
	SpecialCaseOperations GetSpecialCaseOperations() const;

	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override = 0;

};
#endif