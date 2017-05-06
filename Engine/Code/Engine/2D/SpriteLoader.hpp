#pragma once


#ifndef SPRITELOADER_HPP
#define SPRITELOADER_HPP
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

#define STATIC

struct SpriteLoaderLoadingSave
{
	uint64_t m_frameStartTime = 0;
	std::vector<std::string> m_xmlFileLocationSpriteResources;
	std::vector<std::string> m_xmlFileLocationSpriteAnimations;
	std::string m_SpriteResourcesFolderToReadFrom = "";
	std::string m_SpriteAnimationsFolderToReadFrom = "";
	int m_indexPresentlyAt = 0;
	double m_amountOfTimeToTakePerFrameToLoadSpriteResources = 0.25;
	double m_amountOfTimeToTakePerFrameToLoadSpriteAnimations = 0.25;
	bool m_currentlyReadingSpriteResources = true;
	bool m_spriteResourcesRecurseChildren = false;
	bool m_spriteAnimationsRecurseChildren = false;
	bool m_readInXMLFileLocationsForSpriteResources = false;
	bool m_readInXMLFileLocationsForSpriteAnimations = false;
};

class SpriteLoader
{
public:
	static SpriteLoaderLoadingSave* s_saveForWhileLoading;
	static const char* s_SpriteResourceRootXMLNode;
	static const char* s_SpriteResourceChildXMLNode;

	static const char* s_SpriteAnimationRootXMLNode;
	static const char* s_SpriteAnimationChildXMLNode;

	static const char* s_resourceName;
	static const char* s_textureLocation;
	static const char* s_spriteID;
	static const char* s_spriteBounds;
	static const char* s_virtualSize;

	static const char* s_SpriteAnimationName;
	static const char* s_defaultTimeIntervalBetweenFrames;
	static const char* s_MotionMode;
	static const char* s_ReverseDirection;
	static const char* s_SpriteResource;
	static const char* s_currentSpriteTimeInterval;

	//Public Static methods
	static void ReserveSpriteResources(size_t numberOfResources);
	static void ReserveSpriteAnimations(size_t numberOfResources);
	static void LoadAllSpritesResourcesFromFolderAsNew(const char* spriteResourcesXMLfolderLoc, 
		bool recurseChildren = false);
	static void LoadAllSpriteAnimationsFromFolderAsNew(const char* spriteAnimationXMLfolderLoc, 
		bool recurseChildren = false);
	static void LoadAllSpritesResourcesAsNew(const char* spriteResourcesXMLfileLoc);
	static void LoadAllSpriteAnimationsAsNew(const char* spriteAnimationXMLfileLoc);
	static const SpriteResource* LoadInSprite(Texture* tex, const size_t& spriteID, 
		const AABB2& spriteCoords, const Vector2& virtualSize);

	static void SetUpOnUpdateSaveSprites(const char* spriteResourcesXMLfolderLoc,
		bool spritesRecurseChildren,
		double secondsToTakePerFrame);
	static void SetUpOnUpdateSaveAnimations(const char* spriteAnimationsXMLfolderLoc,
		bool animationRecurseChildren,
		double secondsToTakePerFrame);
	static bool OnUpdateLoadSpriteResourcesAndAnimations();
	static bool OnUpdateLoadSpriteResources();
	static bool OnUpdateLoadSpriteAnimations();
	static void ClearAllSprites();

private:
	//TryLoadInSpriteResource
	static bool TryLoadInSpriteResource(XMLNode& child);
	static void TryLoadInSpriteResourceReadAttributes(XMLNode& child, bool& resourceNameSet,
		bool& textureLocationSet, bool& spriteIDRead,
		std::string& resourceName, std::string& textureLocation, size_t& spriteID, AABB2& spriteBounds, 
		Vector2& virtualSize);
	static bool TryLoadInSpriteResourceTestValid(bool& resourceNameSet, bool& textureLocationSet,
		bool& spriteIDIsAvailable, bool& spriteIDRead,
		std::string& resourceName, std::string& textureLocation, size_t& spriteID, AABB2& spriteBounds,
		Vector2& virtualSize);

	static bool TryLoadInSpriteAnimation(XMLNode& child);
	static void TryLoadInSpriteAnimationReadAttributes(XMLNode& child, std::string& name, std::vector<frame_t>& frames,
		Motion_WrapModes& mode, float& end_time, bool& flipped);
	static bool TryLoadInSpriteAnimationTestValid(std::string& name, std::vector<frame_t>& frames, Motion_WrapModes& mode,
		float& end_time, bool& flipped);
};
#endif