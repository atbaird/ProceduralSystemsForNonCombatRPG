#include "Engine/UI/UIStructs/UIOffsetData.hpp"
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidget.hpp"
#include "Engine/UI/UISystem.hpp"

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Constructors
UIOffsetData::UIOffsetData(UIWidgetBase* owner)
	: m_owner(owner)
{

}

UIOffsetData::~UIOffsetData()
{

}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Setters

void UIOffsetData::SetOwner(UIWidgetBase* owner)
{
	m_owner = owner;
}

void UIOffsetData::SetTruePosition(const Vector2& position)
{
	m_truePosition = position;
}

void UIOffsetData::SetPercentagePosition(const Vector2& position)
{
	m_percentagePosition = position;
}

void UIOffsetData::SetPercentagePositionOffsetX(float offsetX)
{
	m_percentagePosition.x = offsetX;
}

void UIOffsetData::SetPercentagePositionOffsetY(float offsetY)
{
	m_percentagePosition.y = offsetY;
}

void UIOffsetData::SetPositionOffset(const Vector2& position)
{
	m_positionOffset = position;
}

void UIOffsetData::SetPositionOffsetX(float offsetX)
{
	m_positionOffset.x = offsetX;
}

void UIOffsetData::SetPositionOffsetY(float offsetY)
{
	m_positionOffset.y = offsetY;
}

void UIOffsetData::SetUIAnchor(UIAnchors anchor)
{
	m_Anchor = anchor;
}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Getters
Vector2 UIOffsetData::GetPosition() const
{
	return m_positionOffset;
}

Vector2 UIOffsetData::GetPercentagePosition() const
{
	return m_percentagePosition;
}

Vector2 UIOffsetData::GetTruePosition() const
{
	return m_truePosition;
}

UIAnchors UIOffsetData::GetUIAnchor() const
{
	return m_Anchor;
}

const UIWidgetBase* UIOffsetData::GetOwner() const
{
	return m_owner;
}

UIWidgetBase* UIOffsetData::GetEditableOwner()
{
	return m_owner;
}


//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Operations
void UIOffsetData::RecalculateTruePosition()
{
	if (m_owner == nullptr)
	{
		return;
	}

	Vector2 mySize = m_owner->GetCurrentSize();

	Vector2 parentPos = Vector2::vec2_zeros;
	Vector2 parentSize = Vector2::vec2_zeros;
	Vector2 trueParentPosition = Vector2::vec2_zeros;
	parentPos = GetRelativeParentPositionBasedOnAnchor(parentSize, mySize, trueParentPosition);
	Vector2 percentOffset = Vector2(m_percentagePosition.x * parentSize.x, m_percentagePosition.y * parentSize.y);

	m_truePosition = parentPos + m_positionOffset + percentOffset;

	UISizeData sizeData = m_owner->GetSizeData();
	if (sizeData.GetFillAvailableParentSpaceX())
	{
		m_truePosition.x = parentPos.x;
	}
	else
	{
		m_truePosition.x = parentPos.x + m_positionOffset.x + percentOffset.x;
	}
	if (sizeData.GetFillAvailableParentSpaceY())
	{
		m_truePosition.x = parentPos.y;
	}
	else
	{
		m_truePosition.y = parentPos.y + m_positionOffset.y + percentOffset.y;
	}


	std::vector<UIWidgetBase*>* children = m_owner->GetEditableChildren();
	for (size_t childIdx = 0; childIdx < children->size(); childIdx++)
	{
		//If my offset has changed, then the children's offset need change.
		UIWidgetBase* child = children->at(childIdx);
		if (child != nullptr)
		{
			child->SetUpdateSize(true);
			child->SetUpdatePosition(true);
		}
	}
}


