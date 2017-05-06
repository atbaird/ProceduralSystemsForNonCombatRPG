#include "Game/SoundManager/SoundRegistration.hpp"
#include "Game/SoundManager/SoundManager.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Game/TheGame.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Static Variables
STATIC SoundRegistrationSaveOfWhereAtInReadingData* SoundRegistration::s_DataForWhileLoading = nullptr;
STATIC std::vector<SoundRegistration>* SoundRegistration::s_AllSoundRegistrations = nullptr;
STATIC std::vector<SoundRegistration>* SoundRegistration::s_SoundEffectsSoundRegistrations = nullptr;
STATIC std::vector<SoundRegistration>* SoundRegistration::s_MusicSoundRegistrations = nullptr;
STATIC const SoundRegistration SoundRegistration::s_EmptyRegistration = SoundRegistration();

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Constructors
SoundRegistration::SoundRegistration()
	: m_SoundID(SoundManager::GetMissingSoundID()),
	m_LoopingSoundID(SoundManager::GetMissingSoundID())
{

}

SoundRegistration::SoundRegistration(const std::string& soundName, const std::string& soundFileLocation)
	: m_SoundID(SoundManager::GetMissingSoundID()),
	m_LoopingSoundID(SoundManager::GetMissingSoundID())
{
	SetSoundName(soundName);
	SetSoundFileLocation(soundFileLocation);
	TryToLoadSoundID();
}

SoundRegistration::SoundRegistration(const XMLNode& node)
	: m_SoundID(SoundManager::GetMissingSoundID()),
	m_LoopingSoundID(SoundManager::GetMissingSoundID())
{
	ReadXMLAttributes(node);
	TryToLoadSoundID();
}

SoundRegistration::SoundRegistration(const SoundRegistration& other)
	: m_SoundName(other.m_SoundName),
	m_LowerCaseSoundName(other.m_LowerCaseSoundName),
	m_SoundFileLocation(other.m_SoundFileLocation),
	m_volume(other.m_volume),
	m_SoundID(other.m_SoundID),
	m_LoopingSoundID(other.m_LoopingSoundID),
	m_SoundLoops(other.m_SoundLoops),
	m_SuccessfullyLoaded(other.m_SuccessfullyLoaded)
{

}

SoundRegistration::~SoundRegistration()
{

}

void SoundRegistration::ReadXMLAttributes(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "filelocation"
			|| attrName == "location")
		{
			m_SoundFileLocation = std::string(attr.lpszValue);
		}
		else if (attrName == "name")
		{
			SetSoundName(std::string(attr.lpszValue));
		}
		else if (attrName == "volume"
			|| attrName == "vol")
		{
			m_volume = (float)atof(attr.lpszValue);
		}
	}
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Setters
void SoundRegistration::SetSoundName(const std::string& name)
{
	m_SoundName = name;
	m_LowerCaseSoundName = MakeLower(name);
}

void SoundRegistration::SetSoundFileLocation(const std::string& soundLocation)
{
	m_SoundFileLocation = soundLocation;
	TryToLoadSoundID();
}

