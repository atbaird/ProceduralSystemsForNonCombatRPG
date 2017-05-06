#pragma once

#ifndef UIANIMATEDPROPERTIES_HPP
#define UIANIMATEDPROPERTIES_HPP
#include "Engine/UI/UIStructs/UIAnimations/UIKeyFramedSequence.hpp"

template<typename T_out>
class UIAnimatedProperties
{
private:
protected:
	UIKeyFramedSequence<T_out> m_keyFramedSequence;
	float m_endTime = 1.f;
public:
	//Constructors
	UIAnimatedProperties(float endTime = 1.f)
		:
		m_endTime(endTime)
	{

	};
	UIAnimatedProperties(const UIKeyFramedSequence<T_out>& keyframedSequence, float endTime = 1.f)
		:
		m_keyFramedSequence(keyframedSequence),
		m_endTime(endTime)
	{

	};
	virtual ~UIAnimatedProperties()
	{
	};

	//Setters and Adders
	virtual void AddOrSetValue(const UIKeyFrame<T_out>& value);
	virtual bool SetValue(const UIKeyFrame<T_out>& value);
	virtual void AddValue(const UIKeyFrame<T_out>& value);
	virtual void AddOrSetValueWithNonPercentageTime(const UIKeyFrame<T_out>& value);
	virtual bool SetValueWithNonPercentageTime(const UIKeyFrame<T_out>& value);

	virtual void AddValueWithNonPercentageTime(const UIKeyFrame<T_out>& value);
	virtual void SetEndTime(float endTime);
	virtual void ClearFrames();

	//Update
	virtual const T_out Update(float& current_time, float deltaTime, bool& timeIsLessThanEqualToZero,
		T_out& nextFrame, int& firstIdx, int& secondIdx) const;
	virtual const T_out* UpdatePtr(float& current_time, float deltaTime, bool& timeIsLessThanEqualToZero, const T_out*& nextFrame,
		int& firstIdx, int& secondIdx) const;
	virtual T_out* UpdateEditablePtr(float& current_time, float deltaTime, bool& timeIsLessThanEqualToZero, T_out*& nextFrame,
		int& firstIdx, int& secondIdx);

	//Getters
	size_t GetNumberOfFrames() const;
	
};

//--------------------------------------------------------------------------------------------------------------------------------
//Constructors

//--------------------------------------------------------------------------------------------------------------------------------
//Setters and Adders
template<typename T_out>
void UIAnimatedProperties<T_out>::AddOrSetValue(const UIKeyFrame<T_out>& value)
{
	m_keyFramedSequence.SetOrAddKeyFrame(value);
}

template<typename T_out>
bool UIAnimatedProperties<T_out>::SetValue(const UIKeyFrame<T_out>& value)
{
	return m_keyFramedSequence.SetKeyFrame(value);
}

template<typename T_out>
void UIAnimatedProperties<T_out>::AddValue(const UIKeyFrame<T_out>& value)
{
	m_keyFramedSequence.AddKeyFrame(value);
}

template<typename T_out>
void UIAnimatedProperties<T_out>::SetEndTime(float endTime)
{
	m_endTime = endTime;
}

template<typename T_out>
void UIAnimatedProperties<T_out>::ClearFrames()
{
	m_keyFramedSequence.ClearFrames();
}

template<typename T_out>
void UIAnimatedProperties<T_out>::AddOrSetValueWithNonPercentageTime(const UIKeyFrame<T_out>& value)
{
	m_keyFramedSequence.SetOrAddKeyFrameWithNonPercentageTime(value, m_endTime);
}

template<typename T_out>
bool UIAnimatedProperties<T_out>::SetValueWithNonPercentageTime(const UIKeyFrame<T_out>& value)
{
	return m_keyFramedSequence.SetKeyFrameWithNonPercentageTime(value, m_endTime);
}

template<typename T_out>
void UIAnimatedProperties<T_out>::AddValueWithNonPercentageTime(const UIKeyFrame<T_out>& value)
{
	m_keyFramedSequence.AddKeyFrameWithNonPercentageTime(value, m_endTime);
}

//--------------------------------------------------------------------------------------------------------------------------------
//Update
template<typename T_out>
const T_out UIAnimatedProperties<T_out>::Update(float& current_time, float deltaTime, bool& timeIsLessThanEqualToZero, T_out& nextFrame
	, int& firstIdx, int& secondIdx) const
{
	firstIdx = -1;
	secondIdx = -1;
	if (m_endTime <= 0.f)
	{
		timeIsLessThanEqualToZero = true;
		nextFrame = T_out();
		return T_out();
	}
	current_time += deltaTime;
	if (current_time > m_endTime)
	{
		current_time = m_endTime;
	}
	float percentageTime = current_time / m_endTime;

	nextFrame = m_keyFramedSequence.GetFrameAfterCopyOfValueBasedOnPercentageTime(percentageTime, firstIdx, secondIdx);
	T_out val = T_out();
	if (secondIdx != -1)
	{
		bool found = false;
		val = m_keyFramedSequence.GetValueAtKeyFrameIdx(secondIdx, found);
		if (found == false)
		{
			val = T_out();
		}
	}
	return val;
}

template<typename T_out>
const T_out* UIAnimatedProperties<T_out>::UpdatePtr(float& current_time, float deltaTime, bool& timeIsLessThanEqualToZero, const T_out*& nextFrame
	, int& firstIdx, int& secondIdx) const
{
	firstIdx = -1;
	secondIdx = -1;
	if (m_endTime <= 0.f)
	{
		timeIsLessThanEqualToZero = true;
		nextFrame = nullptr;
		return nullptr;
	}
	current_time += deltaTime;
	if (current_time > m_endTime)
	{
		current_time = m_endTime;
	}
	float percentageTime = current_time / m_endTime;

	nextFrame = m_keyFramedSequence.GetPtrFrameAfterCopyOfValueBasedOnPercentageTime(percentageTime, firstIdx, secondIdx);
	const T_out* val = nullptr;
	if (secondIdx != -1)
	{
		bool found = false;
		val = m_keyFramedSequence.GetPtrValueAtKeyFrameIdx(secondIdx, found);
		if (found == false)
		{
			val = nullptr;
		}
	}
	return val;
}

template<typename T_out>
T_out* UIAnimatedProperties<T_out>::UpdateEditablePtr(float& current_time, float deltaTime, bool& timeIsLessThanEqualToZero,
	T_out*& nextFrame, int& firstIdx, int& secondIdx)
{
	firstIdx = -1;
	secondIdx = -1;
	if (m_endTime <= 0.f)
	{
		timeIsLessThanEqualToZero = true;
		nextFrame = nullptr;
		return nullptr;
	}
	current_time += deltaTime;
	if (current_time > m_endTime)
	{
		current_time = m_endTime;
	}
	float percentageTime = current_time / m_endTime;

	nextFrame = m_keyFramedSequence.GetEditablePtrFrameAfterCopyOfValueBasedOnPercentageTime(percentageTime, firstIdx, secondIdx);
	T_out* val = nullptr;
	if (secondIdx != -1)
	{
		bool found = false;
		val = m_keyFramedSequence.GetEditablePtrValueAtKeyFrameIdx(secondIdx, found);
		if (found == false)
		{
			val = nullptr;
		}
	}

	return val;
}

template<typename T_out>
//Getters
size_t UIAnimatedProperties<T_out>::GetNumberOfFrames() const
{
	return m_keyFramedSequence.GetNumberOfFrames();
}

#endif;