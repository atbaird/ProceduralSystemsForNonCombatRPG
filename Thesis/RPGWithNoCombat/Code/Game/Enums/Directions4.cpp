#include "Game/Enums/Directions4.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/ExtendedMath.hpp"

STATIC const Vector2 Directions::s_North = Vector2(0.f, 1.f);
STATIC const Vector2 Directions::s_East = Vector2(1.f, 0.f);
STATIC const Vector2 Directions::s_South = Vector2(0.f, -1.f);
STATIC const Vector2 Directions::s_West = Vector2(-1.f, 0.f);

STATIC const Vector2 Directions::s_NorthWest = (Vector2(-1.f, 1.f)).normalize();
STATIC const Vector2 Directions::s_SouthWest = (Vector2(-1.f, -1.f)).normalize();
STATIC const Vector2 Directions::s_SouthEast = (Vector2(1.f, -1.f)).normalize();
STATIC const Vector2 Directions::s_NorthEast = (Vector2(1.f, 1.f)).normalize();

STATIC Vector2 Directions::s_DirectionsInOrderOfEnums[NUMBER_OF_DIRECTIONS8];
STATIC float Directions::s_AngleTowardsDirection[NUMBER_OF_DIRECTIONS8];
STATIC float Directions::s_AngleDegreesTowardsDirection[NUMBER_OF_DIRECTIONS8];
STATIC Directions Directions::s_Directions;

Directions::Directions()
{
	ReloadDirectionsIfSomeHowGetsUnhinged();
}

STATIC void Directions::ReloadDirectionsIfSomeHowGetsUnhinged()
{
	s_DirectionsInOrderOfEnums[NORTH] = Vector2(0.f, 1.f);
	s_DirectionsInOrderOfEnums[EAST] = Vector2(1.f, 0.f);
	s_DirectionsInOrderOfEnums[SOUTH] = Vector2(0.f, -1.f);
	s_DirectionsInOrderOfEnums[WEST] = Vector2(-1.f, 0.f);
	s_DirectionsInOrderOfEnums[NORTHWEST] = (Vector2(-1.f, 1.f)).normalize();
	s_DirectionsInOrderOfEnums[NORTHEAST] = (Vector2(1.f, 1.f)).normalize();
	s_DirectionsInOrderOfEnums[SOUTHEAST] = (Vector2(-1.f, -1.f)).normalize();
	s_DirectionsInOrderOfEnums[SOUTHWEST] = (Vector2(1.f, -1.f)).normalize();

	for (Directions8 dir = (Directions8)NORTH; dir < NUMBER_OF_DIRECTIONS8; dir = (Directions8)(dir + 1))
	{
		Vector2 direction = s_DirectionsInOrderOfEnums[dir];
		s_AngleTowardsDirection[dir] = atan2(direction.y, direction.x);
		s_AngleDegreesTowardsDirection[dir] = RadiansToDegrees(s_AngleTowardsDirection[dir]);
	}
}

STATIC const Vector2& Directions::GetDirectionForDirectionEnum(Directions4 dir)
{
	return GetDirectionForDirectionEnum((Directions8)dir);
}

STATIC const Vector2& Directions::GetDirectionForDirectionEnum(Directions8 dir)
{
	return s_DirectionsInOrderOfEnums[dir];
}

STATIC float Directions::GetAngleForDirectionEnum(Directions4 dir)
{
	return GetAngleForDirectionEnum((Directions8)dir);
}

STATIC float Directions::GetAngleForDirectionEnum(Directions8 dir)
{
	return s_AngleTowardsDirection[dir];
}

STATIC float Directions::GetAngleForDirectionVector(const Vector2& dir)
{
	return atan2(dir.y, dir.x);
}

STATIC float Directions::GetAngleDegreesForDirectionEnum(Directions4 dir)
{
	return GetAngleDegreesForDirectionEnum((Directions8)dir);
}

STATIC float Directions::GetAngleDegreesForDirectionEnum(Directions8 dir)
{
	return s_AngleDegreesTowardsDirection[dir];
}

STATIC float Directions::GetAngleDegreesForDirectionVector(const Vector2& dir)
{
	float dirAngle = GetAngleForDirectionVector(dir);
	return RadiansToDegrees(dirAngle);
}

STATIC float Directions::CalcShortestAngleDifferenceFromDegrees(float angle1, float angle2)
{
	float diff = angle2 - angle1;
	float diffAdd = (angle2 + 360.f) - angle1;
	float diffSub = (angle2 - 360.f) - angle1;

	if (abs(diff) < abs(diffAdd)
		&& abs(diff) < abs(diffSub))
	{
		return diff;
	}
	else if (abs(diffAdd) < abs(diff)
		&& abs(diffAdd) < abs(diffSub))
	{
		return diffAdd;
	}
	else
	{
		return diffSub;
	}
}