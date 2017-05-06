#include "Game/Map/MapFiles/ExtraDataToLoadOnOpenMapFiles.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Constructors
ExtraDataToLoadOnOpenMapFiles::ExtraDataToLoadOnOpenMapFiles(const std::string& AssociatedMap)
	: m_AssociatedMap(AssociatedMap)
{

}

ExtraDataToLoadOnOpenMapFiles::~ExtraDataToLoadOnOpenMapFiles()
{
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Setters
void ExtraDataToLoadOnOpenMapFiles::SetAssociatedMap(const std::string& associatedMap)
{
	m_AssociatedMap = associatedMap;
}

void ExtraDataToLoadOnOpenMapFiles::SetExtraEntities(const std::vector<EntityToLoad>& entities)
{
	m_ExtraEntities = entities;
}

void ExtraDataToLoadOnOpenMapFiles::AddExtraEntitiy(const EntityToLoad& entity)
{
	m_ExtraEntities.push_back(entity);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Getters
const std::string& ExtraDataToLoadOnOpenMapFiles::GetAssociatedMapName() const
{
	return m_AssociatedMap;
}

size_t ExtraDataToLoadOnOpenMapFiles::GetNumberOfExtraEntities() const
{
	return m_ExtraEntities.size();
}
const EntityToLoad ExtraDataToLoadOnOpenMapFiles::GetExtraEntityByIdx(size_t idx, bool& found) const
{
	found = false;
	if (idx >= m_ExtraEntities.size())
	{
		return EntityToLoad();
	}
	found = true;
	return m_ExtraEntities[idx];
}

const std::vector<EntityToLoad>& ExtraDataToLoadOnOpenMapFiles::GetExtraEntities() const
{
	return m_ExtraEntities;
}

const std::vector<EntityToLoad>* ExtraDataToLoadOnOpenMapFiles::GetExtraEntitiesPtr() const
{
	return &m_ExtraEntities;
}

std::vector<EntityToLoad>* ExtraDataToLoadOnOpenMapFiles::GetExtraEntitiesEditablePtr()
{
	return &m_ExtraEntities;
}