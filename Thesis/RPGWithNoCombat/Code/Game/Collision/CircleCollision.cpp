#include "Game/Collision/CircleCollision.hpp"
#include "Game/GameEntities/BaseGameEntity.hpp"


//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//Constructors
CircleCollision::CircleCollision()
	: Collision(CIRCLE_COLLISION)
{

}
CircleCollision::~CircleCollision()
{
	Collision::~Collision();
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//Setters
void CircleCollision::SetRadius(float radius)
{
	m_radius = radius;
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//Getters
float CircleCollision::GetRadius() const
{
	return m_radius;
}

bool CircleCollision::CheckIfPointInCollision(const Vector2& point, const Vector2& myCenter) const
{
	const Vector2 diff = myCenter - point;
	if (diff.GetLength() <= m_radius)
	{
		return true;
	}
	return false;
}