#pragma once

#ifndef COLLISIONTESTFUNCTIONS_HPP
#define COLLISIONTESTFUNCTIONS_HPP
#include "Game/Collision/CollisionTestResult.hpp"
#include "Game/Collision/Collision.hpp"
#define STATIC 

class CircleCollision;
class SquareCollision;
class LineCollision;

class CollisionTestFunctions
{
public:
	//Public Functions
	static CollisionTestResult TestCollisionFromSquareCollisionToCircleCollisionNESW(
		const SquareCollision* collision1, const Vector2& col1BLCoords,
		const CircleCollision* collision2, const Vector2& circCenter, bool expectAlongXAxis = false, 
		bool add1ToEdge = false);
	static CollisionTestResult TestCollisionFromSquareCollisionToCircleCollisionDiag(
		const SquareCollision* collision1, const Vector2& col1BLCoords,
		const CircleCollision* collision2, const Vector2& circCenter, bool add1X = false, bool add1Y = false);
	static CollisionTestResult TestCollisionFromCircleCollisionToCircleCollision(
		const CircleCollision* collision1, const Vector2& circ1Center,
		const CircleCollision* collision2, const Vector2& circ2Center);

protected:
};
#endif