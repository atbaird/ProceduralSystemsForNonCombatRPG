#pragma once

#ifndef FEATUREBEHAVIOR_HPP
#define FEATUREBEHAVIOR_HPP
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehaviorRegistrationHelper.hpp"
#include "Game/Enums/DefinedFeatureBehaviorIdxes.hpp"
#include <string>
#include <vector>

struct XMLNode;
class Feature;
class BaseGameEntity;
class Map;
class FeatureBehavior
{
private:
protected:
	std::vector<bool> m_entitiesCanInteractWith;
	std::string m_featureName = "";
	std::string m_chosenReferenceName = "";
	Feature* m_owningFeature = nullptr;
	size_t m_desiredBehaviorType = 0;
	bool m_desiredBehaviorTypeNotSet = true;
public:
	//Constructors
	FeatureBehavior(DefinedFeatureBehaviorIdxes featureDefault = INVALID_FEATURE_BEHAVIOR);
	FeatureBehavior(const XMLNode& node, DefinedFeatureBehaviorIdxes featureDefault = INVALID_FEATURE_BEHAVIOR);
	FeatureBehavior(const FeatureBehavior& other);
	virtual ~FeatureBehavior();

	void ResetEntitiesCanInteractWith();

	//Setters
	void SetOwningFeature(Feature* feature);

	//Getters
	const std::string& GetFeatureName() const;
	const std::string& GetReferenceName() const;
	const Feature* GetOwningFeature() const;
	Feature* GetEditableOwningFeature();
	size_t GetDesiredBehaviorType() const;
	bool GetDesiredBehaviorTypeNotSet() const;

	//Operations
	virtual int WriteFeatureBehaviorToString(std::string& str, int indentationLevel) const;
	virtual void CheckMapNodeForAttributesOfInterest(const XMLNode& node);
	virtual void TriggerOnPlacement(const Map* map);
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity);
	virtual void UpdateFeature(float dt);
	virtual FeatureBehavior* Clone() = 0;
};
#endif