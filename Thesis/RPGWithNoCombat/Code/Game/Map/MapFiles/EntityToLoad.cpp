#include "Game/Map/MapFiles/EntityToLoad.hpp"
#include "Engine/Math/ExtendedMath.hpp"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Constructors
EntityToLoad::EntityToLoad()
{

}

EntityToLoad::EntityToLoad(const XMLNode& node)
	: m_dataNode(node)
{

}

EntityToLoad::~EntityToLoad()
{

}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Setters
void EntityToLoad::SetDataNode(const XMLNode& node)
{
	m_dataNode = node;
}

void EntityToLoad::SetAssociatedQuest(const std::string& questTitle)
{
	m_AssociatedQuest = questTitle;
	if (m_AssociatedQuest == "")
	{
		m_associatedToAQuest = false;
	}
	else
	{
		m_associatedToAQuest = true;
	}
}

void EntityToLoad::SetPercentageChanceToSpawn(float percentage)
{
	m_percentageChanceToSpawn = percentage;
}

void EntityToLoad::SetSpawnedOnce(bool spawnedOnce)
{
	m_SpawnedOnce = spawnedOnce;
}

void EntityToLoad::SetOnlyEverSpawnOnce(bool onlyEverSpawnOnce)
{
	m_OnlyEverSpawnOnce = onlyEverSpawnOnce;
}

void EntityToLoad::SetDeleteAfterPurposeFulfilled(bool deleteAfterPurposeFulfilled)
{
	m_DeleteAfterPurposeFulfilled = deleteAfterPurposeFulfilled;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Getters
const XMLNode& EntityToLoad::GetDataNode() const
{
	return m_dataNode;
}

const std::string& EntityToLoad::GetAssociatedQuestTitle() const
{
	return m_AssociatedQuest;
}

float EntityToLoad::GetPercentageChanceToSpawn() const
{
	return m_percentageChanceToSpawn;
}

bool EntityToLoad::GetIsAssociatedToAQuest() const
{
	return m_associatedToAQuest;
}


bool EntityToLoad::GetSpawnedOnce() const
{
	return m_SpawnedOnce;
}

bool EntityToLoad::GetOnlyEverSpawnOnce() const
{
	return m_OnlyEverSpawnOnce;
}

bool EntityToLoad::GetShouldSpawn() const
{
	bool spawnOnceTest = m_OnlyEverSpawnOnce == false
		|| (m_OnlyEverSpawnOnce && m_SpawnedOnce == false);
	bool percentageMet = true;
	
	if (m_percentageChanceToSpawn <= 0.f)
	{
		percentageMet = false;
	}
	else if (m_percentageChanceToSpawn < 1.f)
	{
		float ranChance = GetRandomFloatZeroToOne();
		if (ranChance <= m_percentageChanceToSpawn)
		{
			percentageMet = true;
		}
		else
		{
			percentageMet = false;
		}
	}

	bool combinedTest = spawnOnceTest && percentageMet;
	return combinedTest;
}


bool EntityToLoad::GetDeleteAfterPurposeFulfilled() const
{
	return m_DeleteAfterPurposeFulfilled;
}