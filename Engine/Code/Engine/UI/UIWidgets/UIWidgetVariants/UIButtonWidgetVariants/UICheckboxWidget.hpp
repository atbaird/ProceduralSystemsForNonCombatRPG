#pragma once

#ifndef UICHECKBOXWIDGET_HPP
#define UICHECKBOXWIDGET_HPP
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIButtonWidget.hpp"

class UICheckboxWidget : public UIButtonWidget
{
private:
protected:
	bool m_isChecked = false;
	bool m_changedToDiffStateYet = true;
public:
	//Constructors
	UICheckboxWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	UICheckboxWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr);
	virtual ~UICheckboxWidget();

	virtual void Update(float deltaSeconds) override;

	//Getters
	virtual const Texture* GetPreferredMarginImage() const override;
	virtual const Texture* GetPreferredBackgroundImage() const override;
	bool GetIsChecked() const;

	//Static functions
	static UIWidgetBase* SpawnUICheckboxWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnUICheckboxWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet);
};
#endif