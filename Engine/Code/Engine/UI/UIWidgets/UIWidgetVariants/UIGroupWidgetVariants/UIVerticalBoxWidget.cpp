#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidgetVariants/UIVerticalBoxWidget.hpp"

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIVerticalBoxWidget::UIVerticalBoxWidget(size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIGroupWidget(rootNodeID, styleSheet, parent, "VerticalWidget")
{
	if (m_childAdjustmentSetting != VerticalDown &&
		m_childAdjustmentSetting != VerticalUp)
	{
		m_childAdjustmentSetting = VerticalDown;
	}
}

UIVerticalBoxWidget::UIVerticalBoxWidget(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIGroupWidget(node, rootNodeID, styleSheet, parent, "VerticalWidget")
{
	if (m_childAdjustmentSetting != VerticalDown &&
		m_childAdjustmentSetting != VerticalUp)
	{
		m_childAdjustmentSetting = VerticalDown;
	}
}

UIVerticalBoxWidget::~UIVerticalBoxWidget()
{
	UIGroupWidget::~UIGroupWidget();
}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//Setters
bool UIVerticalBoxWidget::SetHowToHandleTheChildren(GroupingWidgetAdjustmentType howToHandleTheChildren)
{
	if (howToHandleTheChildren != VerticalDown &&
		howToHandleTheChildren != VerticalUp)
	{
		return false;
	}
	return UIGroupWidget::SetHowToHandleTheChildren(howToHandleTheChildren);
}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//Static functions
STATIC UIWidgetBase* UIVerticalBoxWidget::SpawnUIVerticalBoxWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet)
{
	UIVerticalBoxWidget* ptr = new UIVerticalBoxWidget(data, packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());

	return ptr;
}
STATIC UIWidgetBase* UIVerticalBoxWidget::SpawnUIVerticalBoxWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet)
{
	UIVerticalBoxWidget* ptr = new UIVerticalBoxWidget(packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());

	return ptr;
}