void SoundRegistration::SetSoundID(SoundID soundID)
{
	m_SoundID = soundID;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Getters
const std::string& SoundRegistration::GetSoundName() const
{
	return m_SoundName;
}

const std::string& SoundRegistration::GetLowerCaseSoundName() const
{
	return m_LowerCaseSoundName;
}

const std::string& SoundRegistration::GetSoundFileLocation() const
{
	return m_SoundFileLocation;
}

float SoundRegistration::GetVolume() const
{
	return m_volume;
}

SoundID SoundRegistration::GetSoundID() const
{
	return m_SoundID;
}

SoundID SoundRegistration::GetLoopingSoundID() const
{
	return m_LoopingSoundID;
}

bool SoundRegistration::GetSuccessfullyLoaded() const
{
	return m_SuccessfullyLoaded;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Operations
bool SoundRegistration::TryToLoadSoundID()
{
	SoundManager* manager = SoundManager::GetSoundManager();
	if (manager == nullptr)
	{
		return false;
	}
	m_SoundID = manager->CreateOrGetSound(m_SoundFileLocation, false);
	m_LoopingSoundID = manager->CreateOrGetSound(m_SoundFileLocation, true);

	if (!SoundManager::DetermineSoundIDDoesNotExist(m_SoundID))
	{
		m_SuccessfullyLoaded = false;
		return false;
	}

	m_SuccessfullyLoaded = true;
	return true;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Operators
const bool SoundRegistration::operator==(const SoundRegistration& other) const
{
	if (m_LowerCaseSoundName == other.m_LowerCaseSoundName
		&& m_SoundLoops == other.m_SoundLoops)
	{
		return true;
	}
	return false;
}

void SoundRegistration::WriteSoundRegistrationToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Sound Name: " + m_SoundName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Sound File Path: " + m_SoundFileLocation + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Sound Internal Volume: " + std::to_string(m_volume) + "\n";

}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Static Functions
STATIC void SoundRegistration::WriteAllSoundRegistrationsToString(std::string& str, int indentationAmt)
{
	int nextLevelUp = indentationAmt + 3;
	if (s_AllSoundRegistrations == nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevelUp);
		str += "No sounds registered!\n";
		return;
	}
	GameDebuggingCommons::AddIndentation(str, nextLevelUp);
	str += "Total number of sounds: " + std::to_string(s_AllSoundRegistrations->size()) + "\n";

	int secondLevelUp = nextLevelUp + 3;
	if (s_MusicSoundRegistrations != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevelUp);
		str += "Music Registrations: \n";
		GameDebuggingCommons::AddIndentation(str, nextLevelUp);
		str += "Number of Music registrations: " + std::to_string(s_MusicSoundRegistrations->size()) + "\n";
		for (size_t i = 0; i < s_MusicSoundRegistrations->size(); i++)
		{
			const SoundRegistration& reg = s_MusicSoundRegistrations->at(i);
			reg.WriteSoundRegistrationToString(str, secondLevelUp);
		}
	}

	if (s_SoundEffectsSoundRegistrations != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevelUp);
		str += "Sound Effect Registrations: \n";
		GameDebuggingCommons::AddIndentation(str, nextLevelUp);
		str += "Number of Sound Effect registrations: " + std::to_string(s_SoundEffectsSoundRegistrations->size()) + "\n";
		for (size_t i = 0; i < s_SoundEffectsSoundRegistrations->size(); i++)
		{
			const SoundRegistration& reg = s_SoundEffectsSoundRegistrations->at(i);
			reg.WriteSoundRegistrationToString(str, secondLevelUp);
		}
	}
}

