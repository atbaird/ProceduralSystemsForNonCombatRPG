#pragma once

#ifndef HAIRCOLORNAMEVALUEPAIR_HPP
#define HAIRCOLORNAMEVALUEPAIR_HPP
#include <string>

struct HairColorNameValuePair
{
private:
protected:
public:
	//Variables
	std::string m_HairName = "";
	std::string m_ColorName = "";
	std::string m_LowerCaseColorName = "";
	int m_ColorIndex = 0;

public:
	//Constructors
	HairColorNameValuePair();
	virtual ~HairColorNameValuePair();
};
#endif