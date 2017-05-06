#pragma once

#ifndef QUESTTRIGGERREGISTRATION_HPP
#define QUESTTRIGGERREGISTRATION_HPP
#include <map>
#include <string>

#define STATIC

struct XMLNode;
class QuestTrigger;
typedef QuestTrigger* (QuestTriggerCreationFunc)(const XMLNode& node);
typedef QuestTrigger* (QuestTriggerDefaultCreationFunc)();

class QuestTriggerRegistration
{
private:
protected:
	//Static Variables
	static std::map<std::string, QuestTriggerRegistration>* s_QuestTriggerRegistrations;

	//Variables
	std::string m_TriggerRegName = "";
	std::string m_LowerCaseTriggerRegName = "";
	QuestTriggerCreationFunc* m_func = nullptr;
	QuestTriggerDefaultCreationFunc* m_defaultFunc = nullptr;
public:
	//Constructors
	QuestTriggerRegistration(const std::string& triggerRegName, 
		QuestTriggerCreationFunc* func, QuestTriggerDefaultCreationFunc* defaultFunc);
	virtual ~QuestTriggerRegistration();

	//Setters
	void SetTriggerRegName(const std::string regName);
	void SetQuestTriggerCreationFunc(QuestTriggerCreationFunc* func);
	void SetQuestTriggerDefaultCreationFunc(QuestTriggerDefaultCreationFunc* func);


	//Getters
	const std::string& GetTriggerRegName() const;
	const std::string& GetLowerCaseTriggerRegName() const;
	QuestTriggerCreationFunc* GetCreationFunc() const;
	QuestTriggerDefaultCreationFunc* GetDefaultCreatonFunc() const;


	//Operations
	QuestTrigger* CreateQuestTrigger(const XMLNode& node);
	QuestTrigger* CreateDefaultQuestTrigger();

	//Static Functions
	static QuestTrigger* CreateDefaultQuestTriggerByName(const std::string& name);
	static QuestTrigger* CreateQuestTriggerByName(const std::string& name, const XMLNode& node);
	static bool DetermineBehaviorExists(const std::string& name);
	static void ClearAllBehaviorRegistrations();
};
#endif