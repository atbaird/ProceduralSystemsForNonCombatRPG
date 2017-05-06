#include "Game/Collision/CollisionTestResult.hpp"


//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Constructors


//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//General Setters
void CollisionTestResult::SetOverlapDetected(bool detected)
{
	m_OverlapDetected = detected;
}

void CollisionTestResult::SetCollision1Type(CollisionType type)
{
	m_collision1Type = type;
}

void CollisionTestResult::SetCollision2Type(CollisionType type)
{
	m_collision2Type = type;
}

void CollisionTestResult::SetCollision1Center(const Vector2& center)
{
	m_collision1Center = center;
}

void CollisionTestResult::SetCollision2Center(const Vector2& center)
{
	m_collision2Center = center;
}

void CollisionTestResult::SetDistanceBetweenCenters(const Vector2& distance)
{
	m_distanceBetweenCenters = distance;
}

void CollisionTestResult::SetCollision1Solid(bool solid)
{
	m_collision1Solid = solid;
}

void CollisionTestResult::SetCollision2Solid(bool solid)
{
	m_collision2Solid = solid;
}

void CollisionTestResult::SetCollision1Size(const Vector2& size)
{
	m_collision1Size = size;
}

void CollisionTestResult::SetCollision2Size(const Vector2& size)
{
	m_collision2Size = size;
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Circle Collision only Setters
void CollisionTestResult::SetCollision1Radius(float radius)
{
	m_collision1Size = Vector2(radius,0.f);
}

void CollisionTestResult::SetCollision2Radius(float radius)
{
	m_collision2Size = Vector2(radius, 0.f);
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//General Getters
bool CollisionTestResult::GetOverlapDetected() const
{
	return m_OverlapDetected;
}

CollisionType CollisionTestResult::GetCollision1Type() const
{
	return m_collision1Type;
}

CollisionType CollisionTestResult::GetCollision2Type() const
{
	return m_collision2Type;
}

const Vector2& CollisionTestResult::GetCollision1Center() const
{
	return m_collision1Center;
}

const Vector2& CollisionTestResult::GetCollision2Center() const
{
	return m_collision2Center;
}

const Vector2& CollisionTestResult::GetDistanceBetweenCenters() const
{
	return m_distanceBetweenCenters;
}

bool CollisionTestResult::GetCollision1Solid() const
{
	return m_collision1Solid;
}

bool CollisionTestResult::GetCollision2Solid() const
{
	return m_collision2Solid;
}

const Vector2& CollisionTestResult::GetCollision1Size() const
{
	return m_collision1Size;
}

const Vector2& CollisionTestResult::GetCollision2Size() const
{
	return m_collision2Size;
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Circle Collision only Getters
float CollisionTestResult::GetCollision1Radius() const
{
	return m_collision1Size.x;
}

float CollisionTestResult::GetCollision2Radius() const
{
	return m_collision2Size.x;
}