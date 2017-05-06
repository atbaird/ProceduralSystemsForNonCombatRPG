#pragma once

#ifndef UIWIDGETBASE_HPP
#define UIWIDGETBASE_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/UI/UIStructs/UISizeData.hpp"
#include "Engine/UI/UIStructs/UIOffsetData.hpp"
#include "Engine/UI/UIStructs/UIStyleSheet.hpp"
#include "Engine/UI/UIStructs/UIWidgetEventCallback.hpp"
#include "Engine/UI/UIStructs/UIWidgetBoolPack.hpp"
#include "Engine/UI/UIStructs/UIWidgetColorPack.hpp"
#include "Engine/UI/UIStructs/UIWidgetVectorPack.hpp"

enum UIEventIdxs
{
	ON_HIDDEN,
	ON_ACTIVE,
	ON_PRESSED,
	ON_UNPRESSED,
	ON_HOVERED,
	ON_UNHOVERED,
	NUM_OF_UIEVENTIDXS
};

class UIWidgetBase
{
private:
protected:
	//static variables
	static std::vector<UIWidgetBase*> s_widgetPtrs;
	static const char* s_defaultImageBindName;

	//variables
	UIWidgetVectorPack m_vectorPack;
	std::vector<std::vector<Vertex_PCT>> m_TextModel;
	std::vector<std::vector<unsigned int>> m_TextIndices;
	Matrix44 m_ModelMatrix;
	Matrix44 m_FontModelMatrix;
	Material* m_BackgroundMat = nullptr;
	Material* m_MarginMat = nullptr;
	std::vector<Material> m_TextMat;
	UISizeData m_sizeData;
	UIOffsetData m_offsetData;
	UIOffsetData m_fontOffsetData;
	Vector2 m_currentOffset = Vector2::vec2_zeros;
	Vector2 m_currentSize = Vector2::vec2_ones;
	std::string m_widgetName = "default";
	std::string m_uniqueName = "default";
	std::string m_text = "";
	std::string m_tag = "";
	float m_opacity = 1.f;
	float m_fontOpacity = 1.f;
	float m_fontSize = 0.01f;
	float m_localFontScaler = 1.f;
	ShaderProgram* m_prog = nullptr;
	UIStyleSheet* m_styleSheet = nullptr;
	UIWidgetBase* m_parent = nullptr;

	Mesh m_backgroundMesh;
	Mesh m_marginMesh;
	std::vector<Mesh> m_textMesh;
	UIWidgetColorPack m_colorPack;
	XMLNode m_xmlNodeCopy;
	Font* m_font = nullptr;
	Font* m_fontUsing = nullptr;
	const Texture* m_manualOverridedBackgroundImage = nullptr;
	size_t m_rootNodeID = 0;
	int m_currentWidgetState = (int)UIWIDGETSTATE_ACTIVE;
	int m_previousWidgetState = (int)UIWIDGETSTATE_ACTIVE;
	UIWidgetBoolPack m_boolPack;
	bool m_manualOverrideBackgroundImage = false;
public:
	//Constructors
	UIWidgetBase(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr, const std::string& widgetName = "default", bool amGroupingWidget = false, size_t additionalEventsToPrealloc = 0);
	UIWidgetBase(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr, const std::string& widgetName = "default", bool amGroupingWidget = false, size_t additionalEventsToPrealloc = 0);
	virtual ~UIWidgetBase();
	virtual void ConstructorCommons(size_t additionalEventsToPrealloc);


	//Read Write XML
	virtual void ReadFromXMLNode(const XMLNode& node);
	virtual void ParseStringForPositionalOffsetData(const std::string& attrVal);
	virtual void ParseStringForFontPositionalOffsetData(const std::string& attrVal);
	virtual void ParseStringForSizeData(const std::string& attrVal);
	virtual void ParseStringForStyleSheet(const std::string& attrVal);
	virtual void ParseStringForAnchor(const std::string& attrVal);
protected:
	virtual void ReadAndHandleXMLNodeAttributes(const XMLNode& node);
	virtual std::string ReadAndHandleXMLAttribute(const XMLAttribute& attr, bool& readOnceAlready); //returns the lowercase string
	virtual void ReadAndHandleXMLNodeChildren(const XMLNode& node);
	virtual void ReadAndHandleXMLNodeUIEvents(const XMLNode& eventsNode);
	virtual void ReadAndHandleXMLNodAttributeUIEvents(const XMLAttribute& attr, bool& readOnceAlready, const std::string& lowerCase);
	/*
	m_vectorPack.m_eventCallbacks[0].SetName("active");
	m_vectorPack.m_eventCallbacks[1].SetName("hovered");
	m_vectorPack.m_eventCallbacks[2].SetName("pressed");
	m_vectorPack.m_eventCallbacks[3].SetName("disabled");
	*/

public:
	//Updates and Render
	virtual void Update(float deltaSeconds);
	virtual void UpdateChildren(float deltaSeconds);
	virtual void MouseUpdate(const Vector2& mouseVirtualCoords, bool mouseCurrentlyDown, bool mouseWasDownLastFrame);
	virtual void Render() const;
	virtual void RenderText() const;
	virtual void RenderMargin() const;
	virtual void RenderBackground() const;
	virtual void RenderChildren() const;
	virtual void SizeUpdate(float deltaSeconds);

