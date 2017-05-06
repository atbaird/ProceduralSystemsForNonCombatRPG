#pragma once

#ifndef SETAGENTSTAT_HPP
#define SETAGENTSTAT_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateFeatureSetAgentStatBehavior();
static FeatureBehavior* CreateFeatureSetAgentStatBehavior(const XMLNode& node);

class SetAgentStat : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureSetAgentStatBehaviorRegistration;

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
	SetAgentStat();
	SetAgentStat(const XMLNode& node);
	SetAgentStat(const SetAgentStat& other);
	virtual ~SetAgentStat();

	//Operations
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};

#endif