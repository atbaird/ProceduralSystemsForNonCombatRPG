#pragma once

#ifndef ITEMGENERATOR_HPP
#define ITEMGENERATOR_HPP
#include "Game/GameEntities/Items/ItemTemplate.hpp"

struct ItemGeneratorLoadingData
{
	std::vector<std::string> m_files;
	std::string m_fileDirectory = "Data/XML/ItemGenerators/";
	std::string m_rootNodeName = "ItemGenerators";
	std::string m_NodeName = "ItemGenerator";
	int m_index = 0;
	double m_amountOfTimeToTakeReadingPerFrame = 0.25;
	bool m_readData = false;
};

class BaseGameEntity;
class Vector2;
struct GameSettings;
class ItemGenerator
{
private:
protected:
	//Static Variables
	static std::vector<ItemGenerator>* s_ItemGenerators;
	static ItemGeneratorLoadingData* s_loadingData;

	//Variables
	ItemTemplate m_itemTemplate;
	std::string m_lowerCaseItemName = "";
	bool m_valid = false;
public:
	//Constructors
	ItemGenerator();
	ItemGenerator(const XMLNode& node);
	virtual ~ItemGenerator();

	//Operations
	Item* SpawnItemAtPosition(const Vector2& position) const;

	//Getters
	const std::string& GetName();
	const std::string& GetLowerCaseName() const;
	const bool GetIsValid() const;
	void WriteGeneratorToString(std::string& str, int currentIndentationLevel = 0) const;

	//Static Functions
	static void WriteAllItemGeneratorsToString(std::string& str, int currentIndentationLevel = 0);
	static void ClearAllGenerators();
	static void ReadInGeneratorsFromCommonItemGeneratorsFolderAllAtOnce();
	static bool OnUpdateReadInGeneratorsFromCommonItemGeneratorsFolder();
protected:
	static void ReadInItemGeneratorsFromXMLFileLocation(GameSettings* settings,
		const std::string& fileStr,
		const std::string& commonItemRootNodeName,
		const std::string& commonItemNodeName);
public:
	static const ItemGenerator* GetItemGeneratorByItemName(const char* name);
	static const ItemGenerator* GetItemGeneratorByIndex(size_t index);
	static BaseGameEntity* SpawnItemByItemName(const char* name, const Vector2& position);
	static BaseGameEntity* SpawnItemByIndex(size_t index, const Vector2& position);
	static BaseGameEntity* SpawnItemAtRandomPosition(const Vector2& position);

};

#endif