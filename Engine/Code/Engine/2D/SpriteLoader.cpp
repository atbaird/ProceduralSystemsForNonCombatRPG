#include "Engine/2D/SpriteLoader.hpp"
#include "Engine/2D/SpriteDatabase.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/2D/SpriteDatabase.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Performance/Performance.hpp"

STATIC SpriteLoaderLoadingSave* SpriteLoader::s_saveForWhileLoading = nullptr;
STATIC const char* SpriteLoader::s_SpriteResourceRootXMLNode = "SpriteResources";
STATIC const char* SpriteLoader::s_SpriteResourceChildXMLNode = "SpriteResource";
STATIC const char* SpriteLoader::s_SpriteAnimationRootXMLNode = "SpriteAnimations";
STATIC const char* SpriteLoader::s_SpriteAnimationChildXMLNode = "SpriteAnimation";

STATIC const char* SpriteLoader::s_resourceName = "resourceName";
STATIC const char* SpriteLoader::s_textureLocation = "textureLocation";
STATIC const char* SpriteLoader::s_spriteID = "spriteID";
STATIC const char* SpriteLoader::s_spriteBounds = "spriteBounds";
STATIC const char* SpriteLoader::s_virtualSize = "virtualSize";

STATIC const char* SpriteLoader::s_SpriteAnimationName = "spriteAnimationName";
STATIC const char* SpriteLoader::s_defaultTimeIntervalBetweenFrames = "defaultIntervalBetweenFrames";
STATIC const char* SpriteLoader::s_MotionMode = "animationMode";
STATIC const char* SpriteLoader::s_ReverseDirection = "reverseDirection";
STATIC const char* SpriteLoader::s_SpriteResource = "resourceName";
STATIC const char* SpriteLoader::s_currentSpriteTimeInterval = "intervalBetweenFrames";
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Public static methods
STATIC void SpriteLoader::ReserveSpriteResources(size_t numberOfResources)
{
	SpriteResourceHandler::ReserveSpriteResources(numberOfResources);
}

STATIC void SpriteLoader::ReserveSpriteAnimations(size_t numberOfResources)
{
	SpriteResourceHandler::ReserveSpriteAnimations(numberOfResources);
}

STATIC void SpriteLoader::LoadAllSpritesResourcesFromFolderAsNew(
	const char* spriteResourcesXMLfolderLoc, 
	bool recurseChildren)
{
	std::vector<std::string> filesInFolderAndChildren =
		FileUtils::EnumerateFilesInDirectory(spriteResourcesXMLfolderLoc, "*", recurseChildren);
	for (size_t i = 0; i < filesInFolderAndChildren.size(); i++)
	{
		std::string file = filesInFolderAndChildren[i];
		std::string fileEnding = IterateInReverseUntilHitChar(file, '.');
		fileEnding = MakeLower(fileEnding);
		if (fileEnding != "xml")
		{
			continue;
		}

		LoadAllSpritesResourcesAsNew(file.c_str());
	}
}


STATIC void SpriteLoader::LoadAllSpriteAnimationsFromFolderAsNew(const char* spriteAnimationXMLfolderLoc, bool recurseChildren)
{
	std::vector<std::string> filesInFolderAndChildren = 
		FileUtils::EnumerateFilesInDirectory(spriteAnimationXMLfolderLoc, "*", recurseChildren);
	for (size_t i = 0; i < filesInFolderAndChildren.size(); i++)
	{
		std::string file = filesInFolderAndChildren[i];
		std::string fileEnding = IterateInReverseUntilHitNthChar(file, '.', 1);
		fileEnding = MakeLower(fileEnding);
		if (fileEnding != "xml")
		{
			continue;
		}
		LoadAllSpriteAnimationsAsNew(file.c_str());
	}
}

STATIC void SpriteLoader::LoadAllSpritesResourcesAsNew(const char* fileLoc)
{
	bool couldNotLoad = false;
	XMLNode spriteResources = EngineXMLParser::ParseXMLFile(fileLoc, s_SpriteResourceRootXMLNode,
		couldNotLoad);
	if (couldNotLoad == false)
	{
		for (int childIdx = 0; childIdx < spriteResources.nChildNode(); childIdx++)
		{
			XMLNode child = spriteResources.getChildNode(childIdx);
			if (SimpleStrCmp(child.getName(), s_SpriteResourceChildXMLNode) == false)
			{
				continue;
			}
			TryLoadInSpriteResource(child);
		}
	}
}

