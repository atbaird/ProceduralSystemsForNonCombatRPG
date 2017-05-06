#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UISystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UINamedWidgetRegistration.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIWidgetRegistration.hpp"
#include "Engine/UI/UIStyleSheetHandler.hpp"
#include "Engine/Core/StringUtils.hpp"

STATIC std::vector<UIWidgetBase*> UIWidgetBase::s_widgetPtrs;
STATIC const char* UIWidgetBase::s_defaultImageBindName = "gDiffuseTex";
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIWidgetBase::UIWidgetBase(size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent, 
	const std::string& widgetName, bool amGroupingWidget, size_t additionalEventsToPrealloc)
	: m_styleSheet(styleSheet),
	m_widgetName(widgetName),
	m_parent(parent),
	m_rootNodeID(rootNodeID),
	m_offsetData(this),
	m_fontOffsetData(this),
	m_sizeData(this)
{
	m_boolPack.m_amANamedWidget = amGroupingWidget;
	if (amGroupingWidget)
	{
		m_sizeData.SetFillParentAvailableParentSpaceX(true);
		m_sizeData.SetFillParentAvailableParentSpaceY(true);
	}
	if (m_styleSheet == nullptr)
	{
		m_styleSheet = UIStyleSheetHandler::GlobalGetDefaultStyleSheet();
	}
	ConstructorCommons(additionalEventsToPrealloc);
}

UIWidgetBase::UIWidgetBase(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet, 
	UIWidgetBase* parent, const std::string& widgetName, bool amGroupingWidget, size_t additionalEventsToPrealloc)
	: m_styleSheet(styleSheet),
	m_widgetName(widgetName),
	m_parent(parent),
	m_rootNodeID(rootNodeID),
	m_offsetData(this),
	m_fontOffsetData(this),
	m_sizeData(this)
{
	m_boolPack.m_amANamedWidget = amGroupingWidget;
	if (amGroupingWidget)
	{
		m_sizeData.SetFillParentAvailableParentSpaceX(true);
		m_sizeData.SetFillParentAvailableParentSpaceY(true);
	}
	if (m_styleSheet == nullptr)
	{
		m_styleSheet = UIStyleSheetHandler::GlobalGetDefaultStyleSheet();
	}
	ConstructorCommons(additionalEventsToPrealloc);
	ReadFromXMLNode(node);
}

UIWidgetBase::~UIWidgetBase()
{
	UnregisterWidget(this);
	if (m_BackgroundMat != nullptr)
	{
		delete m_BackgroundMat;
		m_BackgroundMat = nullptr;
	}
	if (m_MarginMat != nullptr)
	{
		delete m_MarginMat;
		m_MarginMat = nullptr;
	}
	DeleteAllChildren();
}

