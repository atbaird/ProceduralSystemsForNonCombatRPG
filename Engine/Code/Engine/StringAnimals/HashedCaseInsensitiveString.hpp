#pragma once

#ifndef HASHEDCASEINSENSITIVESTRING_HPP
#define HASHEDCASEINSENSITIVESTRING_HPP
#include "Engine/StringAnimals/HashedCaseString.hpp"

class HashedCaseInsensitiveString : public HashedCaseString //Same as HashedCaseString, but uses lower case chars only.
{
private:
protected:
public:
	HashedCaseInsensitiveString();
	HashedCaseInsensitiveString(const char* charArray);
	HashedCaseInsensitiveString(const std::string& str);
	virtual ~HashedCaseInsensitiveString();

	const bool operator<(const HashedCaseInsensitiveString& other) const;

protected:
	virtual void HashThatString(const std::string& str) override;
};
#endif