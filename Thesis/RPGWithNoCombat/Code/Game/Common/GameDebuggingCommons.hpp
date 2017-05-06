#pragma once

#ifndef GAMEDEBUGGINGCOMMONS_HPP
#define GAMEDEBUGGINGCOMMONS_HPP
#include <string>
#define STATIC

enum SpecialCaseOperations;
class GameDebuggingCommons
{
private:
protected:
public:
	static void AddIndentation(std::string& str, int indentationAmt);
	static void WriteBoolOntoString(std::string& str, bool theBool, bool addNewLine = true);
	static void WriteSpecialCaseOperatorToString(std::string& str, SpecialCaseOperations operation, bool addNewLine = false);
};
#endif