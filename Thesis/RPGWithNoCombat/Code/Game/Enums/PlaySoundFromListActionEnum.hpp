#pragma once

#ifndef PLAYSOUNDFROMLISTACTIONENUM_HPP
#define PLAYSOUNDFROMLISTACTIONENUM_HPP
#define STATIC
enum PlaySoundFromListActionEnum
{
	PLAY_SOUND_INVALID = -1,
	PLAY_SOUND_RANDOM,
	PLAY_SOUND_SEQUENTIAL,
	NUMBER_OF_PLAY_SOUND_FROM_LIST_ACTION_ENUMS

};

#include <string>
struct SoundEffectData
{
	std::string m_soundEffectName = "";
	float m_volume = 1.f;

	SoundEffectData(const std::string& soundEffectName = "", float volume = 1.f);

	void WriteSoundEffectDataToString(std::string& str, int indentationAmt) const;
};

#include <vector>
class PlaySoundAction
{
public:
	static bool PlaySoundFromList(const std::vector<SoundEffectData>& list,
		PlaySoundFromListActionEnum action, int& indexLastPlayed);
	static bool PlaySoundFromListSequential(const std::vector<SoundEffectData>& list, int& indexLastPlayed);
	static bool PlaySoundFromListRandom(const std::vector<SoundEffectData>& list, int& indexLastPlayed);
};
#endif