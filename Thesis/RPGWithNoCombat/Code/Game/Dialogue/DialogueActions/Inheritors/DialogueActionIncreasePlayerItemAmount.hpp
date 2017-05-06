#pragma once

#ifndef DIALOGUEACTIONINCREASEPLAYERITEMAMOUNT_HPP
#define DIALOGUEACTIONINCREASEPLAYERITEMAMOUNT_HPP
#include <string>
#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerWhatToDoWithStatOptions.hpp"

//Creation Funcs
DialogueTrigger* CreateDialogueActionIncreasePlayerItemAmount(const XMLNode& node);
DialogueTrigger* CreateDefaultDialogueActionIncreasePlayerItemAmount();

class DialogueActionIncreasePlayerItemAmount : public DialogueTrigger
{
private:
protected:
	//Static Variables
	static const DialogueActionRegistrationHelper s_DialogueActionIncreasePlayerItemAmountRegistrationHelper;

	//Variables
	std::string m_itemName = "";
	std::string m_lowerCaseItemName = "";
	float m_value = 0.f;
public:
	//Constructors
	DialogueActionIncreasePlayerItemAmount();
	DialogueActionIncreasePlayerItemAmount(const XMLNode& node);
	DialogueActionIncreasePlayerItemAmount(const DialogueActionIncreasePlayerItemAmount& other);
	virtual ~DialogueActionIncreasePlayerItemAmount();

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