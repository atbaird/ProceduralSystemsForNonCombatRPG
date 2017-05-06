#pragma once

#ifndef DIALOGUEREQUIREMENTNPCHASJOB_HPP
#define DIALOGUEREQUIREMENTNPCHASJOB_HPP
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

//Spawning Functions
DialogueRequirement* DialogueReqNPCHasJobCreationFunc(const XMLNode& node);
DialogueRequirement* DialogueReqNPCHasJobDefaultCreationFunc();

class DialogueRequirementNPCHasJob : public DialogueRequirement
{
private:
protected:
	//Static Variables
	static const DialogueRequirementRegistrationHelper s_NPCHasAttributeValueRegHelper;

	//Variables
	std::string m_NPCJobTitle = "";
public:
	//Constructors
	DialogueRequirementNPCHasJob();
	DialogueRequirementNPCHasJob(const XMLNode& node);
	DialogueRequirementNPCHasJob(const DialogueRequirementNPCHasJob& other);
	virtual ~DialogueRequirementNPCHasJob();

	//Setters
	void SetNPCJobTitle(const std::string& jobTitle);

	//Getters
	const std::string& GetNPCJobTitle() const;

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual bool CheckIfRequirementIsMet() const override;
	virtual DialogueRequirement* Clone() const override;
};
#endif