#pragma once

#ifndef DIALOGUEREQUIREMENTCHECKGLOBALVARIABLEBOOL_HPP
#define DIALOGUEREQUIREMENTCHECKGLOBALVARIABLEBOOL_HPP
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

//Spawning Functions
DialogueRequirement* DialogueReqCheckGlobalVariableBoolCreationFunc(const XMLNode& node);
DialogueRequirement* DialogueReqCheckGlobalVariableBoolDefaultCreationFunc();

class DialogueRequirementCheckGlobalVariableBool : public DialogueRequirement
{
private:
protected:
	//Static Variables
	static const DialogueRequirementRegistrationHelper s_CheckGlobalVariableBoolRegHelper;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	bool m_value = false;
public:
	//Constructors
	DialogueRequirementCheckGlobalVariableBool();
	DialogueRequirementCheckGlobalVariableBool(const XMLNode& node);
	DialogueRequirementCheckGlobalVariableBool(const DialogueRequirementCheckGlobalVariableBool& other);
	virtual ~DialogueRequirementCheckGlobalVariableBool();

	//Setters
	void SetVariableName(const std::string& name);
	void SetValue(bool value);

	//Getters
	const std::string& GetVariableName() const;
	const std::string& GetLowerCaseVariableName() const;
	bool GetValue() const;

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual bool CheckIfRequirementIsMet() const override;
	virtual DialogueRequirement* Clone() const override;
};
#endif