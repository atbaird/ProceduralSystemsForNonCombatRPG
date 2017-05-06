#include "Engine/StringAnimals/HashedCaseInsensitiveString.hpp"
#include "Engine/Core/StringUtils.hpp"


HashedCaseInsensitiveString::HashedCaseInsensitiveString()
	: HashedCaseString()
{

}

HashedCaseInsensitiveString::HashedCaseInsensitiveString(const char* charArray)
	: HashedCaseString()
{
	if (charArray == nullptr)
	{
		return;
	}
	m_originalString = charArray;
	HashThatString(charArray);
}

HashedCaseInsensitiveString::HashedCaseInsensitiveString(const std::string& str)
	: HashedCaseString()
{
	m_originalString = str;
	HashThatString(str);
}

HashedCaseInsensitiveString::~HashedCaseInsensitiveString()
{
	HashedCaseString::~HashedCaseString();
}


const bool HashedCaseInsensitiveString::operator<(const HashedCaseInsensitiveString& other) const
{
	if (m_hash < other.m_hash)
	{
		return true;
	}
	else if (m_hash > other.m_hash)
	{
		return false;
	}
	return _stricmp(m_originalString.c_str(), other.m_originalString.c_str()) < 0;
}


void HashedCaseInsensitiveString::HashThatString(const std::string& str)
{
	m_hash = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		char ch = (char)tolower(str[i]);
		m_hash *= s_HashMultiplicationVal;
		m_hash += ch;
	}
}