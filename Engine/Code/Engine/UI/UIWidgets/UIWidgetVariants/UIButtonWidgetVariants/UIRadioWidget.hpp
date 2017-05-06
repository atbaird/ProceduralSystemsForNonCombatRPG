#pragma once

#ifndef UIRADIOWIDGET_HPP
#define UIRADIOWIDGET_HPP
#include "Engine/UI/UIWidgets/UIWidgetVariants//UIButtonWidgetVariants/UICheckboxWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidget.hpp"

class UIRadioWidget : public UIGroupWidget
{
private:
protected:
	std::vector<UICheckboxWidget*> m_checkboxes;
	std::vector<UIButtonWidget*> m_buttons;
	int m_idxOfRadio = -1;
	bool m_changedToDiffStateYet = true;
public:
	//Constructors
	UIRadioWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	UIRadioWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	virtual ~UIRadioWidget();

	virtual void Update(float deltaSeconds) override;
	virtual void UpdateRadioButtons(float deltaSeconds);

	//Getters
	int GetIdxOfRadioSelection() const;

	//Static functions
	static UIWidgetBase* SpawnUIRadioWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnUIRadioWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet);
};
#endif