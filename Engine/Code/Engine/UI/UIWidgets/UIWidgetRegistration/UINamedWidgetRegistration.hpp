#pragma once

#ifndef UINAMEDWIDGETREGISTRATION_HPP
#define UINAMEDWIDGETREGISTRATION_HPP
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIRegistrationCommonDefines.hpp"
#include <vector>

class UINamedWidgetRegistration
{
private:
protected:
	//Static Variables
	static std::vector<UINamedWidgetRegistration> s_UINamedWidgetRegistrations;
	static const char* s_NamedWidgetXMLName;

	//Variables
	UIWidgetRegistrationDataType m_data;
	std::string m_fileLocation = "";
	std::string m_NamedWidgetName = "";
	int m_idxInRegistration = 0;
	bool m_canUseNode = false;
public:
	//Constructors
	UINamedWidgetRegistration();
	UINamedWidgetRegistration(const std::string& name, const std::string& fileLocation, 
		int idxInRegistration, const UIWidgetRegistrationDataType& data);
	virtual ~UINamedWidgetRegistration();

	//Operations
	UIWidgetBase* SpawnWidgetFromData(const UIWidgetRegistrationDataPacket& packet);
	UIWidgetBase* SpawnWidgetFromData(const UIWidgetRegistrationDataPacket& packet, const XMLNode& node);

	//Operators
	const bool operator==(const UINamedWidgetRegistration& other) const;
	const bool operator==(const std::string& name) const;

	//Static functions
	static void ReadAllNamedWidgetsFromFolderLocation(const std::string& baseFolderLocation, bool readFromChildFolders);
	static void ReadNamedWidgetsFromXMLFile(const std::string& xmlFileLocation);
	static bool GetIsWidgetTypeAlreadyDefined(const std::string& widgetName);
	static int GetIdxOfWidgetTypeIfAlreadyDefined(const std::string& widgetName);
	static UIWidgetBase* SpawnWidgetFromNamedWidgetName(const std::string& widgetName, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnWidgetFromNamedWidgetName(const std::string& widgetName, const UIWidgetRegistrationDataPacket& packet, const XMLNode& node);
	static void ClearAllWidgetRegistrations();
};
#endif