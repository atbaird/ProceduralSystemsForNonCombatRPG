//---------------------------------------------------------------------------
#include "Engine/Core/Audio.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//---------------------------------------------------------------------------
//SoundIDRegistration
const bool SoundIDRegistration::operator==(const SoundIDRegistration& other) const
{
	if (m_soundFileLocation == other.m_soundFileLocation
		&& m_looping == other.m_looping)
	{
		return true;
	}
	return false;
}

const bool SoundIDRegistration::operator<(const SoundIDRegistration& other) const
{
	if (m_soundFileLocation > other.m_soundFileLocation)
	{
		if (m_looping > other.m_looping)
		{
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
AudioSystem::AudioSystem()
	: m_fmodSystem( nullptr )
{
	InitializeFMOD();
}


//---------------------------------------------------------------------------
// FMOD startup code based on "GETTING STARTED With FMOD Ex Programmerís API for Windows" document
//	from the FMOD programming API at http://www.fmod.org/download/
//
void AudioSystem::InitializeFMOD()
{
	const int MAX_AUDIO_DEVICE_NAME_LEN = 256;
	FMOD_RESULT result;
	unsigned int fmodVersion;
	int numDrivers;
	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS deviceCapabilities;
	char audioDeviceName[ MAX_AUDIO_DEVICE_NAME_LEN ];

	// Create a System object and initialize.
	result = FMOD::System_Create( &m_fmodSystem );
	ValidateResult( result );

	result = m_fmodSystem->getVersion( &fmodVersion );
	ValidateResult( result );

	if( fmodVersion < FMOD_VERSION )
	{
		DebuggerPrintf( "AUDIO SYSTEM ERROR!  Your FMOD .dll is of an older version (0x%08x == %d) than that the .lib used to compile this code (0x%08x == %d).\n", fmodVersion, fmodVersion, FMOD_VERSION, FMOD_VERSION );
	}

	result = m_fmodSystem->getNumDrivers( &numDrivers );
	ValidateResult( result );

	if( numDrivers == 0 )
	{
		result = m_fmodSystem->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
		ValidateResult( result );
	}
	else
	{
		result = m_fmodSystem->getDriverCaps( 0, &deviceCapabilities, 0, &speakerMode );
		ValidateResult( result );

		// Set the user selected speaker mode.
		result = m_fmodSystem->setSpeakerMode( speakerMode );
		ValidateResult( result );

		if( deviceCapabilities & FMOD_CAPS_HARDWARE_EMULATED )
		{
			// The user has the 'Acceleration' slider set to off! This is really bad
			// for latency! You might want to warn the user about this.
			result = m_fmodSystem->setDSPBufferSize( 1024, 10 );
			ValidateResult( result );
		}

		result = m_fmodSystem->getDriverInfo( 0, audioDeviceName, MAX_AUDIO_DEVICE_NAME_LEN, 0 );
		ValidateResult( result );

		if( strstr( audioDeviceName, "SigmaTel" ) )
		{
			// Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			// PCM floating point output seems to solve it.
			result = m_fmodSystem->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR );
			ValidateResult( result );
		}
	}

	result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
	if( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
	{
		// Ok, the speaker mode selected isn't supported by this sound card. Switch it
		// back to stereo...
		result = m_fmodSystem->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
		ValidateResult( result );

		// ... and re-init.
		result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
		ValidateResult( result );
	}
}


//---------------------------------------------------------------------------
AudioSystem::~AudioSystem()
{
// 	FMOD_RESULT result = FMOD_OK;
// 	result = FMOD_System_Close( m_fmodSystem );
// 	result = FMOD_System_Release( m_fmodSystem );
// 	m_fmodSystem = nullptr;
}

//---------------------------------------------------------------------------
const SoundID AudioSystem::CreateOrGetSound(const std::string& soundFileName, const bool& loop)
{
	SoundIDRegistration regCheck;
	regCheck.m_looping = loop;
	regCheck.m_soundFileLocation = soundFileName;
	for (size_t idx = 0; idx < m_registeredSoundIDs.size(); idx++)
	{
		if (m_registeredSoundIDs[idx] == regCheck)
		{
			return m_registeredSoundIDs[idx].m_soundID;
		}
	}

	{
		FMOD::Sound* newSound = nullptr;
		m_fmodSystem->createSound( soundFileName.c_str(), FMOD_DEFAULT, nullptr, &newSound );
		if (loop == false)
		{
			newSound->setMode(FMOD_LOOP_OFF);
		}
		else
		{
			newSound->setMode(FMOD_LOOP_NORMAL);
		}
		if( newSound )
		{
			SoundID newSoundID = m_registeredSounds.size();
			regCheck.m_soundID = newSoundID;
			m_registeredSoundIDs.push_back(regCheck);
			m_registeredSounds.push_back( newSound );
			return newSoundID;
		}
	}

	return MISSING_SOUND_ID;
}

//---------------------------------------------------------------------------
void AudioSystem::StopChannel(FMOD::Channel* channel)
{
	if (channel != nullptr)
	{
		FMOD::Channel* fmodChannel = (FMOD::Channel*) channel;
		fmodChannel->stop();
	}
}

const bool AudioSystem::GetIsPlaying(AudioChannelHandle channel) const
{
	if (channel == nullptr)
	{
		return false;
	}
	bool isPlaying = true;
	channel->isPlaying(&isPlaying);
	return isPlaying;
}

//---------------------------------------------------------------------------
FMOD::Channel* AudioSystem::PlaySound(const SoundID& soundID, const float& volumeLevel)
{
	unsigned int numSounds = m_registeredSounds.size();
	if( soundID < 0 || soundID >= numSounds )
		return nullptr;

	FMOD::Sound* sound = m_registeredSounds[ soundID ];
	if( !sound )
		return nullptr;

	FMOD::Channel* channelAssignedToSound = nullptr;
	

	m_fmodSystem->playSound( FMOD_CHANNEL_FREE, sound, false, &channelAssignedToSound );
	if( channelAssignedToSound )
	{
		channelAssignedToSound->setVolume( volumeLevel );
	}
	return channelAssignedToSound;
}


//---------------------------------------------------------------------------
void AudioSystem::Update(const float& deltaSeconds)
{
	deltaSeconds;
	FMOD_RESULT result = m_fmodSystem->update();
	ValidateResult( result );
}


//---------------------------------------------------------------------------
void AudioSystem::ValidateResult(const FMOD_RESULT& result)
{
	if( result != FMOD_OK )
	{
		DebuggerPrintf( "AUDIO SYSTEM ERROR: Got error result code %d.\n", result );
		__debugbreak();
	}
}
