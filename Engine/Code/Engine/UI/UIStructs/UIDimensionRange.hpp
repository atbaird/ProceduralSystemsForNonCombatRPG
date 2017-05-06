#pragma once

#ifndef UIDIMENSIONRANGE_HPP
#define UIDIMENSIONRANGE_HPP

struct UIDimensionRange
{
private:
protected:
public:
	//Variables
	float m_min = 0.f;
	float m_max = -1.f;
	bool m_negativeValsAreTheSameAsDontCares = true;

	//Constructors
	UIDimensionRange();
	UIDimensionRange(float minMax, bool setMaxToo = false);
	UIDimensionRange(float min, float max);
	virtual ~UIDimensionRange();

	//Operations
	bool TestValueIsInRange(float val) const;
	bool TestValueIsInRangeFrom(float val, float center) const;
	bool TestValueLessThanMin(float val) const;
	bool TestValueGreaterThanMax(float val) const;
};
#endif