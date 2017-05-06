#pragma once

#ifndef UILABELWIDGET_HPP
#define UILABELWIDGET_HPP
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIWidgetRegistration.hpp"

class UILabelWidget : public UIWidgetBase
{
private:
protected:
public:
	//Constructors
	UILabelWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr, const std::string& widgetName = "Button", size_t additionalEventsToPrealloc = 0);
	UILabelWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr, const std::string& widgetName = "Button", size_t additionalEventsToPrealloc = 0);
	virtual ~UILabelWidget();

	//Static Functions
	static UIWidgetBase* SpawnUILabelWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnUILabelWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet);
};
#endif