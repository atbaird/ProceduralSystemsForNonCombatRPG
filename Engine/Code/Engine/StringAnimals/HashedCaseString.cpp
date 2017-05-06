#include "Engine/StringAnimals/HashedCaseString.hpp"

const unsigned int HashedCaseString::s_HashMultiplicationVal = 31; //professor used 31 //Use only prime numbers for this.

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Constructors
HashedCaseString::HashedCaseString()
{

}

HashedCaseString::HashedCaseString(const char* charArray)
{
	if (charArray == nullptr)
	{
		return;
	}
	m_originalString = std::string(charArray);
	HashThatString(m_originalString);
}

HashedCaseString::HashedCaseString(const std::string& str)
	: m_originalString(str)
{
	HashThatString(str);
}

HashedCaseString::~HashedCaseString()
{

}


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Setters
void HashedCaseString::SetString(const std::string& str)
{
	m_originalString = str;
	HashThatString(m_originalString);
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Getters
const unsigned int HashedCaseString::GetHash() const
{
	return m_hash;
}

const std::string HashedCaseString::GetString() const
{
	return m_originalString;
}

const bool HashedCaseString::CompareStrings(const std::string& str) const
{
	return _stricmp(m_originalString.c_str(), str.c_str()) == 0;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Operations
const bool HashedCaseString::operator<(const HashedCaseString& other) const
{
	if (m_hash < other.m_hash)
	{
		return true;
	}
	else if (m_hash > other.m_hash)
	{
		return false;
	}
	return m_originalString < other.m_originalString;
}

const bool HashedCaseString::operator==(const HashedCaseString& other) const
{
	if (m_hash != other.m_hash)
	{
		return false;
	}
	if (other.m_originalString != m_originalString)
	{
		return false;
	}
	return true;
}

void HashedCaseString::HashThatString(const std::string& str)
{
	m_hash = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		char car = str[i];
		m_hash *= s_HashMultiplicationVal;
		m_hash += car;
	}
}

//External Operations

#include "Engine/Core/StringUtils.hpp"
const std::vector<HashedCaseString> ParseHashedString(const HashedCaseString& str, const char& splitBy)
{
	std::vector<std::string> vars = ParseString(str.GetString(), splitBy);
	std::vector<HashedCaseString> out_vars;
	for (size_t i = 0; i < vars.size(); i++)
	{
		out_vars.push_back(vars[i]);
	}
	return out_vars;
}