#pragma once

#ifndef UIHORIZONTALBOXWIDGET_HPP
#define UIHORIZONTALBOXWIDGET_HPP
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIWidgetRegistration.hpp"

class UIHorizontalBoxWidget : public UIGroupWidget
{
private:
protected:
public:
	//Constructors
	UIHorizontalBoxWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	UIHorizontalBoxWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	virtual ~UIHorizontalBoxWidget();

	//Setters
	virtual bool SetHowToHandleTheChildren(GroupingWidgetAdjustmentType howToHandleTheChildren) override;

	//static functions
	static UIWidgetBase* SpawnUIHorizontalBoxWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnUIHorizontalBoxWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet);
};

#endif