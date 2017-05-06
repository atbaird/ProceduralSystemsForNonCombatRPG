#include "Game/SoundManager/SoundManager.hpp"
#include "Game/SoundManager/SoundRegistration.hpp"
#include "Game/SoundManager/ActiveSound.hpp"
#include "Engine/Core/Audio.hpp"

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Static Variables
STATIC SoundManager* SoundManager::s_SoundManager = nullptr;

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Constructors
SoundManager::SoundManager()
	: m_AudioSystem(new AudioSystem())
{
	SetSecondsBeforeMusicFadesOut(m_SecondsBeforeMusicFadesOut);
}

SoundManager::~SoundManager()
{
	SoundRegistration::ClearAllSoundRegistrations();
	if (m_AudioSystem != nullptr)
	{
		delete m_AudioSystem;
		m_AudioSystem = nullptr;
	}

	for (size_t soundIdx = 0; soundIdx < m_ActiveSounds.size(); soundIdx++)
	{
		delete m_ActiveSounds[soundIdx];
	}
	m_ActiveSounds.clear();
	if (m_CurrentMusic != nullptr)
	{
		delete m_CurrentMusic;
		m_CurrentMusic = nullptr;
	}
	if (m_QueuedMusic != nullptr)
	{
		delete m_QueuedMusic;
		m_QueuedMusic = nullptr;
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Default AudioSystem Operations
const SoundID SoundManager::CreateOrGetSound(const std::string& soundFileName, const bool& loop)
{
	return m_AudioSystem->CreateOrGetSound(soundFileName, loop);
}

AudioChannelHandle SoundManager::PlaySoundAndOnlyGetChannel(SoundID soundID)
{
	return m_AudioSystem->PlaySound(soundID);
}

void SoundManager::StopChannel(AudioChannelHandle channel)
{
	m_AudioSystem->StopChannel(channel);
}

const bool SoundManager::GetIsSoundOrMusicIsPlaying(AudioChannelHandle channel) const
{
	return m_AudioSystem->GetIsPlaying(channel);
}

void SoundManager::Update(float dt)
{
	//Sound Effect updates
	for (size_t soundIdx = 0; soundIdx < m_ActiveSounds.size(); soundIdx++)
	{
		ActiveSound* sound = m_ActiveSounds[soundIdx];
		sound->Update(dt);
		if (sound->GetAudioStillPlaying() == false)
		{
			delete sound;
			m_ActiveSounds.erase(m_ActiveSounds.begin() + soundIdx);
			soundIdx--;
		}
	}

	//Music
	if (m_CurrentMusic != nullptr)
	{
		//Would be Cross fading be good?
		if (m_fadeOutCurrentMusicNow)
		{
			m_CurrentMusic->SetVolumeLerpSpeed(m_MusicFadeOutSpeed * -1.f);
		}
		m_CurrentMusic->Update(dt);
		if (m_fadeOutCurrentMusicNow && m_CurrentMusic->GetVolume() == 0.f)
		{
			m_SecondsSinceCurrentMusicStopped += dt;
			if (m_SecondsSinceCurrentMusicStopped >= m_DelayBeforeNewMusicStart)
			{
				m_SecondsSinceCurrentMusicStopped = 0.f;
				m_fadeOutCurrentMusicNow = false;
				delete m_CurrentMusic;
				m_CurrentMusic = m_QueuedMusic;
				m_QueuedMusic = nullptr;
				m_CurrentMusic->TriggerSound();
			}
		}
	}

	m_AudioSystem->Update(dt);
}


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//SoundManager Operations
ActiveSound* SoundManager::PlaySoundEffect(const std::string& soundEffectName, float volume, const bool& loop)
{
	SoundRegistration reg = SoundRegistration::GetSoundEffectByName(soundEffectName);
	if (reg == SoundRegistration::s_EmptyRegistration)
	{
		return nullptr;
	}
	if (!reg.GetSuccessfullyLoaded())
	{
		bool failedToLoad = !reg.TryToLoadSoundID();
		if (failedToLoad)
		{
			return nullptr;
		}
	}

	SoundID id;
	if (loop)
	{
		id = reg.GetLoopingSoundID();
	}
	else
	{
		id = reg.GetSoundID();
	}
	ActiveSound* sound = new ActiveSound(id);
	sound->SetVolume(volume * reg.GetVolume());
	sound->SetSoundName(reg.GetSoundName());

	m_ActiveSounds.push_back(sound);
	sound->TriggerSound();
	return sound;
}

ActiveSound* SoundManager::PlayMusic(const std::string& musicName, float volume, const bool& loop)
{
	SoundRegistration reg = SoundRegistration::GetMusicByName(musicName);
	if (reg == SoundRegistration::s_EmptyRegistration)
	{
		return nullptr;
	}
	if (!reg.GetSuccessfullyLoaded())
	{
		bool failedToLoad = !reg.TryToLoadSoundID();
		if (failedToLoad)
		{
			return nullptr;
		}
	}

	SoundID id;
	if (loop)
	{
		id = reg.GetLoopingSoundID();
	}
	else
	{
		id = reg.GetSoundID();
	}

	if (m_CurrentMusic != nullptr)
	{
		if (m_QueuedMusic != nullptr)
		{
			delete m_QueuedMusic;
			m_QueuedMusic = nullptr;
		}
		m_QueuedMusic = new ActiveSound(id);
		m_QueuedMusic->SetVolume(volume * reg.GetVolume());
		m_QueuedMusic->SetSoundName(reg.GetSoundName());
		m_fadeOutCurrentMusicNow = true;
		return m_QueuedMusic;
	}
	else
	{

		m_CurrentMusic = new ActiveSound(id);
		m_CurrentMusic->SetVolume(volume * reg.GetVolume());
		m_CurrentMusic->SetSoundName(reg.GetSoundName());
		m_CurrentMusic->TriggerSound();
		return m_CurrentMusic;
	}
}


void SoundManager::SetSecondsBeforeMusicFadesOut(float seconds)
{
	m_SecondsBeforeMusicFadesOut = abs(seconds);
	if (seconds == 0.f)
	{
		m_MusicFadeOutSpeed = 0.f;
	}
	else
	{
		m_MusicFadeOutSpeed = 1.f / m_SecondsBeforeMusicFadesOut;
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Getters
ActiveSound* SoundManager::GetCurrentMusicPlaying() const
{
	return m_CurrentMusic;
}

ActiveSound* SoundManager::GetQueuedMusicPlaying() const
{
	return m_QueuedMusic;
}

float SoundManager::GetSecondsBeforeMusicAllowedToFadeOut() const
{
	return m_SecondsBeforeMusicFadesOut;
}

float SoundManager::GetMusicFadeOutSpeed() const
{
	return m_MusicFadeOutSpeed;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Static Functions
STATIC SoundManager* SoundManager::CreateOrGetSoundManager()
{
	if (s_SoundManager == nullptr)
	{
		s_SoundManager = new SoundManager();
	}
	return s_SoundManager;
}

STATIC SoundManager* SoundManager::GetSoundManager()
{
	return s_SoundManager;
}

STATIC void SoundManager::ClearSoundManager()
{
	if (s_SoundManager != nullptr)
	{
		delete s_SoundManager;
		s_SoundManager = nullptr;
	}
}

STATIC bool SoundManager::OnUpdateReadInAllSoundsFromXML()
{
	return SoundRegistration::OnUpdateReadInAllSoundsFromCommonSoundFolder();
}

STATIC void SoundManager::ReadInAllSoundsFromXMLAllAtOnce()
{
	SoundRegistration::ReadInAllSoundsFromCommonSoundFolderAllAtOnce();
}

STATIC void SoundManager::ClearAllRegisteredSounds()
{
	SoundRegistration::ClearAllSoundRegistrations();
}

STATIC const bool SoundManager::DetermineSoundIDDoesNotExist(SoundID soundID)
{
	if (soundID == MISSING_SOUND_ID)
	{
		return false;
	}
	return true;
}

STATIC unsigned int SoundManager::GetMissingSoundID()
{
	return MISSING_SOUND_ID;
}