STATIC void SpriteLoader::LoadAllSpriteAnimationsAsNew(const char* spriteAnimationXMLfileLoc)
{
	bool couldNotLoad = false;
	XMLNode spriteAnimations = EngineXMLParser::ParseXMLFile(spriteAnimationXMLfileLoc, s_SpriteAnimationRootXMLNode,
		couldNotLoad);
	if (couldNotLoad == false)
	{
		for (int childIdx = 0; childIdx < spriteAnimations.nChildNode(); childIdx++)
		{
			XMLNode child = spriteAnimations.getChildNode(childIdx);
			if (SimpleStrCmp(child.getName(), s_SpriteAnimationChildXMLNode) == false)
			{
				continue;
			}
			TryLoadInSpriteAnimation(child);
		}
	}
}

STATIC const SpriteResource* SpriteLoader::LoadInSprite(Texture* tex, const size_t& spriteID, 
	const AABB2& spriteCoords, const Vector2& virtualSize)
{
	if (tex == nullptr)
	{
		return nullptr;
	}
	const SpriteResource* toReturn = nullptr;
	bool foundIt = false;
	size_t spriteResourceID = 
		SpriteDatabase::g_SpriteDatabase->GetSpriteResourceIDForSpriteSheetAndSpriteID(tex, spriteID, foundIt);
	if (foundIt)
	{
		toReturn = SpriteDatabase::g_SpriteDatabase->GetSprite(spriteResourceID);
	}
	else
	{
		SpriteResource* redSmileyResource = new SpriteResource(tex, spriteCoords, spriteID, virtualSize);
		SpriteDatabase::g_SpriteDatabase->RegisterNewSprite(tex, spriteID, redSmileyResource);
		toReturn = redSmileyResource;
	}
	return toReturn;
}

STATIC void SpriteLoader::SetUpOnUpdateSaveSprites(const char* spriteResourcesXMLfolderLoc,
	bool spritesRecurseChildren,
	double secondsToTakePerFrame)
{
	if (s_saveForWhileLoading == nullptr)
	{
		s_saveForWhileLoading = new SpriteLoaderLoadingSave();
	}
	s_saveForWhileLoading->m_SpriteResourcesFolderToReadFrom = std::string(spriteResourcesXMLfolderLoc);
	s_saveForWhileLoading->m_spriteResourcesRecurseChildren = spritesRecurseChildren;
	s_saveForWhileLoading->m_readInXMLFileLocationsForSpriteResources = false;
	s_saveForWhileLoading->m_currentlyReadingSpriteResources = true;
	s_saveForWhileLoading->m_amountOfTimeToTakePerFrameToLoadSpriteResources = secondsToTakePerFrame;
}

STATIC void SpriteLoader::SetUpOnUpdateSaveAnimations(const char* spriteAnimationsXMLfolderLoc,
	bool animationRecurseChildren,
	double secondsToTakePerFrame)
{
	if (s_saveForWhileLoading == nullptr)
	{
		s_saveForWhileLoading = new SpriteLoaderLoadingSave();
	}
	s_saveForWhileLoading->m_SpriteAnimationsFolderToReadFrom = std::string(spriteAnimationsXMLfolderLoc);
	s_saveForWhileLoading->m_spriteAnimationsRecurseChildren = animationRecurseChildren;
	s_saveForWhileLoading->m_readInXMLFileLocationsForSpriteAnimations = false;
	s_saveForWhileLoading->m_amountOfTimeToTakePerFrameToLoadSpriteAnimations = secondsToTakePerFrame;
}

STATIC bool SpriteLoader::OnUpdateLoadSpriteResourcesAndAnimations()
{
	if (s_saveForWhileLoading == nullptr)
	{
		return true;
	}
	s_saveForWhileLoading->m_frameStartTime = Performance::GetCurrentPerformanceCount();

	if (s_saveForWhileLoading->m_currentlyReadingSpriteResources)
	{
		if (OnUpdateLoadSpriteResources())
		{
			s_saveForWhileLoading->m_currentlyReadingSpriteResources = false;
			s_saveForWhileLoading->m_indexPresentlyAt = 0;
		}
		return false;
	}
	else
	{
		return OnUpdateLoadSpriteAnimations();
	}
}

