#pragma once

#ifndef DIALGOUEACTIONGLOBALSETVARIABLEFLOAT_HPP
#define DIALOGUEACTIONGLOBALSETVARIABLEFLOAT_HPP
#include <string>
#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerWhatToDoWithStatOptions.hpp"


//Creation Funcs
DialogueTrigger* CreateDialogueActionGlobalSetVariableFloat(const XMLNode& node);
DialogueTrigger* CreateDefaultDialogueActionGlobalSetVariableFloat();

class DialogueActionGlobalSetVariableFloat : public DialogueTrigger
{
private:
protected:
	//Static Variables
	static const DialogueActionRegistrationHelper s_DialogueActionGlobalSetVariableFloatRegistrationHelper;

	//Variables
	std::string m_variableName = "";
	std::string m_lowerCaseVariableName = "";
	QuestTriggerWhatToDoWithStatOptions m_StatOption = SET;
	float m_value = 0.f;
public:
	//Constructors
	DialogueActionGlobalSetVariableFloat();
	DialogueActionGlobalSetVariableFloat(const XMLNode& node);
	DialogueActionGlobalSetVariableFloat(const DialogueActionGlobalSetVariableFloat& other);
	virtual ~DialogueActionGlobalSetVariableFloat();

	//Setters
	void SetVariableName(const std::string& name);
	void SetWhatToDoWithValue(QuestTriggerWhatToDoWithStatOptions whatToDo);
	void SetValue(float value);

	//Getters
	const std::string& GetVariableName() const;
	const std::string& GetLowerCaseVariableName() const;
	QuestTriggerWhatToDoWithStatOptions GetWhatToDoWithValue() const;
	float GetValue() const;

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual void TriggerAction() override;
	virtual DialogueTrigger* Clone() const override;
};
#endif