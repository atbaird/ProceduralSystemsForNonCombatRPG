#pragma once

#ifndef DIALOGUEREQUIREMENTREGISTRATIONHELPER_HPP
#define DIALOGUEREQUIREMENTREGISTRATIONHELPER_HPP
#include <map>
#include <string>

#define STATIC 

class DialogueRequirement;
struct XMLNode;
typedef DialogueRequirement* (DialogueRequirementCreationFunc)(const XMLNode& node);
typedef DialogueRequirement* (DialogueRequirementDefaultCreationFunc)();

class DialogueRequirementRegistrationHelper
{
private:
protected:
	//Static Variables
	static std::map<std::string, DialogueRequirementRegistrationHelper>* s_DialogueRequirementRegistrations;

	//Variables
	std::string m_registrationName = "";
	std::string m_lowerCaseRegistrationName = "";
	DialogueRequirementCreationFunc* m_creationFunc = nullptr;
	DialogueRequirementDefaultCreationFunc* m_defaultCreationFunc = nullptr;
public:
	//Constructors
	DialogueRequirementRegistrationHelper(const std::string& nam, DialogueRequirementCreationFunc* bevCreatFunc,
		DialogueRequirementDefaultCreationFunc* defCreatFunc);

	//Setters
	void SetName(const std::string& str);
	void SetCreationFunc(DialogueRequirementCreationFunc* func);
	void SetDefaultCreationFunc(DialogueRequirementDefaultCreationFunc* func);

	//Getters
	const std::string& GetName() const;
	const std::string& GetLowerCaseName() const;
	DialogueRequirementCreationFunc* GetCreationFunc() const;
	DialogueRequirementDefaultCreationFunc* GetDefaultCreationFunc() const;

	//Operations
	DialogueRequirement* CreateDialogueRequirement(const XMLNode& node);
	DialogueRequirement* CreateDefaultDialogueRequirement();

	//Static Functions
	static DialogueRequirement* CreateDefaultDialogueRequirementByName(const std::string& name);
	static DialogueRequirement* CreateDialogueRequirementByName(const std::string& name, const XMLNode& node);
	static bool DetermineDialogueRequirementExists(const std::string& name);
	static void ClearAllDialogueRequirementRegistrations();
};
#endif