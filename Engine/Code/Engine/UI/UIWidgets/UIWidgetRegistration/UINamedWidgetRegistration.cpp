#include "Engine/UI/UIWidgets/UIWidgetRegistration/UINamedWidgetRegistration.hpp"
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UINamedWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIWidgetRegistration.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC std::vector<UINamedWidgetRegistration> UINamedWidgetRegistration::s_UINamedWidgetRegistrations;
const char* UINamedWidgetRegistration::s_NamedWidgetXMLName = "UINamedWidget";

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Constructors
UINamedWidgetRegistration::UINamedWidgetRegistration()
{

}

UINamedWidgetRegistration::UINamedWidgetRegistration(const std::string& name, const std::string& fileLocation, int idxInRegistration,
	const UIWidgetRegistrationDataType& data)
	: m_NamedWidgetName(name),
	m_fileLocation(fileLocation),
	m_idxInRegistration(idxInRegistration),
	m_data(data),
	m_canUseNode(true)
{
	
}

UINamedWidgetRegistration::~UINamedWidgetRegistration()
{
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Operations
UIWidgetBase* UINamedWidgetRegistration::SpawnWidgetFromData(const UIWidgetRegistrationDataPacket& packet)
{
	UIWidgetBase* namedWidget = nullptr;
	if (m_canUseNode == false)
	{
		return namedWidget;
	}
	namedWidget = new UINamedWidget(m_data, packet.GetRootNodeID());
	packet.CopyValuesOntoUIWidgetBase(namedWidget);
	return namedWidget;
}

UIWidgetBase* UINamedWidgetRegistration::SpawnWidgetFromData(const UIWidgetRegistrationDataPacket& packet, const XMLNode& node)
{
	UIWidgetBase* namedWidget = nullptr;
	if (m_canUseNode == false)
	{
		return namedWidget;
	}
	namedWidget = new UINamedWidget(m_data, packet.GetRootNodeID());
	namedWidget->ReadFromXMLNode(node);
	packet.CopyValuesOntoUIWidgetBase(namedWidget);
	return namedWidget;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Operators
const bool UINamedWidgetRegistration::operator==(const UINamedWidgetRegistration& other) const
{
	if (m_NamedWidgetName == other.m_NamedWidgetName)
	{
		return true;
	}
	return false;
}

const bool UINamedWidgetRegistration::operator==(const std::string& name) const
{
	if (m_NamedWidgetName == name)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Static Functions
STATIC void UINamedWidgetRegistration::ReadAllNamedWidgetsFromFolderLocation(const std::string& baseFolderLocation, bool readFromChildFolders)
{
	std::vector<std::string> filesAndFolders = FileUtils::EnumerateFilesInDirectory(baseFolderLocation, "*", readFromChildFolders);
	
	for (size_t fileIdx = 0; fileIdx < filesAndFolders.size(); fileIdx++)
	{
		std::string file = filesAndFolders[fileIdx];

		ReadNamedWidgetsFromXMLFile(file);
	}

}

STATIC void UINamedWidgetRegistration::ReadNamedWidgetsFromXMLFile(const std::string& xmlFileLocation)
{
	std::string xmlFileLocationCopy(xmlFileLocation);
	std::string fileEnding = xmlFileLocationCopy.substr(xmlFileLocationCopy.length() - 3, 3);
	if (SimpleStrCmpLower(fileEnding, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode rootNode = EngineXMLParser::ParseXMLFile(xmlFileLocation, s_NamedWidgetXMLName, errorOccurred);
	if (errorOccurred)
	{
		return;
	}
	for (int childIdx = 0; childIdx < rootNode.nChildNode(); childIdx++)
	{
		XMLNode childNode = rootNode.getChildNode(childIdx);
		std::string childName = childNode.getName();
		if (UIWidgetRegistration::GetIsWidgetTypeAlreadyDefined(childName))
		{
			ERROR_AND_DIE("Can NOT have a NamedWidget named after a pre-existing Widget type! " + childName + " already defined");
			return;
		}

		int idxAlreadyAt = GetIdxOfWidgetTypeIfAlreadyDefined(childName);
		if (idxAlreadyAt != -1)
		{
			DebuggerPrintf("NamedWidget already defined once! Overwriting current with new.");
			UINamedWidgetRegistration reg = UINamedWidgetRegistration(childName, xmlFileLocationCopy, childIdx, childNode);
			s_UINamedWidgetRegistrations[idxAlreadyAt] = reg;

		}
		else
		{
			UINamedWidgetRegistration reg = UINamedWidgetRegistration(childName, xmlFileLocationCopy, childIdx, childNode);
			s_UINamedWidgetRegistrations.push_back(reg);
		}
	}
	


	//Need to verify that the name does not already exist as a predefined Widget type.
	//if it already exists as a Named Widget type, then just overwrite the data, spit out a warning.
}

STATIC bool UINamedWidgetRegistration::GetIsWidgetTypeAlreadyDefined(const std::string& widgetName)
{
	for (size_t regIdx = 0; regIdx < s_UINamedWidgetRegistrations.size(); regIdx++)
	{
		UINamedWidgetRegistration* widgetReg = &s_UINamedWidgetRegistrations[regIdx];
		if (*widgetReg == widgetName)
		{
			return true;
		}
	}
	return false;
}

STATIC int UINamedWidgetRegistration::GetIdxOfWidgetTypeIfAlreadyDefined(const std::string& widgetName)
{
	for (size_t regIdx = 0; regIdx < s_UINamedWidgetRegistrations.size(); regIdx++)
	{
		UINamedWidgetRegistration* widgetReg = &s_UINamedWidgetRegistrations[regIdx];
		if (*widgetReg == widgetName)
		{
			return regIdx;
		}
	}
	return -1;
}

STATIC UIWidgetBase* UINamedWidgetRegistration::SpawnWidgetFromNamedWidgetName(const std::string& widgetName, 
	const UIWidgetRegistrationDataPacket& packet)
{
	for (size_t regIdx = 0; regIdx < s_UINamedWidgetRegistrations.size(); regIdx++)
	{
		UINamedWidgetRegistration* widgetReg = &s_UINamedWidgetRegistrations[regIdx];
		if (widgetReg->m_NamedWidgetName == widgetName)
		{
			return widgetReg->SpawnWidgetFromData(packet);
		}
	}
	return nullptr;
}

STATIC UIWidgetBase* UINamedWidgetRegistration::SpawnWidgetFromNamedWidgetName(const std::string& widgetName, 
	const UIWidgetRegistrationDataPacket& packet, const XMLNode& node)
{
	for (size_t regIdx = 0; regIdx < s_UINamedWidgetRegistrations.size(); regIdx++)
	{
		UINamedWidgetRegistration* widgetReg = &s_UINamedWidgetRegistrations[regIdx];
		if (widgetReg->m_NamedWidgetName == widgetName)
		{
			return widgetReg->SpawnWidgetFromData(packet, node);
		}
	}
	return nullptr;
}

STATIC void UINamedWidgetRegistration::ClearAllWidgetRegistrations()
{
	s_UINamedWidgetRegistrations.clear();
	s_UINamedWidgetRegistrations.shrink_to_fit();
}