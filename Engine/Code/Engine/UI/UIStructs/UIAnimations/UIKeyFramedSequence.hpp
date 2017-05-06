#pragma once

#ifndef UIKEYFRAMEDSEQUENCE_HPP
#define UIKEYFRAMEDSEQUENCE_HPP
#include "Engine/UI/UIStructs/UIAnimations/UIKeyFrame.hpp"
#include <vector>

template <typename T>
class UIKeyFramedSequence
{
private:
protected:
	std::vector<UIKeyFrame<T>> m_keyFrames;
public:
	//Constructors
	UIKeyFramedSequence()
	{
	};
	UIKeyFramedSequence(const std::vector<UIKeyFrame<T>>& keyFrames)
		: m_keyFrames(keyFrames)
	{
	};
	virtual ~UIKeyFramedSequence()
	{
	};

	//Set or Add
	void SetOrAddKeyFrame(const UIKeyFrame<T>& keyFrame);
	bool SetKeyFrame(const UIKeyFrame<T>& keyFrame);
	void AddKeyFrame(const UIKeyFrame<T>& keyFrame);
	void SetOrAddKeyFrameWithNonPercentageTime(const UIKeyFrame<T>& keyFrame, float end_time);
	bool SetKeyFrameWithNonPercentageTime(const UIKeyFrame<T>& keyFrame, float end_time);
	void AddKeyFrameWithNonPercentageTime(const UIKeyFrame<T>& keyFrame, float end_time);
	void ClearFrames();

	//Getters
	const T* GetPtrCopyOfValueBasedOnPercentageTime(float percentageTime) const;
	T* GetEditablePtrCopyOfValueBasedOnPercentageTime(float percentageTime);
	const T GetCopyOfValueBasedOnPercentageTime(float percentageTime) const;
	const T* GetPtrFrameAfterCopyOfValueBasedOnPercentageTime(float percentageTime, int& firstIdx, int& secondIdx) const;
	T* GetEditablePtrFrameAfterCopyOfValueBasedOnPercentageTime(float percentageTime, int& firstIdx, int& secondIdx);
	const T GetFrameAfterCopyOfValueBasedOnPercentageTime(float percentageTime, int& firstIdx, int& secondIdx) const;
	UIKeyFrame<T> GetKeyFrameAtIdx(size_t idx, bool& found) const;

	T GetValueAtKeyFrameIdx(size_t idx, bool& found) const;
	const T* UIKeyFramedSequence<T>::GetPtrValueAtKeyFrameIdx(size_t idx, bool& found) const;
	T* UIKeyFramedSequence<T>::GetEditablePtrValueAtKeyFrameIdx(size_t idx, bool& found);
	size_t GetNumberOfFrames() const;
};

//------------------------------------------------------------------------------------------------------------------------------
//Constructors

//------------------------------------------------------------------------------------------------------------------------------
//Set or Add
template <typename T>
void UIKeyFramedSequence<T>::SetOrAddKeyFrame(const UIKeyFrame<T>& keyFrame)
{
	if (SetKeyFrame(keyFrame) == false)
	{
		AddKeyFrame(keyFrame);
	}
}