STATIC bool SoundRegistration::OnUpdateReadInAllSoundsFromCommonSoundFolder()
{
	GameSettings* settings = TheGame::GetGameSettings();
	if (s_DataForWhileLoading == nullptr)
	{
		s_DataForWhileLoading = new SoundRegistrationSaveOfWhereAtInReadingData();
		if (settings != nullptr)
		{
			s_DataForWhileLoading->m_soundCommonFolder = settings->m_soundCommonFolder;
			s_DataForWhileLoading->m_soundCommonRootXMLNode = settings->m_soundCommonRootXMLNode;
			s_DataForWhileLoading->m_soundCommonSoundXMLNode = settings->m_soundCommonSoundXMLNode;
			s_DataForWhileLoading->m_soundCommonMusicXMLNode = settings->m_soundCommonMusicXMLNode;
			s_DataForWhileLoading->m_altSoundCommonSoundXMLNode = settings->m_altSoundCommonSoundXMLNode;
			s_DataForWhileLoading->m_altSoundCommonMusicXMLNode = settings->m_altSoundCommonMusicXMLNode;
			s_DataForWhileLoading->m_amountOfTimeToTakeToLoadSoundsPerFrame = settings->m_amountOfTimeToSpendLoadingSoundsPerFrame;
		}
	}

	if (!s_DataForWhileLoading->m_gotAllValidFiles)
	{
		s_DataForWhileLoading->m_files = FileUtils::EnumerateFilesInDirectory(
			s_DataForWhileLoading->m_soundCommonFolder, "*", true); //I expect this to take a bit.
		s_DataForWhileLoading->m_gotAllValidFiles = true;
		if (s_DataForWhileLoading->m_files.size() == 0)
		{
			return true;
		}
		return false;
	}
	if (s_DataForWhileLoading->m_amountOfTimeToTakeToLoadSoundsPerFrame <= 0)
	{
		return true;
	}

	int numberHaveReadIn = 0;
	while (
		(size_t)s_DataForWhileLoading->m_fileIndexWhereLastAt
		< s_DataForWhileLoading->m_files.size())
	{
		std::string fileStr = 
			s_DataForWhileLoading->m_files[s_DataForWhileLoading->m_fileIndexWhereLastAt];
		ReadSingleSoundRegistrationFromXMLFileLocation(settings, fileStr,
			s_DataForWhileLoading->m_soundCommonRootXMLNode, 
			s_DataForWhileLoading->m_soundCommonSoundXMLNode,
			s_DataForWhileLoading->m_altSoundCommonSoundXMLNode,
			s_DataForWhileLoading->m_soundCommonMusicXMLNode,
			s_DataForWhileLoading->m_altSoundCommonMusicXMLNode);

		s_DataForWhileLoading->m_fileIndexWhereLastAt++;
		numberHaveReadIn++;

		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() 
			- g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_DataForWhileLoading->m_amountOfTimeToTakeToLoadSoundsPerFrame)
		{
			return false;
		}
	}

	if ((size_t)s_DataForWhileLoading->m_fileIndexWhereLastAt
		>= s_DataForWhileLoading->m_files.size())
	{
		return true;
	}
	return false;
}

STATIC void SoundRegistration::ReadInAllSoundsFromCommonSoundFolderAllAtOnce()
{
	std::string soundCommonFolder = "Data/XML/Sound/";
	std::string soundCommonRootXMLNode = "Sounds";
	std::string soundCommonSoundXMLNode = "sound";
	std::string altSoundCommonSoundXMLNode = "soundeffect";
	std::string soundCommonMusicXMLNode = "music";
	std::string altSoundCommonMusicXMLNode = "soundtrack";
	GameSettings* settings = TheGame::GetGameSettings();

	if (settings != nullptr)
	{
		soundCommonFolder = settings->m_soundCommonFolder;
		soundCommonRootXMLNode = settings->m_soundCommonRootXMLNode;
		soundCommonSoundXMLNode = settings->m_soundCommonSoundXMLNode;
		soundCommonMusicXMLNode = settings->m_soundCommonMusicXMLNode;
		altSoundCommonSoundXMLNode = settings->m_altSoundCommonSoundXMLNode;
		altSoundCommonMusicXMLNode = settings->m_altSoundCommonMusicXMLNode;
	}

	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(soundCommonFolder, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadSingleSoundRegistrationFromXMLFileLocation(
			settings, fileStr, 
			soundCommonRootXMLNode, 
			soundCommonSoundXMLNode, altSoundCommonSoundXMLNode,
			soundCommonMusicXMLNode, altSoundCommonMusicXMLNode);
	}
}

