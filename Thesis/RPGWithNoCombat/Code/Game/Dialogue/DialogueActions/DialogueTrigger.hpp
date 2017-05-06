#pragma once

#ifndef DIALOGUEACTION_HPP
#define DIALOGUEACTION_HPP
#include <string>

struct XMLNode;
class DialogueTrigger
{
private:
protected:
public:
	//Constructors
	DialogueTrigger();
	DialogueTrigger(const XMLNode& node);
	DialogueTrigger(const DialogueTrigger& other);
	virtual ~DialogueTrigger();

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const = 0;
	virtual void TriggerAction() = 0;
	virtual DialogueTrigger* Clone() const = 0;
};

#endif