void UIOffsetData::RecalculateTruePositionBasedOffOfRelativeGroupWidgetGivenPosition(const Vector2& position)
{
	if (m_owner == nullptr)
	{
		return;
	}
	Vector2 mySize = m_owner->GetCurrentSize();
	Vector2 parentSize = Vector2::vec2_zeros;
	Vector2 trueParentPosition = Vector2::vec2_zeros;
	GetRelativeParentPositionBasedOnAnchor(parentSize, mySize, trueParentPosition);
	Vector2 percentOffset = Vector2(m_percentagePosition.x * parentSize.x, m_percentagePosition.y * parentSize.y);
	m_truePosition = position + m_positionOffset + percentOffset;

	std::vector<UIWidgetBase*>* children = m_owner->GetEditableChildren();
	for (size_t childIdx = 0; childIdx < children->size(); childIdx++)
	{
		//If my offset has changed, then the children's offset need change.
		UIWidgetBase* child = children->at(childIdx);
		if (child != nullptr)
		{
			child->SetUpdateSize(true);
			child->SetUpdatePosition(true);
		}
	}
}


Vector2 UIOffsetData::GetRelativeParentPositionBasedOnAnchor(Vector2& parentSize,
	const Vector2& myCurrentSize, Vector2& absoluteTrueParentPosition, bool accountOwnSize) const
{
	const Vector2 halfMySize = myCurrentSize * 0.5f;
	const UIWidgetBase* parent = m_owner->GetParentNode();
	Vector2 parentPos = Vector2::vec2_zeros;
	parentSize = Vector2::vec2_zeros;
	if (parent != nullptr)
	{
		parentPos = parent->GetCurrentPosition();
		parentSize = parent->GetCurrentSize();

	}
	else
	{
		UISystem* system = UISystem::GetUISystemPtr();
		parentSize = system->GetCurrentSystemScreenSize();
		parentPos = parentSize * 0.5f;
		//Gonna let this error, cause if using UIWidgets, shouldn't be doing things without a UISystem.
	}
	absoluteTrueParentPosition = parentPos;
	Vector2 halfParentSize = parentSize * 0.5f;

	//Handle X Adjustment
	if (m_Anchor == UIANCHOR_BottomLeft
		|| m_Anchor == UIANCHOR_CenterLeft
		|| m_Anchor == UIANCHOR_TopLeft)
	{
		parentPos.x = parentPos.x - halfParentSize.x;
		if (accountOwnSize)
		{
			parentPos.x = parentPos.x + halfMySize.x;
		}
	}
	else if (m_Anchor == UIANCHOR_BottomCenter
		|| m_Anchor == UIANCHOR_CenterCenter
		|| m_Anchor == UIANCHOR_TopCenter)
	{
		//already centered, should not need to do anything.
	}
	else if (m_Anchor == UIANCHOR_BottomRight
		|| m_Anchor == UIANCHOR_CenterRight
		|| m_Anchor == UIANCHOR_TopRight)
	{
		parentPos.x = parentPos.x + halfParentSize.x;
		if (accountOwnSize)
		{
			parentPos.x = parentPos.x - halfMySize.x;
		}
	}

	//Handle Y addjustment
	if (m_Anchor == UIANCHOR_BottomCenter
		|| m_Anchor == UIANCHOR_BottomLeft
		|| m_Anchor == UIANCHOR_BottomRight)
	{
		parentPos.y = parentPos.y - halfParentSize.y;
		if (accountOwnSize)
		{
			parentPos.y = parentPos.y + halfMySize.y;
		}
	}
	else if (m_Anchor == UIANCHOR_CenterCenter
		|| m_Anchor == UIANCHOR_CenterLeft
		|| m_Anchor == UIANCHOR_CenterRight)
	{

	}
	else if (m_Anchor == UIANCHOR_TopCenter
		|| m_Anchor == UIANCHOR_TopLeft
		|| m_Anchor == UIANCHOR_TopRight)
	{
		parentPos.y = parentPos.y + halfParentSize.y;
		if (accountOwnSize)
		{
			parentPos.y = parentPos.y - halfMySize.y;
		}
	}

	return parentPos;
}