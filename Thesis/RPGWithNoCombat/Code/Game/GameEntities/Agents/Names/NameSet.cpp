#include "Game/GameEntities/Agents/Names/NameSet.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Constructors
NameSet::NameSet()
{

}

NameSet::NameSet(const XMLNode& node)
{
	std::string setName = node.getName();
	m_SetKeyName = MakeLower(setName);

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string name = MakeLower(attr.lpszName);
		if (name == "names")
		{
			std::string val = std::string(attr.lpszValue);
			val = ReplaceCharInString(val, ',', ' ');
			std::vector<std::string> parsed = ParseString(val, ' ');
			m_names.insert(m_names.end(), parsed.begin(), parsed.end());
		}
		else if (name == "optional")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "t" || val == "true")
			{
				m_optional = true;
			}
		}
		else if (name == "optionalpercentage")
		{
			float percentage = (float)atof(attr.lpszValue);
			if (percentage < 0.f)
			{
				m_PercentageNeedToBeHigherThanToReceiveOptionalName = 0.f;
			}
			else if (percentage > 1.f)
			{
				m_PercentageNeedToBeHigherThanToReceiveOptionalName = 1.f;
			}
			else
			{
				m_PercentageNeedToBeHigherThanToReceiveOptionalName = percentage;
			}
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string orgName = std::string(child.getName());
		for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = child.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "name")
			{
				orgName = std::string(attr.lpszValue);
			}
		}
		m_names.push_back(orgName);
	}
}

NameSet::~NameSet()
{

}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Setters
void NameSet::AddName(const std::string& name)
{
	m_names.push_back(name);
}

void NameSet::SetKeyName(const std::string& keyName)
{
	m_SetKeyName = MakeLower(keyName);
}

void NameSet::SetOptional(bool optional)
{
	m_optional = optional;
}

void NameSet::SetPercentageNeedToBeHigherThanToReceiveOptionalName(float percentage)
{
	m_PercentageNeedToBeHigherThanToReceiveOptionalName = percentage;
}


//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Getters
const std::vector<std::string> NameSet::GetNamesWithoutLastUsedIfCan(std::vector<int>& indexes) const
{
	std::vector<std::string> names;
	GetNamesWithoutLastUsedIfCan(names, indexes);

	return names;
}

void NameSet::GetNamesWithoutLastUsedIfCan(std::vector<std::string>& names, std::vector<int>& indexes) const
{
	names.clear();
	indexes.clear();
	if (m_indexLastUsed == -1)
	{
		for (size_t i = 0; i < m_names.size(); i++)
		{
			indexes.push_back(i);
		}
		names.insert(names.end(), m_names.begin(), m_names.end());
		return;
	}
	for (size_t i = 0; i < m_names.size(); i++)
	{
		if (m_indexLastUsed == (int)i)
		{
			continue;
		}
		names.push_back(m_names[i]);
		indexes.push_back(i);
	}
	if (names.size() == 0)
	{
		indexes.push_back(m_indexLastUsed);
		names.push_back(m_names[m_indexLastUsed]);
	}
}

const std::string& NameSet::GetKeyName() const
{
	return m_SetKeyName;
}

const std::string NameSet::GenerateRandomName() const
{
	std::vector<int> indexes;
	std::vector<std::string> names;
	NameSet::GetNamesWithoutLastUsedIfCan(names, indexes);
	if (names.size() == 0)
	{
		return "";
	}
	int idx = GetRandomIntLessThan(names.size());
	m_indexLastUsed = indexes[idx];
	return names[idx];
}

const bool NameSet::GetOptional() const
{
	return m_optional;
}

const float NameSet::GetPercentageNeedToBeHigherThanToReceiveOptionalName() const
{
	return m_PercentageNeedToBeHigherThanToReceiveOptionalName;
}

void NameSet::WriteNameSetToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Set Name: " + m_SetKeyName + "\n";
	int nextLevel = indentationAmt + 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Set Values: \n";
	for (size_t i = 0; i < m_names.size(); i++)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += m_names[i] + "\n";
	}
}