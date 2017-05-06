#include "Engine/UI/UISystem.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UINamedWidgetRegistration.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIWidgetRegistration.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Window/Window.hpp"

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC UISystem* UISystem::s_UISystem = nullptr;
STATIC const std::string UISystem::s_defaultFolderLocationForNamedWidgets = "Data/XML/UI/";
STATIC std::string UISystem::s_folderLocationToReadForNamedWidgets = "";
STATIC const std::string UISystem::s_defaultFolderLocationForStyleSheetDeclarations = "Data/XML/UI";
STATIC std::string UISystem::s_folderLocationToReadForStyleSheetDeclarations = "";
STATIC bool UISystem::s_readFromChildFolders = true;

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Constructors
UISystem::UISystem()
{
	UIWidgetRegistration::RegisterAllWidgetTypes();
	s_folderLocationToReadForNamedWidgets = (s_defaultFolderLocationForNamedWidgets);
	s_folderLocationToReadForStyleSheetDeclarations = s_defaultFolderLocationForStyleSheetDeclarations;
	PlugInDefaultStyleSheet();
	LoadAllStyleSheetDeclarationsFromXMLInFolder(s_folderLocationToReadForStyleSheetDeclarations, s_readFromChildFolders);
	LoadAllNamedWidgetsFromXMLInFolder(s_folderLocationToReadForNamedWidgets, s_readFromChildFolders);
}

UISystem::UISystem(const char* baseFolderToReadNamedWidgetsFrom, const char* folderLocationToReadForStyleSheetDeclarations, 
	bool readFromChildFolders, const Texture* defaultTex, 
	ShaderProgram* defaultprog, Font* defaultFont,
	const Texture* defaultCheckedStateBackgroundImage, const Texture* defaultRadioStateBackgroundImage
	, const Texture* defaultRadioStateSetBackgroundImage)
	: m_defaultShaderProgram(defaultprog),
	m_defaultBackgroundImage(defaultTex),
	m_defaultFont(defaultFont),
	m_defaultCheckedStateBackgroundImage(defaultCheckedStateBackgroundImage),
	m_defaultRadioStateBackgroundImage(defaultRadioStateBackgroundImage),
	m_defaultRadioSelectedStateBackgroundImage(defaultRadioStateSetBackgroundImage)
{
	UIWidgetRegistration::RegisterAllWidgetTypes();
	s_folderLocationToReadForNamedWidgets = baseFolderToReadNamedWidgetsFrom;
	s_readFromChildFolders = readFromChildFolders;
	s_folderLocationToReadForStyleSheetDeclarations = folderLocationToReadForStyleSheetDeclarations;
	PlugInDefaultStyleSheet();
	LoadAllStyleSheetDeclarationsFromXMLInFolder(s_folderLocationToReadForStyleSheetDeclarations, s_readFromChildFolders);
	LoadAllNamedWidgetsFromXMLInFolder(s_folderLocationToReadForNamedWidgets, s_readFromChildFolders);
}

