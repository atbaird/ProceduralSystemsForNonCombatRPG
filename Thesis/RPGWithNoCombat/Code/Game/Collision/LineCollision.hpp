#pragma once

#ifndef LINECOLLISION_HPP
#define LINECOLLISION_HPP
#include "Game/Collision/Collision.hpp"

class LineCollision : public Collision
{
private:
protected:
	//m_position is being treated as the start point.
	Vector2 m_DistDir = Vector2::vec2_zeros;
	Vector2 m_position = Vector2::vec2_zeros;
public:
	//Constructors
	LineCollision(Vector2 startPt = Vector2(0.f, 0.f), Vector2 endPt = Vector2(1.f,1.f));

	//Setters
	void SetEndPoint(Vector2 endPt);
	void SetDistanceInDirection(Vector2 distDir);

	//Getters
	const Vector2 GetEndPoint() const;

	//operations
	virtual bool CheckIfPointInCollision(const Vector2& point, const Vector2& myPosition) const override;
};

#endif // !LINECOLLISION_HPP