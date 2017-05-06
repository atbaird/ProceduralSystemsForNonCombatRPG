#pragma once

#ifndef QUESTGENERATOR_HPP
#define QUESTGENERATOR_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Quest/QuestTemplate.hpp"
#include <vector>
#define STATIC
#include "Engine/Math/Vector2.hpp"
#include "Game/Quest/MapQuestImportantData.hpp"
#include "Game/Quest/TilePositionsByEntityName.hpp"

struct QuestGeneratorLoadingSave
{
	std::vector<std::string> m_files;
	std::string m_commonQuestXMLFolder = "Data/XML/Quests/";
	std::string m_commonQuestXMLRootNode = "Quests";
	double m_amountOfTimeToTakeToReadInQuestsPerFrame = 0.25;
	int m_currentFileIndex = 0;
	bool m_readInFiles = false;
};
struct GameSettings;
class MapFiles;
class QuestGenerator
{
private:
protected:
	//Static Variables
	static std::vector<QuestGenerator>* s_QuestGenerators;
	static QuestGeneratorLoadingSave* s_loadingSave;

	//Variables
	std::string m_QuestName = "";
	std::string m_QuestLowerCase = "";
	QuestTemplate m_Template;
public:
	//Constructors
	QuestGenerator();
	QuestGenerator(const XMLNode& node);
	QuestGenerator(const QuestGenerator& other);
	virtual ~QuestGenerator();

	//Setters
	void SetQuestName(const std::string& name);

	//Getters
	const std::string& GetQuestName() const;
	const std::string& GetQuestNameLowerCase() const;
	const QuestTemplate& GetQuestTemplate() const;
	QuestTemplate* GetEditableQuestTemplatePtr();

	//Operations
	Quest* SpawnQuest(const std::string& currentMapFileName, 
		const XMLNode& mapQuestNode, MapQuestImportantData& data,
		MapFiles* files);
	void WriteQuestGeneratorToString(std::string& str, int indentationAmt) const;

	//Static Functions
	static void WriteAllQuestGenerators(std::string& str, int indentationAmt);
	static void ClearAllGenerators();
	static void ReadInGeneratorsFromCommonQuestGeneratorsFolderAllAtOnce();
	static bool OnUpdateReadInGeneratorsFromCommonQuestGeneratorsFolder();
protected:
	static void ReadInQuestGeneratorFromFileLocation(GameSettings* settings,
		const std::string& fileStr,
		const std::string& commonQuestXMLRootNode);
public:
	static const QuestGenerator* GetQuestGeneratorByQuestName(const char* name);
	static const QuestGenerator* GetQuestGeneratorByQuestName(const std::string& name);
	static const QuestGenerator* GetQuestGeneratorByIndex(size_t index);
	static Quest* SpawnQuestByName(const std::string& name, 
		const XMLNode& mapQuestNode, const std::string& currentMapFileName = "",
		MapFiles* files = nullptr);
	static Quest* SpawnQuestByIndex(size_t index, 
		const XMLNode& mapQuestNode, const std::string& currentMapFileName = "",
		MapFiles* files = nullptr);
	static void ParseMapQuestNodeForImportantQuestData(const XMLNode& mapQuestNode, MapQuestImportantData& data);
protected:
	static void ParseMapQuestNodeForPositionalData(const XMLNode& mapFileChildNode,
		std::vector<TilePositionsByEntityName>& positions,
		const std::string& currentMapName);


};
#endif