#include "Engine/UI/UIWidgets/UIWidgetVariants/UIButtonWidget.hpp"



UIButtonWidget::UIButtonWidget(size_t rootNodeID, UIStyleSheet* styleSheet, 
	UIWidgetBase* parent, const std::string& widgetName, size_t additionalEventsToPrealloc)
	: UIWidgetBase(rootNodeID, styleSheet, parent, widgetName, false, additionalEventsToPrealloc)
{
	if (m_uniqueName == "default")
	{
		m_uniqueName = m_widgetName;
	}
}

UIButtonWidget::UIButtonWidget(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet, 
	UIWidgetBase* parent, const std::string& widgetName, size_t additionalEventsToPrealloc)
	: UIWidgetBase(node, rootNodeID, styleSheet, parent, widgetName, false , additionalEventsToPrealloc)
{
	if (m_uniqueName == "default")
	{
		m_uniqueName = m_widgetName;
	}
}

UIButtonWidget::~UIButtonWidget()
{
	UIWidgetBase::~UIWidgetBase();
}

STATIC UIWidgetBase* UIButtonWidget::SpawnUIButtonWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet)
{
	UIButtonWidget* button = new UIButtonWidget(data, packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return button;
}

STATIC UIWidgetBase* UIButtonWidget::SpawnUIButtonWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet)
{
	UIButtonWidget* button = new UIButtonWidget(packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return button;
}