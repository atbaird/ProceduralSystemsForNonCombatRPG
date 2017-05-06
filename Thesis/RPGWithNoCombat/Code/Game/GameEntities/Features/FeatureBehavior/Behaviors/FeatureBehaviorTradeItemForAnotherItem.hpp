#pragma once

#ifndef FEATUERBEHAVIORTRADEITEMFORANOTHERITEM_HPP
#define FEATUREBEHAVIORTRADEITEMFORANOTHERITEM_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateFeatureTradeItemForItemBehavior();
static FeatureBehavior* CreateFeatureTradeItemForItemBehavior(const XMLNode& node);

class FeatureBehaviorTradeItemForAnotherItem : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureTradeItemForItemBehaviorRegistration;

	//Variables
	std::string m_itemWantedName = "";
	std::string m_itemNameWillGive = "";
	float m_amountOfItemNameWanted = 1;
	float m_amountOfItemNameWillGive = 1;
public:
	//Constructors
	FeatureBehaviorTradeItemForAnotherItem();
	FeatureBehaviorTradeItemForAnotherItem(const XMLNode& node);
	FeatureBehaviorTradeItemForAnotherItem(const FeatureBehaviorTradeItemForAnotherItem& other);
	virtual ~FeatureBehaviorTradeItemForAnotherItem();


	//Operations
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;
};

#endif