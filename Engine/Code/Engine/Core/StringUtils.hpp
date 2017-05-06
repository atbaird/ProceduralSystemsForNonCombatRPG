#pragma once
//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>


//-----------------------------------------------------------------------------------------------
const std::string Stringf( const char* format, ... );
const std::string Stringf( const int& maxLength, const char* format, ... );

//-----------------------------------------------------------------------------------------------
const std::vector<std::string> ParseString(const std::string& str, const char& splitBy = ' ');
const std::string ReplaceCharInString(const std::string& original, const char& toReplace, const char& replaceBy);
const std::string ReplaceStringWithString(const std::string& original, const std::string& toReplace, const std::string& replaceBy);

//------------------------------------------------------------------------------------------------
const std::string StartFromBackAndChopOffAfterEncounterChar(const std::string& str, const char& chopOff);
const std::string StartFromFrontAndChopOffAfterEncounterChar(const std::string& str, const char& chopOff);

//------------------------------------------------------------------------------------------------
const std::string MakeLower(const std::string& str);
const std::string MakeUpper(const std::string& str);

//------------------------------------------------------------------------------------------------
const std::string ToString(const int& intVal);
const bool SimpleStrCmp(const std::string& str1, const std::string& str2);
const bool SimpleStrCmpLower(const std::string& str1, const std::string& str2);

//------------------------------------------------------------------------------------------------
void StringCopy(char* copyTo, const size_t& numOfCharsToCopyUpTo, const char* toCopy);

//------------------------------------------------------------------------------------------------
const bool VerifyDirectoryExists(const std::string& directoryPath);

const int GetCountOfCharInstanceInString(const std::string& str, const char car);
const bool DoesStringHaveThisChar(const std::string& str, const char car);
const bool DoesStringHaveThisCharAndStopIfSeeOtherChar(const std::string& str, const char car, const char stopHere);

std::string IterateInReverseUntilHitChar(const std::string& str, const char car, bool includeStoppingChar = false);
std::string IterateInReverseUntilHitNthChar(const std::string& str, const char car, int numToHit = 1);

int GetNthEncounterIdxOfCharInStr(const std::string& str, const char car, int numToHit = 1);
int GetNthEncounterIdxOfCharInStrReverse(const std::string& str, const char car, int numToHit = 1);
