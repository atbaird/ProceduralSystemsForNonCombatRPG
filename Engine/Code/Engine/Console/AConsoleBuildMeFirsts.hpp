#pragma once

#ifndef ACONSOLEBUILDMEFIRSTS_HPP
#define ACONSOLEBUILDMEFIRSTS_HPP
#include "Engine/StringAnimals/HashedCaseInsensitiveString.hpp"
#include <map>

class Command;
//-----------------------------------------------------
typedef void(*console_command_cb)(Command&);

class ConsoleGlobals
{

public:
	static std::map<HashedCaseInsensitiveString, console_command_cb>* s_commands;
	static std::map<HashedCaseInsensitiveString, console_command_cb>* s_helpMessages;
};
#endif