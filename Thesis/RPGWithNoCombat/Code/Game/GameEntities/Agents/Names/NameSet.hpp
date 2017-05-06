#pragma once

#ifndef NAMESET_HPP
#define NAMESET_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include <string>
#include <vector>

class NameSet
{
private:
protected:
	std::string m_SetKeyName = "";
	std::vector<std::string> m_names;
	float m_PercentageNeedToBeHigherThanToReceiveOptionalName = 0.5f;
	mutable int m_indexLastUsed = -1;
	bool m_optional = false;
public:
	//Constructors
	NameSet();
	NameSet(const XMLNode& node);
	virtual ~NameSet();

	//Setters
	void AddName(const std::string& name);
	void SetKeyName(const std::string& keyName);
	void SetOptional(bool optional);
	void SetPercentageNeedToBeHigherThanToReceiveOptionalName(float percentage);

	//Getters
	const std::vector<std::string> GetNamesWithoutLastUsedIfCan(std::vector<int>& indexes) const;
	void GetNamesWithoutLastUsedIfCan(std::vector<std::string>& names, std::vector<int>& indexes) const;
	const std::string& GetKeyName() const;
	const std::string GenerateRandomName() const;
	const bool GetOptional() const;
	const float GetPercentageNeedToBeHigherThanToReceiveOptionalName() const;
	void WriteNameSetToString(std::string& str, int indentationAmt) const;
};

#endif