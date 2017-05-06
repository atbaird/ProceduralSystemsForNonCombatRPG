#pragma once

#ifndef ADDTOAGENTSTAT_HPP
#define ADDTOAGENTSTAT_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateFeatureAddToAgentStatBehavior();
static FeatureBehavior* CreateFeatureAddToAgentStatBehavior(const XMLNode& node);

class AddToAgentStat : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureAddToAgentStatBehaviorRegistration;

	//Variables
	std::string m_variableNameToSet = "";
	std::string m_variableAbbreviationToSet = "";
	int m_valueToSet = 0;

	bool m_variableNameToSetIsSet = false;
	bool m_variableAbbreviationToSetIsSet = false;
	bool m_variableValueToSetIsSet = false;
	bool m_careWhetherItsBeenUsedOnce = false;
	bool m_usedOnce = false;
public:
	//Constructors
	AddToAgentStat();
	AddToAgentStat(const XMLNode& node);
	AddToAgentStat(const AddToAgentStat& other);
	virtual ~AddToAgentStat();
protected:
	void ReadXMLAttributes(const XMLNode& node);

public:

	//Operations
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};

#endif