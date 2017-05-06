#pragma once

#ifndef UIBUTTON_HPP
#define UIBUTTON_HPP
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIWidgetRegistration.hpp"

/*
typedef UIWidgetBase* (SpawnWidgetFunc)(const UIWidgetRegistrationDataType& data);
typedef UIWidgetBase* (SpawnWidgetWithoutDataFunc)();
*/

class UIButtonWidget : public UIWidgetBase
{
private:
protected:
public:
	static UIWidgetRegistration s_UIButtonRegistration;

	//Constructors
	UIButtonWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr, const std::string& widgetName = "Button", size_t additionalEventsToPrealloc = 0);
	UIButtonWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr, const std::string& widgetName = "Button", size_t additionalEventsToPrealloc = 0);
	virtual ~UIButtonWidget();

	//Static Functions
	static UIWidgetBase* SpawnUIButtonWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnUIButtonWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet);
};
#endif