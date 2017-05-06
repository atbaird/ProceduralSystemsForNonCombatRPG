#pragma once

#ifndef HAIRGENERATOR_HPP
#define HAIRGENERATOR_HPP
#include "Game/GameEntities/Hair/HairTemplate.hpp"


struct HairGeneratorLoadingData
{
	std::vector<std::string> m_files;
	std::string m_fileDirectory = "";
	std::string m_rootNodeName = "";
	std::string m_NodeName = "";
	int m_index = 0;
	double m_amountOfTimeToTakeReadingHairPerFrame = 0.25;
	bool m_readData = false;
};

class BaseGameEntity;
struct GameSettings;
class HairGenerator
{
private:
protected:
	//Static Variables
	static std::vector<HairGenerator>* s_HairGenerators;
	static HairGeneratorLoadingData* s_loadingData;

	//Variables
	HairTemplate m_template;
	std::string m_lowerCaseHairName = "";
public:
	//Constructors
	HairGenerator();
	HairGenerator(const XMLNode& node);
	virtual ~HairGenerator();

	//Operations
	Hair* SpawnHair() const;
	Hair* SpawnHair(const std::vector<std::string>& colorChoiceLimits) const;

	//Setters

	//Getters
	void WriteGeneratorToString(std::string& str, int indentationLevel) const;
	const std::string& GetName();
	const std::string& GetLowerCaseName() const;


	//Static Functions
	static void WriteAllHairGeneratorsToString(std::string& str, int indentationLevel = 0);
	static void ClearAllGenerators();
	static void ReadInGeneratorsFromCommonHairGeneratorsFolderAllAtOnce();
	static bool OnUpdateReadInGeneratorsFromCommonHairGeneratorsFolder();
protected:
	static void ReadSingleHairGeneratorFromXMLFile(GameSettings* settings,
		const std::string fileStr,
		const std::string& commonHairRootNodeName,
		const std::string& commonHairNodeName);
public:
	static const HairGenerator* GetHairGeneratorByItemName(const char* name);
	static const HairGenerator* GetHairGeneratorByIndex(size_t index);
	static BaseGameEntity* SpawnHairByHairName(const char* name);
	static BaseGameEntity* SpawnHairByIndex(size_t index);
	static BaseGameEntity* SpawnHairByHairName(const char* name, const std::vector<std::string>& colorChoiceLimits);
	static BaseGameEntity* SpawnHairByIndex(size_t index, const std::vector<std::string>& colorChoiceLimits);
};

#endif