STATIC bool SpriteLoader::OnUpdateLoadSpriteResources()
{
	if (s_saveForWhileLoading == nullptr)
	{
		//Cant do anything; no data to read
		return true;
	}
	if (!s_saveForWhileLoading->m_readInXMLFileLocationsForSpriteResources)
	{
		//Prepare Sprite Resources to load
		s_saveForWhileLoading->m_readInXMLFileLocationsForSpriteResources = true;
		s_saveForWhileLoading->m_xmlFileLocationSpriteResources =
			FileUtils::EnumerateFilesInDirectory(s_saveForWhileLoading->m_SpriteResourcesFolderToReadFrom,
				"*", s_saveForWhileLoading->m_spriteResourcesRecurseChildren);
		s_saveForWhileLoading->m_indexPresentlyAt = 0;
		if (s_saveForWhileLoading->m_xmlFileLocationSpriteResources.size() == 0)
		{
			return true;
		}
		return false;
	}
	int currentlyReadIn = 0;
	if (s_saveForWhileLoading->m_amountOfTimeToTakePerFrameToLoadSpriteResources <= 0)
	{
		//cant do anything, would result in infinite loop if let it be less than or equal to 0.
		return true;
	}

	//Load in sprite resources
	while ((size_t)s_saveForWhileLoading->m_indexPresentlyAt
		< s_saveForWhileLoading->m_xmlFileLocationSpriteResources.size())
	{
		std::string fileLocation = 
			s_saveForWhileLoading->m_xmlFileLocationSpriteResources[
				s_saveForWhileLoading->m_indexPresentlyAt];
		LoadAllSpritesResourcesAsNew(fileLocation.c_str());
		currentlyReadIn++;
		s_saveForWhileLoading->m_indexPresentlyAt++;


		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - s_saveForWhileLoading->m_frameStartTime;
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_saveForWhileLoading->m_amountOfTimeToTakePerFrameToLoadSpriteResources)
		{
			return false;
		}
	}
	if ((size_t)s_saveForWhileLoading->m_indexPresentlyAt
		>= s_saveForWhileLoading->m_xmlFileLocationSpriteResources.size())
	{
		//cant do anything, would result in infinite loop if let it be less than or equal to 0.
		return true;
	}
	return false;
}

STATIC bool SpriteLoader::OnUpdateLoadSpriteAnimations()
{
	if (s_saveForWhileLoading == nullptr)
	{
		//Cant do anything; no data to read
		return true;
	}

	if (!s_saveForWhileLoading->m_readInXMLFileLocationsForSpriteAnimations)
	{
		//Prepare Sprite Animations to be read
		s_saveForWhileLoading->m_readInXMLFileLocationsForSpriteAnimations = true;
		s_saveForWhileLoading->m_xmlFileLocationSpriteAnimations =
			FileUtils::EnumerateFilesInDirectory(s_saveForWhileLoading->m_SpriteAnimationsFolderToReadFrom,
				"*", s_saveForWhileLoading->m_spriteAnimationsRecurseChildren);
		s_saveForWhileLoading->m_indexPresentlyAt = 0;
		if (s_saveForWhileLoading->m_xmlFileLocationSpriteAnimations.size() == 0)
		{
			return true;
		}
		return false;
	}

	int currentlyReadIn = 0;
	if (s_saveForWhileLoading->m_amountOfTimeToTakePerFrameToLoadSpriteAnimations <= 0)
	{
		//cant do anything, would result in infinite loop if let it be less than or equal to 0.
		return true;
	}
	//Load in sprite Animations
	while ((size_t)s_saveForWhileLoading->m_indexPresentlyAt
		< s_saveForWhileLoading->m_xmlFileLocationSpriteAnimations.size())
	{
		std::string fileLocation =
			s_saveForWhileLoading->m_xmlFileLocationSpriteAnimations[
				s_saveForWhileLoading->m_indexPresentlyAt];
		LoadAllSpriteAnimationsAsNew(fileLocation.c_str());
		currentlyReadIn++;
		s_saveForWhileLoading->m_indexPresentlyAt++;

		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - s_saveForWhileLoading->m_frameStartTime;
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_saveForWhileLoading->m_amountOfTimeToTakePerFrameToLoadSpriteResources)
		{
			return false;
		}
	}
	if ((size_t)s_saveForWhileLoading->m_indexPresentlyAt
		>= s_saveForWhileLoading->m_xmlFileLocationSpriteAnimations.size())
	{
		return true;
	}
	return false;
}

