#pragma once

#ifndef ORDERNAMEANDINDEXPAIR_HPP
#define ORDERNAMEANDINDEXPAIR_HPP
#include <string>

struct OrderNameAndIndexPair
{
public:
	//Variables
	std::string m_orderName = "";
	int m_index = -1;

	//Constructors
	OrderNameAndIndexPair(const std::string& name = "");

	void WriteOrderNameAndIndexPairToString(std::string& str, int indentationAmt) const;
};

#endif