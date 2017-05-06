#pragma once

#ifndef NAMEDPROPERTIESTOSTRING_HPP
#define NAMEDPROPERTIESTOSTRING_HPP
#include "Engine/EventSystem/NamedProperties.hpp"
#define STATIC

class BaseGameEntity;
class NamedPropertiesToString
{
private:
protected:
	static void PullOutValueFromProperty(const NamedPropertyBase* value, std::string& str, int indentationLevel);
	static void PullOutEntityValues(BaseGameEntity* entity, std::string& str, int indentationLevel);
public:
	static void ConvertAllKnownNamedPropertyValuesToString(const NamedProperties* properties, 
		std::string& str, int indentationLevel);
};

#endif