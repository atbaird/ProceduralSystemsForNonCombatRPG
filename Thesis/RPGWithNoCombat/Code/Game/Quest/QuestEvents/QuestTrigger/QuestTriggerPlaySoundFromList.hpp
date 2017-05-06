#pragma once

#ifndef QUESTTRIGGERPLAYSOUNDFROMLIST_HPP
#define QUESTTRIGGERPLAYSOUNDFROMLIST_HPP
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Game/Enums/PlaySoundFromListActionEnum.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerPlaySoundFromListFloatFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultPlaySoundFromListFunc();

class QuestTriggerPlaySoundFromList : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_QuestTriggerPlaySoundFromListRegistration;

	//Variables
	std::vector<SoundEffectData> m_soundEffectNames;
	PlaySoundFromListActionEnum m_action = PLAY_SOUND_RANDOM;
	int m_indexLastPlayed = -1;
public:
	//Constructors
	QuestTriggerPlaySoundFromList();
	QuestTriggerPlaySoundFromList(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerPlaySoundFromList(const QuestTriggerPlaySoundFromList& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerPlaySoundFromList();

	//Setters
	void AddSoundEffectName(const std::string& name, float volume = 1.f);
	void SetPlaySoundFromListAction(PlaySoundFromListActionEnum action);

	//Getters
	const std::vector<SoundEffectData>& GetSoundEffectNames() const;
	int GetIndexLastPlayed() const;
	PlaySoundFromListActionEnum GetPlaySoundFromListAction() const;

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};

#endif