#pragma once

#ifndef UIPOSITIONDATA_HPP
#define UIPOSITIONDATA_HPP
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/UI/UIStructs/UIAnchors.hpp"

class UIWidgetBase;
struct UIOffsetData
{
private:
protected:
	//For right now, only support for Bottom left.
	UIAnchors m_Anchor = UIANCHOR_CenterCenter;
	Vector2 m_truePosition = Vector2::vec2_zeros;
	Vector2 m_percentagePosition = Vector2::vec2_zeros; //uses parent widet's own size to calculate this one.
	Vector2 m_positionOffset = Vector2::vec2_zeros; //Based on PixelPosition.
	UIWidgetBase* m_owner = nullptr;
public:
	//Constructors
	UIOffsetData(UIWidgetBase* owner = nullptr);
	virtual ~UIOffsetData();

	//Setters
	void SetOwner(UIWidgetBase* owner);
	void SetTruePosition(const Vector2& position);
	void SetPercentagePosition(const Vector2& position);
	void SetPercentagePositionOffsetX(float offsetX);
	void SetPercentagePositionOffsetY(float offsetY);
	void SetPositionOffset(const Vector2& position);
	void SetPositionOffsetX(float offsetX);
	void SetPositionOffsetY(float offsetY);
	void SetUIAnchor(UIAnchors anchor);

	//Getters
	Vector2 GetPosition() const;
	Vector2 GetPercentagePosition() const;
	Vector2 GetTruePosition() const;
	UIAnchors GetUIAnchor() const;
	const UIWidgetBase* GetOwner() const;
	UIWidgetBase* GetEditableOwner();

	//Operations
	void RecalculateTruePosition();
	void RecalculateTruePositionBasedOffOfRelativeGroupWidgetGivenPosition(const Vector2& position);
	Vector2 GetRelativeParentPositionBasedOnAnchor(Vector2& parentSize, const Vector2& myCurrentSize, Vector2& absoluteTrueParentPosition, bool accountOwnSize = true) const;
};
#endif