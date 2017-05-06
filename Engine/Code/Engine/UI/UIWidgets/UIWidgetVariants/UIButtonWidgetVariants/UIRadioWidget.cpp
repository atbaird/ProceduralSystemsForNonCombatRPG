#include "Engine/UI/UIWidgets/UIWidgetVariants/UIButtonWidgetVariants/UIRadioWidget.hpp"


//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIRadioWidget::UIRadioWidget(size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIGroupWidget(rootNodeID, styleSheet, parent, "radio", 1)
	//UIWidgetBase(rootNodeID, styleSheet, parent, "Radio", 1)
{
	if (m_childAdjustmentSetting == Default_NotSet)
	{
		m_childAdjustmentSetting = HorizontalRight;
	}
	for (size_t i = 0; i < m_vectorPack.m_children.size(); i++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[i];
		if (child != nullptr)
		{
			UICheckboxWidget* checkbox = dynamic_cast<UICheckboxWidget*>(child);
			UIButtonWidget* button = dynamic_cast<UIButtonWidget*>(child);
			if (button != nullptr)
			{
				m_buttons.push_back(button);
			}
			if (checkbox != nullptr)
			{
				m_checkboxes.push_back(checkbox);
			}

		}
	}
	UpdateRadioButtons(0.f);
}

UIRadioWidget::UIRadioWidget(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIGroupWidget(node, rootNodeID, styleSheet, parent, "radio", 1)
	//UIWidgetBase(node, rootNodeID, styleSheet, parent, "Radio", 1)
{
	ReadAndHandleXMLNodeAttributes(node);
	if (m_childAdjustmentSetting == Default_NotSet)
	{
		m_childAdjustmentSetting = HorizontalRight;
	}
	for (size_t i = 0; i < m_vectorPack.m_children.size(); i++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[i];
		if (child != nullptr)
		{
			UICheckboxWidget* checkbox = dynamic_cast<UICheckboxWidget*>(child);
			UIButtonWidget* button = dynamic_cast<UIButtonWidget*>(child);
			if (button != nullptr)
			{
				m_buttons.push_back(button);
			}
			if (checkbox != nullptr)
			{
				m_checkboxes.push_back(checkbox);
			}

		}
	}
	UpdateRadioButtons(0.f);
}

UIRadioWidget::~UIRadioWidget()
{
	UIGroupWidget::~UIGroupWidget();
}

void UIRadioWidget::Update(float deltaSeconds)
{
	UIWidgetBase::Update(deltaSeconds);
	if (m_previousWidgetState == UIWIDGETSTATE_PRESSED)
	{
		if (m_changedToDiffStateYet)
		{
			for (size_t childIdx = 0; childIdx < m_vectorPack.m_children.size(); childIdx++)
			{
				UIWidgetBase* child = m_vectorPack.m_children[childIdx];
				if (child->GetCurrentState() == UIWIDGETSTATE_PRESSED)
				{
					m_idxOfRadio = childIdx;
					break;
				}
			}

			m_vectorPack.m_eventCallbacks[4].CallEnterEvent(this);

			m_vectorPack.m_eventCallbacks[4].CallUpdateEvent(this);
			m_vectorPack.m_eventCallbacks[4].CallExitEvent(this);
		}
		m_changedToDiffStateYet = false;
	}
	else
	{
		m_changedToDiffStateYet = true;
	}

	UpdateRadioButtons(deltaSeconds);
}

void UIRadioWidget::UpdateRadioButtons(float deltaSeconds)
{
	deltaSeconds;
	const Texture* unselectedRadio = nullptr;
	const Texture* selectedRadio = nullptr;
	if (m_styleSheet != nullptr)
	{
		const UIStyleWidgetData* data = m_styleSheet->GetStyleDataForWidgetType("radio");
		if (data != nullptr)
		{

			UIStateAppearanceData dataVal = data->GetAppearanceDataForIdx(UIWIDGETSTATE_ACTIVE);
			unselectedRadio = dataVal.GetRadioBackgroundImage();
			selectedRadio = dataVal.GetRadioSetBackgroundImage();
		}
	}
	for (size_t childIdx = 0; childIdx < m_buttons.size(); childIdx++)
	{
		UIButtonWidget* child = m_buttons[childIdx];
		if (child != nullptr)
		{
			if ((int)childIdx == m_idxOfRadio)
			{
				child->SetManualBackgroundImage(true, selectedRadio);
			}
			else
			{
				child->SetManualBackgroundImage(true, unselectedRadio);
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Getters
int UIRadioWidget::GetIdxOfRadioSelection() const
{
	return m_idxOfRadio;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Static functions
STATIC UIWidgetBase* UIRadioWidget::SpawnUIRadioWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet)
{
	UIRadioWidget* radio = new UIRadioWidget(data, packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return radio;
}

STATIC UIWidgetBase* UIRadioWidget::SpawnUIRadioWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet)
{
	UIRadioWidget* radio = new UIRadioWidget(packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return radio;
}