#pragma once

#ifndef UISTYLESHEETHANDLER_HPP
#define UISTYLESHEETHANDLER_HPP
#include "Engine/UI/UIStructs/UIStyleSheet.hpp"

class UIStyleSheetHandler
{
private:
protected:
	//Static Variables
	static UIStyleSheetHandler s_StyleSheetHandler;
	static const char* s_UIStyleSheetTag;

	//Variables
	std::vector<UIStyleSheet> m_styleSheets;
	UIStyleSheet* m_defaultStyleSheet = nullptr;

public:
	//Constructors
	UIStyleSheetHandler();
	virtual ~UIStyleSheetHandler();

	//Setters
	void RegisterOrSetStyleSheet(const UIStyleSheet& styleSheet);
	void Update();

	//Getters
	void ClearAllStyleSheets();
	UIStyleSheet* GetEditableStyleSheetForName(const std::string& name);
	const UIStyleSheet* GetStyleSheetForName(const std::string& name);
	void LoadAllStyleSheetsFromXMLInFolders(const std::string& baseFolderLocation, bool readChildFolders);
	void ReadStyleSheetFromXMLFile(const std::string& xmlFileLocation);
	const UIStyleSheet* GetDefaultStyleSheet() const;

	//Static functions
	static void GlobalUpdate();
	static void ReloadValuesForDefaultStyleSheet();
	static void GlobalClearAllStyleSheets();
	static void GlobalRegisterOrSetStyleSheet(const UIStyleSheet& styleSheet);
	static UIStyleSheet* GlobalGetEditableStyleSheetForName(const std::string& name);
	static const UIStyleSheet* GlobalGetStyleSheetForName(const std::string& name);
	static void GlobalLoadAllStyleSheetsFromXMLInFolders(const  std::string& baseFolderLocation, bool readChildFolders);
	static void GlobalReadStyleSheetFromXMLFile(const std::string& xmlFileLocation);
	static UIStyleSheet* GlobalGetDefaultStyleSheet();
};
#endif