#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathToStringUtils.hpp"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Static Variables
std::vector<SpriteResourceContainer>* SpriteResourceHandler::s_Resources = nullptr;
std::vector<SpriteAnimationContainer>* SpriteResourceHandler::s_Sequences = nullptr;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//SpriteResourceContainer Constructors
SpriteResourceContainer::SpriteResourceContainer(const std::string& resourceName, const SpriteResource* resource)
	: m_resource(resource)
{
	if (m_resource != nullptr)
	{
		SpriteResource* unconst_resource = const_cast<SpriteResource*>(m_resource);
		unconst_resource->SetSpriteResourceName(resourceName);
	}
}

void SpriteResourceContainer::WriteSpriteResourceToString(std::string& str, int indentationAmt,
	int howMuchToIncreaseIndentation) const
{
	if (m_resource == nullptr)
	{
		return;
	}
	Texture* tex = m_resource->GetSpriteSheet();
	if (tex == nullptr)
	{
		return;
	}

	int nextLevel = indentationAmt + howMuchToIncreaseIndentation;
	SpriteResourceHandler::AddedSpacesToString(str, indentationAmt);
	str += "\nSprite Resource Name: " + m_resource->GetSpriteResourceName() + "\n";

	str += "Texture File Path: " + tex->m_imageFilePath + "\n";

	SpriteResourceHandler::AddedSpacesToString(str, nextLevel);
	const AABB2& texCoords = m_resource->GetTextureCoords();
	str += "Texture Coordinates as Percentages: " + MathToStringUtils::ToString(texCoords)
		+ "\n";

	IntVector2 mins = IntVector2((int)(texCoords.mins.x * (float)tex->m_texelSize.x),
		(int)(texCoords.mins.y * (float)tex->m_texelSize.y));
	IntVector2 maxs = IntVector2((int)(texCoords.mins.x * (float)tex->m_texelSize.x),
		(int)(texCoords.mins.y * (float)tex->m_texelSize.y));

	SpriteResourceHandler::AddedSpacesToString(str, nextLevel);
	str += "Texture Coordinates converted back to Normal: " + MathToStringUtils::ToString(mins)
		+ "," + MathToStringUtils::ToString(maxs) + "\n";
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//SpriteAnimationContainer Constructors
SpriteAnimationContainer::SpriteAnimationContainer(const std::string& resourceName, 
	const SpriteAnimationSequence& sequence)
	: m_animationName(resourceName),
	m_sequence(sequence)
{
}

void SpriteAnimationContainer::WriteSpriteAnimationsToString(std::string& str, int indentationAmt,
	int howMuchToIncreaseIndentation) const
{
	int nextLevel = indentationAmt + howMuchToIncreaseIndentation;
	SpriteResourceHandler::AddedSpacesToString(str, indentationAmt);
	str += "\nSprite Animation Name: " + m_animationName + "\n";

	int numberOfFrames = m_sequence.GetNumberOfFrames();
	for (int i = 0; i < numberOfFrames; i++)
	{
		bool found = false;
		frame_t frame = m_sequence.GetFrameAsFrame((int)i, found);
		SpriteResourceHandler::AddedSpacesToString(str, nextLevel);
		str += "Frame Sprite Resource: ";
		if (frame.m_sprite == nullptr)
		{
			str += "NULL\n";
		}
		else
		{
			str += frame.m_sprite->GetSpriteResourceName() + "\n";
		}

		SpriteResourceHandler::AddedSpacesToString(str, nextLevel);
		str += "Frame Duration: " + std::to_string(frame.m_duration) + "\n";
	}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Static Functions
STATIC void SpriteResourceHandler::AddedSpacesToString(std::string& str, int indentationAmt)
{
	for (int i = 0; i < indentationAmt; i++)
	{
		str += " ";
	}
}

STATIC void SpriteResourceHandler::WriteSpriteResourcesToString(std::string& str, int indentationAmt,
	int howMuchToIncreaseIndentation)
{
	if (s_Resources == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < s_Resources->size(); i++)
	{
		const SpriteResourceContainer& container = s_Resources->at(i);
		container.WriteSpriteResourceToString(str, indentationAmt, howMuchToIncreaseIndentation);
	}

}

STATIC void SpriteResourceHandler::WriteSpriteAnimationsToString(std::string& str, int indentationAmt,
	int howMuchToIncreaseIndentation)
{
	if (s_Sequences == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < s_Sequences->size(); i++)
	{
		const SpriteAnimationContainer& container = s_Sequences->at(i);
		container.WriteSpriteAnimationsToString(str, indentationAmt, howMuchToIncreaseIndentation);
	}
}

STATIC void SpriteResourceHandler::ReserveSpriteResources(size_t numOfResources)
{
	if (numOfResources == 0)
	{
		return;
	}
	if (s_Resources == nullptr)
	{
		s_Resources = new std::vector<SpriteResourceContainer>();
	}
	s_Resources->reserve(numOfResources);
}

STATIC void SpriteResourceHandler::ReserveSpriteAnimations(size_t numOfAnimations)
{
	if (numOfAnimations == 0)
	{
		return;
	}
	if(s_Sequences == nullptr)
	{
		s_Sequences = new std::vector<SpriteAnimationContainer>();
	}
	s_Sequences->reserve(numOfAnimations);
}

STATIC void SpriteResourceHandler::RegisterSpriteResource(const std::string& name, const SpriteResource* resource)
{
	if (SimpleStrCmp(name, "") == true || resource == nullptr)
	{
		return;
	}
	if (s_Resources == nullptr)
	{
		s_Resources = new std::vector<SpriteResourceContainer>();
	}

	std::string lowerCase = MakeLower(name);
	s_Resources->push_back(SpriteResourceContainer(lowerCase, resource));
}

STATIC void SpriteResourceHandler::RegisterSpriteAnimationSequence(const std::string& name,
	const SpriteAnimationSequence& sequence)
{
	if (SimpleStrCmp(name, "") == true)
	{
		return;
	}
	if (s_Sequences == nullptr)
	{
		s_Sequences = new std::vector<SpriteAnimationContainer>();
	}
	std::string lowerCase = MakeLower(name);
	s_Sequences->push_back(SpriteAnimationContainer(lowerCase, sequence));
}

STATIC const SpriteResource* SpriteResourceHandler::GetSpriteResourceByName(const std::string& name)
{
	if (s_Resources == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);

	for (size_t idx = 0; idx < s_Resources->size(); idx++)
	{
		if (s_Resources->at(idx).m_resource != nullptr
			&& lowerCase == s_Resources->at(idx).m_resource->GetSpriteResourceName())
		{
			return s_Resources->at(idx).m_resource;
		}
	}

	return nullptr;
}

STATIC SpriteAnimationSequence* SpriteResourceHandler::GetSpriteAnimationSequenceByName(const std::string& name)
{
	if (s_Sequences == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);

	for (size_t idx = 0; idx < s_Sequences->size(); idx++)
	{
		if (lowerCase == s_Sequences->at(idx).m_animationName)
		{
			return &s_Sequences->at(idx).m_sequence;
		}
	}

	return nullptr;
}

STATIC const size_t SpriteResourceHandler::GetResourceIDByName(const std::string& name, bool& foundIt)
{
	foundIt = false;
	if (s_Resources == nullptr)
	{
		return 0;
	}
	std::string lowerCase = MakeLower(name);

	for (size_t idx = 0; idx < s_Resources->size(); idx++)
	{
		if (s_Resources->at(idx).m_resource != nullptr
			&& lowerCase == s_Resources->at(idx).m_resource->GetSpriteResourceName())
		{
			foundIt = true;
			return s_Resources->at(idx).m_resource->GetSpriteResourceID();
		}
	}

	return 0;
}

STATIC const bool SpriteResourceHandler::GetIfNameAlreadyExistsSpriteResource(std::string& name)
{
	if (s_Resources == nullptr)
	{
		return false;
	}
	std::string lowerCase = MakeLower(name);

	for (size_t idx = 0; idx < s_Resources->size(); idx++)
	{
		if (s_Resources->at(idx).m_resource != nullptr
			&& lowerCase == s_Resources->at(idx).m_resource->GetSpriteResourceName())
		{
			return true;
		}
	}
	return false;
}

STATIC const bool SpriteResourceHandler::GetIfNameAlreadyExistsSpriteAnimationResource(std::string& name)
{
	if (s_Sequences == nullptr)
	{
		return false;
	}
	std::string lowerCase = MakeLower(name);

	for (size_t idx = 0; idx < s_Sequences->size(); idx++)
	{
		if (lowerCase == s_Sequences->at(idx).m_animationName)
		{
			return true;
		}
	}

	return false;
}


STATIC void SpriteResourceHandler::UnregisterSpriteResource(const SpriteResource* resource)
{
	if (s_Resources == nullptr)
	{
		return;
	}
	for (size_t idx = 0; idx < s_Resources->size(); idx++)
	{
		if (s_Resources->at(idx).m_resource == resource)
		{
			s_Resources->erase(s_Resources->begin() + idx);
			return;
		}
	}
}

STATIC void SpriteResourceHandler::UnregisterSpriteResourceByName(const std::string& name)
{
	if (s_Resources == nullptr)
	{
		return;
	}
	std::string lowerCase = MakeLower(name);
	for (size_t idx = 0; idx < s_Resources->size(); idx++)
	{
		if (s_Resources->at(idx).m_resource != nullptr
			&& lowerCase == s_Resources->at(idx).m_resource->GetSpriteResourceName())
		{
			s_Resources->erase(s_Resources->begin() + idx);
			return;
		}
	}
}

STATIC void SpriteResourceHandler::UnregisterSpriteAnimationSequence(SpriteAnimationSequence* sequence)
{
	if (s_Sequences == nullptr)
	{
		return;
	}

	for (size_t idx = 0; idx < s_Sequences->size(); idx++)
	{
		if (&s_Sequences->at(idx).m_sequence == sequence)
		{
			s_Sequences->erase(s_Sequences->begin() + idx);
			return;
		}
	}
}

STATIC void SpriteResourceHandler::UnregisterSpriteANimationSequenceByName(const std::string& name)
{
	if (s_Sequences == nullptr)
	{
		return;
	}
	std::string lowerCase = MakeLower(name);

	for (size_t idx = 0; idx < s_Sequences->size(); idx++)
	{
		if (s_Sequences->at(idx).m_animationName == lowerCase)
		{
			s_Sequences->erase(s_Sequences->begin() + idx);
			return;
		}
	}
}

STATIC void SpriteResourceHandler::ClearAll()
{
	if (s_Resources != nullptr)
	{
		s_Resources->clear();
		delete s_Resources;
		s_Resources = nullptr;
	}
	if (s_Sequences != nullptr)
	{
		s_Sequences->clear();
		delete s_Sequences;
		s_Sequences = nullptr;
	}
}