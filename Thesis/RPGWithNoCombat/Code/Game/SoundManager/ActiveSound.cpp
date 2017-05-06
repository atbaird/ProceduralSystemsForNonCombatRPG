#include "Game/SoundManager/ActiveSound.hpp"
#include "Game/SoundManager/SoundManager.hpp"
#include "Engine/Core/Audio.hpp"
#include "Engine/Core/StringUtils.hpp"

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Constructors
ActiveSound::ActiveSound()
	: m_soundID(MISSING_SOUND_ID)
{

}

ActiveSound::ActiveSound(SoundID soundID)
	: m_soundID(soundID)
{

}

ActiveSound::~ActiveSound()
{

}


//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Operations
void ActiveSound::Update(float dt)
{
	if (m_lerpVolumeSpeed != 0.f)
	{
		float newVolume = m_volume + (m_lerpVolumeSpeed * dt);
		SetVolume(newVolume);
	}
}

void ActiveSound::TriggerSound()
{
	SoundManager* manager = SoundManager::GetSoundManager();
	if (manager == nullptr
		|| m_handle != nullptr
		|| m_soundID == MISSING_SOUND_ID)
	{
		return;
	}
	m_handle = manager->PlaySoundAndOnlyGetChannel(m_soundID);
	m_handle->setVolume(m_volume);
}


void ActiveSound::StopSound()
{
	SoundManager* manager = SoundManager::GetSoundManager();
	if (manager == nullptr
		|| m_handle == nullptr)
	{
		return;
	}
	manager->StopChannel(m_handle);
}


//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Setters
void ActiveSound::SetSoundName(const std::string& soundName)
{
	m_soundName = soundName;
	m_lowerCaseSoundName = MakeLower(soundName);
}

void ActiveSound::SetSoundID(SoundID soundID)
{
	m_soundID = soundID;
}

void ActiveSound::SetVolume(float volume)
{
	if (volume < 0.f)
	{
		m_volume = 0.f;
	}
	else if (m_volume > 1.f)
	{
		m_volume = 1.f;
	}
	else
	{
		m_volume = volume;
	}

	if (m_handle != nullptr
		&& GetAudioStillPlaying())
	{
		m_handle->setVolume(m_volume);
	}
}

void ActiveSound::SetVolumeLerpSpeed(float lerpSpeed)
{
	m_lerpVolumeSpeed = lerpSpeed;
}

void ActiveSound::DontLerpVolume()
{
	m_lerpVolumeSpeed = 0.f;
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//Getters
const std::string& ActiveSound::GetSoundName() const
{
	return m_soundName;
}

const std::string& ActiveSound::GetLowerCaseSoundName() const
{
	return m_lowerCaseSoundName;
}

AudioChannelHandle ActiveSound::GetAudioChannelHandle() const
{
	return m_handle;
}

SoundID ActiveSound::GetSoundID() const
{
	return m_soundID;
}

float ActiveSound::GetVolume() const
{
	return m_volume;
}

bool ActiveSound::GetAudioStillPlaying() const
{
	SoundManager* manager = SoundManager::GetSoundManager();
	if (manager == nullptr
		|| m_handle == nullptr)
	{
		return false;
	}
	return manager->GetIsSoundOrMusicIsPlaying(m_handle);
}