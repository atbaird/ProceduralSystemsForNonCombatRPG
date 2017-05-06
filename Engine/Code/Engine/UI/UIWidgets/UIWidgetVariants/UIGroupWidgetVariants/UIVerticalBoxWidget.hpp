#pragma once

#ifndef UIVERTICALBOXWIDGET_HPP
#define UIVERTICALBOXWIDGET_HPP
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIWidgetRegistration.hpp"

class UIVerticalBoxWidget : public UIGroupWidget
{
private:
protected:
public:
	//Constructors
	UIVerticalBoxWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	UIVerticalBoxWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	virtual ~UIVerticalBoxWidget();

	//Setters
	virtual bool SetHowToHandleTheChildren(GroupingWidgetAdjustmentType howToHandleTheChildren) override;

	//Static functions
	static UIWidgetBase* SpawnUIVerticalBoxWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnUIVerticalBoxWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet);
};

#endif