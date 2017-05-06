#include "Engine/UI/UIStructs/UIDimensionRange.hpp"


//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIDimensionRange::UIDimensionRange()
{

}

UIDimensionRange::UIDimensionRange(float minMax, bool setMaxToo)
	: m_min(minMax)
{
	if (setMaxToo)
	{
		m_max = minMax;
	}
}

UIDimensionRange::UIDimensionRange(float min, float max)
	: m_min(min),
	m_max(max)
{

}

UIDimensionRange::~UIDimensionRange()
{

}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Operations
bool UIDimensionRange::TestValueIsInRange(float val) const
{
	if (m_negativeValsAreTheSameAsDontCares)
	{
		if (m_min < 0.f && m_max < 0.f)
		{
			return true;
		}
		else if (m_min < 0.f && val <= m_max)
		{
			return true;
		}
		else if (m_max < 0.f && val >= m_min)
		{
			return true;
		}
		else if (val < m_min || val > m_max)
		{
			return false;
		}
	}
	else if (val < m_min || val > m_max)
	{
		return false;
	}
	return true;
}


bool UIDimensionRange::TestValueIsInRangeFrom(float val, float center) const
{
	float temp = (val - (center / 2.f));
	return TestValueIsInRange(temp);
}


bool UIDimensionRange::TestValueLessThanMin(float val) const
{
	if (m_negativeValsAreTheSameAsDontCares && m_min < 0.f)
	{
		return false;
	}
	else if (val < m_min)
	{
		return true;
	}
	return false;
}

bool UIDimensionRange::TestValueGreaterThanMax(float val) const
{
	if (m_negativeValsAreTheSameAsDontCares && m_max < 0.f)
	{
		return false;
	}
	else if (val > m_max)
	{
		return true;
	}
	return false;
}