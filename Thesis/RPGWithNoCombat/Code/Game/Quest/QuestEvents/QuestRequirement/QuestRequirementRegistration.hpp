#pragma once

#ifndef QUESTREQUIREMENTREGISTRATION_HPP
#define QUESTREQUIREMENTREGISTRATION_HPP
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementType.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementCheckVariables/QuestRequirementCheckVariableType.hpp"
#include <string>
#include <map>

#define STATIC


struct XMLNode;
class QuestRequirement;
typedef QuestRequirement* (QuestRequirementCreationFunc)(const XMLNode& node);
typedef QuestRequirement* (QuestRequirementDefaultCreationFunc)();


class QuestRequirementRegistration
{
private:
protected:
	//Static Variables
	static std::map<std::string, QuestRequirementRegistration>* s_QuestRequirementRegistrations;

	//Variables
	std::string m_QuestRequirementRegistrationName = "";
	std::string m_LowerCaseQuestRequirementRegistrationName = "";
	QuestRequirementDefaultCreationFunc* m_defaultCreationFunc = nullptr;
	QuestRequirementCreationFunc* m_creationFunc = nullptr;
public:
	//Constructors
	QuestRequirementRegistration(const std::string& nam, QuestRequirementCreationFunc* bevCreatFunc,
		QuestRequirementDefaultCreationFunc* defCreatFunc);
	virtual ~QuestRequirementRegistration();

	//Setters
	void SetName(const std::string& str);
	void SetCreationFunc(QuestRequirementCreationFunc* func);
	void SetDefaultCreationFunc(QuestRequirementDefaultCreationFunc* func);

	//Getters
	const std::string& GetName() const;
	const std::string& GetLowerCaseName() const;
	QuestRequirementCreationFunc* GetCreationFunc() const;
	QuestRequirementDefaultCreationFunc* GetDefaultCreationFunc() const;

	//Operations
	QuestRequirement* CreateQuestRequirement(const XMLNode& node);
	QuestRequirement* CreateDefaultQuestRequirement();

	//Static Functions
	static const std::string GetRegistrationNameForQuestRequirementTypes(QuestRequirementType reqType,
		QuestRequirementCheckVariableType varType = INVALID_CHECK_VARIABLE_TYPE);
	static QuestRequirement* CreateDefaultQuestRequirementByName(const std::string& name);
	static QuestRequirement* CreateDefaultQuestRequirementByType(QuestRequirementType reqType,
		QuestRequirementCheckVariableType varType = INVALID_CHECK_VARIABLE_TYPE);
	static QuestRequirement* CreateQuestRequirementByName(const std::string& name, const XMLNode& node);
	static QuestRequirement* CreateQuestRequirementByType(const XMLNode& node, QuestRequirementType reqType,
		QuestRequirementCheckVariableType varType = INVALID_CHECK_VARIABLE_TYPE);
	static bool DetermineQuestRequirementRegistrationExists(const std::string& name);
	static void ClearAllQuestRequirementRegistrations();
};
#endif