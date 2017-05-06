#pragma once

#ifndef DIALOGUEREQUIREMENT_HPP
#define DIALOGUEREQUIREMENT_HPP
#include <string>

struct XMLNode;
class DialogueRequirement
{
private:
protected:
public:
	//Constructors
	DialogueRequirement();
	DialogueRequirement(const XMLNode& node);
	DialogueRequirement(const DialogueRequirement& other);
	virtual ~DialogueRequirement();

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const = 0;
	virtual bool CheckIfRequirementIsMet() const = 0;
	virtual DialogueRequirement* Clone() const = 0;
};
#endif