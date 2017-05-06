#include "Engine/UI/UIWidgets/UIWidgetVariants/UINamedWidget.hpp"

UINamedWidget::UINamedWidget(size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIWidgetBase(rootNodeID, styleSheet, parent, "NamedWidget")
{
	m_sizeData.SetFillParentAvailableParentSpaceX(true);
	m_sizeData.SetFillParentAvailableParentSpaceY(true);
	m_boolPack.m_amANamedWidget = true;
	SetOpacity(0.f);
	m_uniqueName = m_widgetName;
}

UINamedWidget::UINamedWidget(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent)
	: UIWidgetBase(node, rootNodeID, styleSheet, parent, "NamedWidget")
{
	m_sizeData.SetFillParentAvailableParentSpaceX(true);
	m_sizeData.SetFillParentAvailableParentSpaceY(true);
	m_boolPack.m_amANamedWidget = true;
	SetOpacity(0.f);
	if (m_uniqueName == "default")
	{
		m_uniqueName = m_widgetName;
	}
}

UINamedWidget::~UINamedWidget()
{
	UIWidgetBase::~UIWidgetBase();
}