STATIC void SpriteLoader::ClearAllSprites()
{
	SpriteResourceHandler::ClearAll();
	if (s_saveForWhileLoading != nullptr)
	{
		delete s_saveForWhileLoading;
		s_saveForWhileLoading = nullptr;
	}
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//TryLoadInSpriteResource
STATIC bool SpriteLoader::TryLoadInSpriteResource(XMLNode& child)
{
	bool resourceNameSet = false;
	bool textureLocationSet = false;
	bool spriteIDIsAvailable = true;
	bool spriteIDRead = false;
	std::string resourceName = "";
	std::string textureLocation = "";
	size_t spriteID = 0;
	AABB2 spriteBounds = AABB2::ZERO_TO_ONE;
	Vector2 virtualSize = Vector2::vec2_ones;

	TryLoadInSpriteResourceReadAttributes(child, resourceNameSet, textureLocationSet, 
		spriteIDRead, resourceName, textureLocation, spriteID, spriteBounds, virtualSize);

	return TryLoadInSpriteResourceTestValid(resourceNameSet, textureLocationSet, 
		spriteIDIsAvailable, spriteIDRead, resourceName, textureLocation,
		spriteID, spriteBounds, virtualSize);
}

STATIC void SpriteLoader::TryLoadInSpriteResourceReadAttributes(XMLNode& child, 
	bool& resourceNameSet, bool& textureLocationSet, bool& spriteIDRead,
	std::string& resourceName, std::string& textureLocation, size_t& spriteID,
	AABB2& spriteBounds, Vector2& virtualSize)
{
	spriteIDRead;
	for (int i = 0; i < child.nAttribute(); i++)
	{
		XMLAttribute attr = child.getAttribute(i);
		if (SimpleStrCmpLower(attr.lpszName, s_resourceName) == true)
		{
			resourceName = std::string(attr.lpszValue);
			if (SimpleStrCmp(resourceName, "") != true
				&& SpriteResourceHandler::GetIfNameAlreadyExistsSpriteResource(resourceName) == false)
			{
				resourceNameSet = true;
			}
		}
		else if (SimpleStrCmpLower(attr.lpszName, s_textureLocation) == true)
		{
			textureLocation = std::string(attr.lpszValue);
			if (SimpleStrCmp(textureLocation, "") != true)
			{
				textureLocationSet = true;
			}
		}
		else if (SimpleStrCmpLower(attr.lpszName, s_spriteID) == true)
		{
			spriteID = (size_t)atoi(attr.lpszValue);
		}
		else if (SimpleStrCmpLower(attr.lpszName, s_spriteBounds) == true)
		{
			std::string parse = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(parse, ' ');
			if (parsed.size() > 0)
			{
				spriteBounds.mins.x = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				spriteBounds.mins.y = (float)atof(parsed.at(1).c_str());
			}
			if (parsed.size() > 2)
			{
				spriteBounds.maxs.x = (float)atof(parsed.at(2).c_str());
			}
			if (parsed.size() > 3)
			{
				spriteBounds.maxs.y = (float)atof(parsed.at(3).c_str());
			}
		}
		else if (SimpleStrCmpLower(attr.lpszName, s_virtualSize) == true)
		{
			std::string parse = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(parse, ' ');
			if (parsed.size() > 0)
			{
				virtualSize.x = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				virtualSize.y = (float)atof(parsed.at(1).c_str());
			}
		}
	}
}

STATIC bool SpriteLoader::TryLoadInSpriteResourceTestValid(bool& resourceNameSet, bool& textureLocationSet, bool& spriteIDIsAvailable, bool& spriteIDRead,
	std::string& resourceName, std::string& textureLocation, size_t& spriteID, AABB2& spriteBounds, Vector2& virtualSize)
{
	Texture* loadedTex = nullptr;
	bool addedSuccessfully = true;
	if (resourceNameSet == false || textureLocationSet == false)
	{
		addedSuccessfully = false;
	}
	else
	{
		loadedTex = Texture::CreateOrGetTexture(textureLocation);
		if (loadedTex == nullptr)
		{
			GUARANTEE_RECOVERABLE(false, "A texture attempted to load in was null!");
			return false;
		}
		if (loadedTex->m_texelSize.x > 0)
		{
			spriteBounds.mins.x = spriteBounds.mins.x / (float)loadedTex->m_texelSize.x;
			spriteBounds.maxs.x = spriteBounds.maxs.x / (float)loadedTex->m_texelSize.x;
		}
		if (loadedTex->m_texelSize.y > 0)
		{
			spriteBounds.mins.y = spriteBounds.mins.y / (float)loadedTex->m_texelSize.y;
			spriteBounds.maxs.y = spriteBounds.maxs.y / (float)loadedTex->m_texelSize.y;
		}
		if (spriteIDRead 
			&& SpriteDatabase::g_SpriteDatabase->FindIfSpriteSheetAndIDAlreadyExists(loadedTex, spriteID) == true)
		{
			addedSuccessfully = false;
			spriteIDIsAvailable = false;
		}
	}

	if (addedSuccessfully == false)
	{
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, "Did not add a resource!");
			if (resourceNameSet == false)
			{
				g_Console->ConsolePrintf(WARNING, "Resource name was not set or the resource name already exists!");
			}
			if (textureLocationSet == false)
			{
				g_Console->ConsolePrintf(WARNING, "Texture location was not set!");
			}
			if (spriteIDIsAvailable == false)
			{
				g_Console->ConsolePrintf(WARNING, "Wanted Sprite ID, or Default Sprite ID of 0, is not available for given texture!");
			}
		}
	}
	else
	{
		if (spriteIDRead == false)
		{
			spriteID = SpriteDatabase::g_SpriteDatabase->GetNextAvailableSpriteID(loadedTex);
		}
		SpriteResourceHandler::RegisterSpriteResource(
			resourceName, LoadInSprite(loadedTex, spriteID, spriteBounds, virtualSize));
	}


	return addedSuccessfully;
}


