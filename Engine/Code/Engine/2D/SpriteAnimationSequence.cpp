#include "Engine/2D/SpriteAnimationSequence.hpp"
#include <cmath>

STATIC const frame_t frame_t::s_EmptyFrame = frame_t();

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//frame_t
frame_t::frame_t(const SpriteResource* resource, const float& startTimeAfterLastFrame)
	: m_sprite(resource),
	m_duration(startTimeAfterLastFrame)
{
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Constructors
SpriteAnimationSequence::SpriteAnimationSequence(const Motion_WrapModes& loopMode, const bool& flipp)
	: m_loopMode(loopMode),
	m_animDuration(0.f),
	m_flipped(flipp)
{
}
SpriteAnimationSequence::~SpriteAnimationSequence()
{
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Insert Frame
const bool SpriteAnimationSequence::PushBackFrame(const frame_t& frame)
{
	if (frame.m_sprite == nullptr)
	{
		return false;
	}
	m_frames.push_back(frame);
	m_animDuration += frame.m_duration;
	return true;
}
const bool SpriteAnimationSequence::AddFrame(const frame_t& frame, const int& idx)
{
	if (frame.m_sprite == nullptr
		|| idx < 0 || (size_t)idx >= m_frames.size())
	{
		return false;
	}
	m_frames.insert(m_frames.begin() + idx, 1, frame);
	return true;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Getters
size_t SpriteAnimationSequence::GetNumberOfFrames() const
{
	return m_frames.size();
}

const SpriteResource* SpriteAnimationSequence::GetFrame(const float& time, int& currentFrameIdx) const
{
	//This one is suppose to go through and find the correct frame to use.
	//PingPong is the only case that requires special treatment.
	currentFrameIdx = -1;
	if (m_frames.size() <= 0)
	{
		return nullptr;
	}
	float timeHolder = 0.f;
	float stepForwardTest = 0.f;
	float timeHolderAfterThisFrame = 0.f;

	for (size_t i = 0; i < m_frames.size(); i++)
	{
		size_t curFrameIdx = i;
		if (m_flipped)
		{
			curFrameIdx = m_frames.size() - (i + 1);
		}
		stepForwardTest = m_frames[curFrameIdx].m_duration;
		timeHolderAfterThisFrame = stepForwardTest + timeHolder;
		if (time >= timeHolder && time < timeHolderAfterThisFrame)
		{
			currentFrameIdx = curFrameIdx;
			return m_frames[curFrameIdx].m_sprite;
		}
		else
		{
			timeHolder = timeHolderAfterThisFrame;
		}
	}
	currentFrameIdx = m_frames.size() - 1;
	return m_frames[currentFrameIdx].m_sprite;
}

const SpriteResource* SpriteAnimationSequence::GetFrame(int spriteIdx) const
{
	if (spriteIdx < 0 || spriteIdx >= (int)m_frames.size())
	{
		return nullptr;
	}
	return m_frames[spriteIdx].m_sprite;
}

frame_t SpriteAnimationSequence::GetFrameAsFrame(int spriteIdx, bool& found) const
{
	if (spriteIdx < 0 || spriteIdx >= (int)m_frames.size())
	{
		found = false;
		return frame_t::s_EmptyFrame;
	}
	found = true;
	return m_frames[spriteIdx];
}

const float SpriteAnimationSequence::UpdateTime(float time, float deltaSeconds, float playSpeed)
{
	//This method returns the time suppose to use to get the correct sprite.
	//Allows for clamped animation, looped animation, and pingponging
	if (m_animDuration == 0.f)
	{
		return 0.f;
	}

	float test = time + (deltaSeconds * playSpeed);

	const float percentage = test / m_animDuration;
	const int floored = (int)floor(percentage);
	const float fractionalPercentage = percentage - (float)floored;

	switch (m_loopMode)
	{
	case MOTION_WRAPMODE_CLAMP:
		if (test >= m_animDuration)
		{
			return m_animDuration;
		}
		else
		{
			return test;
		}
	case MOTION_WRAPMODE_LOOP:
		return fractionalPercentage * m_animDuration;
	case MOTION_WRAPMODE_PINGPONG:
		if (percentage >= 1.f)
		{
			m_flipped = !m_flipped;
		}
		return fractionalPercentage * m_animDuration;
	default:
		break;
	}
	return time;
}