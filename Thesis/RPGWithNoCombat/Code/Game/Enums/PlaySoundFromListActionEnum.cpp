#include "Game/Enums/PlaySoundFromListActionEnum.hpp"
#include "Game/SoundManager/SoundManager.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/SoundManager/ActiveSound.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

SoundEffectData::SoundEffectData(const std::string& soundEffectName, float volume)
	: m_soundEffectName(soundEffectName),
	m_volume(volume)
{

}

void SoundEffectData::WriteSoundEffectDataToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Sound Effect Name: " + m_soundEffectName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Sound Effect Volume: " + std::to_string(m_volume) + "\n";
}

STATIC bool PlaySoundAction::PlaySoundFromList(const std::vector<SoundEffectData>& list,
	PlaySoundFromListActionEnum action, int& indexLastPlayed)
{
	switch (action)
	{
	case PLAY_SOUND_SEQUENTIAL:		return PlaySoundFromListSequential(list, indexLastPlayed);
	case PLAY_SOUND_RANDOM:			return PlaySoundFromListRandom(list, indexLastPlayed);
	default:
		break;
	}

	return false;
}

STATIC bool PlaySoundAction::PlaySoundFromListSequential(const std::vector<SoundEffectData>& list, int& indexLastPlayed)
{
	SoundManager* manager = SoundManager::GetSoundManager();
	if (manager == nullptr)
	{
		return false;
	}
	if (indexLastPlayed < 0 || (size_t)indexLastPlayed >= list.size())
	{
		indexLastPlayed = 0;
	}
	else
	{
		indexLastPlayed++;
		if ((size_t)indexLastPlayed >= list.size())
		{
			indexLastPlayed = 0;
		}
	}

	ActiveSound* sound = manager->PlaySoundEffect(list[indexLastPlayed].m_soundEffectName,
		list[indexLastPlayed].m_volume, false);

	if (sound == nullptr)
	{
		return false;
	}
	return true;
}

STATIC bool PlaySoundAction::PlaySoundFromListRandom(const std::vector<SoundEffectData>& list, int& indexLastPlayed)
{
	SoundManager* manager = SoundManager::GetSoundManager();
	if (manager == nullptr)
	{
		return false;
	}
	std::vector<int> indexes;
	indexes.reserve(list.size());
	for (int idx = 0; (size_t)idx < list.size(); idx++)
	{
		if (idx == indexLastPlayed)
		{
			continue;
		}
		indexes.push_back(idx);
	}
	if (indexes.size() == 0)
	{
		indexes.push_back(indexLastPlayed);
	}
	indexLastPlayed = GetRandomIntLessThan(indexes.size());

	ActiveSound* sound = manager->PlaySoundEffect(list[indexLastPlayed].m_soundEffectName,
		list[indexLastPlayed].m_volume, false);

	if (sound == nullptr)
	{
		return false;
	}
	return true;
}