void UIWidgetBase::ConstructorCommons(size_t additionalEventsToPrealloc)
{
	RegisterWidget(this);
	m_vectorPack.m_eventCallbacks.resize(UIWIDGETSTATE_NUM_TO_HOLD_DATA_FOR + additionalEventsToPrealloc);

	m_vectorPack.m_eventCallbacks[0].SetName("active");
	m_vectorPack.m_eventCallbacks[1].SetName("hovered");
	m_vectorPack.m_eventCallbacks[2].SetName("pressed");
	m_vectorPack.m_eventCallbacks[3].SetName("disabled");
	if (m_vectorPack.m_eventCallbacks.size() > 4)
	{
		m_vectorPack.m_eventCallbacks[4].SetName("onvaluechanged");
	}

	m_BackgroundMat = new Material();
	m_MarginMat = new Material();
	m_sizeData.SetMarginBL(Vector2(0.2f, 0.2f));
	m_sizeData.SetMarginTR(Vector2(0.2f, 0.2f));
	m_backgroundMesh.SetDrawMode(PRIMITIVE_TRIANGLES);
	m_marginMesh.SetDrawMode(PRIMITIVE_TRIANGLES);
	UISystem* ptr = UISystem::GetUISystemPtr();
	m_prog = ptr->GetDefaultShaderProgram();
	m_BackgroundMat->SetProgram(m_prog);
	m_MarginMat->SetProgram(m_prog);
	m_styleSheet = UIStyleSheetHandler::GlobalGetDefaultStyleSheet();
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Read Write XML
void UIWidgetBase::ReadFromXMLNode(const XMLNode& node)
{
	m_boolPack.m_readFromXMLNode = true;
	m_xmlNodeCopy = node;
	m_widgetName = node.getName();
	ReadAndHandleXMLNodeAttributes(node);
	ReadAndHandleXMLNodeChildren(node);
}


void UIWidgetBase::ParseStringForPositionalOffsetData(const std::string& attVal)
{
	std::string attrVal = attVal;
	attrVal = ReplaceCharInString(attrVal, ',', ' ');
	attrVal = ReplaceCharInString(attrVal, ':', ' ');
	attrVal = ReplaceCharInString(attrVal, ';', ' ');
	std::vector<std::string> vals = ParseString(attrVal, ' ');

	if (vals.size() > 0)
	{
		float val = (float)atof(vals[0].c_str());
		if (DoesStringHaveThisChar(vals[0], '%'))
		{
			m_offsetData.SetPercentagePositionOffsetX(val);
		}
		else
		{
			m_offsetData.SetPositionOffsetX(val);
		}
	}
	if (vals.size() > 1)
	{
		float val = (float)atof(vals[1].c_str());
		if (DoesStringHaveThisChar(vals[1], '%'))
		{
			m_offsetData.SetPercentagePositionOffsetY(val);
		}
		else
		{
			m_offsetData.SetPositionOffsetY(val);
		}
	}
	if (vals.size() > 2)
	{
		float val = (float)atof(vals[2].c_str());
		if (DoesStringHaveThisChar(vals[2], '%'))
		{
			m_offsetData.SetPercentagePositionOffsetX(val);
		}
		else
		{
			m_offsetData.SetPositionOffsetX(val);
		}
	}
	if (vals.size() > 3)
	{
		float val = (float)atof(vals[3].c_str());
		if (DoesStringHaveThisChar(vals[3], '%'))
		{
			m_offsetData.SetPercentagePositionOffsetY(val);
		}
		else
		{
			m_offsetData.SetPositionOffsetY(val);
		}
	}
}
void UIWidgetBase::ParseStringForFontPositionalOffsetData(const std::string& attVal)
{
	std::string attrVal = attVal;
	attrVal = ReplaceCharInString(attrVal, ',', ' ');
	attrVal = ReplaceCharInString(attrVal, ':', ' ');
	attrVal = ReplaceCharInString(attrVal, ';', ' ');
	std::vector<std::string> vals = ParseString(attrVal, ' ');

	if (vals.size() > 0)
	{
		float val = (float)atof(vals[0].c_str());
		if (DoesStringHaveThisChar(vals[0], '%'))
		{
			m_fontOffsetData.SetPercentagePositionOffsetX(val);
		}
		else
		{
			m_fontOffsetData.SetPositionOffsetX(val);
		}
	}
	if (vals.size() > 1)
	{
		float val = (float)atof(vals[1].c_str());
		if (DoesStringHaveThisChar(vals[1], '%'))
		{
			m_fontOffsetData.SetPercentagePositionOffsetY(val);
		}
		else
		{
			m_fontOffsetData.SetPositionOffsetY(val);
		}
	}
	if (vals.size() > 2)
	{
		float val = (float)atof(vals[2].c_str());
		if (DoesStringHaveThisChar(vals[2], '%'))
		{
			m_fontOffsetData.SetPercentagePositionOffsetX(val);
		}
		else
		{
			m_fontOffsetData.SetPositionOffsetX(val);
		}
	}
	if (vals.size() > 3)
	{
		float val = (float)atof(vals[3].c_str());
		if (DoesStringHaveThisChar(vals[3], '%'))
		{
			m_fontOffsetData.SetPercentagePositionOffsetY(val);
		}
		else
		{
			m_fontOffsetData.SetPositionOffsetY(val);
		}
	}
}

void UIWidgetBase::ParseStringForSizeData(const std::string& attVal)
{
	std::string attrVal = attVal;
	attrVal = ReplaceCharInString(attrVal, ',', ' ');
	attrVal = ReplaceCharInString(attrVal, ':', ' ');
	attrVal = ReplaceCharInString(attrVal, ';', ' ');
	std::vector<std::string> vals = ParseString(attrVal, ' ');
	if (vals.size() > 0)
	{
		float val = (float)atof(vals[0].c_str());
		if (DoesStringHaveThisChar(vals[0], '%'))
		{
			m_sizeData.SetPercentageSizeX(val);
		}
		else
		{
			m_sizeData.SetCurrentSizeX(val);
		}
	}
	if (vals.size() > 1)
	{
		float val = (float)atof(vals[1].c_str());
		if (DoesStringHaveThisChar(vals[1], '%'))
		{
			m_sizeData.SetPercentageSizeY(val);
		}
		else
		{
			m_sizeData.SetCurrentSizeY(val);
		}
	}
	if (vals.size() > 2)
	{
		float val = (float)atof(vals[2].c_str());
		if (DoesStringHaveThisChar(vals[2], '%'))
		{
			m_sizeData.SetPercentageSizeX(val);
		}
		else
		{
			m_sizeData.SetCurrentSizeX(val);
		}
	}
	if (vals.size() > 3)
	{
		float val = (float)atof(vals[3].c_str());
		if (DoesStringHaveThisChar(vals[3], '%'))
		{
			m_sizeData.SetPercentageSizeY(val);
		}
		else
		{
			m_sizeData.SetCurrentSizeY(val);
		}
	}
}

void UIWidgetBase::ParseStringForStyleSheet(const std::string& attrVal)
{
	const UIStyleSheet* stylesheet = UIStyleSheetHandler::GlobalGetStyleSheetForName(attrVal);
	if (stylesheet == nullptr)
	{
		m_styleSheet = UIStyleSheetHandler::GlobalGetDefaultStyleSheet();
	}
	else
	{
		UIStyleSheet* nonConstStyleSheet = const_cast<UIStyleSheet*>(stylesheet);
		m_styleSheet = nonConstStyleSheet;
	}
}

void UIWidgetBase::ParseStringForAnchor(const std::string& attrVal)
{
	std::string lowerCase = MakeLower(attrVal);
	if (attrVal == "topleft")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_TopLeft);
	}
	else if (attrVal == "topcenter" || attrVal == "topmiddle")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_TopCenter);
	}
	else if (attrVal == "topright")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_TopRight);
	}
	else if (attrVal == "centerleft" || attrVal == "middleleft")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_CenterLeft);
	}
	else if (attrVal == "centercenter" || attrVal == "middlemiddle" || attrVal == "centermiddle" || attrVal == "middlecenter")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_CenterCenter);
	}
	else if (attrVal == "centerright" || attrVal == "middleright")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_CenterRight);
	}
	else if (attrVal == "bottomleft")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_BottomLeft);
	}
	else if (attrVal == "bottomcenter" || attrVal == "bottommiddle")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_BottomCenter);
	}
	else if (attrVal == "bottomright")
	{
		m_offsetData.SetUIAnchor(UIANCHOR_BottomRight);
	}

}

void UIWidgetBase::ReadAndHandleXMLNodeAttributes(const XMLNode& node)
{
	bool readAttrOnce = false;
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		ReadAndHandleXMLAttribute(attr, readAttrOnce);
	}
}

std::string UIWidgetBase::ReadAndHandleXMLAttribute(const XMLAttribute& attr, bool& readOnceAlready)
{
	readOnceAlready = false;
	std::string attrName = MakeLower(attr.lpszName);
	std::string attrVal = attr.lpszValue;
	if (attrName == "text")
	{
		readOnceAlready = true;
		m_text = attrVal;
	}
	else if (attrName == "position")
	{
		readOnceAlready = true;
		ParseStringForPositionalOffsetData(attrVal);
	}
	else if (attrName == "scale"
		|| attrName == "size")
	{
		readOnceAlready = true;
		ParseStringForSizeData(attrVal);
	}
	else if (attrName == "stylesheet")
	{
		readOnceAlready = true;
		ParseStringForStyleSheet(attrVal);
	}
	else if (attrName == "tag")
	{
		readOnceAlready = true;
		m_tag = attrVal;
	}
	else if (attrName == "uniquename" || attrName == "name")
	{
		readOnceAlready = true;
		m_uniqueName = attrVal;
	}
	else if (attrName == "anchor" || attrName == "uianchor")
	{
		readOnceAlready = true;
		ParseStringForAnchor(attrVal);
	}
	else if (attrName == "fontscale" || attrName == "localfontscale")
	{
		m_localFontScaler = (float)(atof(attrVal.c_str()));
	}
	else if (attrName == "fontoffset" || attrName == "fontposition")
	{
		readOnceAlready = true;
		ParseStringForFontPositionalOffsetData(attrVal);
	}
	else
	{
		ReadAndHandleXMLNodAttributeUIEvents(attr, readOnceAlready, attrName);
	}
	return attrName;
}

