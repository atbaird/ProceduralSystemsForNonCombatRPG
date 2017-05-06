#pragma once

#ifndef HASHEDCASESTRING_HPP
#define HASHEDCASESTRING_HPP
#include <string>

struct HashedCaseString
{
protected:
	//Static variables
	static const unsigned int s_HashMultiplicationVal;

	//variables
	unsigned int m_hash = 0;
	std::string m_originalString = "";
public:
	//Constructors
	HashedCaseString();
	HashedCaseString(const char* charArray);
	HashedCaseString(const std::string& str);
	virtual ~HashedCaseString();

	//Setters
	void SetString(const std::string& str);

	//Getters
	const unsigned int GetHash() const;
	const std::string GetString() const;
	const bool CompareStrings(const std::string& str) const;

	//Operations
	const bool operator<(const HashedCaseString& other) const;
	const bool operator==(const HashedCaseString& other) const;
protected:
	virtual void HashThatString(const std::string& str);
};

#include <vector>
//external operations
const std::vector<HashedCaseString> ParseHashedString(const HashedCaseString& str, const char& splitBy = ' ');

#endif