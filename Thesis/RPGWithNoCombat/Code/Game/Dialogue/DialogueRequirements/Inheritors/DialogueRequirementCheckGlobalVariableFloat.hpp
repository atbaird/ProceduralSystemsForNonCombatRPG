#pragma once

#ifndef DIALOGUEREQUIREMENTCHECKGLOBALVARIABLEFLOAT_HPP
#define DIALOGUEREQUIREMENTCHECKGLOBALVARIABLEFLOAT_HPP
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseOperations.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"

//Spawning Functions
DialogueRequirement* DialogueReqCheckGlobalVariableFloatCreationFunc(const XMLNode& node);
DialogueRequirement* DialogueReqCheckGlobalVariableFloatDefaultCreationFunc();

class DialogueRequirementCheckGlobalVariableFloat : public DialogueRequirement
{
private:
protected:
	//Static Variables
	static const DialogueRequirementRegistrationHelper s_CheckGlobalVariableFloatRegHelper;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	float m_value = 0.f;
	SpecialCaseOperations m_specialCaseOperation = IS_SAME;
public:
	//Constructors
	DialogueRequirementCheckGlobalVariableFloat();
	DialogueRequirementCheckGlobalVariableFloat(const XMLNode& node);
	DialogueRequirementCheckGlobalVariableFloat(const DialogueRequirementCheckGlobalVariableFloat& other);
	virtual ~DialogueRequirementCheckGlobalVariableFloat();

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