#include "Engine/UI/UIStructs/UIWidgetRegistrationDataPacket.hpp"
#include "Engine/UI/UIStyleSheetHandler.hpp"
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIWidgetRegistrationDataPacket::UIWidgetRegistrationDataPacket(UIWidgetBase* parent, UIStyleSheet* defaultStyleSheet, size_t currentRootNodeID)
	: m_UIWidgetParent(parent),
	m_defaultStyleSheet(defaultStyleSheet),
	m_currentRootNodeID(currentRootNodeID)
{

}

UIWidgetRegistrationDataPacket::~UIWidgetRegistrationDataPacket()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//Setters
void UIWidgetRegistrationDataPacket::LoadDataValues(UIWidgetBase* parent)
{
	m_UIWidgetParent = parent;
}


void UIWidgetRegistrationDataPacket::CopyValuesOntoUIWidgetBase(UIWidgetBase* base) const
{
	if (base == nullptr)
	{
		return;
	}
	UIWidgetBase* widget = m_UIWidgetParent;
	base->SetParentWidget(widget);
	base->SetRootNodeID(m_currentRootNodeID);
}

void UIWidgetRegistrationDataPacket::SetCurrentRootNodeID(size_t rootNodeID)
{
	m_currentRootNodeID = rootNodeID;
}

void UIWidgetRegistrationDataPacket::SetDefaultStyleSheet(UIStyleSheet* styleSheet)
{
	m_defaultStyleSheet = styleSheet;
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//Getters
UIWidgetBase* UIWidgetRegistrationDataPacket::GetUIWidgetParent() const
{
	return m_UIWidgetParent;
}

UIStyleSheet* UIWidgetRegistrationDataPacket::GetStyleSheet() const
{
	return m_defaultStyleSheet;
}

size_t UIWidgetRegistrationDataPacket::GetRootNodeID() const
{
	return m_currentRootNodeID;
}