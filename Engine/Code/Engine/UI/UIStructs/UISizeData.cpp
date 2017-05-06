#include "Engine/UI/UIStructs/UISizeData.hpp"
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UISystem.hpp"

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Constructors
UISizeData::UISizeData(UIWidgetBase* owner)
	: m_owner(owner)
{
}

UISizeData::~UISizeData()
{
}


//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Operations
void UISizeData::RecalculateSize()
{
	if (m_owner == nullptr)
	{
		return;
	}
	UIWidgetBase* parent = m_owner->GetEditableParentNode();
	Vector2 parentSize = Vector2::vec2_zeros;
	if (parent != nullptr)
	{
		parentSize = parent->GetCurrentSize();
	}
	else
	{
		UISystem* system = UISystem::GetUISystemPtr();
		parentSize = system->GetCurrentSystemScreenSize();
	}

	Vector2 percentSize = Vector2(m_percentSize.x * parentSize.x, m_percentSize.y * parentSize.y);
	m_trueSize = percentSize + m_currentSize;
	if (m_fillAvailableParentSpaceX)
	{
		m_trueSize.x = parentSize.x;
	}
	else
	{
		m_trueSize.x = percentSize.x + m_currentSize.x;
		if (m_sizeRangeX.m_min > 0.f && m_trueSize.x < m_sizeRangeX.m_min)
		{
			m_trueSize.x = m_sizeRangeX.m_min;
		}
		else if (m_sizeRangeX.m_max > 0.f && m_trueSize.x > m_sizeRangeX.m_max)
		{
			m_trueSize.x = m_sizeRangeX.m_max;
		}
	}

	if (m_fillAvailableParentSpaceY)
	{
		m_trueSize.y = parentSize.y;
	}
	else
	{
		m_trueSize.y = percentSize.y + m_currentSize.y;
		if (m_sizeRangeY.m_min > 0.f && m_trueSize.y < m_sizeRangeY.m_min)
		{
			m_trueSize.y = m_sizeRangeY.m_min;
		}
		else if (m_sizeRangeY.m_min > 0.f && m_trueSize.y > m_sizeRangeY.m_max)
		{
			m_trueSize.y = m_sizeRangeY.m_max;
		}
	}
	
	std::vector<UIWidgetBase*>* children = m_owner->GetEditableChildren();
	for (size_t childIdx = 0; childIdx < children->size(); childIdx++)
	{
		UIWidgetBase* child = children->at(childIdx);
		if (child != nullptr)
		{
			child->SetUpdatePosition(true);
			child->SetUpdateSize(true);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Setters
void UISizeData::SetOwner(UIWidgetBase* owner)
{
	m_owner = owner;
}

void UISizeData::SetCurrentSize(const Vector2& currentSize)
{
	m_currentSize = currentSize;
}

void UISizeData::SetCurrentSizeX(float currentSizeX)
{
	m_currentSize.x = currentSizeX;
}

void UISizeData::SetCurrentSizeY(float currentSizeY)
{
	m_currentSize.y = currentSizeY;
}

void UISizeData::SetSizeRangeX(const UIDimensionRange& rangeX)
{
	m_sizeRangeX = rangeX;
}

void UISizeData::SetSizeRangeY(const UIDimensionRange& rangeY)
{
	m_sizeRangeY = rangeY;
}

void UISizeData::SetSizeRangeX(const Vector2& rangeX)
{
	m_sizeRangeX = UIDimensionRange(rangeX.x, rangeX.y);
}

void UISizeData::SetSizeRangeY(const Vector2& rangeY)
{
	m_sizeRangeY = UIDimensionRange(rangeY.x, rangeY.y);
}

void UISizeData::SetSizeRangeMaxs(const Vector2& maxs)
{
	m_sizeRangeX.m_max = maxs.x;
	m_sizeRangeY.m_max = maxs.y;
}

void UISizeData::SetSizeRangeMins(const Vector2& mins)
{
	m_sizeRangeX.m_min = mins.x;
	m_sizeRangeY.m_min = mins.y;
}

void UISizeData::SetAbsoluteMinSize(const Vector2& mins)
{
	SetSizeRangeMins(mins);
}

void UISizeData::SetAbsoluteMaxSize(const Vector2& maxs)
{
	SetSizeRangeMaxs(maxs);
}

void UISizeData::SetMarginBL(const Vector2& marginBL)
{
	m_marginBL = marginBL;
}

void UISizeData::SetMarginTR(const Vector2& marginTR)
{
	m_marginTR = marginTR;
}

void UISizeData::SetMarginTop(float topMargin)
{
	m_marginTR.y = topMargin;
}

void UISizeData::SetMarginBottom(float bottomMargin)
{
	m_marginBL.y = bottomMargin;
}

void UISizeData::SetMarginRight(float rightMargin)
{
	m_marginTR.x = rightMargin;
}

void UISizeData::SetMarginLeft(float leftMargin)
{
	m_marginBL.x = leftMargin;
}

void UISizeData::SetPercentageSize(const Vector2& percentageSize)
{
	m_percentSize = percentageSize;
}

void UISizeData::SetPercentageSizeX(float percentageSizeX)
{
	m_percentSize.x = percentageSizeX;
}

void UISizeData::SetPercentageSizeY(float percentageSizeY)
{
	m_percentSize.y = percentageSizeY;
}

void UISizeData::SetFillAvailableParentSpace(bool fillParentSpaceX, bool fillParentSpaceY)
{
	m_fillAvailableParentSpaceX = fillParentSpaceX;
	m_fillAvailableParentSpaceY = fillParentSpaceY;
}

void UISizeData::SetFillParentAvailableParentSpaceX(bool fillParentSpaceX)
{
	m_fillAvailableParentSpaceX = fillParentSpaceX;
}

void UISizeData::SetFillParentAvailableParentSpaceY(bool fillParentSpaceY)
{
	m_fillAvailableParentSpaceY = fillParentSpaceY;
}

void UISizeData::SetClampWithinParentSize(bool clampWithinParentSizeX, bool clampWithinParentSizeY)
{
	m_clampWithinParentSizeX = clampWithinParentSizeX;
	m_clampWithinParentSizeY = clampWithinParentSizeY;
}

void UISizeData::SetClampWithinParentSizeX(bool clampWithinParentSizeX)
{
	m_clampWithinParentSizeX = clampWithinParentSizeX;
}

void UISizeData::SetClampWithinParentSizeY(bool clampWithinParentSizeY)
{
	m_clampWithinParentSizeY = clampWithinParentSizeY;
}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Getters
UIDimensionRange UISizeData::GetSizeRangeX() const
{
	return m_sizeRangeX;
}

UIDimensionRange UISizeData::GetSizeRangeY() const
{
	return m_sizeRangeY;
}

Vector2 UISizeData::GetTrueSize() const
{
	return m_trueSize;
}

Vector2 UISizeData::GetPercentageSize() const
{
	return m_percentSize;
}

Vector2 UISizeData::GetAbsoluteMinSize() const
{
	return Vector2(m_sizeRangeX.m_min, m_sizeRangeY.m_min);
}

Vector2 UISizeData::GetAbsoluteMaxSize() const
{
	return Vector2(m_sizeRangeX.m_max, m_sizeRangeY.m_max);
}

Vector2 UISizeData::GetMarginBL() const
{
	return m_marginBL;
}

Vector2 UISizeData::GetMarginTR() const
{
	return m_marginTR;
}

Vector2 UISizeData::GetTotalMarginWidthHeight() const
{
	return Vector2((m_marginBL.x + m_marginTR.x), (m_marginBL.y + m_marginTR.y));
}

Vector2 UISizeData::GetHalfMarginWidthHeight() const
{
	Vector2 marginSize(GetTotalMarginWidthHeight());
	return marginSize * 0.5f;
}

bool UISizeData::GetFillAvailableParentSpaceX() const
{
	return m_fillAvailableParentSpaceX;
}

bool UISizeData::GetFillAvailableParentSpaceY() const
{
	return m_fillAvailableParentSpaceY;
}

bool UISizeData::GetClampWithinParentSizeX() const
{
	return m_clampWithinParentSizeX;
}

bool UISizeData::GetClampWithinParentSizeY() const
{
	return m_clampWithinParentSizeY;
}