UISystem::~UISystem()
{
	ClearAllNamedWidgetRegistrations();
	ClearAllStyleSheetRegistrations();
	ClearAllUIWidgets();
	UIWidgetRegistration::ClearAllWidgetTypes();
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Updates and Render
void UISystem::Update(const float& deltaSeconds)
{
	m_mousePosition = Vector2::vec2_zeros;
	m_mouseOverWindow = false;
	if (Window::g_Window != nullptr)
	{
		m_mousePosition = Window::g_Window->GetMouseVirtualCoordsRelativeToTheWindow(m_currentScreenSize, m_mouseOverWindow);
	}
	m_Projection.MatrixMakeIdentity();
	m_View.MatrixMakeIdentity();

	Vector2 halfScreen = Vector2(m_currentScreenSize.x / 2.f, m_currentScreenSize.y / 2.f);
	Matrix44::MakeEulerRotation(m_View, DegreesToRadians(0.f), DegreesToRadians(-0.f),
		DegreesToRadians(0.f), Vector3(halfScreen, 0.f));
	Matrix44::MatrixInvertOrthogonal(m_View);
	float width = (float)g_Renderer->GetScreenWidth() / 100.f;
	float height = (float)g_Renderer->GetScreenHeight() / 100.f;
	Matrix44::MatrixMakeOrthogonal(m_Projection, Vector2(width, height), -1.f, 1.f);

	bool mouseCurrentlyDown = (GetKeyState(VK_LBUTTON) & 0x100) != 0;
	
	for (size_t rootWidgetIdx = 0; rootWidgetIdx < m_currentActiveRootWidgets.size(); rootWidgetIdx++)
	{
		UIWidgetBase* rootWidget = m_currentActiveRootWidgets[rootWidgetIdx];
		if (rootWidget != nullptr)
		{
			rootWidget->Update(deltaSeconds);
			if (m_mouseOverWindow)
			{
				rootWidget->MouseUpdate(m_mousePosition, mouseCurrentlyDown, m_mouseDownLastFrame);
			}
		}
	}

	if (mouseCurrentlyDown)
	{
		m_mouseDownLastFrame = true;
	}
	else
	{
		m_mouseDownLastFrame = false;
	}

	UIStyleSheetHandler::GlobalUpdate();
}

void UISystem::Render() const
{
	float width = (float)g_Renderer->GetScreenWidth();
	float height = (float)g_Renderer->GetScreenHeight();
	g_Renderer->CullFaceTrueFalse(true);
	g_Renderer->DepthTestingTrueFalse(true);

	g_Renderer->SetOrtho(Vector2(0.f, 0.f), Vector2(width, height));
	ShaderProgram::SetProjectionAndViewAll(m_Projection, m_View);

	if (m_currentActiveRootWidgets.size() == 0)
	{
		return;
	}

	for (int rootWidgetIdx = (int)m_currentActiveRootWidgets.size() - 1; rootWidgetIdx >= 0; rootWidgetIdx--)
	{
		UIWidgetBase* rootWidget = m_currentActiveRootWidgets[rootWidgetIdx];
		if (rootWidget != nullptr)
		{
			rootWidget->Render();
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Widget Loading
void UISystem::LoadAllStyleSheetDeclarationsFromXMLInFolder(const std::string& folderLocation, bool readFromChildFolders)
{
	UIStyleSheetHandler::GlobalLoadAllStyleSheetsFromXMLInFolders(folderLocation, readFromChildFolders);
}

void UISystem::LoadAllStyleSheetDeclarationsFromXML(const std::string& xmlFile)
{
	UIStyleSheetHandler::GlobalReadStyleSheetFromXMLFile(xmlFile);
}

void UISystem::LoadAllNamedWidgetsFromXMLInFolder(const std::string& folderLocation, bool readFromChildFolders)
{
	UINamedWidgetRegistration::ReadAllNamedWidgetsFromFolderLocation(folderLocation, readFromChildFolders);
}

void UISystem::LoadAllNamedWidgetsFromXML(const std::string& xmlFile)
{
	UINamedWidgetRegistration::ReadNamedWidgetsFromXMLFile(xmlFile);
}

void UISystem::ReloadAllNamedWidgetRegistrationsFromDefinedFolderLocation()
{
	UINamedWidgetRegistration::ClearAllWidgetRegistrations();
	UINamedWidgetRegistration::ReadAllNamedWidgetsFromFolderLocation(s_folderLocationToReadForNamedWidgets, s_readFromChildFolders);
}

void UISystem::ReloadAllStyleSheetRegistrationsFromDefinedFolderLocation()
{
	UIStyleSheetHandler::GlobalClearAllStyleSheets();
	UIStyleSheetHandler::GlobalLoadAllStyleSheetsFromXMLInFolders(s_folderLocationToReadForStyleSheetDeclarations, s_readFromChildFolders);
}

void UISystem::ClearAllNamedWidgetRegistrations()
{
	UINamedWidgetRegistration::ClearAllWidgetRegistrations();
}

void UISystem::ClearAllStyleSheetRegistrations()
{
	UIStyleSheetHandler::GlobalClearAllStyleSheets();
}


//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Setters
UIWidgetBase* UISystem::SpawnNamedWidget(const std::string& namedWidgetName)
{
	UIWidgetRegistrationDataPacket packet;
	UIWidgetBase* namedWidget = UINamedWidgetRegistration::SpawnWidgetFromNamedWidgetName(namedWidgetName, packet);
	if (namedWidget != nullptr)
	{
		m_currentActiveRootWidgets.push_back(namedWidget);
	}

	return namedWidget;
}

UIWidgetBase* UISystem::SpawnUISystemWidgetByName(const std::string& widgetName)
{
	UIStyleSheet* style = UIStyleSheetHandler::GlobalGetDefaultStyleSheet();
	UIWidgetRegistrationDataPacket packet(nullptr, style, m_currentActiveRootWidgets.size());
	UIWidgetBase* widget = UIWidgetRegistration::SpawnWidgetOfType(widgetName, packet);
	if (widget != nullptr)
	{
		m_currentActiveRootWidgets.push_back(widget);
	}

	return widget;
}

void UISystem::SetDefaultShaderProgram(ShaderProgram* prog)
{
	m_defaultShaderProgram = prog;
}

void UISystem::SetCurrentUIScreenSize(const Vector2& size)
{
	m_currentScreenSize = size;
}

void UISystem::SetDefaultBackgroundImage(const Texture* image)
{
	m_defaultBackgroundImage = image;
}

void UISystem::ClearWidgetByIdx(size_t idx)
{
	if (idx >= m_currentActiveRootWidgets.size())
	{
		return;
	}

	UIWidgetBase* widgetToDelete = m_currentActiveRootWidgets[idx];
	m_currentActiveRootWidgets.erase(m_currentActiveRootWidgets.begin() + idx);
	delete widgetToDelete;
	widgetToDelete = nullptr;
	for (size_t rootNodeIdx = idx; rootNodeIdx < m_currentActiveRootWidgets.size(); rootNodeIdx++)
	{
		m_currentActiveRootWidgets[rootNodeIdx]->SetRootNodeID(rootNodeIdx);
	}
}

void UISystem::ClearAllUIWidgets()
{
	for (size_t widgetIdx = 0; widgetIdx < m_currentActiveRootWidgets.size(); widgetIdx++)
	{
		if (m_currentActiveRootWidgets[widgetIdx] != nullptr)
		{
			delete m_currentActiveRootWidgets[widgetIdx];
			m_currentActiveRootWidgets[widgetIdx] = nullptr;
		}
	}
	m_currentActiveRootWidgets.clear();
	UIWidgetBase::ClearAllRegisterWidgetData();
}


void UISystem::ShowHideWidgetByTag(const std::string& tag, bool showElseHide, bool effectChildren)
{
	for (size_t widgetIdx = 0; widgetIdx < m_currentActiveRootWidgets.size(); widgetIdx++)
	{
		UIWidgetBase* child = m_currentActiveRootWidgets[widgetIdx];
		if (child != nullptr)
		{
			child->ShowHideWidgetByTag(tag, showElseHide, effectChildren);
		}
	}

}

void UISystem::ShowHideWidgetByWidgetType(const std::string& tag, bool showElseHide, bool effectChildren)
{
	for (size_t widgetIdx = 0; widgetIdx < m_currentActiveRootWidgets.size(); widgetIdx++)
	{
		UIWidgetBase* child = m_currentActiveRootWidgets[widgetIdx];
		if (child != nullptr)
		{
			child->ShowHideWidgetByWidgetType(tag, showElseHide, effectChildren);
		}
	}
}

void UISystem::ShowHideWidgetByUniqueName(const std::string& tag, bool showElseHide, bool effectChildren)
{
	for (size_t widgetIdx = 0; widgetIdx < m_currentActiveRootWidgets.size(); widgetIdx++)
	{
		UIWidgetBase* child = m_currentActiveRootWidgets[widgetIdx];
		if (child != nullptr)
		{
			child->ShowHideWidgetByUniqueName(tag, showElseHide, effectChildren);
		}
	}
}

void UISystem::PlugInDefaultStyleSheet()
{
	UIStyleSheet sheet;
	sheet.SetStyleSheetName("default");
	UIStyleWidgetData defaultType = UIStyleWidgetData();
	UIStyleWidgetData buttonType = UIStyleWidgetData();

	//Default Type
	defaultType.SetAllStatesBackgroundColors(Rgba::s_White);
	defaultType.SetAllStatesMarginColors(Rgba::s_Gray);
	defaultType.SetAllStatesFontColors(Rgba::s_Black);
	defaultType.SetAllStatesBackgroundImages(m_defaultBackgroundImage);
	defaultType.SetAllStatesMarginImages(m_defaultBackgroundImage);
	defaultType.SetAllStatesCheckedBackgroundImages(m_defaultCheckedStateBackgroundImage);
	defaultType.SetAllStatesCheckedMarginImages(m_defaultBackgroundImage);
	defaultType.SetAllStatesRadioBackgroundImages(m_defaultRadioStateBackgroundImage);
	defaultType.SetAllStatesRadioSetBackgroundImages(m_defaultRadioSelectedStateBackgroundImage);
	defaultType.SetAllStatesFillBarImages(m_defaultBackgroundImage);
	defaultType.SetAllStatesFonts(m_defaultFont);

	UIStateAppearanceData disabledDefaultState;
	defaultType.LoadDefaultsOntoUIStateAppearanceData(disabledDefaultState);
	disabledDefaultState.SetBackgroundColor(Rgba::s_Gray6);
	disabledDefaultState.SetBackgroundMarginColor(Rgba(0.1f, 0.1f, 0.1f, 1.f));
	defaultType.SetStateAppearance(UIWIDGETSTATE_DISABLED, disabledDefaultState);

	UIStateAppearanceData hoveredDefaultState;
	defaultType.LoadDefaultsOntoUIStateAppearanceData(hoveredDefaultState);
	hoveredDefaultState.SetBackgroundColor(Rgba::s_Blue);
	hoveredDefaultState.SetBackgroundMarginColor(Rgba(0.1f, 0.1f, 0.1f, 1.f));
	defaultType.SetStateAppearance(UIWIDGETSTATE_HOVERED, hoveredDefaultState);
	sheet.SetOrAddStateDataPerWidget(defaultType);


	/*
	UIWIDGETSTATE_ACTIVE,
	UIWIDGETSTATE_HIGHLIGHTED, //Hovered
	UIWIDGETSTATE_HOVERED = UIWIDGETSTATE_HIGHLIGHTED,
	UIWIDGETSTATE_PRESSED,
	UIWIDGETSTATE_DISABLED,
	*/

	//Button Type
	buttonType.SetWidgetType("button");
	buttonType.SetAllStatesBackgroundColors(Rgba::s_White);
	buttonType.SetAllStatesMarginColors(Rgba::s_Gray);
	buttonType.SetAllStatesFontColors(Rgba::s_Black);
	buttonType.SetAllStatesBackgroundImages(m_defaultBackgroundImage);
	buttonType.SetAllStatesMarginImages(m_defaultBackgroundImage);
	buttonType.SetAllStatesCheckedBackgroundImages(m_defaultCheckedStateBackgroundImage);
	buttonType.SetAllStatesCheckedMarginImages(m_defaultBackgroundImage);
	buttonType.SetAllStatesRadioBackgroundImages(m_defaultRadioStateBackgroundImage);
	buttonType.SetAllStatesRadioSetBackgroundImages(m_defaultRadioSelectedStateBackgroundImage);
	buttonType.SetAllStatesFillBarImages(m_defaultBackgroundImage);
	buttonType.SetAllStatesFonts(m_defaultFont);

	UIStateAppearanceData disabledButtonState;
	buttonType.LoadDefaultsOntoUIStateAppearanceData(disabledButtonState);
	disabledButtonState.SetBackgroundColor(Rgba(0.4f, 0.4f, 0.4f, 1.f));
	disabledButtonState.SetBackgroundMarginColor(Rgba(0.2f, 0.4f, 0.2f, 1.f));
	buttonType.SetStateAppearance(UIWIDGETSTATE_DISABLED, disabledButtonState);

	UIStateAppearanceData hoveredButtonState;
	buttonType.LoadDefaultsOntoUIStateAppearanceData(hoveredButtonState);
	hoveredButtonState.SetBackgroundColor(Rgba::s_Gray);
	hoveredButtonState.SetBackgroundMarginColor(Rgba(0.5f, 0.7f, 0.5f, 1.f));
	buttonType.SetStateAppearance(UIWIDGETSTATE_HOVERED, hoveredButtonState);
	sheet.SetOrAddStateDataPerWidget(buttonType);

	UIStateAppearanceData pressedButtonState;
	buttonType.LoadDefaultsOntoUIStateAppearanceData(pressedButtonState);
	pressedButtonState.SetBackgroundColor(Rgba(0.2f, 0.2f, 0.2f, 1.f));
	pressedButtonState.SetBackgroundMarginColor(Rgba(0.5f, 0.7f, 0.5f, 1.f));
	buttonType.SetStateAppearance(UIWIDGETSTATE_PRESSED, pressedButtonState);
	sheet.SetOrAddStateDataPerWidget(buttonType);

	UIStyleSheetHandler::GlobalRegisterOrSetStyleSheet(sheet);
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Getters
ShaderProgram* UISystem::GetDefaultShaderProgram() const
{
	return m_defaultShaderProgram;
}

const Texture* UISystem::GetDefaultBackgroundImageForWidgets() const
{
	return m_defaultBackgroundImage;
}

Vector2 UISystem::GetCurrentSystemScreenSize() const
{
	return m_currentScreenSize;
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Static Functions
STATIC UISystem* UISystem::GetUISystemPtr()
{
	return s_UISystem;
}

STATIC void UISystem::CreateUISystem(const Texture* defaultTex, ShaderProgram* defaultProg, Font* defaultFont,
	const Texture* defaultCheckedStateBackgroundImage, const Texture* defaultRadioStateBackgroundImage
	, const Texture* defaultRadioStateSetBackgroundImage)
{
	if (s_UISystem != nullptr)
	{
		return;
	}
	/*
	const char* baseFolderToReadNamedWidgetsFrom, const char* folderLocationToReadForStyleSheetDeclarations,
	bool readFromChildFolders, const Texture* defaultTex,
	ShaderProgram* defaultprog
	*/
	s_UISystem = new UISystem(s_defaultFolderLocationForNamedWidgets.c_str(), s_defaultFolderLocationForStyleSheetDeclarations.c_str(),
		true, defaultTex, defaultProg, defaultFont, defaultCheckedStateBackgroundImage, defaultRadioStateBackgroundImage,
		defaultRadioStateSetBackgroundImage);
}

STATIC void UISystem::CreateUISystem(const char* baseFolderToReadNamedWidgetsFrom, const char* baseFolderToReadStyleSheetsFrom,
	bool readfromChildFolders, const Texture* defaultTex, ShaderProgram* defaultProg, Font* defaultFont,
	const Texture* defaultCheckedStateBackgroundImage, const Texture* defaultRadioStateBackgroundImage
	, const Texture* defaultRadioStateSetBackgroundImage)
{
	if (s_UISystem != nullptr)
	{
		return;
	}
	s_UISystem = new UISystem(baseFolderToReadNamedWidgetsFrom, baseFolderToReadStyleSheetsFrom,
		readfromChildFolders, defaultTex, defaultProg, defaultFont, defaultCheckedStateBackgroundImage, defaultRadioStateBackgroundImage,
		defaultRadioStateSetBackgroundImage);
}

STATIC void UISystem::DestroyUISystem()
{
	if (s_UISystem == nullptr)
	{
		return;
	}
	delete s_UISystem;
	s_UISystem = nullptr;
}

STATIC void UISystem::GlobalShowHideWidgetByTag(const std::string& tag, bool showElseHide, bool effectChildren)
{
	if (s_UISystem == nullptr)
	{
		return;
	}
	s_UISystem->ShowHideWidgetByTag(tag, showElseHide, effectChildren);
}

STATIC void UISystem::GlobalShowHideWidgetByWidgetType(const std::string& tag, bool showElseHide, bool effectChildren)
{
	if (s_UISystem == nullptr)
	{
		return;
	}
	s_UISystem->ShowHideWidgetByWidgetType(tag, showElseHide, effectChildren);
}

STATIC void UISystem::GlobalShowHideWidgetByUniqueName(const std::string& tag, bool showElseHide, bool effectChildren)
{
	if (s_UISystem == nullptr)
	{
		return;
	}
	s_UISystem->ShowHideWidgetByUniqueName(tag, showElseHide, effectChildren);
}