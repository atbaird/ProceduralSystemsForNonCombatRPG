#pragma once

#ifndef UIDIMENSION_HPP
#define UIDIMENSION_HPP
#include "Engine/UI/UIStructs/UIDimensionRange.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"

class UIWidgetBase;
struct UISizeData
{
private:
protected:
	UIDimensionRange m_sizeRangeX = UIDimensionRange(-1.f, -1.f); //whats the smallest or biggest this widget can be?
	UIDimensionRange m_sizeRangeY = UIDimensionRange(-1.f, -1.f); //whats the smallest or biggest this widget can be?
	Vector2 m_currentSize = Vector2::vec2_ones;
	Vector2 m_percentSize = Vector2::vec2_zeros; //uses parent widget's size to calculate how much should fill.
	Vector2 m_trueSize = Vector2::vec2_zeros;
	Vector2 m_marginBL = Vector2::vec2_zeros;
	Vector2 m_marginTR = Vector2::vec2_zeros;
	UIWidgetBase* m_owner = nullptr;
	bool m_fillAvailableParentSpaceX = false;
	bool m_fillAvailableParentSpaceY = false;
	bool m_clampWithinParentSizeX = false;
	bool m_clampWithinParentSizeY = false;

public:
	//Constructors
	UISizeData(UIWidgetBase* owner = nullptr);
	virtual ~UISizeData();

	//Operations
	void RecalculateSize();

	//Setters
	void SetOwner(UIWidgetBase* owner);
	void SetCurrentSize(const Vector2& currentSize);
	void SetCurrentSizeX(float currentSizeX);
	void SetCurrentSizeY(float currentSizeY);
	void SetSizeRangeX(const UIDimensionRange& rangeX);
	void SetSizeRangeY(const UIDimensionRange& rangeY);
	void SetSizeRangeX(const Vector2& rangeX);
	void SetSizeRangeY(const Vector2& rangeY);
	void SetSizeRangeMaxs(const Vector2& maxs);
	void SetSizeRangeMins(const Vector2& mins);
	void SetAbsoluteMinSize(const Vector2& mins);
	void SetAbsoluteMaxSize(const Vector2& maxs);
	void SetMarginBL(const Vector2& marginBL);
	void SetMarginTR(const Vector2& marginTR);
	void SetMarginTop(float topMargin);
	void SetMarginBottom(float bottomMargin);
	void SetMarginRight(float rightMargin);
	void SetMarginLeft(float leftMargin);
	void SetPercentageSize(const Vector2& percentageSize);
	void SetPercentageSizeX(float percentageSizeX);
	void SetPercentageSizeY(float percentageSizeY);
	void SetFillAvailableParentSpace(bool fillParentSpaceX, bool fillParentSpaceY);
	void SetFillParentAvailableParentSpaceX(bool fillParentSpaceX);
	void SetFillParentAvailableParentSpaceY(bool fillParentSpaceY);
	void SetClampWithinParentSize(bool clampWithinParentSizeX, bool clampWithinParentSizeY);
	void SetClampWithinParentSizeX(bool clampWithinParentSizeX);
	void SetClampWithinParentSizeY(bool clampWithinParentSizeY);

	//Getters
	UIDimensionRange GetSizeRangeX() const;
	UIDimensionRange GetSizeRangeY() const;
	Vector2 GetTrueSize() const;
	Vector2 GetPercentageSize() const;
	Vector2 GetAbsoluteMinSize() const;
	Vector2 GetAbsoluteMaxSize() const;
	Vector2 GetMarginBL() const;
	Vector2 GetMarginTR() const;
	Vector2 GetTotalMarginWidthHeight() const;
	Vector2 GetHalfMarginWidthHeight() const;
	bool GetFillAvailableParentSpaceX() const;
	bool GetFillAvailableParentSpaceY() const;
	bool GetClampWithinParentSizeX() const;
	bool GetClampWithinParentSizeY() const;

};
#endif