STATIC bool SpriteLoader::TryLoadInSpriteAnimation(XMLNode& child)
{
	std::vector<frame_t> frames;
	Motion_WrapModes motionMode = MOTION_WRAPMODE_LOOP;
	float end_time = 0.f;
	bool flipped = false;
	std::string name = "";

	TryLoadInSpriteAnimationReadAttributes(child, name, frames, motionMode, end_time, flipped);
	return TryLoadInSpriteAnimationTestValid(name, frames, motionMode, end_time, flipped);
}


STATIC void SpriteLoader::TryLoadInSpriteAnimationReadAttributes(XMLNode& child, std::string& name, std::vector<frame_t>& frames, Motion_WrapModes& mode, float& end_time, bool& flipped)
{
	end_time;
	float defaultIntervalForFrames = 1.f;
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		if (SimpleStrCmpLower(attr.lpszName, s_defaultTimeIntervalBetweenFrames))
		{
			defaultIntervalForFrames = (float)atof(attr.lpszValue);
		}
		else if (SimpleStrCmpLower(attr.lpszName, s_MotionMode))
		{
			std::string val(attr.lpszValue);
			if (SimpleStrCmpLower(val, "Loop"))
			{
				mode = MOTION_WRAPMODE_LOOP;
			}
			else if (SimpleStrCmpLower(val, "Clamp"))
			{
				mode = MOTION_WRAPMODE_CLAMP;
			}
			else if (SimpleStrCmpLower(val, "PingPong"))
			{
				mode = MOTION_WRAPMODE_PINGPONG;
			}
			else
			{
				mode = MOTION_INVALID_WRAPMODE;
			}
		}
		else if (SimpleStrCmpLower(attr.lpszName, s_ReverseDirection))
		{
			if (SimpleStrCmpLower("t", attr.lpszValue)
				|| SimpleStrCmpLower("true", attr.lpszValue))
			{
				flipped = true;
			}
		}
		else if (SimpleStrCmpLower(attr.lpszName, s_SpriteAnimationName))
		{
			name = std::string(attr.lpszValue);
		}
	}

	for (int childIdx = 0; childIdx < child.nChildNode(); childIdx++)
	{
		frame_t newFrame(nullptr, defaultIntervalForFrames);
		XMLNode node = child.getChildNode(childIdx);
		for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = node.getAttribute(attrIdx);
			if (SimpleStrCmpLower(attr.lpszName, s_SpriteResource))
			{
				std::string tag(attr.lpszValue);
				newFrame.m_sprite = SpriteResourceHandler::GetSpriteResourceByName(tag);
			}
			else if (SimpleStrCmpLower(attr.lpszName, s_currentSpriteTimeInterval))
			{
				newFrame.m_duration = (float)atof(attr.lpszValue);
			}
		}
		frames.push_back(newFrame);
	}
}

STATIC bool SpriteLoader::TryLoadInSpriteAnimationTestValid(std::string& name, std::vector<frame_t>& frames, Motion_WrapModes& mode, float& end_time, bool& flipped)
{
	end_time;
	if (SimpleStrCmp(name, ""))
	{
		return false;
	}
	SpriteAnimationSequence sequence(mode, flipped);
	for (size_t i = 0; i < frames.size(); i++)
	{
		sequence.PushBackFrame(frames[i]);
	}
	SpriteResourceHandler::RegisterSpriteAnimationSequence(name, sequence);
	return true;
}