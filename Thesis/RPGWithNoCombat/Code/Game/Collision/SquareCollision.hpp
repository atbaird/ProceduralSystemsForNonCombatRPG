#pragma once

#ifndef SQUARECOLLISION_HPP
#define SQUARECOLLISION_HPP
#include "Game/Collision/Collision.hpp"

class SquareCollision : public Collision
{
private:
protected:
	Vector2 m_boxSize = Vector2::vec2_ones;
public:
	//Constructors
	SquareCollision();
	virtual ~SquareCollision();

	//Setters
	void SetBoxSize(Vector2 size);

	//Getters
	const Vector2& GetBoxSize() const;

	//operations
	virtual bool CheckIfPointInCollision(const Vector2& point, const Vector2& myBLPos) const override;
};
#endif