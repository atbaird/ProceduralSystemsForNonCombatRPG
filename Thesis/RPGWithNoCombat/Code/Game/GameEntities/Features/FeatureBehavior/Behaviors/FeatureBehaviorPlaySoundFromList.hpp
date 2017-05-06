#pragma once

#ifndef FEATUREBEHAVIORPLAYSOUNDFROMLIST_HPP
#define FEATUREBEHAVIORPLAYSOUNDFROMLIST_HPP
#include <vector>
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"
#include "Game/Enums/PlaySoundFromListActionEnum.hpp"

//Spawner Functions
static FeatureBehavior* DefaultCreateFeaturePlaySoundFromListBehavior();
static FeatureBehavior* CreateFeaturePlaySoundFromListBehavior(const XMLNode& node);

class FeatureBehaviorPlaySoundFromList : public FeatureBehavior
{
private:
protected:
	//Static Variables
	static const FeatureBehaviorRegistrationHelper s_FeatureAddToAgentStatBehaviorRegistration;

	//Variables
	std::vector<SoundEffectData> m_soundEffectNames;
	PlaySoundFromListActionEnum m_action = PLAY_SOUND_RANDOM;
	int m_indexLastPlayed = -1;
public:
	//Constructors
	FeatureBehaviorPlaySoundFromList();
	FeatureBehaviorPlaySoundFromList(const XMLNode& node);
	FeatureBehaviorPlaySoundFromList(const FeatureBehaviorPlaySoundFromList& other);
	virtual ~FeatureBehaviorPlaySoundFromList();

	//Setters
	void AddSoundEffectName(const std::string& name, float volume = 1.f);

	//Operations
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity) override;
	virtual FeatureBehavior* Clone() override;

};

#endif