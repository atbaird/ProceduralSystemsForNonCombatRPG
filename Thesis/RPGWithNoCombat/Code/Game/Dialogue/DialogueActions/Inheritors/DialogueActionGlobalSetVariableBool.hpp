#pragma once

#ifndef DIALOGUEACTIONGLOBALSETVARIABLEBOOL_HPP
#define DIALOGUEACTIONGLOBALSETVARIABLEBOOL_HPP
#include <string>
#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"

//Creation Funcs
DialogueTrigger* CreateDialogueActionGlobalSetVariableBool(const XMLNode& node);
DialogueTrigger* CreateDefaultDialogueActionGlobalSetVariableBool();

class DialogueActionGlobalSetVariableBool : public DialogueTrigger
{
private:
protected:
	//Static Variables
	static const DialogueActionRegistrationHelper s_DialogueActionGlobalSetVariableBoolRegistrationHelper;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	bool m_value = false;
public:

	//Constructors
	DialogueActionGlobalSetVariableBool();
	DialogueActionGlobalSetVariableBool(const XMLNode& node);
	DialogueActionGlobalSetVariableBool(const DialogueActionGlobalSetVariableBool& other);
	virtual ~DialogueActionGlobalSetVariableBool();

	//Setters
	void SetVariableName(const std::string& name);
	void SetValue(bool value);

	//Getters
	const std::string& GetVariableName() const;
	const std::string& GetLowerCaseVariableName() const;
	bool GetValue() const;

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual void TriggerAction() override;
	virtual DialogueTrigger* Clone() const override;
};
#endif