STATIC void SoundRegistration::ReadSingleSoundRegistrationFromXMLFileLocation(
	GameSettings* settings,
	const std::string& fileStr,
	const std::string& soundCommonRootXMLNode,
	const std::string& soundCommonSoundXMLNode,
	const std::string& altSoundCommonSoundXMLNode,
	const std::string& soundCommonMusicXMLNode,
	const std::string& altSoundCommonMusicXMLNode)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, soundCommonRootXMLNode.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		std::string lowerCaseName = MakeLower(child.getName());
		if (soundCommonSoundXMLNode == lowerCaseName
			|| altSoundCommonSoundXMLNode == lowerCaseName)
		{
			SoundRegistration registration = SoundRegistration(child);
			if (s_AllSoundRegistrations == nullptr)
			{
				s_AllSoundRegistrations = new std::vector<SoundRegistration>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountSoundRegistrations;
				}
				s_AllSoundRegistrations->reserve(preallocationAmt);
			}
			if (s_SoundEffectsSoundRegistrations == nullptr)
			{
				s_SoundEffectsSoundRegistrations = new std::vector<SoundRegistration>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountSoundEffectRegistrations;
				}
				s_SoundEffectsSoundRegistrations->reserve(preallocationAmt);
			}
			s_AllSoundRegistrations->push_back(registration);
			s_SoundEffectsSoundRegistrations->push_back(registration);
		}
		else if (lowerCaseName == soundCommonMusicXMLNode
			|| lowerCaseName == altSoundCommonMusicXMLNode)
		{
			SoundRegistration registration = SoundRegistration(child);
			if (s_AllSoundRegistrations == nullptr)
			{
				s_AllSoundRegistrations = new std::vector<SoundRegistration>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountSoundRegistrations;
				}
				s_AllSoundRegistrations->reserve(preallocationAmt);
			}
			if (s_MusicSoundRegistrations == nullptr)
			{
				s_MusicSoundRegistrations = new std::vector<SoundRegistration>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountMusicRegistrations;
				}
				s_MusicSoundRegistrations->reserve(preallocationAmt);
			}
			s_AllSoundRegistrations->push_back(registration);
			s_MusicSoundRegistrations->push_back(registration);
		}
	}
}

STATIC void SoundRegistration::ClearAllSoundRegistrations()
{
	if (s_DataForWhileLoading != nullptr)
	{
		delete s_DataForWhileLoading;
		s_DataForWhileLoading = nullptr;
	}
	if (s_MusicSoundRegistrations != nullptr)
	{
		delete s_MusicSoundRegistrations;
		s_MusicSoundRegistrations = nullptr;
	}
	if (s_SoundEffectsSoundRegistrations != nullptr)
	{
		delete s_SoundEffectsSoundRegistrations;
		s_SoundEffectsSoundRegistrations = nullptr;
	}
	if (s_AllSoundRegistrations == nullptr)
	{
		return;
	}
	delete s_AllSoundRegistrations;
	s_AllSoundRegistrations = nullptr;
}


STATIC const SoundRegistration& SoundRegistration::GetSoundByName(const std::string& soundName)
{
	if (s_AllSoundRegistrations == nullptr)
	{
		return s_EmptyRegistration;
	}
	std::string lowerCaseName = MakeLower(soundName);
	for (size_t registrationIdx = 0; registrationIdx < s_AllSoundRegistrations->size(); registrationIdx++)
	{
		SoundRegistration& reg = s_AllSoundRegistrations->at(registrationIdx);
		if (reg.GetLowerCaseSoundName() == lowerCaseName)
		{
			return reg;
		}
	}
	return s_EmptyRegistration;
}

STATIC const SoundRegistration& SoundRegistration::GetSoundEffectByName(const std::string& soundEffectName)
{
	if (s_SoundEffectsSoundRegistrations == nullptr)
	{
		return s_EmptyRegistration;
	}
	std::string lowerCaseName = MakeLower(soundEffectName);
	for (size_t registrationIdx = 0; registrationIdx < s_SoundEffectsSoundRegistrations->size(); registrationIdx++)
	{
		SoundRegistration& reg = s_SoundEffectsSoundRegistrations->at(registrationIdx);
		if (reg.GetLowerCaseSoundName() == lowerCaseName)
		{
			return reg;
		}
	}
	return s_EmptyRegistration;
}

STATIC const SoundRegistration& SoundRegistration::GetMusicByName(const std::string& musicName)
{
	if (s_MusicSoundRegistrations == nullptr)
	{
		return s_EmptyRegistration;
	}
	std::string lowerCaseName = MakeLower(musicName);
	for (size_t registrationIdx = 0; registrationIdx < s_MusicSoundRegistrations->size(); registrationIdx++)
	{
		SoundRegistration& reg = s_MusicSoundRegistrations->at(registrationIdx);
		if (reg.GetLowerCaseSoundName() == lowerCaseName)
		{
			return reg;
		}
	}

	return s_EmptyRegistration;
}