void UIWidgetBase::ReadAndHandleXMLNodeChildren(const XMLNode& node)
{
	UIStyleSheet* def = UIStyleSheetHandler::GlobalGetDefaultStyleSheet();
	UIWidgetRegistrationDataPacket dataPacket(this, def, m_rootNodeID);
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string name = MakeLower(child.getName());

		if (name == "uievents" || name == "events")
		{
			ReadAndHandleXMLNodeUIEvents(child);
			continue;
		}

		UIWidgetBase* widget = UINamedWidgetRegistration::SpawnWidgetFromNamedWidgetName(name, dataPacket, child);
		if (widget == nullptr)
		{
			widget = UIWidgetRegistration::SpawnWidgetOfType(name, child, dataPacket);
		}

		if (widget != nullptr)
		{
			m_vectorPack.m_children.push_back(widget);
		}
	}
}

void UIWidgetBase::ReadAndHandleXMLNodeUIEvents(const XMLNode& eventsNode)
{
	for (int childIdx = 0; childIdx < eventsNode.nChildNode(); childIdx++)
	{
		UIWidgetEventCallback callback;
		std::string OnEnterEvent = "";
		std::string onExitEvent = "";
		std::string onUpdateEvent = "";
		XMLNode eventChild = eventsNode.getChildNode(childIdx);
		std::string lowerCaseName = MakeLower(eventChild.getName());
		if (lowerCaseName == "highlighted" || lowerCaseName == "hovered")
		{
			lowerCaseName = "hovered";
		}

		for (int attrIdx = 0; attrIdx < eventChild.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = eventChild.getAttribute(attrIdx);
			std::string lowerCaseAttrName = MakeLower(attr.lpszName);
			if (lowerCaseAttrName == "onenter" || lowerCaseAttrName == "enter")
			{
				OnEnterEvent = attr.lpszValue;
			}
			else if (lowerCaseAttrName == "onexit" || lowerCaseAttrName == "exit")
			{
				onExitEvent = attr.lpszValue;
			}
			else if (lowerCaseAttrName == "onupdate" || lowerCaseAttrName == "update")
			{
				onUpdateEvent = attr.lpszValue;
			}
		}
		callback.ChangeEnterEventSystemCallToUse(OnEnterEvent);
		callback.ChangeExitEventSystemCallToUse(onExitEvent);
		callback.ChangeUpdateEventSystemCallToUse(onUpdateEvent);

		SetOrAddEventCallback(lowerCaseName, callback);
	}
}


