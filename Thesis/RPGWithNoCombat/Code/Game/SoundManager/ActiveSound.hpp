#pragma once

#ifndef ACTIVESOUND_HPP
#define ACTIVESOUND_HPP
#include <string>

namespace FMOD
{
	class Channel;
};

typedef unsigned int SoundID;
typedef FMOD::Channel* AudioChannelHandle;

class ActiveSound
{
private:
protected:
	//Variables
	std::string m_soundName = "";
	std::string m_lowerCaseSoundName = "";
	AudioChannelHandle m_handle = nullptr;
	SoundID m_soundID;
	float m_volume = 1.f;
	float m_lerpVolumeSpeed = 0.f;
public:
	//Constructors
	ActiveSound();
	ActiveSound(SoundID soundID);
	virtual ~ActiveSound();

	//Operations
	void Update(float dt);
	void TriggerSound();
	void StopSound();

	//Setters
	void SetSoundName(const std::string& soundName);
	void SetSoundID(SoundID soundID);
	void SetVolume(float volume);
	void SetVolumeLerpSpeed(float lerpSpeed);
	void DontLerpVolume();

	//Getters
	const std::string& GetSoundName() const;
	const std::string& GetLowerCaseSoundName() const;
	AudioChannelHandle GetAudioChannelHandle() const;
	SoundID GetSoundID() const;
	float GetVolume() const;
	bool GetAudioStillPlaying() const;
};
#endif