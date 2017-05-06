#include "Engine/UI/UIWidgets/UIWidgetVariants/UIButtonWidgetVariants/UICheckboxWidget.hpp"


//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Constructors
UICheckboxWidget::UICheckboxWidget(size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIButtonWidget(rootNodeID, styleSheet, parent, "Checkbox", 1)
{
	m_vectorPack.m_eventCallbacks[4].SetName("onvaluechanged");
}
UICheckboxWidget::UICheckboxWidget(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet,
	UIWidgetBase* parent)
	: UIButtonWidget(node, rootNodeID, styleSheet, parent, "Checkbox", 1)
{
	m_vectorPack.m_eventCallbacks[4].SetName("onvaluechanged");
	ReadAndHandleXMLNodeAttributes(node);
}
UICheckboxWidget::~UICheckboxWidget()
{
	UIButtonWidget::~UIButtonWidget();
}

void UICheckboxWidget::Update(float deltaSeconds)
{
	if (m_previousWidgetState == UIWIDGETSTATE_PRESSED)
	{
		if (m_changedToDiffStateYet)
		{
			m_vectorPack.m_eventCallbacks[4].CallEnterEvent(this);
			m_isChecked = !m_isChecked;
			m_vectorPack.m_eventCallbacks[4].CallUpdateEvent(this);
			m_vectorPack.m_eventCallbacks[4].CallExitEvent(this);
		}
		m_changedToDiffStateYet = false;
	}
	else
	{
		m_changedToDiffStateYet = true;
	}

	UIButtonWidget::Update(deltaSeconds);
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Getters
const Texture* UICheckboxWidget::GetPreferredMarginImage() const
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

	if (m_currentWidgetState == UIWIDGETSTATE_HIDDEN)
	{
		return nullptr;
	}

	UIStateAppearanceData appData = data->GetAppearanceDataForIdx(m_currentWidgetState);
	if (m_isChecked)
	{
		return appData.GetCheckedMarginImage();
	}
	else
	{
		return appData.GetMarginImage();
	}
}

const Texture* UICheckboxWidget::GetPreferredBackgroundImage() const
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

	if (m_currentWidgetState == UIWIDGETSTATE_HIDDEN)
	{
		return nullptr;
	}

	UIStateAppearanceData appData = data->GetAppearanceDataForIdx(m_currentWidgetState);
	if (m_isChecked)
	{
		return appData.GetCheckedBackgroundImage();
	}
	else
	{
		return appData.GetBackgroundImage();
	}
}

bool UICheckboxWidget::GetIsChecked() const
{
	return m_isChecked;
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Static functions
STATIC UIWidgetBase* UICheckboxWidget::SpawnUICheckboxWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet)
{
	UICheckboxWidget* checkbox = new UICheckboxWidget(data, packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return checkbox;
}

STATIC UIWidgetBase* UICheckboxWidget::SpawnUICheckboxWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet)
{
	UICheckboxWidget* checkbox = new UICheckboxWidget(packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return checkbox;
}