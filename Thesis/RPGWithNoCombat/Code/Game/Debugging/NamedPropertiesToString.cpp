#include "Game/Debugging/NamedPropertiesToString.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

STATIC void NamedPropertiesToString::PullOutValueFromProperty(const NamedPropertyBase* value, std::string& str,
	int indentationLevel)
{
	if (value == nullptr)
	{
		return;
	}
	const TypedNamedProperty<float>* float_test = 
		dynamic_cast<const TypedNamedProperty<float>*>(value);
	if (float_test != nullptr)
	{
		str += std::to_string(float_test->m_data);
		return;
	}

	const TypedNamedProperty<int>* int_test = 
		dynamic_cast<const TypedNamedProperty<int>*>(value);
	if (int_test != nullptr)
	{
		str += std::to_string(int_test->m_data);
		return;
	}
	const TypedNamedProperty<bool>* bool_test = 
		dynamic_cast<const TypedNamedProperty<bool>*>(value);
	if (bool_test != nullptr)
	{
		GameDebuggingCommons::WriteBoolOntoString(str, bool_test->m_data, false);
		return;
	}

	const TypedNamedProperty<std::string>* text_test = 
		dynamic_cast<const TypedNamedProperty<std::string>*>(value);
	if (text_test != nullptr)
	{
		str += text_test->m_data;

		return;
	}


	const TypedNamedProperty<BaseGameEntity*>* entity_test = 
		dynamic_cast<const TypedNamedProperty<BaseGameEntity*>*>(value);
	if (entity_test != nullptr)
	{
		PullOutEntityValues(entity_test->m_data, str, indentationLevel);
		return;
	}
}

STATIC void NamedPropertiesToString::PullOutEntityValues(BaseGameEntity* entity, std::string& str,
	int indentationLevel)
{
	if (entity == nullptr)
	{
		return;
	}
	entity->WriteOutEntityImportantDataToStr(str, indentationLevel);
}

STATIC void NamedPropertiesToString::ConvertAllKnownNamedPropertyValuesToString(
	const NamedProperties* properties, std::string& str, int indentationLevel)
{
	if (properties == nullptr)
	{
		return;
	}
	const std::map<PropertyName, NamedPropertyBase*>& props = properties->GetAllProperties();

	int nextLevel = indentationLevel + 3;
	for (std::map<PropertyName, NamedPropertyBase*>::const_iterator it = props.begin();
		it != props.end(); it++)
	{
		const PropertyName& variableName = it->first;
		const NamedPropertyBase* value = it->second;
		std::string value_str = "";
		PullOutValueFromProperty(value, value_str, nextLevel);
		str += "\n";
		GameDebuggingCommons::AddIndentation(str, indentationLevel);
		str += variableName + ": " + value_str;
	}
	str += "\n";
}