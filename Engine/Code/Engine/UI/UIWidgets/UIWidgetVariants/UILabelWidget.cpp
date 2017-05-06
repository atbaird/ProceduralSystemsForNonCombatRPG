#include "Engine/UI/UIWidgets/UIWidgetVariants/UILabelWidget.hpp"


//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Constructors
UILabelWidget::UILabelWidget(size_t rootNodeID, UIStyleSheet* styleSheet,
	UIWidgetBase* parent, const std::string& widgetName, size_t additionalEventsToPrealloc)
	: UIWidgetBase(rootNodeID, styleSheet, parent, widgetName, false, additionalEventsToPrealloc)
{
}

UILabelWidget::UILabelWidget(const XMLNode& node, size_t rootNodeID , UIStyleSheet* styleSheet, 
	UIWidgetBase* parent, const std::string& widgetName , size_t additionalEventsToPrealloc)
	: UIWidgetBase(node, rootNodeID, styleSheet, parent, widgetName, false, additionalEventsToPrealloc)
{
	ReadAndHandleXMLNodeAttributes(node);
}

UILabelWidget::~UILabelWidget()
{
	UIWidgetBase::~UIWidgetBase();
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Static Functions
STATIC UIWidgetBase* UILabelWidget::SpawnUILabelWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet)
{
	UILabelWidget* label = new UILabelWidget(data, packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return label;
}

STATIC UIWidgetBase* UILabelWidget::SpawnUILabelWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet)
{
	UILabelWidget* label = new UILabelWidget(packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return label;
}