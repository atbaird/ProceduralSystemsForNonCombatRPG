#include "Engine/Console/AConsoleBuildMeFirsts.hpp"



std::map<HashedCaseInsensitiveString, console_command_cb>* ConsoleGlobals::s_commands = nullptr;
std::map<HashedCaseInsensitiveString, console_command_cb>* ConsoleGlobals::s_helpMessages = nullptr;