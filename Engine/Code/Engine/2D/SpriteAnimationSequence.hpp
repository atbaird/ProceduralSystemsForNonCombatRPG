#pragma once

#ifndef SPRITEANIMATIONSEQUENCE_HPP
#define SPRITEANIMATIONSEQUENCE_HPP
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/Renderer/3DModelling/Motion_WrapModes.hpp"

struct frame_t
{
	static const frame_t s_EmptyFrame;
	//frame_t
	frame_t(const SpriteResource* resource = nullptr, const float& startTimeAfterLastFrame = 1.f);

	const SpriteResource* m_sprite;
	float m_duration;
};

class SpriteAnimationSequence
{
private:
	std::vector<frame_t> m_frames;
	Motion_WrapModes m_loopMode;
	float m_animDuration;
	bool m_flipped;

public:
	//Constructors
	SpriteAnimationSequence(const Motion_WrapModes& loopMode = MOTION_WRAPMODE_CLAMP, const bool& flipp = false);
	virtual ~SpriteAnimationSequence();

	//Insert Frame
	const bool PushBackFrame(const frame_t&);
	const bool AddFrame(const frame_t&, const int& idx);

	//Getters
	size_t GetNumberOfFrames() const;
	const SpriteResource* GetFrame(const float& time, int& currentFrameIdx) const;
	const SpriteResource* GetFrame(int spriteIdx) const;
	frame_t GetFrameAsFrame(int spriteIdx, bool& found) const;
	const float UpdateTime(float time, float deltaSeconds, float playSpeed = 1.f);

};
#endif