	//Protected Updates
protected:
	virtual void PositionUpdate(float deltaSeconds);
	virtual void SpriteSheetUpdate(float deltaSeconds);
	virtual void UpdateText(float deltaSeconds);
	virtual void UpdateModel(float deltaSeconds);

public:
	//Event Callbacks

	void SetOrAddEventCallback(const std::string eventName, UIWidgetEventCallback& callback);
	bool SetEventCallback(const std::string eventName, UIWidgetEventCallback& callback);
	void AddEventCallback(const std::string eventName, UIWidgetEventCallback& callback);
	void RemoveCallback(const std::string eventName);

	void ChangeState(int stateID);
	void ChangeState(UIWidgetStates stateID);

	//Setters
	void SetManualBackgroundImage(bool overrideImage, const Texture* backgroundImage);
	void ShowHideWidgetByTag(const std::string& tag, bool showElseHide, bool effectChildren);
	void ShowHideWidgetByWidgetType(const std::string& tag, bool showElseHide, bool effectChildren);
	void ShowHideWidgetByUniqueName(const std::string& tag, bool showElseHide, bool effectChildren);
	void SetTrueLocalPositionBasedOnGroupWidgetInput(const Vector2& position);
	void SetTrueLocalPosition(const Vector2& position);
	void SetLocalFontScale(float fontScale);
	void SetAbleToRender(bool render, bool renderChildren = true);
	void SetUniqueName(const std::string& name);
	void SetTag(const std::string& tag);
	void AddChildWidget(UIWidgetBase* child);
	void RemoveChildWidget(UIWidgetBase* child);
	void SetParentWidget(UIWidgetBase* parent);
	void SetOpacity(float opacity);
	void SetUpdateStyleSheet(bool updateStyleSheet);
	void SetUpdatePosition(bool updatePosition);
	void SetUpdateSize(bool updateSize);
	void SetUpdateMesh(bool updateMesh);
	void SetUpdateText(bool updateText);
	void SetRootNodeID(size_t id);
	void SetPosition(const Vector2& pos);
	void SetPercentagePosition(const Vector2& percentPos);
	void SetSize(const Vector2& size);
	void SetPercentageSize(const Vector2& size);
	void SetMarginSize(const Vector2& blSize, const Vector2& trSize);
	void SetMarginBottomLeftSize(const Vector2& blSize);
	void SetMarginTopRightSize(const Vector2& trSize);
	void SetMarginBottomSize(float marginBottom);
	void SetMarginTopSize(float topMargin);
	void SetMarginLeftSize(float leftMargin);
	void SetMarginRightSize(float rightMargin);
	void SetPositionHandledByParent(bool positionHandledByParent);

	void DeleteAllChildren();
	std::vector<UIWidgetBase*> ClearAndGetAllChildren();

protected:
	//Protected Setters
	bool AddChildWidgetNoFeedback(UIWidgetBase* child);
	bool RemoveChildWidgetNoFeedback(UIWidgetBase* child);
	UIWidgetBase* SetParentWidgetNoFeedback(UIWidgetBase* parent);

public:

	//Getters
	const std::string GetUniqueName() const;
	const std::string GetTag() const;
	const std::string GetText() const;
	const std::string GetWidgetName() const;
	const UIWidgetBase* GetParentNode() const;
	UIWidgetBase* GetEditableParentNode() const;
	const std::vector<UIWidgetBase*>* GetChildren() const;
	std::vector<UIWidgetBase*>* GetEditableChildren();
	const XMLNode GetXMLNodeReadFrom() const;
	const bool GetReadFromXMLNode() const;
	Vector2 GetCurrentPosition() const;
	Vector2 GetCurrentSize() const;
	UISizeData GetSizeData() const;
	UIOffsetData GetOffsetData() const;
	virtual const Texture* GetPreferredMarginImage() const;
	virtual const Texture* GetPreferredBackgroundImage() const;
	int GetCurrentState() const;
	int GetPreviousState() const;

	//Static Functions
	static void ClearAllRegisterWidgetData();
	static void RegisterWidget(UIWidgetBase* widget);
	static void UnregisterWidget(UIWidgetBase* widget);

};
#endif