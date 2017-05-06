#pragma once

#ifndef DIALOGUEREQUIREMENTPLAYERHASITEM_HPP
#define DIALOGUEREQUIREMENTPLAYERHASITEM_HPP
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseOperations.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"

//Spawning Functions
DialogueRequirement* DialogueReqCheckPlayerHasItemCreationFunc(const XMLNode& node);
DialogueRequirement* DialogueReqCheckPlayerHasItemDefaultCreationFunc();

class DialogueRequirementCheckPlayerHasItem : public DialogueRequirement
{
private:
protected:
	//Static Variables
	static const DialogueRequirementRegistrationHelper s_CheckPlayerHasItemRegHelper;

	//Variables
	std::string m_itemName = "";
	std::string m_lowerCaseItemName = "";
	float m_value = 0.f;
	SpecialCaseOperations m_specialCaseOperation = IS_SAME;
public:
	//Constructors
	DialogueRequirementCheckPlayerHasItem();
	DialogueRequirementCheckPlayerHasItem(const XMLNode& node);
	DialogueRequirementCheckPlayerHasItem(const DialogueRequirementCheckPlayerHasItem& other);
	virtual ~DialogueRequirementCheckPlayerHasItem();

	//Setters
	void SetVariableName(const std::string& name);
	void SetVariable(float variable);
	void SetSpecialCaseOperation(SpecialCaseOperations specialCase);

	//Getters
	const std::string& GetVariableName() const;
	const std::string& GetLowerCaseVariableName() const;
	float GetVariable() const;
	SpecialCaseOperations GetSpecialCaseOperation() const;

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual bool CheckIfRequirementIsMet() const override;
	virtual DialogueRequirement* Clone() const override;

};
#endif