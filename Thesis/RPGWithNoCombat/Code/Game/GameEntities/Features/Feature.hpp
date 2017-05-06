#pragma once

#ifndef FEATURE_HPP
#define FEATURE_HPP
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/Collision/SquareCollision.hpp"
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"

class Feature : public BaseGameEntity
{
private:
protected:
	std::vector < std::vector<FeatureBehavior*>> m_FeatureBehaviors;

	std::string m_RelatedQuest = "";
	unsigned int m_FeatureMode = 0;
	bool m_solid = false;
	bool m_overwriteTileOnIsSolid = false;
	bool m_lastInteractRanCorrectly = false;
public:
	//Constructors
	Feature(const SpriteResource* resource = nullptr, Directions4 defaultDirection = SOUTH);
	virtual ~Feature();

	//Updates and render
	virtual void Update(const float& dt) override;

	//Setters
	void SetRelatedQuest(const std::string& questTitle);
	void AddFeatureBehaviors(const std::vector<FeatureBehavior*>& behaviors,
		size_t behaviorType, bool isCopiedAlready = false);
	void AddFeatureBehavior(FeatureBehavior* bev, size_t behaviorType);
	void AddInteractiveFeatureBehavior(FeatureBehavior* bev);
	void AddOnUpdateFeatureBehavior(FeatureBehavior* bev);
	void AddOnPlacementFeatureBehavior(FeatureBehavior* bev);
	void SetSolid(bool solid, bool overwriteTileOnIsSolid);
	void SetFeatureMode(unsigned int mode);

	//Getters
	const std::string& GetRelatedQuestTitle() const;
	const std::vector<FeatureBehavior*>& GetInteractiveFeatureBehaviors() const;
	std::vector<FeatureBehavior*>* GetInteractiveFeatureBehaviorsEditablePtr();
	const std::vector<FeatureBehavior*>* GetInteractiveFeatureBehaviorsPtr() const;
	bool GetSolid() const;
	bool GetOverwriteTileOnIsSolid() const;
	bool GetLastInteractBehaviorRan() const;
	unsigned int GetFeatureMode() const;

	//Operations
	virtual void WriteDeveloperDebugInfo(DeveloperDebugInfoBox& info) const override;
	void EntityInteractWithFeature(BaseGameEntity* entity);
	void TriggerPlacementActions(const Map* map);
};

#endif