#pragma once

#ifndef FEATUREGENERATOR_HPP
#define FEATUREGENERATOR_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/GameEntities/Features/FeatureTemplate.hpp"

struct FeatureGeneratorLoadingData
{
	std::vector<std::string> m_files;
	std::string m_fileDirectory = "Data/XML/FeatureGenerators/";
	std::string m_rootNodeName = "FeatureGenerators";
	std::string m_NodeName = "FeatureGenerator";
	int m_index = 0;
	double m_amountOfTimeToTakeReadingPerFrame = 0.25;
	bool m_readData = false;
};

class Map;
struct GameSettings;
class FeatureGenerator
{
private:
protected:
	//Static Variables
	static std::vector<FeatureGenerator>* s_FeatureGenerators;
	static FeatureGeneratorLoadingData* s_loadingData;

	//Variables
	FeatureTemplate m_template;
	std::string m_FeatureGenName = "";
public:
	//Constructors
	FeatureGenerator();
	FeatureGenerator(const XMLNode& node);
	FeatureGenerator(const FeatureGenerator& other);
	virtual ~FeatureGenerator();
protected:
	void GetAnimationsOutOfXML(const XMLNode& child);
	void GetFeatureBehaviorsOutOfXML(const XMLNode& child);
public:

	//Setters

	//Getters
	void WriteGeneratorToString(std::string& str, int indentationLvl = 0) const;

	//Operations
	BaseGameEntity* SpawnFeature();
	BaseGameEntity* SpawnFeature(const XMLNode& node);

	//Static Functions
	static void WriteAllFeatureGeneratorsToString(std::string& str, int indentationLvl = 0);
	static void LoadAllFeaturesFromXMLAllAtOnce();
	static bool OnUpdateLoadAllFeaturesFromXML();
protected:
	static void LoadSingleFeatureGeneratorFromXMLFileLocation(GameSettings* settings,
		const std::string& fileStr,
		const std::string& featureGeneratorsRootNodeName,
		const std::string& featureGeneratorsNodeName);

public:
	static void ClearAllGenerators();
	static BaseGameEntity* SpawnFeatureByFeatureName(const char* name, const Vector2& position, 
		const Map* map);
	static BaseGameEntity* SpawnFeatureByIndex(size_t index, const Vector2& position, 
		const Map* map);
	static BaseGameEntity* SpawnFeatureByFeatureName(const char* name, const Vector2& position, const XMLNode& node,
		const Map* map);
	static BaseGameEntity* SpawnFeatureByIndex(size_t index, const Vector2& position, const XMLNode& node, 
		const Map* map);

};

#endif