//------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool UIKeyFramedSequence<T>::SetKeyFrame(const UIKeyFrame<T>& keyFrameToInsert)
{
	for (size_t keyFrameIdx = 0; keyFrameIdx < m_keyFrames.size(); keyFrameIdx++)
	{
		UIKeyFrame<T>& keyFrame = m_keyFrames[keyFrameIdx];
		if (keyFrame.GetTimePercentage() == keyFrame.GetTimePercentage())
		{
			m_keyFrames[keyFrameIdx] = keyFrameToInsert;
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void UIKeyFramedSequence<T>::AddKeyFrame(const UIKeyFrame<T>& keyFrame)
{
	m_keyFrames.push_back(keyFrame);
}

template <typename T>
void UIKeyFramedSequence<T>::SetOrAddKeyFrameWithNonPercentageTime(const UIKeyFrame<T>& keyFrame, float end_time)
{
	if (SetKeyFrameWithNonPercentageTime(keyFrame, end_time) == false)
	{
		AddKeyFrameWithNonPercentageTime(keyFrame, end_time);
	}
}

template <typename T>
bool UIKeyFramedSequence<T>::SetKeyFrameWithNonPercentageTime(const UIKeyFrame<T>& keyFrame, float end_time)
{
	float timePercentage = keyFrame.GetTimePercentage() / end_time;
	for (size_t keyFrameIdx = 0; keyFrameIdx < m_keyFrames.size(); keyFrameIdx++)
	{
		if (m_keyFrames[keyFrameIdx].GetTimePercentage() == timePercentage)
		{
			m_keyFrames[keyFrameIdx] = keyFrame;
			m_keyFrames[keyFrameIdx].SetTimePercentage(timePercentage);
			return true;
		}
	}
	return false;
}

template <typename T>
void UIKeyFramedSequence<T>::AddKeyFrameWithNonPercentageTime(const UIKeyFrame<T>& keyFrame, float end_time)
{
	float timePercentage = keyFrame.GetTimePercentage() / end_time;
	m_keyFrames.push_back(keyFrame);
	m_keyFrames[m_keyFrames.size() - 1].SetTimePercentage(timePercentage);
}

template <typename T>
void UIKeyFramedSequence<T>::ClearFrames()
{
	m_keyFrames.clear();
}

//------------------------------------------------------------------------------------------------------------------------------
//Getters
template <typename T>
const T* UIKeyFramedSequence<T>::GetPtrCopyOfValueBasedOnPercentageTime(float percentageTime) const
{
	const T* value;

	for (size_t keyFrameIdx = 0; keyFrameIdx < m_keyFrames.size(); keyFrameIdx++)
	{
		UIKeyFrame<T>& keyFrame = m_keyFrames[keyFrameIdx];
		if (percentageTime < keyFrame.GetTimePercentage())
		{
			break;
		}
		value = keyFrame.GetValueCopy();
	}

	return value;
}

//------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* UIKeyFramedSequence<T>::GetEditablePtrCopyOfValueBasedOnPercentageTime(float percentageTime)
{
	T* value;

	for (size_t keyFrameIdx = 0; keyFrameIdx < m_keyFrames.size(); keyFrameIdx++)
	{
		UIKeyFrame<T>& keyFrame = m_keyFrames[keyFrameIdx];
		if (percentageTime < keyFrame.GetTimePercentage())
		{
			break;
		}
		value = keyFrame.GetValueCopy();
	}

	return value;
}

//------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T UIKeyFramedSequence<T>::GetCopyOfValueBasedOnPercentageTime(float percentageTime) const
{
	T value;

	for (size_t keyFrameIdx = 0; keyFrameIdx < m_keyFrames.size(); keyFrameIdx++)
	{
		UIKeyFrame<T>& keyFrame = m_keyFrames[keyFrameIdx];
		if (percentageTime < keyFrame.GetTimePercentage())
		{
			break;
		}
		value = keyFrame.GetValueCopy();
	}

	return value;
}

template <typename T>
const T* UIKeyFramedSequence<T>::GetPtrFrameAfterCopyOfValueBasedOnPercentageTime(float percentageTime, int& firstIdx, int& secondIdx) const
{
	firstIdx = -1;
	secondIdx = 0;
	const T* value = nullptr;
	if (m_keyFrames.size() == 0)
	{
		secondIdx = -1;
		return value;
	}

	for (size_t keyFrameIdx = 0; keyFrameIdx < m_keyFrames.size(); keyFrameIdx++)
	{
		const UIKeyFrame<T>& keyFrame = m_keyFrames[keyFrameIdx];
		if (percentageTime < keyFrame.GetTimePercentage())
		{
			firstIdx = keyFrameIdx;
			if (keyFrameIdx + 1 != m_keyFrames.size())
			{
				secondIdx = keyFrameIdx + 1;
				value = m_keyFrames[keyFrameIdx + 1].GetStoredValue();
			}
			else
			{
				secondIdx = firstIdx;
				value = m_keyFrames[keyFrameIdx].GetStoredValue();
			}
			break;
		}
	}

	return value;
}

template <typename T>
T* UIKeyFramedSequence<T>::GetEditablePtrFrameAfterCopyOfValueBasedOnPercentageTime(float percentageTime, int& firstIdx, int& secondIdx)
{
	firstIdx = -1;
	secondIdx = 0;
	T* value = nullptr;
	if (m_keyFrames.size() == 0)
	{
		secondIdx = -1;
		return value;
	}

	for (size_t keyFrameIdx = 0; keyFrameIdx < m_keyFrames.size(); keyFrameIdx++)
	{
		UIKeyFrame<T>& keyFrame = m_keyFrames[keyFrameIdx];
		if (percentageTime < keyFrame.GetTimePercentage())
		{
			firstIdx = keyFrameIdx;
			if (keyFrameIdx + 1 != m_keyFrames.size())
			{
				secondIdx = keyFrameIdx + 1;
				value = m_keyFrames[keyFrameIdx + 1].GetEditableStoredValue();
			}
			else
			{
				secondIdx = firstIdx;
				value = m_keyFrames[keyFrameIdx].GetEditableStoredValue();
			}
			break;
		}
	}

	return value;
}

template <typename T>
const T UIKeyFramedSequence<T>::GetFrameAfterCopyOfValueBasedOnPercentageTime(float percentageTime, int& firstIdx, int& secondIdx) const
{
	firstIdx = -1;
	secondIdx = 0;
	T value;
	if (m_keyFrames.size() == 0)
	{
		secondIdx = -1;
		return value;
	}

	for (size_t keyFrameIdx = 0; keyFrameIdx < m_keyFrames.size(); keyFrameIdx++)
	{
		const UIKeyFrame<T>& keyFrame = m_keyFrames[keyFrameIdx];
		if (percentageTime < keyFrame.GetTimePercentage())
		{
			firstIdx = keyFrameIdx;
			if (keyFrameIdx + 1 != m_keyFrames.size())
			{
				secondIdx = keyFrameIdx + 1;
				value = m_keyFrames[keyFrameIdx + 1].GetValueCopy();
			}
			else
			{
				secondIdx = firstIdx;
				value = m_keyFrames[keyFrameIdx].GetValueCopy();
			}
			break;
		}
	}

	return value;
}

template <typename T>
UIKeyFrame<T> UIKeyFramedSequence<T>::GetKeyFrameAtIdx(size_t idx, bool& found) const
{
	if (idx >= m_keyFrames.size())
	{
		found = false;
		return UIKeyFrame<T>();
	}
	found = true;
	return m_keyFrames[idx];
}

template <typename T>
T UIKeyFramedSequence<T>::GetValueAtKeyFrameIdx(size_t idx, bool& found) const
{
	if (idx >= m_keyFrames.size())
	{
		found = false;
		return T();
	}
	found = true;
	return m_keyFrames[idx].GetValueCopy();
}

template <typename T>
const T* UIKeyFramedSequence<T>::GetPtrValueAtKeyFrameIdx(size_t idx, bool& found) const
{
	if (idx >= m_keyFrames.size())
	{
		found = false;
		return nullptr;
	}
	found = true;
	return m_keyFrames[idx].GetStoredValue();
}

template <typename T>
T* UIKeyFramedSequence<T>::GetEditablePtrValueAtKeyFrameIdx(size_t idx, bool& found)
{
	if (idx >= m_keyFrames.size())
	{
		found = false;
		return nullptr;
	}
	found = true;
	return m_keyFrames[idx].GetEditableStoredValue();
}

template <typename T>
size_t UIKeyFramedSequence<T>::GetNumberOfFrames() const
{
	return m_keyFrames.size();
}
#endif