void UIWidgetBase::ReadAndHandleXMLNodAttributeUIEvents(const XMLAttribute& attr, bool& readOnceAlready, const std::string& lowerCase) 
{
	/*
	m_vectorPack.m_eventCallbacks[0].SetName("active");
	m_vectorPack.m_eventCallbacks[1].SetName("hovered");
	m_vectorPack.m_eventCallbacks[2].SetName("pressed");
	m_vectorPack.m_eventCallbacks[3].SetName("disabled");
	*/
	if (lowerCase == "activeonenter" || lowerCase == "activeenter" 
		|| lowerCase == "active.onenter" || lowerCase == "active.enter")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_ACTIVE].ChangeEnterEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "activeonexit" || lowerCase == "activeexit"
		|| lowerCase == "active.onexit" || lowerCase == "active.exit")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_ACTIVE].ChangeExitEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "activeonupdate" || lowerCase == "activeupdate"
		|| lowerCase == "active.onupdate" || lowerCase == "active.update")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_ACTIVE].ChangeUpdateEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "hoveredonenter" || lowerCase == "hoveredenter"
		|| lowerCase == "hovered.onenter" || lowerCase == "hovered.enter"
		|| lowerCase == "highlightedonenter" || lowerCase == "highlightedenter" 
		|| lowerCase == "highlighted.onenter" || lowerCase == "highlighted.enter")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_HOVERED].ChangeEnterEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "hoveredonexit" || lowerCase == "hoveredexit"
		|| lowerCase == "hovered.onexit" || lowerCase == "hovered.exit"
		|| lowerCase == "highlightedonexit" || lowerCase == "hoveredexit"
		|| lowerCase == "highlighted.onexit" || lowerCase == "highlighted.exit")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_HOVERED].ChangeExitEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "hoveredonupdate" || lowerCase == "hoveredupdate"
		|| lowerCase == "hovered.onupdate" || lowerCase == "hovered.update"
		|| lowerCase == "highlightedonupdate" || lowerCase == "highlightedupdate" 
		|| lowerCase == "highlighted.onupdate" || lowerCase == "highlighted.update")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_HOVERED].ChangeUpdateEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "pressedonenter" || lowerCase == "pressedenter"
		|| lowerCase == "pressed.onenter" || lowerCase == "pressed.enter")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_PRESSED].ChangeEnterEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "pressedonexit" || lowerCase == "pressedexit"
		|| lowerCase == "pressed.onexit" || lowerCase == "pressed.exit")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_PRESSED].ChangeExitEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "pressedonupdate" || lowerCase == "pressedupdate"
		|| lowerCase == "pressed.onupdate" || lowerCase == "pressed.update")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_PRESSED].ChangeUpdateEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "disabledonenter" || lowerCase == "disabledenter" 
		|| lowerCase == "disabled.onenter" || lowerCase == "disabled.enter")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_DISABLED].ChangeEnterEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "disabledonexit" || lowerCase == "disabledexit"
		|| lowerCase == "disabled.onexit" || lowerCase == "disabled.exit")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_DISABLED].ChangeExitEventSystemCallToUse(attr.lpszValue);
	}
	else if (lowerCase == "disabledonupdate" || lowerCase == "disabledupdate" 
		|| lowerCase == "disabled.onupdate" || lowerCase == "disabled.update")
	{
		readOnceAlready = true;
		m_vectorPack.m_eventCallbacks[UIWIDGETSTATE_DISABLED].ChangeUpdateEventSystemCallToUse(attr.lpszValue);
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Updates and Render
void UIWidgetBase::Update(float deltaSeconds)
{
	if (m_currentWidgetState >= 0 && m_currentWidgetState < (int)m_vectorPack.m_eventCallbacks.size())
	{
		m_vectorPack.m_eventCallbacks[m_currentWidgetState].CallUpdateEvent(this);
	}
	if (m_manualOverrideBackgroundImage && m_manualOverridedBackgroundImage == nullptr)
	{
		m_manualOverrideBackgroundImage = false;
		m_boolPack.m_StyleSheetChanged = true;
	}
	SizeUpdate(deltaSeconds);
	PositionUpdate(deltaSeconds);
	SpriteSheetUpdate(deltaSeconds);
	UpdateText(deltaSeconds);
	UpdateChildren(deltaSeconds);
	UpdateModel(deltaSeconds);
	m_boolPack.m_stateChanged = false;
}

void UIWidgetBase::UpdateChildren(float deltaSeconds)
{
	for (size_t childIdx = 0; childIdx <m_vectorPack.m_children.size(); childIdx++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[childIdx];
		if (child != nullptr)
		{
			child->Update(deltaSeconds);
		}
	}
}

void UIWidgetBase::MouseUpdate(const Vector2& mouseVirtualCoords, bool mouseCurrentlyDown, bool mouseWasDownLastFrame)
{
	Vector2 halfSize = m_currentSize * 0.5f;
	Vector2 bl = m_currentOffset - halfSize;
	Vector2 tr = m_currentOffset + halfSize;

	if (mouseVirtualCoords.x >= bl.x && mouseVirtualCoords.x < tr.x
		&& mouseVirtualCoords.y >= bl.y && mouseVirtualCoords.y < tr.y)
	{
		m_boolPack.m_currentlyHovered = true;
		if (m_currentWidgetState != UIWIDGETSTATE_DISABLED
			&& m_currentWidgetState != UIWIDGETSTATE_HIDDEN)
		{
			if (mouseCurrentlyDown)
			{
				ChangeState(UIWIDGETSTATE_PRESSED);
			}
			else
			{
				ChangeState(UIWIDGETSTATE_HOVERED);
			}
		}
	}
	else
	{
		m_boolPack.m_currentlyHovered = false;
		if (m_currentWidgetState != UIWIDGETSTATE_DISABLED
			&& m_currentWidgetState != UIWIDGETSTATE_HIDDEN)
		{
			ChangeState(UIWIDGETSTATE_ACTIVE);
		}
	}

	for (size_t childIdx = 0; childIdx < m_vectorPack.m_children.size(); childIdx++)
	{
		m_vectorPack.m_children[childIdx]->MouseUpdate(mouseVirtualCoords, mouseCurrentlyDown, mouseWasDownLastFrame);
	}
}

void UIWidgetBase::Render() const
{
	if (m_boolPack.m_Render
		&& m_currentWidgetState != UIWIDGETSTATE_HIDDEN
		&& m_boolPack.m_amANamedWidget == false)
	{
		RenderText();
		RenderBackground();
		RenderMargin();
	}
	RenderChildren();
}

void UIWidgetBase::RenderText() const
{
	for (size_t pageIdx = 0; pageIdx < m_textMesh.size(); pageIdx++)
	{
		const Mesh& mesh = m_textMesh[pageIdx];
		const Material& mat = m_TextMat[pageIdx];
		mat.SetModelMatrix(m_FontModelMatrix);
		mesh.DrawWith(&mat);
	}
}

void UIWidgetBase::RenderMargin() const
{
	if (m_MarginMat != nullptr)
	{
		m_MarginMat->SetModelMatrix(m_ModelMatrix);
		m_marginMesh.DrawWith(m_MarginMat);
	}
}

void UIWidgetBase::RenderBackground() const
{
	if (m_BackgroundMat != nullptr)
	{
		m_BackgroundMat->SetModelMatrix(m_ModelMatrix);
		m_backgroundMesh.DrawWith(m_BackgroundMat);
	}
}

void UIWidgetBase::RenderChildren() const
{
	if (m_vectorPack.m_children.size() == 0
		|| m_boolPack.m_RenderChildren == false
		|| m_currentWidgetState == UIWIDGETSTATE_HIDDEN)
	{
		return;
	}

	for (int childIdx = (int)m_vectorPack.m_children.size() - 1; childIdx >= 0; childIdx--)
	{
		UIWidgetBase* child = m_vectorPack.m_children[childIdx];
		if(child != nullptr)
		{
			child->Render();
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Protected Updates
void UIWidgetBase::SizeUpdate(float deltaSeconds)
{
	deltaSeconds;
	if (m_boolPack.m_SizeChanged == false
		&& m_boolPack.m_stateChanged == false)
	{
		return;
	}
	m_boolPack.m_SizeChanged = false;
	m_boolPack.m_UpdateMesh = true;


	m_sizeData.RecalculateSize();
	m_currentSize = m_sizeData.GetTrueSize();
	{
		MeshBuilder builder;
		builder.BuildQuad(Vector3(0.f, 1.f), Vector3(1.f, 0.f), m_currentSize.x, m_currentSize.y, 1, 1, Rgba::s_White);
		builder.GetVertices(m_vectorPack.m_BackgroundModel);//m_opacity
		builder.GetVertices(m_vectorPack.m_MarginModel);//m_opacity
		builder.GetIndices(m_vectorPack.m_BackgroundIndices);
		builder.GetIndices(m_vectorPack.m_MarginIndices);
	}

	for (size_t vertIdx = 0; vertIdx < m_vectorPack.m_BackgroundModel.size(); vertIdx++)
	{
		Vertex_PCT& vert = m_vectorPack.m_BackgroundModel[vertIdx];
		vert.m_pos.y = (vert.m_pos.y / abs(vert.m_pos.y)) * (m_currentSize.y * 0.5f);
		vert.m_pos.x = (vert.m_pos.x / abs(vert.m_pos.x)) * (m_currentSize.x * 0.5f);
	}

	Vector2 bl = m_sizeData.GetMarginBL();
	Vector2 tr = m_sizeData.GetMarginTR();
	for (size_t vertIdx = 0; vertIdx < m_vectorPack.m_MarginModel.size(); vertIdx++)
	{
		Vertex_PCT& vert = m_vectorPack.m_MarginModel[vertIdx];
		
		if (vert.m_pos.y > 0.f)
		{
			//top
			vert.m_pos.y = m_vectorPack.m_BackgroundModel[vertIdx].m_pos.y + tr.y;
		}
		else
		{
			//bottom
			vert.m_pos.y = m_vectorPack.m_BackgroundModel[vertIdx].m_pos.y - bl.y;
		}

		if (vert.m_pos.x > 0.f)
		{
			//right
			vert.m_pos.x = m_vectorPack.m_BackgroundModel[vertIdx].m_pos.x + tr.x;
		}
		else
		{
			//left
			vert.m_pos.x = m_vectorPack.m_BackgroundModel[vertIdx].m_pos.x - bl.x;
		}
	}
}

void UIWidgetBase::PositionUpdate(float deltaSeconds)
{
	deltaSeconds;
	if (m_boolPack.m_PositionChanged == false
		&& m_boolPack.m_stateChanged == false)
	{
		return;
	}
	m_boolPack.m_PositionChanged = false;
	m_boolPack.m_UpdateMesh = true;

	if (m_boolPack.m_OffsetSetByParent == false)
	{
		m_offsetData.RecalculateTruePosition();
	}
	m_fontOffsetData.RecalculateTruePosition();
	m_currentOffset = m_offsetData.GetTruePosition();
	m_ModelMatrix.MatrixMakeIdentity();
	m_FontModelMatrix.MatrixMakeIdentity();

	Matrix44::MakeEulerRotation(m_ModelMatrix, 0.f, 0.f, 0.f, m_currentOffset);
	Matrix44::MakeEulerRotation(m_FontModelMatrix, 0.f, 0.f, 0.f, m_currentOffset);

}

void UIWidgetBase::SpriteSheetUpdate(float deltaSeconds)
{
	if (m_styleSheet == nullptr)
	{
		Font* fontTest = m_font;
		if (m_fontUsing != fontTest)
		{
			m_fontUsing = fontTest;
			m_boolPack.m_UpdateText = true;
		}
		return;
	}
	deltaSeconds;
	if (m_boolPack.m_StyleSheetChanged == false
		&& m_boolPack.m_stateChanged == false)
	{
		if (m_styleSheet->GetStyleSheetChanged() == false)
		{
			return;
		}
	}
	m_boolPack.m_StyleSheetChanged = false;
	m_boolPack.m_UpdateMesh = true;
	m_boolPack.m_UpdateText = true;
	
	float realOpacity = 0.f;
	if (m_currentWidgetState != UIWIDGETSTATE_HIDDEN)
	{
		realOpacity = m_opacity;
		UISystem* system = UISystem::GetUISystemPtr();
		if (system == nullptr)
		{
			return;
		}
		const Texture* texture = system->GetDefaultBackgroundImageForWidgets();
		ShaderProgram* prog = m_prog;
		if (prog == nullptr)
		{
			prog = system->GetDefaultShaderProgram();
		}
		Rgba m_tint = Rgba::s_White;
		if (m_styleSheet != nullptr)
		{
			const UIStyleWidgetData* data = m_styleSheet->GetStyleDataForWidgetType(m_widgetName);
			if (data == nullptr)
			{
				data = m_styleSheet->GetStyleDataForWidgetType("default");
				ERROR_AND_DIE("Why does a style sheet have no default data?");
			}

			UIStateAppearanceData appData = data->GetAppearanceDataForIdx(m_currentWidgetState);

			m_colorPack.m_fontColor = appData.GetFontColor();
			m_colorPack.m_marginColor = appData.GetBackgroundMarginColor();
			m_colorPack.m_backgroundColor = appData.GetBackgroundColor();
			if (m_font == nullptr)
			{
				Font* fontTest = appData.GetFont();
				if (m_fontUsing != fontTest)
				{
					m_fontUsing = fontTest;
					m_boolPack.m_UpdateText = true;
				}
			}
			else
			{
				Font* fontTest = m_font;
				if (m_fontUsing != fontTest)
				{
					m_fontUsing = fontTest;
					m_boolPack.m_UpdateText = true;
				}
			}
			m_font = appData.GetFont();
			if (m_font == nullptr)
			{
				//Gonna want a default on the UISystem for this.
			}

			m_BackgroundMat->SetProgram(prog);
			m_MarginMat->SetProgram(prog);
			m_BackgroundMat->SetOrAddTexture(GetPreferredBackgroundImage(), 0, s_defaultImageBindName, 0);
			m_MarginMat->SetOrAddTexture(GetPreferredMarginImage(), 0, s_defaultImageBindName, 0);
			m_fontSize = appData.GetFontSize();
		}
		else
		{
			m_colorPack.m_fontColor = Rgba::s_Black;
			m_colorPack.m_marginColor = Rgba::s_White;
			m_colorPack.m_backgroundColor = Rgba::s_White;
			m_BackgroundMat->SetProgram(prog);
			m_MarginMat->SetProgram(prog);
			m_BackgroundMat->SetOrAddTexture(texture, 0, s_defaultImageBindName, 0);
			m_MarginMat->SetOrAddTexture(texture, 0, s_defaultImageBindName, 0);
		}
	}

	for (size_t vertIdx = 0; vertIdx < m_vectorPack.m_MarginModel.size(); vertIdx++)
	{
		m_vectorPack.m_MarginModel[vertIdx].m_color = Rgba(m_colorPack.m_marginColor.Red,
			m_colorPack.m_marginColor.Green, 
			m_colorPack.m_marginColor.Blue, 
			m_colorPack.m_marginColor.Alpha * realOpacity);
		m_vectorPack.m_BackgroundModel[vertIdx].m_color = Rgba(m_colorPack.m_backgroundColor.Red,
			m_colorPack.m_backgroundColor.Green, 
			m_colorPack.m_backgroundColor.Blue, 
			m_colorPack.m_backgroundColor.Alpha * realOpacity);
	}
}

void UIWidgetBase::UpdateText(float deltaSeconds)
{
	deltaSeconds;
	if (m_boolPack.m_UpdateText == false
		&& m_boolPack.m_stateChanged == false)
	{
		return;
	}
	m_boolPack.m_UpdateText = false;
	m_TextModel.clear();
	m_TextIndices.clear();

	if (m_fontUsing != nullptr)
	{
		m_fontUsing->GetTextToRender(m_TextModel, m_TextIndices, Vector3::vec3_zeros, m_text, m_fontSize * m_localFontScaler, Vector3(1.f),
			Vector3(0.f, 1.f), Rgba(m_colorPack.m_fontColor.GetRGB(), m_colorPack.m_fontColor.Alpha * m_fontOpacity), Font_Center_Aligned);
	}

	m_TextMat.resize(m_TextModel.size());
	m_textMesh.resize(m_TextModel.size());
	for (size_t pageIdx = 0; pageIdx < m_TextModel.size(); pageIdx++)
	{
		m_TextMat[pageIdx].SetProgram(m_prog);
		m_TextMat[pageIdx].SetOrAddTexture(m_fontUsing->RequestPage(pageIdx), 0, "gDiffuseTex", 0);
		m_textMesh[pageIdx].SetDrawMode(PRIMITIVE_TRIANGLES);
		m_textMesh[pageIdx].setVBO(m_TextModel[pageIdx]);
		m_textMesh[pageIdx].setIBO(m_TextIndices[pageIdx]);
	}
}

void UIWidgetBase::UpdateModel(float deltaSeconds)
{
	deltaSeconds;
	if (m_boolPack.m_UpdateMesh == false
		&& m_boolPack.m_stateChanged == false)
	{
		return;
	}
	m_boolPack.m_UpdateMesh = false;

	m_marginMesh.setVBO(m_vectorPack.m_MarginModel);
	m_marginMesh.setIBO(m_vectorPack.m_MarginIndices);

	m_backgroundMesh.setVBO(m_vectorPack.m_BackgroundModel);
	m_backgroundMesh.setIBO(m_vectorPack.m_BackgroundIndices);
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Event Callbacks

/*

m_vectorPack.m_eventCallbacks[0].SetName("active");
m_vectorPack.m_eventCallbacks[1].SetName("hovered");
m_vectorPack.m_eventCallbacks[2].SetName("pressed");
m_vectorPack.m_eventCallbacks[3].SetName("disabled");
*/

void UIWidgetBase::SetOrAddEventCallback(const std::string eventName, UIWidgetEventCallback& callback)
{
	if (SetEventCallback(eventName, callback) == false)
	{
		AddEventCallback(eventName, callback);
	}
}

bool UIWidgetBase::SetEventCallback(const std::string eventName, UIWidgetEventCallback& callback)
{
	std::string lowerCase = MakeLower(eventName);
	callback.SetName(lowerCase);

	for (size_t eventIdx = 0; eventIdx < m_vectorPack.m_eventCallbacks.size(); eventIdx++)
	{
		UIWidgetEventCallback& curCallback = m_vectorPack.m_eventCallbacks[eventIdx];
		if (curCallback.GetName() == lowerCase)
		{
			m_vectorPack.m_eventCallbacks[eventIdx] = callback;
			return true;
		}
	}
	return false;
}

void UIWidgetBase::AddEventCallback(const std::string eventName, UIWidgetEventCallback& callback)
{
	std::string lowerCase = MakeLower(eventName);
	callback.SetName(lowerCase);
	m_vectorPack.m_eventCallbacks.push_back(callback);
}

void UIWidgetBase::RemoveCallback(const std::string eventName)
{
	std::string lowerCase = MakeLower(eventName);
	for (size_t eventIdx = 0; eventIdx < m_vectorPack.m_eventCallbacks.size(); eventIdx++)
	{
		UIWidgetEventCallback& callback = m_vectorPack.m_eventCallbacks[eventIdx];
		if (callback.GetName() == lowerCase)
		{
			if (eventIdx < UIWIDGETSTATE_NUM_TO_HOLD_DATA_FOR)
			{
				callback.ChangeEnterEventSystemCallToUse("");
				callback.ChangeExitEventSystemCallToUse("");
				callback.ChangeUpdateEventSystemCallToUse("");
			}
			else
			{
				m_vectorPack.m_eventCallbacks.erase(m_vectorPack.m_eventCallbacks.begin() + eventIdx);
			}
			return;
		}
	}
}

void UIWidgetBase::ChangeState(int stateID)
{
	if (m_currentWidgetState != stateID)
	{
		if (m_currentWidgetState >= 0 && m_currentWidgetState < (int)m_vectorPack.m_eventCallbacks.size())
		{
			m_vectorPack.m_eventCallbacks[m_currentWidgetState].CallExitEvent(this);
		}
		m_colorPack.m_fontColorWhenOriginallyChangedState = m_colorPack.m_fontColor;
		m_colorPack.m_marginColorWhenOriginallyChangedState = m_colorPack.m_marginColor;
		m_colorPack.m_backgroundColorWhenOriginallyChangedState = m_colorPack.m_backgroundColor;
		m_boolPack.m_stateChanged = true;
		m_previousWidgetState = m_currentWidgetState;
		m_currentWidgetState = stateID;
		if (m_currentWidgetState >= 0 && m_currentWidgetState < (int)m_vectorPack.m_eventCallbacks.size())
		{
			m_vectorPack.m_eventCallbacks[m_currentWidgetState].CallEnterEvent(this);
		}
	}
}

void UIWidgetBase::ChangeState(UIWidgetStates stateID)
{
	ChangeState((int)stateID);
}


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Setters
void UIWidgetBase::SetManualBackgroundImage(bool overrideImage, const Texture* backgroundImage)
{
	m_manualOverrideBackgroundImage = overrideImage;
	m_manualOverridedBackgroundImage = backgroundImage;
}

void UIWidgetBase::ShowHideWidgetByTag(const std::string& tag, bool showElseHide, bool effectChildren)
{
	if (m_tag == tag)
	{
		SetAbleToRender(showElseHide, effectChildren);
		if (effectChildren)
		{
			return;
		}
	}

	for (size_t i = 0; i < m_vectorPack.m_children.size(); i++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[i];
		if (child != nullptr)
		{
			child->ShowHideWidgetByTag(tag, showElseHide, effectChildren);
		}
	}
}

void UIWidgetBase::ShowHideWidgetByWidgetType(const std::string& tag, bool showElseHide, bool effectChildren)
{
	if (m_widgetName == tag)
	{
		SetAbleToRender(showElseHide, effectChildren);
		if (effectChildren)
		{
			return;
		}
	}

	for (size_t i = 0; i < m_vectorPack.m_children.size(); i++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[i];
		if (child != nullptr)
		{
			child->ShowHideWidgetByWidgetType(tag, showElseHide, effectChildren);
		}
	}
}

void UIWidgetBase::ShowHideWidgetByUniqueName(const std::string& tag, bool showElseHide, bool effectChildren)
{
	if (m_uniqueName == tag)
	{
		SetAbleToRender(showElseHide, effectChildren);
		if (effectChildren)
		{
			return;
		}
	}

	for (size_t i = 0; i < m_vectorPack.m_children.size(); i++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[i];
		if (child != nullptr)
		{
			child->ShowHideWidgetByUniqueName(tag, showElseHide, effectChildren);
		}
	}
}

void UIWidgetBase::SetTrueLocalPositionBasedOnGroupWidgetInput(const Vector2& position)
{
	if (m_boolPack.m_OffsetSetByParent == false)
	{
		return;
	}
	m_offsetData.RecalculateTruePositionBasedOffOfRelativeGroupWidgetGivenPosition(position);
}

void UIWidgetBase::SetTrueLocalPosition(const Vector2& position)
{
	m_offsetData.SetTruePosition(position);
}

void UIWidgetBase::SetLocalFontScale(float fontScale)
{
	m_localFontScaler = fontScale;
}

void UIWidgetBase::SetAbleToRender(bool render, bool renderChildren)
{
	m_boolPack.m_Render = render;
	m_boolPack.m_RenderChildren = renderChildren;
}

void UIWidgetBase::SetUniqueName(const std::string& name)
{
	m_uniqueName = name;
}

void UIWidgetBase::SetTag(const std::string& tag)
{
	m_tag = tag;
}


void UIWidgetBase::AddChildWidget(UIWidgetBase* child)
{
	//This adds a child widget to the current widget, and notifies the child that it has a parent.
	//If the original parent is not null, then it also notifies the original parent that it no longer owns that child.
	bool added = AddChildWidgetNoFeedback(child);
	if (added)
	{
		UIWidgetBase* orgParent = child->SetParentWidgetNoFeedback(this);
		if (orgParent != nullptr)
		{
			orgParent->RemoveChildWidgetNoFeedback(child);
		}
	}
}

void UIWidgetBase::RemoveChildWidget(UIWidgetBase* child)
{
	//This removes a child widget from the current widget, and notified the child that it has no parent.
	bool found = RemoveChildWidgetNoFeedback(child);

	if (found)
	{
		child->SetParentWidgetNoFeedback(nullptr); //orgParent would be this node. So no need to do more.
	}
}

void UIWidgetBase::SetParentWidget(UIWidgetBase* parent)
{
	//This is to set this widget's parent to the correct widget; its also important that the parents be kept up to date.
	if (m_parent != nullptr)
	{
		m_parent->RemoveChildWidgetNoFeedback(this);
	}
	m_parent = parent;
	if (m_parent != nullptr)
	{
		m_parent->AddChildWidgetNoFeedback(this);
	}
	m_boolPack.m_StyleSheetChanged = true;
	m_boolPack.m_PositionChanged = true;
	m_boolPack.m_SizeChanged = true;
	m_boolPack.m_UpdateMesh = true;
	m_boolPack.m_UpdateText = true;
}

void UIWidgetBase::SetOpacity(float opacity)
{
	m_opacity = opacity;
	m_boolPack.m_StyleSheetChanged = true;
	m_boolPack.m_UpdateMesh = true;
}

void UIWidgetBase::SetUpdateStyleSheet(bool updateStyleSheet)
{
	m_boolPack.m_StyleSheetChanged = updateStyleSheet;
}

void UIWidgetBase::SetUpdatePosition(bool updatePosition)
{
	m_boolPack.m_PositionChanged = updatePosition;
}

void UIWidgetBase::SetUpdateSize(bool updateSize)
{
	m_boolPack.m_SizeChanged = updateSize;
}

void UIWidgetBase::SetUpdateMesh(bool updateMesh)
{
	m_boolPack.m_UpdateMesh = updateMesh;
}

void UIWidgetBase::SetUpdateText(bool updateText)
{
	m_boolPack.m_UpdateText = updateText;
}

void UIWidgetBase::SetRootNodeID(size_t id)
{
	m_rootNodeID = id;
	for (size_t childIdx = 0; childIdx < m_vectorPack.m_children.size(); childIdx++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[childIdx];
		if (child != nullptr)
		{
			m_vectorPack.m_children[childIdx]->SetRootNodeID(id);
		}
	}
}

void UIWidgetBase::SetPosition(const Vector2& pos)
{
	m_offsetData.SetPositionOffset(pos);
	m_boolPack.m_PositionChanged = true;
}

void UIWidgetBase::SetPercentagePosition(const Vector2& percentPos)
{
	m_offsetData.SetPercentagePosition(percentPos);
	m_boolPack.m_PositionChanged = true;
}

void UIWidgetBase::SetSize(const Vector2& size)
{
	m_sizeData.SetCurrentSize(size);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetPercentageSize(const Vector2& size)
{
	m_sizeData.SetPercentageSize(size);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetMarginSize(const Vector2& blSize, const Vector2& trSize)
{
	m_sizeData.SetMarginBL(blSize);
	m_sizeData.SetMarginTR(trSize);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetMarginBottomLeftSize(const Vector2& blSize)
{
	m_sizeData.SetMarginBL(blSize);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetMarginTopRightSize(const Vector2& trSize)
{
	m_sizeData.SetMarginTR(trSize);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetMarginBottomSize(float marginBottom)
{
	m_sizeData.SetMarginBottom(marginBottom);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetMarginTopSize(float marginTop)
{
	m_sizeData.SetMarginTop(marginTop);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetMarginLeftSize(float marginLeft)
{
	m_sizeData.SetMarginLeft(marginLeft);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetMarginRightSize(float marginRight)
{
	m_sizeData.SetMarginRight(marginRight);
	m_boolPack.m_SizeChanged = true;
}

void UIWidgetBase::SetPositionHandledByParent(bool positionHandledByParent)
{
	m_boolPack.m_OffsetSetByParent = positionHandledByParent;
}


void UIWidgetBase::DeleteAllChildren()
{
	for (size_t i = 0; i < m_vectorPack.m_children.size(); i++)
	{
		delete m_vectorPack.m_children[i];
		m_vectorPack.m_children[i] = nullptr;
	}
	m_vectorPack.m_children.clear();
}

std::vector<UIWidgetBase*> UIWidgetBase::ClearAndGetAllChildren()
{
	std::vector<UIWidgetBase*> copyOfPtrs = m_vectorPack.m_children;

	m_vectorPack.m_children.clear();
	return copyOfPtrs;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Protected Setters
bool UIWidgetBase::AddChildWidgetNoFeedback(UIWidgetBase* child)
{
	//Basically this adds a child node without communicating to any parent or children nodes.
	UIWidgetBase* currentNode = nullptr;
	for (size_t childIdx = 0; childIdx < m_vectorPack.m_children.size(); childIdx++)
	{
		currentNode = m_vectorPack.m_children[childIdx];
		if (child == currentNode)
		{
			return false;
		}
	}

	m_vectorPack.m_children.push_back(child);
	return true;
}

bool UIWidgetBase::RemoveChildWidgetNoFeedback(UIWidgetBase* child)
{
	//This removes a child node without communicating to the parent or children.
	UIWidgetBase* currentNode = nullptr;
	for (size_t childIdx = 0; childIdx < m_vectorPack.m_children.size(); childIdx++)
	{
		currentNode = m_vectorPack.m_children[childIdx];
		if (child == currentNode)
		{
			m_vectorPack.m_children.erase(m_vectorPack.m_children.begin() + childIdx);
			return true;
		}
	}
	return false;
}

UIWidgetBase* UIWidgetBase::SetParentWidgetNoFeedback(UIWidgetBase* parent)
{
	UIWidgetBase* orgParent = m_parent;
	m_parent = parent;
	return orgParent;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Getters

const std::string UIWidgetBase::GetUniqueName() const
{
	return m_uniqueName;
}

const std::string UIWidgetBase::GetTag() const
{
	return m_tag;
}

const std::string UIWidgetBase::GetText() const
{
	return m_text;
}

const std::string UIWidgetBase::GetWidgetName() const
{
	return m_uniqueName;
}

const UIWidgetBase* UIWidgetBase::GetParentNode() const
{
	return m_parent;
}

UIWidgetBase* UIWidgetBase::GetEditableParentNode() const
{
	return m_parent;
}

const std::vector<UIWidgetBase*>* UIWidgetBase::GetChildren() const
{
	return &m_vectorPack.m_children;
}

std::vector<UIWidgetBase*>* UIWidgetBase::GetEditableChildren()
{
	return &m_vectorPack.m_children;
}

const XMLNode UIWidgetBase::GetXMLNodeReadFrom() const
{
	return m_xmlNodeCopy;
}

const bool UIWidgetBase::GetReadFromXMLNode() const
{
	return m_boolPack.m_readFromXMLNode;
}

Vector2 UIWidgetBase::GetCurrentPosition() const
{
	return m_offsetData.GetTruePosition();
}

Vector2 UIWidgetBase::GetCurrentSize() const
{
	return m_sizeData.GetTrueSize();
}

UISizeData UIWidgetBase::GetSizeData() const
{
	return m_sizeData;
}

UIOffsetData UIWidgetBase::GetOffsetData() const
{
	return m_offsetData;
}

const Texture* UIWidgetBase::GetPreferredMarginImage() const
{
	if (m_styleSheet == nullptr)
	{
		return nullptr;
	}

	const UIStyleWidgetData* data = m_styleSheet->GetStyleDataForWidgetType(m_widgetName);
	if (data == nullptr)
	{
		data = m_styleSheet->GetStyleDataForWidgetType("default");
		if (data == nullptr)
		{
			return nullptr;
		}
	}

	UIStateAppearanceData appData = data->GetAppearanceDataForIdx(m_currentWidgetState);
	return appData.GetBackgroundImage();
}

const Texture* UIWidgetBase::GetPreferredBackgroundImage() const
{
	if (m_manualOverrideBackgroundImage)
	{
		return m_manualOverridedBackgroundImage;
	}
	if (m_styleSheet == nullptr)
	{
		return nullptr;
	}

	const UIStyleWidgetData* data = m_styleSheet->GetStyleDataForWidgetType(m_widgetName);
	if (data == nullptr)
	{
		data = m_styleSheet->GetStyleDataForWidgetType("default");
		if (data == nullptr)
		{
			return nullptr;
		}
	}

	UIStateAppearanceData appData = data->GetAppearanceDataForIdx(m_currentWidgetState);
	return appData.GetBackgroundImage();
}

int UIWidgetBase::GetCurrentState() const
{
	return m_currentWidgetState;
}

int UIWidgetBase::GetPreviousState() const
{
	return m_currentWidgetState;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Static Functions
STATIC void UIWidgetBase::ClearAllRegisterWidgetData()
{
	s_widgetPtrs.clear();
	s_widgetPtrs.shrink_to_fit();
}

STATIC void UIWidgetBase::RegisterWidget(UIWidgetBase* widget)
{
	for (size_t i = 0; i < s_widgetPtrs.size(); i++)
	{
		if (s_widgetPtrs[i] == widget)
		{
			return;
		}
	}
	s_widgetPtrs.push_back(widget);
}

STATIC void UIWidgetBase::UnregisterWidget(UIWidgetBase* widget)
{
	for (size_t i = 0; i < s_widgetPtrs.size(); i++)
	{
		if (s_widgetPtrs[i] == widget)
		{
			s_widgetPtrs.erase(s_widgetPtrs.begin() + i);
			return;
		}
	}
}