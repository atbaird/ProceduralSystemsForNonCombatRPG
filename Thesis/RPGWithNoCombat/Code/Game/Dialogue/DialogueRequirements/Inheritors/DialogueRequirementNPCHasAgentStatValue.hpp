#pragma once

#ifndef DIALOGUEREQUIREMENTNPCHASAGENTSTATVALUE_HPP
#define DIALOGUEREQUIREMENTNPCHASAGENTSTATVALUE_HPP
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseOperations.hpp"

//Spawning Functions
DialogueRequirement* DialogueReqNPCHasAgentStatValueCreationFunc(const XMLNode& node);
DialogueRequirement* DialogueReqNPCHasAgentStatValueDefaultCreationFunc();

class DialogueRequirementNPCHasAgentStatValue : public DialogueRequirement
{
private:
protected:
	//Static Variables
	static const DialogueRequirementRegistrationHelper s_NPCHasAgentStatValueRegHelper;

	//Variables
	std::string m_statName = "";
	std::string m_lowerCaseStatName = "";
	int m_statValue = 0;
	SpecialCaseOperations m_specialCaseOperation = IS_SAME;

public:
	//Constructors
	DialogueRequirementNPCHasAgentStatValue();
	DialogueRequirementNPCHasAgentStatValue(const XMLNode&);
	DialogueRequirementNPCHasAgentStatValue(const DialogueRequirementNPCHasAgentStatValue& other);
	virtual ~DialogueRequirementNPCHasAgentStatValue();

	//Setters
	void SetStatName(const std::string& statName);
	void SetStatValue(int statValue);
	void SetSpecialCaseOperation(SpecialCaseOperations operation);

	//Getters
	const std::string& GetStatName() const;
	const std::string& GetLowerCaseStatName() const;
	int GetStatValue() const;
	SpecialCaseOperations GetSpecialCaseOperation() const;


	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual bool CheckIfRequirementIsMet() const override;
	virtual DialogueRequirement* Clone() const override;

};

#endif