#pragma once

#ifndef DIALOGUEACTIONDECREMENTPLAYERITEMAMOUNT_HPP
#define DIALOGUEACTIONDECREMENTPLAYERITEMAMOUNT_HPP
#include <string>
#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerWhatToDoWithStatOptions.hpp"

//Creation Funcs
DialogueTrigger* CreateDialogueActionDecreasePlayerItemAmount(const XMLNode& node);
DialogueTrigger* CreateDefaultDialogueActionDecreasePlayerItemAmount();

class DialogueActionDecreasePlayerItemAmount : public DialogueTrigger
{
private:
protected:
	//Static Variables
	static const DialogueActionRegistrationHelper s_DialogueActionDecreasePlayerItemAmountRegistrationHelper;

	//Variables
	std::string m_itemName = "";
	std::string m_lowerCaseItemName = "";
	float m_value = 0.f;
public:
	//Constructors
	DialogueActionDecreasePlayerItemAmount();
	DialogueActionDecreasePlayerItemAmount(const XMLNode& node);
	DialogueActionDecreasePlayerItemAmount(const DialogueActionDecreasePlayerItemAmount& other);
	virtual ~DialogueActionDecreasePlayerItemAmount();

	//Setters
	void SetVariableName(const std::string& name);
	void SetValue(float value);

	//Getters
	const std::string& GetVariableName() const;
	const std::string& GetLowerCaseVariableName() const;
	float GetValue() const;

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual void TriggerAction() override;
	virtual DialogueTrigger* Clone() const override;
};

#endif