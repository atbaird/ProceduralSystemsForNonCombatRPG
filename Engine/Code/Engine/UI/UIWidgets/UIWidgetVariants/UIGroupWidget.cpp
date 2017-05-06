#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidget.hpp"
#include "Engine/Core/StringUtils.hpp"


UIGroupWidget::UIGroupWidget(size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent, const std::string& groupWidgetName,
	size_t numAdditionalEventsToAlloc)
	: UIWidgetBase(rootNodeID, styleSheet, parent, groupWidgetName, true, numAdditionalEventsToAlloc)
{
	m_opacity = 0.f;
	m_fontOpacity = 0.f;
	for (size_t childIdx = 0; childIdx < m_vectorPack.m_children.size(); childIdx++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[childIdx];
		if (child != nullptr)
		{
			child->SetPositionHandledByParent(true);
		}
	}
}

UIGroupWidget::UIGroupWidget(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet,
	UIWidgetBase* parent, const std::string& groupWidgetName, size_t numAdditionalEventsToAlloc)
	: UIWidgetBase(node, rootNodeID, styleSheet, parent, groupWidgetName, true, numAdditionalEventsToAlloc)
{
	ReadAndHandleXMLNodeAttributes(node);
	m_opacity = 0.f;
	m_fontOpacity = 0.f;
	for (size_t childIdx = 0; childIdx < m_vectorPack.m_children.size(); childIdx++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[childIdx];
		if (child != nullptr)
		{
			child->SetPositionHandledByParent(true);
		}
	}
}

UIGroupWidget::~UIGroupWidget()
{
	UIWidgetBase::~UIWidgetBase();
}

void UIGroupWidget::UpdateChildren(float deltaSeconds)
{
	Vector2 halfMySize = m_currentSize * 0.5f;
	Vector2 directionToGo = Vector2::vec2_zeros;
	Vector2 currentAdjustsMent = Vector2::vec2_zeros;
	Vector2 mySize = GetCurrentSize();

	Vector2 parentPos = Vector2::vec2_zeros;
	Vector2 parentSize = Vector2::vec2_zeros;
	Vector2 trueParentPosition = Vector2::vec2_zeros;
	parentPos = m_offsetData.GetRelativeParentPositionBasedOnAnchor(parentSize, mySize, trueParentPosition, false);
	

	m_currentOffset = parentPos;
	if (m_childAdjustmentSetting == HorizontalLeft)
	{
		currentAdjustsMent = m_currentOffset - Vector2(halfMySize.x + m_spacing, 0.f);
		directionToGo = Vector2(1.f, 0.f);
		
	}
	else if (m_childAdjustmentSetting == HorizontalRight)
	{
		currentAdjustsMent = m_currentOffset + Vector2(halfMySize.x + m_spacing, 0.f);
		directionToGo = Vector2(-1.f, 0.f);
	}
	else if (m_childAdjustmentSetting == VerticalDown)
	{
		currentAdjustsMent = m_currentOffset + Vector2(0.f, halfMySize.y + m_spacing);
		directionToGo = Vector2(0.f, -1.f);
	}
	else if (m_childAdjustmentSetting == VerticalUp)
	{
		currentAdjustsMent = m_currentOffset - Vector2(0.f, halfMySize.y + m_spacing);
		directionToGo = Vector2(0.f, 1.f);
	}
	
	for (size_t childIdx = 0; childIdx < m_vectorPack.m_children.size(); childIdx++)
	{
		UIWidgetBase* child = m_vectorPack.m_children[childIdx];
		if (child != nullptr)
		{
			HandleUpdatePerChild(deltaSeconds, child, currentAdjustsMent, directionToGo);
		}
	}
}

void UIGroupWidget::HandleUpdatePerChild(float deltaSeconds, UIWidgetBase* child, Vector2& currentAdjustment, const Vector2& directionToGo)
{
	UISizeData data = child->GetSizeData();
	data.RecalculateSize();

	Vector2 childSize = data.GetTrueSize();
	Vector2 marginSize = data.GetTotalMarginWidthHeight();
	childSize += marginSize;
	Vector2 halfChildSizeAndDir = Vector2(directionToGo.x * childSize.x * 0.5f, directionToGo.y * childSize.y * 0.5f);
	Vector2 spacingInDir = Vector2(m_spacing * directionToGo.x * 0.5f, m_spacing * directionToGo.y * 0.5f);
	Vector2 addIn = halfChildSizeAndDir + spacingInDir;
	Vector2 positionToSetRelativeTo = currentAdjustment + addIn + addIn;
	currentAdjustment = positionToSetRelativeTo + halfChildSizeAndDir;

	child->SetPositionHandledByParent(true);
	child->SetTrueLocalPositionBasedOnGroupWidgetInput(positionToSetRelativeTo);
	child->Update(deltaSeconds);
}

std::string UIGroupWidget::ReadAndHandleXMLAttribute(const XMLAttribute& attr, bool& readOnceAlready)
{
	std::string lowerCase = UIWidgetBase::ReadAndHandleXMLAttribute(attr, readOnceAlready);
	if (readOnceAlready)
	{
		return lowerCase;
	}
	if (readOnceAlready)
	{
		return lowerCase;
	}

	if (lowerCase == "spacing")
	{
		readOnceAlready = true;
		m_spacing = (float)atof(attr.lpszValue);
	}
	else if (lowerCase == "adjustmenttype"
		|| lowerCase == "adjustment")
	{
		readOnceAlready = true;
		std::string lowerCaseVal = MakeLower(attr.lpszValue);
		if (lowerCaseVal == "horizontalright"
			|| lowerCaseVal == "right")
		{
			m_childAdjustmentSetting = HorizontalRight;
		}
		else if (lowerCaseVal == "horizontalleft"
			|| lowerCaseVal == "left")
		{
			m_childAdjustmentSetting = HorizontalLeft;
		}
		else if (lowerCaseVal == "verticaldown"
			|| lowerCaseVal == "down")
		{
			m_childAdjustmentSetting = VerticalDown;
		}
		else if (lowerCaseVal == "verticalUp"
			|| lowerCaseVal == "up")
		{
			m_childAdjustmentSetting = VerticalUp;
		}
		else
		{
			m_childAdjustmentSetting = Default_NotSet;
		}
	}

	return lowerCase;
}

bool UIGroupWidget::SetHowToHandleTheChildren(GroupingWidgetAdjustmentType howToHandleTheChildren)
{
	m_childAdjustmentSetting = howToHandleTheChildren;
	return true;
}


//Getters
GroupingWidgetAdjustmentType UIGroupWidget::GetGroupingAdjustmentType() const
{
	return m_childAdjustmentSetting;
}