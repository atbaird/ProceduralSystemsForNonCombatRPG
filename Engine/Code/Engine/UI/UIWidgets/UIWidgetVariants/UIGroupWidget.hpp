#pragma once

#ifndef UIGROUPWIDGET_HPP
#define UIGROUPWIDGET_HPP
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UIStructs/GroupingWidgetAdjustmentType.hpp"

class UIGroupWidget : public UIWidgetBase
{
private:
protected:
	GroupingWidgetAdjustmentType m_childAdjustmentSetting = Default_NotSet;
	float m_spacing = 0.f;
public:
	//Constructors
	UIGroupWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, 
		UIWidgetBase* parent = nullptr, const std::string& groupWidgetName = "", size_t numAdditionalEventsToAlloc = 0);
	UIGroupWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr, 
		const std::string& groupWidgetName = "", size_t numAdditionalEventsToAlloc = 0);
	virtual ~UIGroupWidget();

	//Setters
	virtual bool SetHowToHandleTheChildren(GroupingWidgetAdjustmentType howToHandleTheChildren);

	virtual void UpdateChildren(float deltaSeconds) override;
	virtual void HandleUpdatePerChild(float deltaSeconds, UIWidgetBase* child, Vector2& currentAdjustment, const Vector2& directionToGo);
protected:

	virtual std::string ReadAndHandleXMLAttribute(const XMLAttribute& attr, bool& readOnceAlready) override; //returns the lowercase string
public:

	//Getters
	GroupingWidgetAdjustmentType GetGroupingAdjustmentType() const;
};
#endif