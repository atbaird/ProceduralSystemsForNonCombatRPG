#pragma once

#ifndef SOUNDREGISTRATION_HPP
#define SOUNDREGISTRATION_HPP
#include <string>
#include <vector>

#define STATIC
typedef unsigned int SoundID;

struct SoundRegistrationSaveOfWhereAtInReadingData
{
	std::vector<std::string> m_files;
	std::string m_soundCommonFolder = "Data/XML/Sound/";
	std::string m_soundCommonRootXMLNode = "Sounds";
	std::string m_soundCommonSoundXMLNode = "sound";
	std::string m_altSoundCommonSoundXMLNode = "soundeffect";
	std::string m_soundCommonMusicXMLNode = "music";
	std::string m_altSoundCommonMusicXMLNode = "soundtrack";
	int m_fileIndexWhereLastAt = 0;
	double m_amountOfTimeToTakeToLoadSoundsPerFrame = 0.25;
	bool m_gotAllValidFiles = false;
};

struct GameSettings;
struct XMLNode;
class SoundRegistration
{
private:
protected:
	//Static Variables
	static SoundRegistrationSaveOfWhereAtInReadingData* s_DataForWhileLoading;
	static std::vector<SoundRegistration>* s_AllSoundRegistrations;
	static std::vector<SoundRegistration>* s_SoundEffectsSoundRegistrations;
	static std::vector<SoundRegistration>* s_MusicSoundRegistrations;
public:
	static const SoundRegistration s_EmptyRegistration;
protected:

	//Variables
	std::string m_SoundName = "";
	std::string m_LowerCaseSoundName = "";
	std::string m_SoundFileLocation = "";
	float m_volume = 1.f;
	SoundID m_SoundID;
	SoundID m_LoopingSoundID;
	bool m_SoundLoops = false;
	bool m_SuccessfullyLoaded = false;
public:
	//Constructors
	SoundRegistration();
	SoundRegistration(const std::string& soundName, const std::string& soundFileLocation);
	SoundRegistration(const XMLNode& node);
	SoundRegistration(const SoundRegistration& other);
	virtual ~SoundRegistration();
protected:
	void ReadXMLAttributes(const XMLNode& node);

public:

	//Setters
	void SetSoundName(const std::string& name);
	void SetSoundFileLocation(const std::string& soundLocation);
	void SetSoundID(SoundID soundID);

	//Getters
	const std::string& GetSoundName() const;
	const std::string& GetLowerCaseSoundName() const;
	const std::string& GetSoundFileLocation() const;
	float GetVolume() const;
	SoundID GetSoundID() const;
	SoundID GetLoopingSoundID() const;
	bool GetSuccessfullyLoaded() const;

	//Operations
	bool TryToLoadSoundID();

	//Operators
	const bool operator==(const SoundRegistration& other) const;
	void WriteSoundRegistrationToString(std::string& str, int indentationAmt) const;

	//Static Functions
	static void WriteAllSoundRegistrationsToString(std::string& str, int indentationAmt);
	static bool OnUpdateReadInAllSoundsFromCommonSoundFolder();
	static void ReadInAllSoundsFromCommonSoundFolderAllAtOnce();
	static void ReadSingleSoundRegistrationFromXMLFileLocation(
		GameSettings* settings,
		const std::string& fileStr,
		const std::string& soundCommonRootXMLNode,
		const std::string& soundCommonSoundXMLNode,
		const std::string& altSoundCommonSoundXMLNode,
		const std::string& soundCommonMusicXMLNode,
		const std::string& altSoundCommonMusicXMLNode);
	static void ClearAllSoundRegistrations();
	static const SoundRegistration& GetSoundByName(const std::string& soundName);
	static const SoundRegistration& GetSoundEffectByName(const std::string& soundEffectName);
	static const SoundRegistration& GetMusicByName(const std::string& musicName);
};
#endif