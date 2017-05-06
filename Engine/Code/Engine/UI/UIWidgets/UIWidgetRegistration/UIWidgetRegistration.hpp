#pragma once

#ifndef UIWIDGETREGISTRATION_HPP
#define UIWIDGETREGISTRATION_HPP
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIRegistrationCommonDefines.hpp"
#include <vector>


class UIWidgetBase;

typedef UIWidgetBase* (SpawnWidgetFunc)(const UIWidgetRegistrationDataType& data, const UIWidgetRegistrationDataPacket& packet);
typedef UIWidgetBase* (SpawnWidgetWithoutDataFunc)(const UIWidgetRegistrationDataPacket& packet);


//This class is specifically for predefined Widget types.
//It is defined to have tighter constrictions than the Named Widget registrations.
class UIWidgetRegistration
{
private:
protected:
	//Static Variables
	static std::vector<UIWidgetRegistration> s_Registrations;

	//Variables
	std::string m_WidgetName = "";
	SpawnWidgetFunc* m_SpawnFunction = nullptr;
	SpawnWidgetWithoutDataFunc* m_SpawnWithoutDataFunction = nullptr;
public:
	//Constructors
	UIWidgetRegistration();
	UIWidgetRegistration(const std::string& name, SpawnWidgetFunc* func, SpawnWidgetWithoutDataFunc* func2);
	virtual ~UIWidgetRegistration();


	//Operators
	const bool operator==(const UIWidgetRegistration& other);
	const bool operator==(const std::string& name);

	//Static Functions
	static UIWidgetBase* SpawnWidgetOfType(const std::string& widgetName, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnWidgetOfType(const std::string& widgetName, const UIWidgetRegistrationDataType& data, const UIWidgetRegistrationDataPacket& packet);
	static bool GetIsWidgetTypeAlreadyDefined(const std::string& widgetName);
	static int GetIdxOfWidgetTypeIfAlreadyDefined(const std::string& widgetName);
	static void RegisterAllWidgetTypes();
	static void ClearAllWidgetTypes();
};
#endif