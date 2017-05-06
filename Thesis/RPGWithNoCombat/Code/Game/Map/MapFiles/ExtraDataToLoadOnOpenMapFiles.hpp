#pragma once

#ifndef EXTRADATATOLOADONOPENMAPFILES_HPP
#define EXTRADATATOLOADONOPENMAPFILES_HPP
#include "Game/Map/MapFiles/EntityToLoad.hpp"
#include <vector>

struct ExtraDataToLoadOnOpenMapFiles
{
private:
protected:
	//variables
	std::string m_AssociatedMap = "";
	std::vector<EntityToLoad> m_ExtraEntities;
public:
	//Constructors
	ExtraDataToLoadOnOpenMapFiles(const std::string& AssociatedMap = "");
	virtual ~ExtraDataToLoadOnOpenMapFiles();

	//Setters
	void SetAssociatedMap(const std::string& associatedMap);
	void SetExtraEntities(const std::vector<EntityToLoad>& entities);
	void AddExtraEntitiy(const EntityToLoad& entity);

	//Getters
	const std::string& GetAssociatedMapName() const;
	size_t GetNumberOfExtraEntities() const;
	const EntityToLoad GetExtraEntityByIdx(size_t idx, bool& found) const;
	const std::vector<EntityToLoad>& GetExtraEntities() const;
	const std::vector<EntityToLoad>* GetExtraEntitiesPtr() const;
	std::vector<EntityToLoad>* GetExtraEntitiesEditablePtr();

};
#endif