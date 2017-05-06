#include "Game/Collision/LineCollision.hpp"
#include <cmath>


//Constructors
LineCollision::LineCollision(Vector2 startPt, Vector2 endPt)
	: Collision(LINE_COLLISION),
	m_DistDir(endPt)
{
	m_position = startPt;
	m_DistDir -= startPt;
}

void LineCollision::SetEndPoint(Vector2 endPt)
{
	m_DistDir = endPt - m_position;
}

void LineCollision::SetDistanceInDirection(Vector2 end)
{
	m_DistDir = end;
}

//Getters
const Vector2 LineCollision::GetEndPoint() const
{
	return m_DistDir + m_position;
}

//operations
bool LineCollision::CheckIfPointInCollision(const Vector2& point, const Vector2& myPosition) const
{
	const Vector2 pointStartDiff = point - myPosition;
	const Vector2 endStartDiff = m_DistDir;
	Vector2 normPointStartDiff = pointStartDiff;
	Vector2 normEndStartDiff = endStartDiff;
	Vector2 endPoint = GetEndPoint();
	
	const float lineLength = normEndStartDiff.GetLength();
	const float pointLength = normPointStartDiff.normalize();

	{
		//referencing this bit of code: http://stackoverflow.com/questions/11907947/how-to-check-if-a-point-lies-on-a-line-between-2-other-points
		//This bit of code just guarantees that the point is along the same direction as the start to end, no matter the direction or distance along that line.
		float crossMult = (pointStartDiff.x * endStartDiff.y) - (pointStartDiff.y * endStartDiff.x);

		if (crossMult != 0)
		{
			//Point not along the line.
			return false;
		}
	}

	{
		//referencing this bit of code: http://stackoverflow.com/questions/11907947/how-to-check-if-a-point-lies-on-a-line-between-2-other-points
		float absEndStartX = abs(endStartDiff.x);
		float absEndStartY = abs(endStartDiff.y);
	
		if (absEndStartX >= absEndStartY)
		{
			//x movement greater than y movement.
			if (endStartDiff.x > 0.f)
			{
				//line moves in pos x direction
				if (point.x >= myPosition.x && point.x <= endPoint.x)
				{
					return true;
				}
			}
			else
			{
				//line moves in negative x direction
				if (point.x >= endPoint.x && point.x <= myPosition.x)
				{
					return true;
				}
			}
		}
		else
		{
			//y movement greater than x movement
			if (endStartDiff.y > 0.f)
			{
				//line moves in pos y direction
				if (point.y >= myPosition.y && point.y <= endPoint.y)
				{
					return true;
				}
			}
			else
			{
				//line moves in neg y direction
				if (point.y >= endPoint.y && point.y <= myPosition.y)
				{
					return true;
				}
			}
		}
		return false;
	}
}