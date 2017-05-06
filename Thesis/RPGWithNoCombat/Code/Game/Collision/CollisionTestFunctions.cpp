#include "Game/Collision/CollisionTestFunctions.hpp"
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/Collision/CircleCollision.hpp"
#include "Game/Collision/SquareCollision.hpp"
#include "Game/Collision/LineCollision.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/Map/Tile.hpp"

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Public functions


STATIC CollisionTestResult CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionNESW(
	const SquareCollision* collision1,
	const Vector2& col1BLCoords,
	const CircleCollision* collision2, 
	const Vector2& circCenter, 
	bool expectAlongXAxis,
	bool add1ToEdge)
{
	CollisionTestResult test;
	if (collision1 == nullptr || collision2 == nullptr)
	{
		return test;
	}
	Vector2 coords = col1BLCoords;
	Vector2 tileScale = Tile::GetTileScale();
	test.SetCollision1Center(coords + (tileScale * 0.5f));
	test.SetCollision1Size(tileScale);
	test.SetCollision1Type(SQUARE_COLLISION);
	test.SetCollision2Center(circCenter);
	test.SetCollision2Radius(collision2->GetRadius());
	test.SetCollision2Type(CIRCLE_COLLISION);
	float CircleEdge = 0.f;
	if (add1ToEdge)
	{
		coords += tileScale;
		CircleEdge = -collision2->GetRadius();
	}
	else
	{
		CircleEdge = collision2->GetRadius();
	}

	float axisDifference = 0.f;
	if (expectAlongXAxis)
	{
		axisDifference = (circCenter.x + CircleEdge) - coords.x;
		if ((add1ToEdge && axisDifference < 0.f)
			|| (add1ToEdge == false && axisDifference > 0.f))
		{
			test.SetOverlapDetected(true);
		}
		test.SetDistanceBetweenCenters(Vector2(axisDifference, 0.f));
	}
	else
	{
		axisDifference = (circCenter.y + CircleEdge) - coords.y;
		if ((add1ToEdge && axisDifference < 0.f)
			|| (add1ToEdge == false && axisDifference > 0.f))
		{
			test.SetOverlapDetected(true);
		}
		test.SetDistanceBetweenCenters(Vector2(0.f, axisDifference));
	}

	/*
	TileCoords North = TileCoords(entityTileCoords.x, entityTileCoords.y + 1);
	if (IsTileSolid(North) == true)
	{
		float yDifference = (entityPosition.y + CollisionRadius) - North.y;
		if (yDifference > 0)
		{
			entityPosition = Vector2(entityPosition.x, entityPosition.y - yDifference);
			entity->SetPosition(entityPosition);
		}
	}
	TileCoords East = TileCoords(entityTileCoords.x + 1, entityTileCoords.y);
	if (IsTileSolid(East) == true)
	{
		float xDifference = (entityPosition.x + CollisionRadius) - East.x;
		if (xDifference > 0)
		{
			entityPosition = Vector2(entityPosition.x - xDifference, entityPosition.y);
			entity->SetPosition(entityPosition);
		}
	}
	*/

	return test;
}
STATIC CollisionTestResult CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionDiag(const SquareCollision* collision1, 
	const Vector2& col1BLCoords,
	const CircleCollision* collision2,
	const Vector2& circCenter,
	bool add1X,
	bool add1Y)
{
	CollisionTestResult test;
	if (collision1 == nullptr || collision2 == nullptr)
	{
		return test;
	}
	Vector2 cornerCoords = col1BLCoords;
	if (add1X)
	{
		cornerCoords.x += 1.f;
	}

	if (add1Y)
	{
		cornerCoords.y += 1.f;
	}

	Vector2 tileScale = Tile::GetTileScale();
	test.SetCollision1Center(cornerCoords + (tileScale * 0.5f));
	test.SetCollision1Size(tileScale);
	test.SetCollision1Type(SQUARE_COLLISION);
	test.SetCollision2Center(circCenter);
	test.SetCollision2Radius(collision2->GetRadius());
	test.SetCollision2Type(CIRCLE_COLLISION);

	Vector2 displacement = circCenter - cornerCoords;
	float distanceBetweenThem = displacement.GetLength();
	if (distanceBetweenThem < collision2->GetRadius())
	{
		float overlapDepth = distanceBetweenThem - collision2->GetRadius();
		test.SetOverlapDetected(true);
		test.SetDistanceBetweenCenters(displacement.normalize() * overlapDepth);
	}
	return test;
}

CollisionTestResult CollisionTestFunctions::TestCollisionFromCircleCollisionToCircleCollision(const CircleCollision* collision1, 
	const Vector2& circ1Center,
	const CircleCollision* collision2, 
	const Vector2& circ2Center)
{
	CollisionTestResult test;
	if (collision1 == nullptr || collision2 == nullptr)
	{
		return test;
	}

	test.SetCollision1Type(CIRCLE_COLLISION);
	test.SetCollision1Radius(collision1->GetRadius());
	test.SetCollision1Center(circ1Center);
	test.SetCollision2Type(CIRCLE_COLLISION);
	test.SetCollision2Radius(collision2->GetRadius());
	test.SetCollision2Center(circ2Center);

	return test;
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//Protected functions Same Collisions