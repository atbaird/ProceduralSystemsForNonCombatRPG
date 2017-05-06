#pragma once

#ifndef NPCBEHAVIORREGISTRATIONHELPER_HPP
#define NPCBEHAVIORREGISTRATIONHELPER_HPP
#include <map>
#include "Engine/Core/EngineXMLParser.hpp"

class NPCBehavior;

typedef NPCBehavior* (BehaviorCreationFunc)(const XMLNode& node);
typedef NPCBehavior* (BehaviorDefaultCreationFunc)();

class NPCBehaviorRegistrationHelper
{
private:
protected:
	//Static Variables
	static std::map<std::string, NPCBehaviorRegistrationHelper>* s_behaviorRegistrations;

	//Variables
	std::string m_name;
	std::string m_lowerCaseName;
	BehaviorCreationFunc* m_behaviorCreationFunc = nullptr;
	BehaviorDefaultCreationFunc* m_defaultBehaviorCreationFunc = nullptr;
public:
	//Constructors
	NPCBehaviorRegistrationHelper(const std::string& nam, BehaviorCreationFunc* bevCreatFunc,
		BehaviorDefaultCreationFunc* defCreatFunc);
	virtual ~NPCBehaviorRegistrationHelper();

	//Operations
	NPCBehavior* CreateBehavior(const XMLNode& node);
	NPCBehavior* CreateDefaultBehavior();

	//Setters
	void SetName(const std::string& str, bool handleChangingRegistration = false);
	void SetCreationFunc(BehaviorCreationFunc* func);
	void SetDefaultCreationFunc(BehaviorDefaultCreationFunc* func);

	//Getters
	const std::string& GetName() const;
	const std::string& GetLowerCaseName() const;
	BehaviorCreationFunc* GetCreationFunc() const;
	BehaviorDefaultCreationFunc* GetDefaultCreationFunc() const;

	//Static functions
	static NPCBehavior* CreateDefaultBehaviorByName(const std::string& name);
	static NPCBehavior* CreateBehaviorByName(const std::string& name, const XMLNode& node);
	static bool DetermineBehaviorExists(const std::string& name);
	static void ClearAllBehaviorRegistrations();
};

#endif