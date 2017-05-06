#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidgetVariants/UIHorizontalBoxWidget.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//Constructors
UIHorizontalBoxWidget::UIHorizontalBoxWidget(size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIGroupWidget(rootNodeID, styleSheet, parent, "HorizontalWidget")
{
	if (m_childAdjustmentSetting != HorizontalLeft &&
		m_childAdjustmentSetting != HorizontalRight)
	{
		m_childAdjustmentSetting = HorizontalRight;
	}
}

UIHorizontalBoxWidget::UIHorizontalBoxWidget(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIGroupWidget(node, rootNodeID, styleSheet, parent, "HorizontalWidget")
{
	if (m_childAdjustmentSetting != HorizontalLeft &&
		m_childAdjustmentSetting != HorizontalRight)
	{
		m_childAdjustmentSetting = HorizontalRight;
	}
}

UIHorizontalBoxWidget::~UIHorizontalBoxWidget()
{
	UIGroupWidget::~UIGroupWidget();
}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//Setters
bool UIHorizontalBoxWidget::SetHowToHandleTheChildren(GroupingWidgetAdjustmentType howToHandleTheChildren)
{
	if (howToHandleTheChildren != HorizontalLeft &&
		howToHandleTheChildren != HorizontalRight)
	{
		return false;
	}
	return UIGroupWidget::SetHowToHandleTheChildren(howToHandleTheChildren);
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//Static Functions
UIWidgetBase* UIHorizontalBoxWidget::SpawnUIHorizontalBoxWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet)
{
	UIHorizontalBoxWidget* horizontalWidget = new UIHorizontalBoxWidget(data, packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return horizontalWidget;
}

UIWidgetBase* UIHorizontalBoxWidget::SpawnUIHorizontalBoxWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet)
{
	UIHorizontalBoxWidget* horizontalWidget = new UIHorizontalBoxWidget(packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return horizontalWidget;
}