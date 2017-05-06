#pragma once

#ifndef DIALOGUEACTIONREGISTRATIONHELPER_HPP
#define DIALOGUEACTIONREGISTRATIONHELPER_HPP
#include <map>
#include <string>

#define STATIC
class DialogueTrigger;
struct XMLNode;
typedef DialogueTrigger* (DialogueActionCreationFunc)(const XMLNode& node);
typedef DialogueTrigger* (DialogueActionDefaultCreationFunc)();

class DialogueActionRegistrationHelper
{
private:
protected:
	//Static Variables
	static std::map<std::string, DialogueActionRegistrationHelper>* s_DialogueActionRegistrations;

	//Variables
	std::string m_registrationName = "";
	std::string m_lowerCaseRegistrationName = "";
	DialogueActionCreationFunc* m_creationFunc = nullptr;
	DialogueActionDefaultCreationFunc* m_defaultCreationFunc = nullptr;
public:
	//Constructors
	DialogueActionRegistrationHelper(const std::string& registrationName,
		DialogueActionCreationFunc* creationFunc,
		DialogueActionDefaultCreationFunc* defaultCreationFunc);
	virtual ~DialogueActionRegistrationHelper();

	//Setters
	void SetDialogueActionRegistrationHelperName(const std::string& str);
	void SetCreationFunc(DialogueActionCreationFunc* func);
	void SetDefaultCreationFunc(DialogueActionDefaultCreationFunc* func);

	//Getters
	const std::string& GetName() const;
	const std::string& GetLowerCaseName() const;
	DialogueActionCreationFunc* GetCreationFunc() const;
	DialogueActionDefaultCreationFunc* GetDefaultCreationFunc() const;

	//Operations
	DialogueTrigger* CreateDialogueAction(const XMLNode& node) const;
	DialogueTrigger* CreateDefaultDialogueAction() const;

	//Static Functions
	static DialogueTrigger* CreateDefaultDialogueActionByName(const std::string& name);
	static DialogueTrigger* CreateDialogueActionByName(const std::string& name, const XMLNode& node);
	static bool DetermineDialogueActionExists(const std::string& name);
	static void ClearAllDialogueActionRegistrations();
};
#endif