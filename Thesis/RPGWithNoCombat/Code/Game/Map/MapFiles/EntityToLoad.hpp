#pragma once

#ifndef ENTITYTOLOAD_HPP
#define ENTITYTOLOAD_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include <string>

struct XMLNode;
class EntityToLoad
{
private:
protected:
	//Variables
	XMLNode m_dataNode;
	std::string m_AssociatedQuest = "";
	float m_percentageChanceToSpawn = 1.f;
	bool m_associatedToAQuest = false;
	bool m_SpawnedOnce = false;
	bool m_OnlyEverSpawnOnce = true;
	bool m_DeleteAfterPurposeFulfilled = false;
public:
	//Constructors
	EntityToLoad();
	EntityToLoad(const XMLNode& node);
	virtual ~EntityToLoad();

	//Setters
	void SetDataNode(const XMLNode& node);
	void SetAssociatedQuest(const std::string& questTitle);
	void SetPercentageChanceToSpawn(float percentage);
	void SetSpawnedOnce(bool spawnedOnce);
	void SetOnlyEverSpawnOnce(bool onlyEverSpawnOnce);
	void SetDeleteAfterPurposeFulfilled(bool deleteAfterPurposeFulfilled);

	//Getters
	const XMLNode& GetDataNode() const;
	const std::string& GetAssociatedQuestTitle() const;
	float GetPercentageChanceToSpawn() const;
	bool GetIsAssociatedToAQuest() const;
	bool GetSpawnedOnce() const;
	bool GetOnlyEverSpawnOnce() const;
	bool GetShouldSpawn() const;
	bool GetDeleteAfterPurposeFulfilled() const;
};
#endif