#pragma once

#ifndef DIALOGUEREQUIREMENTNPCHASATTRIBUTEVALUE_HPP
#define DIALOGUEREQUIREMENTNPCHASATTRIBUTEVALUE_HPP
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

//Spawning Functions
DialogueRequirement* DialogueReqNPCHasAttributeValueCreationFunc(const XMLNode& node);
DialogueRequirement* DialogueReqNPCHasAttributeValueDefaultCreationFunc();

class DialogueRequirementNPCHasAttributeValue : public DialogueRequirement
{
private:
protected:
	//Static Variables
	static const DialogueRequirementRegistrationHelper s_NPCHasAttributeValueRegHelper;

	//Variables
	std::string m_NPCAttributeName = "";
	std::string m_NPCAttributeValue = "";
public:
	//Constructors
	DialogueRequirementNPCHasAttributeValue();
	DialogueRequirementNPCHasAttributeValue(const XMLNode& node);
	DialogueRequirementNPCHasAttributeValue(const DialogueRequirementNPCHasAttributeValue& other);
	virtual ~DialogueRequirementNPCHasAttributeValue();

	//Setters
	void SetNPCAttributeName(const std::string& name);
	void SetNPCAttributeValue(const std::string& value);

	//Getters
	const std::string& GetNPCAttributeName() const;
	const std::string& GetNPCAttributeValue() const;

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual bool CheckIfRequirementIsMet() const override;
	virtual DialogueRequirement* Clone() const override;
};
#endif