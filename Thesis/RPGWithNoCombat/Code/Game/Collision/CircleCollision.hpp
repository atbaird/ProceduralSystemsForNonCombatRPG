#pragma once

#ifndef CIRCLECOLLISION_HPP
#define CIRCLECOLLISION_HPP
#include "Game/Collision/Collision.hpp"

class CircleCollision : public Collision
{
private:
protected:
	float m_radius = 0.45f;
public:
	//Constructors
	CircleCollision();
	virtual ~CircleCollision();

	//Setters
	void SetRadius(float radius);

	//Getters
	float GetRadius() const;
	
	//operations
	virtual bool CheckIfPointInCollision(const Vector2& point, const Vector2& myCenter) const override;
};
#endif