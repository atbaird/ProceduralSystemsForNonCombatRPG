#pragma once

#ifndef COLLISIONTESTRESULT_HPP
#define COLLISIONTESTRESULT_HPP
#include "Game/Collision/CollisionType.hpp"
#include "Engine/Math/Vector2.hpp"

struct CollisionTestResult
{
private:
protected:
	//General Variables
	CollisionType m_collision1Type = INVALID_COLLISION;
	CollisionType m_collision2Type = INVALID_COLLISION;
	Vector2 m_collision1Center = Vector2::vec2_zeros;
	Vector2 m_collision2Center = Vector2::vec2_zeros;
	Vector2 m_distanceBetweenCenters = Vector2::vec2_zeros;
	Vector2 m_collision1Size = Vector2::vec2_zeros;
	Vector2 m_collision2Size = Vector2::vec2_zeros;
	bool m_OverlapDetected = false;
	bool m_collision1Solid = false;
	bool m_collision2Solid = false;

	//Circle Collision only Variables

	//Square Collision only Variables
public:
	//Constructors

	//General Setters
	void SetOverlapDetected(bool detected);
	void SetCollision1Type(CollisionType type);
	void SetCollision2Type(CollisionType type);
	void SetCollision1Center(const Vector2& center);
	void SetCollision2Center(const Vector2& center);
	void SetDistanceBetweenCenters(const Vector2& distance);
	void SetCollision1Solid(bool solid);
	void SetCollision2Solid(bool solid);
	void SetCollision1Size(const Vector2& size);
	void SetCollision2Size(const Vector2& size);
	
	//Circle Collision only Setters
	void SetCollision1Radius(float radius);
	void SetCollision2Radius(float radius);

	//General Getters
	bool GetOverlapDetected() const;
	CollisionType GetCollision1Type() const;
	CollisionType GetCollision2Type() const;
	const Vector2& GetCollision1Center() const;
	const Vector2& GetCollision2Center() const;
	const Vector2& GetDistanceBetweenCenters() const;
	bool GetCollision1Solid() const;
	bool GetCollision2Solid() const;
	const Vector2& GetCollision1Size() const;
	const Vector2& GetCollision2Size() const;

	//Circle Collision only Getters
	float GetCollision1Radius() const;
	float GetCollision2Radius() const;
};
#endif // !COLLISIONTESTRESULT_HPP