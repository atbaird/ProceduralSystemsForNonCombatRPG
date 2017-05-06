#pragma once

#ifndef UISYSTEM_HPP
#define UISYSTEM_HPP
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UIStyleSheetHandler.hpp"
#define STATIC 

class UISystem
{
private:
protected:
	//Static Variables
	static UISystem* s_UISystem;
	static const std::string s_defaultFolderLocationForNamedWidgets;
	static std::string s_folderLocationToReadForNamedWidgets;
	static const std::string s_defaultFolderLocationForStyleSheetDeclarations;
	static std::string s_folderLocationToReadForStyleSheetDeclarations;
	static bool s_readFromChildFolders;

	std::vector<UIWidgetBase*> m_currentActiveRootWidgets;
	const Texture* m_defaultBackgroundImage = nullptr;
	const Texture* m_defaultCheckedStateBackgroundImage = nullptr;
	const Texture* m_defaultRadioStateBackgroundImage = nullptr;
	const Texture* m_defaultRadioSelectedStateBackgroundImage = nullptr;
	ShaderProgram* m_defaultShaderProgram = nullptr;
	Font* m_defaultFont = nullptr;
	Matrix44 m_Projection;
	Matrix44 m_View;
	Vector2 m_currentScreenSize = Vector2(16.f, 9.f);
	Vector2 m_mousePosition = Vector2::vec2_zeros;
	bool m_mouseOverWindow = false;
	bool m_mouseDownLastFrame = false;
public:
	//Constructors
	UISystem();
	UISystem(const char* baseFolderToReadNamedWidgetsFrom, const char* folderLocationToReadForStyleSheetDeclarations, 
		bool readfromChildFolders = true, const Texture* defaultTex = nullptr,
		ShaderProgram* defaultprog = nullptr, Font* defaultFont = nullptr,
		const Texture* defaultCheckedStateBackgroundImage = nullptr, 
		const Texture* defaultRadioStateBackgroundImage = nullptr,
		const Texture* defaultRadioSetStateBackgroundImage = nullptr);
	virtual ~UISystem();

	//Updates and Render
	void Update(const float& deltaSeconds);
	void Render() const;

	//Widget Loading
	void LoadAllStyleSheetDeclarationsFromXMLInFolder(const std::string& folderLocation, bool readFromChildFolders = true);
	void LoadAllStyleSheetDeclarationsFromXML(const std::string& xmlFile);
	void LoadAllNamedWidgetsFromXMLInFolder(const std::string& folderLocation, bool readFromChildFolders = true);
	void LoadAllNamedWidgetsFromXML(const std::string& xmlFile);
	void ReloadAllNamedWidgetRegistrationsFromDefinedFolderLocation();
	void ReloadAllStyleSheetRegistrationsFromDefinedFolderLocation();
	void ClearAllNamedWidgetRegistrations();
	void ClearAllStyleSheetRegistrations();

	//Setters
	UIWidgetBase* SpawnNamedWidget(const std::string& namedWidgetName);
	UIWidgetBase* SpawnUISystemWidgetByName(const std::string& widgetName);
	void SetDefaultShaderProgram(ShaderProgram* prog);
	void SetCurrentUIScreenSize(const Vector2& size);
	void SetDefaultBackgroundImage(const Texture* image);
	void ClearWidgetByIdx(size_t idx);
	void ClearAllUIWidgets();
	void ShowHideWidgetByTag(const std::string& tag, bool showElseHide, bool effectChildren = true);
	void ShowHideWidgetByWidgetType(const std::string& widgetTypeName, bool showElseHide, bool effectChildren = true);
	void ShowHideWidgetByUniqueName(const std::string& uniqueName, bool showElseHide, bool effectChildren = true);

protected:
	void PlugInDefaultStyleSheet();
public:


	//Getters
	ShaderProgram* GetDefaultShaderProgram() const;
	const Texture* GetDefaultBackgroundImageForWidgets() const;
	Vector2 GetCurrentSystemScreenSize() const;

	//Static Functions
	static UISystem* GetUISystemPtr();
	static void CreateUISystem(const Texture* defaultTex = nullptr, ShaderProgram* defaultProg = nullptr, Font* defaultFont = nullptr,
		const Texture* defaultCheckedStateBackgroundImage = nullptr, const Texture* defaultRadioStateBackgroundImage = nullptr
		, const Texture* defaultRadioStateSetBackgroundImage = nullptr);
	static void CreateUISystem(const char* baseFolderToReadNamedWidgetsFrom, const char* baseFolderToReadStyleSheetsFrom, 
		bool readfromChildFolders = true, const Texture* defaultTex = nullptr, ShaderProgram* defaultProg = nullptr, 
		Font* defaultFont = nullptr, const Texture* defaultCheckedStateBackgroundImage = nullptr, 
		const Texture* defaultRadioStateBackgroundImage = nullptr, const Texture* defaultRadioStateSetBackgroundImage = nullptr);
	static void DestroyUISystem();
	static void GlobalShowHideWidgetByTag(const std::string& tag, bool showElseHide, bool effectChildren = true);
	static void GlobalShowHideWidgetByWidgetType(const std::string& widgetTypeName, bool showElseHide, bool effectChildren = true);
	static void GlobalShowHideWidgetByUniqueName(const std::string& uniqueName, bool showElseHide, bool effectChildren = true);
};
#endif