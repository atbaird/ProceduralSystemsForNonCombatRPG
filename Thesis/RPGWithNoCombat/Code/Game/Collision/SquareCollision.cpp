#include "Game/Collision/SquareCollision.hpp"

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//Constructors
SquareCollision::SquareCollision()
	: Collision(SQUARE_COLLISION)
{

}

SquareCollision::~SquareCollision()
{
	Collision::~Collision();
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//Setters
void SquareCollision::SetBoxSize(Vector2 size)
{
	m_boxSize = size;
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//Getters
const Vector2& SquareCollision::GetBoxSize() const
{
	return m_boxSize;
}


//operations
bool SquareCollision::CheckIfPointInCollision(const Vector2& point, const Vector2& myBLPos) const
{
	const Vector2 bl = myBLPos - (m_boxSize / 2.f);
	const Vector2 tr = bl + m_boxSize;
	if (point.x >= bl.x && point.x <= tr.x
		&& point.y >= bl.y && point.y <= tr.y)
	{
		return true;
	}
	return false;
}