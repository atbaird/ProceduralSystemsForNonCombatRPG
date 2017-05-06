#include "Engine/UI/UIStyleSheetHandler.hpp"
#include "Engine//UI/UISystem.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC UIStyleSheetHandler UIStyleSheetHandler::s_StyleSheetHandler;
STATIC const char* UIStyleSheetHandler::s_UIStyleSheetTag = "UIStyleSheets";

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIStyleSheetHandler::UIStyleSheetHandler()
{
}

UIStyleSheetHandler::~UIStyleSheetHandler()
{
	ClearAllStyleSheets();
}


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Setters
void UIStyleSheetHandler::RegisterOrSetStyleSheet(const UIStyleSheet& styleSheet)
{
	for (size_t styleSheetIdx = 0; styleSheetIdx < m_styleSheets.size(); styleSheetIdx++)
	{
		UIStyleSheet& curStyleSheet = m_styleSheets[styleSheetIdx];
		if (curStyleSheet.GetStyleSheetName() == styleSheet.GetStyleSheetName())
		{
			curStyleSheet = styleSheet;
			if (styleSheet.GetStyleSheetName() == "default")
			{
				m_defaultStyleSheet = &m_styleSheets[styleSheetIdx];
			}
			return;
		}
	}
	m_styleSheets.push_back(styleSheet);
	if (styleSheet.GetStyleSheetName() == "default")
	{
		m_defaultStyleSheet = &m_styleSheets[m_styleSheets.size() - 1];
	}
}

void UIStyleSheetHandler::Update()
{
	for (size_t styleSheetIdx = 0; styleSheetIdx < m_styleSheets.size(); styleSheetIdx++)
	{
		UIStyleSheet& curStyleSheet = m_styleSheets[styleSheetIdx];
		if (curStyleSheet.GetStyleSheetChanged() == true)
		{
			curStyleSheet.SetStyleSheetChanged(false);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Getters
void UIStyleSheetHandler::ClearAllStyleSheets()
{
	m_styleSheets.clear();
	m_styleSheets.shrink_to_fit();
}

UIStyleSheet* UIStyleSheetHandler::GetEditableStyleSheetForName(const std::string& name)
{
	for (size_t styleSheetIdx = 0; styleSheetIdx < m_styleSheets.size(); styleSheetIdx++)
	{
		UIStyleSheet* styleSheet = &m_styleSheets[styleSheetIdx];
		if (styleSheet->GetStyleSheetName() == name)
		{
			return styleSheet;
		}
	}
	return nullptr;
}

const UIStyleSheet* UIStyleSheetHandler::GetStyleSheetForName(const std::string& name)
{
	for (size_t styleSheetIdx = 0; styleSheetIdx < m_styleSheets.size(); styleSheetIdx++)
	{
		UIStyleSheet* styleSheet = &m_styleSheets[styleSheetIdx];
		if (styleSheet->GetStyleSheetName() == name)
		{
			return styleSheet;
		}
	}
	return nullptr;
}
void UIStyleSheetHandler::LoadAllStyleSheetsFromXMLInFolders(const std::string& baseFolderLocation, bool readChildFolders)
{
	std::vector<std::string> filesAndFolders = FileUtils::EnumerateFilesInDirectory(baseFolderLocation, "*", readChildFolders);

	for (size_t fileIdx = 0; fileIdx < filesAndFolders.size(); fileIdx++)
	{
		std::string file = filesAndFolders[fileIdx];

		ReadStyleSheetFromXMLFile(file.c_str());
	}
}

void UIStyleSheetHandler::ReadStyleSheetFromXMLFile(const std::string& xmlFileLocation)
{
	std::string fileEnding = xmlFileLocation.substr(xmlFileLocation.length() - 3, 3);
	if (SimpleStrCmpLower(fileEnding, "xml") != true)
	{
		return;
	}

	bool errorOccured = false;
	XMLNode node = EngineXMLParser::ParseXMLFile(xmlFileLocation, s_UIStyleSheetTag, errorOccured);
	if (errorOccured == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		UIStyleSheet styleSheet;
		XMLNode child = node.getChildNode(childIdx);
		styleSheet.ReadXMLNodeData(child);

		bool found = false;
		for (size_t styleSheetIdx = 0; styleSheetIdx < m_styleSheets.size() ; styleSheetIdx++)
		{
			UIStyleSheet& curSheet = m_styleSheets[styleSheetIdx];
			if (curSheet.GetStyleSheetName() == styleSheet.GetStyleSheetName())
			{
				curSheet = styleSheet;
				found = true;
				break;
			}
		}
		if (found)
		{
			continue;
		}
		m_styleSheets.push_back(styleSheet);

	}

}

const UIStyleSheet* UIStyleSheetHandler::GetDefaultStyleSheet() const
{
	for (size_t i = 0; i < m_styleSheets.size(); i++)
	{
		if (m_styleSheets[i].GetStyleSheetName() == "default")
		{
			return &m_styleSheets[i];
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Static functions
STATIC void UIStyleSheetHandler::GlobalUpdate()
{
	s_StyleSheetHandler.Update();
}

STATIC void UIStyleSheetHandler::ReloadValuesForDefaultStyleSheet()
{
	UISystem* system = UISystem::GetUISystemPtr();
	if (system == nullptr)
	{
		return;
	}
	UIStyleSheet* defaultStyleSheet = s_StyleSheetHandler.m_defaultStyleSheet;
	if (defaultStyleSheet != nullptr)
	{
		UIStyleWidgetData* data = defaultStyleSheet->GetEditableStyleDataForWidgetType("default");
		if (data == nullptr)
		{
			UIStyleWidgetData newData;
			
			defaultStyleSheet->SetOrAddStateDataPerWidget(newData);
		}
		else
		{

		}
	}
}
STATIC void UIStyleSheetHandler::GlobalClearAllStyleSheets()
{
	s_StyleSheetHandler.ClearAllStyleSheets();
}

STATIC void UIStyleSheetHandler::GlobalRegisterOrSetStyleSheet(const UIStyleSheet& styleSheet)
{
	s_StyleSheetHandler.RegisterOrSetStyleSheet(styleSheet);
}

STATIC UIStyleSheet* UIStyleSheetHandler::GlobalGetEditableStyleSheetForName(const std::string& name)
{
	return s_StyleSheetHandler.GetEditableStyleSheetForName(name);
}

STATIC const UIStyleSheet* UIStyleSheetHandler::GlobalGetStyleSheetForName(const std::string& name)
{
	return s_StyleSheetHandler.GetStyleSheetForName(name);
}

STATIC void UIStyleSheetHandler::GlobalLoadAllStyleSheetsFromXMLInFolders(const  std::string& baseFolderLocation, bool readChildFolders)
{
	s_StyleSheetHandler.LoadAllStyleSheetsFromXMLInFolders(baseFolderLocation, readChildFolders);
}

STATIC void UIStyleSheetHandler::GlobalReadStyleSheetFromXMLFile(const std::string& xmlFileLocation)
{
	s_StyleSheetHandler.ReadStyleSheetFromXMLFile(xmlFileLocation);
}

STATIC UIStyleSheet* UIStyleSheetHandler::GlobalGetDefaultStyleSheet()
{	
	const UIStyleSheet* styleSheet = s_StyleSheetHandler.GetDefaultStyleSheet();
	UIStyleSheet* outSheet = const_cast<UIStyleSheet*>(styleSheet);
	return outSheet;
}