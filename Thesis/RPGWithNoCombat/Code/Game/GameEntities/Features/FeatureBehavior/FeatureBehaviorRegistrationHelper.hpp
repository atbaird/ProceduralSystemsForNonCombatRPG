#pragma once

#ifndef FEATUREBEHAVIORREGISTRATIONHELPER_HPP
#define FEATUREBEHAVIORREGISTRATIONHELPER_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include <map>

class FeatureBehavior;

typedef FeatureBehavior* (FeatureBehaviorCreationFunc)(const XMLNode& node);
typedef FeatureBehavior* (FeatureBehaviorDefaultCreationFunc)();

class FeatureBehaviorRegistrationHelper
{
private:
protected:
	//Static Variables
	static std::map<std::string, FeatureBehaviorRegistrationHelper>* s_behaviorRegistrations;

	//Variables
	std::string m_registrationName = "";
	std::string m_lowerCaseName = "";
	FeatureBehaviorCreationFunc* m_featureBehaviorCreationFunc = nullptr;
	FeatureBehaviorDefaultCreationFunc* m_featureBehaviorDefaultCreationFunc = nullptr;
public:
	//Constructors
	FeatureBehaviorRegistrationHelper();
	FeatureBehaviorRegistrationHelper(const std::string& registrationName,
		FeatureBehaviorCreationFunc* creationFunc,
		FeatureBehaviorDefaultCreationFunc* defaultCreationFunc);
	virtual ~FeatureBehaviorRegistrationHelper();

	//Setters
	void SetName(const std::string& str, bool handleChangingRegistration = false);
	void SetCreationFunc(FeatureBehaviorCreationFunc* func);
	void SetDefaultCreationFunc(FeatureBehaviorDefaultCreationFunc* func);

	//Getters
	const std::string& GetRegistrationName() const;
	const std::string& GetLowerCaseRegistrationName() const;
	FeatureBehaviorCreationFunc* GetBehaviorCreationFunc() const;
	FeatureBehaviorDefaultCreationFunc* GetDefaultBehaviorCreationFunc() const;

	//Operations
	FeatureBehavior* CreateFeatureBehavior(const XMLNode& node);
	FeatureBehavior* DefaultCreateFeatureBehavior();

	//Static functions
	static FeatureBehavior* CreateDefaultFeatureBehaviorByName(const std::string& name);
	static FeatureBehavior* CreateFeatureBehaviorByName(const std::string& name, const XMLNode& node);
	static bool DetermineFeatureBehaviorExists(const std::string& name);
	static void ClearAllFeatureBehaviorRegistrations();
};

#endif