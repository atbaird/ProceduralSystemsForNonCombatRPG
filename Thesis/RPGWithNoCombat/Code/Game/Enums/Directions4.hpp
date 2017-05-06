#pragma once

#ifndef DIRECTIONS4_HPP
#define DIRECTIONS4_HPP

enum Directions4
{
	INVALID_DIR4 = -1,
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NUMBER_OF_DIRECTIONS4
};

enum Directions8
{
	NORTHWEST = NUMBER_OF_DIRECTIONS4,
	SOUTHWEST,
	SOUTHEAST,
	NORTHEAST,
	NUMBER_OF_DIRECTIONS8
};

#define STATIC
class Vector2;
struct Directions
{
	static const Vector2 s_North;
	static const Vector2 s_East;
	static const Vector2 s_South;
	static const Vector2 s_West;

	static const Vector2 s_NorthWest;
	static const Vector2 s_SouthWest;
	static const Vector2 s_SouthEast;
	static const Vector2 s_NorthEast;

protected:
	static Vector2 s_DirectionsInOrderOfEnums[NUMBER_OF_DIRECTIONS8];
	static float s_AngleTowardsDirection[NUMBER_OF_DIRECTIONS8];
	static float s_AngleDegreesTowardsDirection[NUMBER_OF_DIRECTIONS8];

	static Directions s_Directions;
	Directions();

public:

	static void ReloadDirectionsIfSomeHowGetsUnhinged();
	static const Vector2& GetDirectionForDirectionEnum(Directions4 dir);
	static const Vector2& GetDirectionForDirectionEnum(Directions8 dir);
	static float GetAngleForDirectionEnum(Directions4 dir);
	static float GetAngleForDirectionEnum(Directions8 dir);
	static float GetAngleForDirectionVector(const Vector2& dir);
	static float GetAngleDegreesForDirectionEnum(Directions4 dir);
	static float GetAngleDegreesForDirectionEnum(Directions8 dir);
	static float GetAngleDegreesForDirectionVector(const Vector2& dir);
	static float CalcShortestAngleDifferenceFromDegrees(float angle1, float angle2);


};

#endif
