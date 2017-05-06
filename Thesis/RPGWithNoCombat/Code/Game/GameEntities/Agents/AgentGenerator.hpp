#pragma once

#ifndef AGENTGENERATOR_HPP
#define AGENTGENERATOR_HPP
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/GameEntities/Agents/AgentTemplate.hpp"

struct AgentGeneratorLoadingData
{
	std::vector<std::string> m_files;
	std::string m_fileDirectory = "Data/XML/AgentGenerators/";
	std::string m_rootNodeName = "AgentGenerators";
	std::string m_NodeName = "AgentGenerator";
	int m_index = 0;
	double m_amountOfTimeToTakePerFrame = 0.25;
	bool m_readData = false;
	bool m_loadingPhase = true;
	bool m_readInAgentStats = false;
};

class AgentTemplate;
struct GameSettings;
struct MapAgentSpecifications;

class AgentGenerator
{
private:
protected:
	//Variables
	AgentTemplate m_template;
	std::string m_generatorsToInheritFrom = "";
	bool m_checkForInheritance = false;

	//Static Variables
	static std::vector<AgentGenerator>* s_AgentGenerators;
	static AgentGeneratorLoadingData* s_loadingData;
public:
	//Constructors
	AgentGenerator();
	AgentGenerator(const XMLNode& node);
	AgentGenerator(const AgentGenerator& other);
	virtual ~AgentGenerator();

protected:
	void ReadXMLNodeAttributes(const XMLNode& node);
	void ReadXMLNodeChildren(const XMLNode& node);
	void GetHairColorLimitsOutOfXML(const XMLNode& node);
	void GetSkinColorsOutOfXML(const XMLNode& node);
	void GetHairsOutOfXML(const XMLNode& node);
	void GetDialogueOutOfXML(const XMLNode& child);
	void GetAnimationsOutOfXML(const XMLNode& child);
	void GetNPCBehaviorsOutOfXML(const XMLNode& child);
	void GetClothingOutOfXML(const XMLNode& child);
	void GetAgentAttributesOutOfXML(const XMLNode& child);

public:
	void InheritPropertiesFromGenerator(AgentGenerator* generator);
	void ParseStringOfGeneratorsToInheritFrom();
	void WriteAgentGeneratorToString(std::string& str, int indentation = 0) const;

	//Getters
	const char* GetAgentName() const;
	std::string GetGeneratorsToInheritFrom() const;
	bool GetCheckForInheritance() const;


	//Spawn Objects
	BaseGameEntity* SpawnPlayerType(const Vector2& position, const Map* map = nullptr) const;
	BaseGameEntity* SpawnNPCType(const Vector2& position, const Map* map = nullptr) const;
	BaseGameEntity* SpawnNPCType(const MapAgentSpecifications& specifications, const Map* map = nullptr) const;

	//Static methods
	static void WriteAllAgentGeneratorsToString(std::string& str, int indentation = 0);
	static void RegisterDefaultHair(std::string hairName);
	static void ClearAllGenerators();
	static void ReadInGeneratorsFromCommonAgentGeneratorsFolderAllAtOnce();
	static bool OnUpdateReadInGeneratorsFromCommonAgentGeneratorsFolder();
protected:
	static void ReadAgentGeneratorFromSingleXMLFileLocation(GameSettings* settings,
		const std::string& fileStr,
		const std::string& agentGeneratorsRootNodeName,
		const std::string& agentGeneratorsNodeName);
public:
	static const AgentGenerator* GetAgentGeneratorByAgentName(const char* name);
	static const AgentGenerator* GetAgentGeneratorByIndex(size_t index);
	static AgentGenerator* GetEditableAgentGeneratorByAgentName(const char* name);
	static AgentGenerator* GetEditableAgentGeneratorByIndex(size_t index);
	static BaseGameEntity* SpawnPlayerInAgentGeneratorByAgentName(const char* name, const Vector2& position,
		const Map* map = nullptr);
	static BaseGameEntity* SpawnPlayerInAgentGeneratorByIndex(size_t index, const Vector2& position,
		const Map* map = nullptr);
	static BaseGameEntity* SpawnNPCInAgentGeneratorByAgentName(const char* name, const Vector2& position,
		const Map* map = nullptr);
	static BaseGameEntity* SpawnNPCInAgentGeneratorByIndex(size_t index, const Vector2& position,
		const Map* map = nullptr);
	static BaseGameEntity* SpawnNPCInAgentGeneratorRandom(const Vector2& position,
		const Map* map = nullptr);
	static void SpawnNPCsInAgentGeneratorsBasedOffMapAgentSpecifications(const MapAgentSpecifications& specifications,
		std::vector<BaseGameEntity*>& agentsOut,
		const Map* map = nullptr);
	static void SpawnNPCsInAgentGeneratorsBasedOffMapAgentSpecifications(
		const std::vector<MapAgentSpecifications>& specifications, std::vector<BaseGameEntity*>& agentsOut,
		const Map* map = nullptr);
};
#endif