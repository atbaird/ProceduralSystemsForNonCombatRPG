#pragma once

#ifndef UINAMEDWIDGET_HPP
#define UINAMEDWIDGET_HPP
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"

class UINamedWidget : public UIWidgetBase
{
private:
protected:
public:
	UINamedWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	UINamedWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	virtual ~UINamedWidget();
};

#endif