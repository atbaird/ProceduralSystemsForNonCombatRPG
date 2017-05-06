#pragma once

#ifndef UIKEYFRAME_HPP
#define UIKEYFRAME_HPP

template <typename T>
class UIKeyFrame
{
private:
protected:
	float m_timePercentage = 0.f; //clamp from 0 to 1.
	T m_value;
public:
	//Constructors
	UIKeyFrame(const T& val, float time = 0.f)
		: m_timePercentage(time),
		m_value(val)
	{
	};
	virtual ~UIKeyFrame()
	{
	};

	//Setters
	void SetTimePercentage(float timePercentage);
	void SetValue(const T& value);

	//Getters
	float GetTimePercentage() const;
	T GetValueCopy() const;
	const T* GetStoredValue() const;
	T* GetEditableStoredValue();
};

//----------------------------------------------------------------------------------------------------------------------------------------------
//Constructors

//----------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------
//Setters
template<typename T>
void UIKeyFrame<T>::SetTimePercentage(float timePercentage)
{
	m_timePercentage = timePercentage;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
void UIKeyFrame<T>::SetValue(const T& value)
{
	m_value = value;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//Getters
template<typename T>
float UIKeyFrame<T>::GetTimePercentage() const
{
	return m_timePercentage;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
T UIKeyFrame<T>::GetValueCopy() const
{
	return m_value;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
const T* UIKeyFrame<T>::GetStoredValue() const
{
	return &m_value;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
T* UIKeyFrame<T>::GetEditableStoredValue()
{
	return &m_value;
}

#endif