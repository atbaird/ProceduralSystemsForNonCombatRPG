#pragma once

#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP
#include <vector>
#include <string>

#define STATIC
namespace FMOD
{
	class Channel;
};
typedef unsigned int SoundID;
typedef FMOD::Channel* AudioChannelHandle;
class AudioSystem;
class ActiveSound;

class SoundManager
{
private:
protected:
	//Static Variables
	static SoundManager* s_SoundManager;
	
	//Variables
	float m_SecondsBeforeMusicFadesOut = 1.f;
	float m_MusicFadeOutSpeed = 0.1f;
	float m_DelayBeforeNewMusicStart = 0.5f;
	float m_SecondsSinceCurrentMusicStopped = 0.f;
	std::vector<ActiveSound*> m_ActiveSounds;
	ActiveSound* m_CurrentMusic = nullptr;
	ActiveSound* m_QueuedMusic = nullptr;
	AudioSystem* m_AudioSystem = nullptr;
	bool m_fadeOutCurrentMusicNow = false;
public:
	//Constructors
	SoundManager();
	virtual ~SoundManager();

	//Default AudioSystem Operations
	const SoundID CreateOrGetSound(const std::string& soundFileName, const bool& loop = false);
	AudioChannelHandle PlaySoundAndOnlyGetChannel(SoundID soundID);
	void StopChannel(AudioChannelHandle channel);
	const bool GetIsSoundOrMusicIsPlaying(AudioChannelHandle channel) const;
	void Update(float dt);

	//SoundManager Operations
	ActiveSound* PlaySoundEffect(const std::string& soundEffectName, float volume = 1.f, const bool& loop = false);
	ActiveSound* PlayMusic(const std::string& musicName, float volume = 1.f, const bool& loop = true);
	void SetSecondsBeforeMusicFadesOut(float seconds);

	//Getters
	ActiveSound* GetCurrentMusicPlaying() const;
	ActiveSound* GetQueuedMusicPlaying() const;
	float GetSecondsBeforeMusicAllowedToFadeOut() const;
	float GetMusicFadeOutSpeed() const;

	//Static Functions
	static SoundManager* CreateOrGetSoundManager();
	static SoundManager* GetSoundManager();
	static void ClearSoundManager();
	static bool OnUpdateReadInAllSoundsFromXML();
	static void ReadInAllSoundsFromXMLAllAtOnce();
	static void ClearAllRegisteredSounds();
	static const bool DetermineSoundIDDoesNotExist(SoundID soundID);
	static unsigned int GetMissingSoundID();
};

#endif