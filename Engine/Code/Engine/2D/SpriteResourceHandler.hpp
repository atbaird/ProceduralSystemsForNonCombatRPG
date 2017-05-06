#pragma once

#ifndef SPRITERESOURCEHANDLER_HPP
#define SPRITERESOURCEHANDLER_HPP
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/2D/SpriteAnimationSequence.hpp"

#define STATIC

struct SpriteResourceContainer
{
public:
	//Variables
	const SpriteResource* m_resource = nullptr;
public:
	//SpriteResourceContainer Constructors
	SpriteResourceContainer(const std::string& resourceName, const SpriteResource* resource);

	void WriteSpriteResourceToString(std::string& str, int indentationAmt, int howMuchToIncreaseIndentation = 3) const;
};

struct SpriteAnimationContainer
{
public:
	//variables
	std::string m_animationName = "";
	SpriteAnimationSequence m_sequence;
public:
	//SpriteAnimationContainer Constructors
	SpriteAnimationContainer(const std::string& resourceName, const SpriteAnimationSequence& sequence);

	void WriteSpriteAnimationsToString(std::string& str, int indentationAmt, int howMuchToIncreaseIndentation = 3) const;
};

class SpriteResourceHandler
{
protected:
	//Static Variables
	static std::vector<SpriteResourceContainer>* s_Resources;
	static std::vector<SpriteAnimationContainer>* s_Sequences;

protected:
public:
	static void AddedSpacesToString(std::string& str, int indentationAmt);
	static void WriteSpriteResourcesToString(std::string& str, int indentationAmt, int howMuchToIncreaseIndentation = 3);
	static void WriteSpriteAnimationsToString(std::string& str, int indentationAmt, int howMuchToIncreaseIndentation = 3);
	static void ReserveSpriteResources(size_t numOfResources);
	static void ReserveSpriteAnimations(size_t numOfAnimations);
	static void RegisterSpriteResource(const std::string& name, const SpriteResource* resource);
	static void RegisterSpriteAnimationSequence(const std::string& name, const SpriteAnimationSequence& sequence);
	static const SpriteResource* GetSpriteResourceByName(const std::string& name);
	static SpriteAnimationSequence* GetSpriteAnimationSequenceByName(const std::string& name);
	static const size_t GetResourceIDByName(const std::string& name, bool& foundIt);
	static const bool GetIfNameAlreadyExistsSpriteResource(std::string& name);
	static const bool GetIfNameAlreadyExistsSpriteAnimationResource(std::string& name);
	static void UnregisterSpriteResource(const SpriteResource* resource);
	static void UnregisterSpriteResourceByName(const std::string& name);
	static void UnregisterSpriteAnimationSequence(SpriteAnimationSequence* sequence);
	static void UnregisterSpriteANimationSequenceByName(const std::string& name);
	static void ClearAll();
};
#endif