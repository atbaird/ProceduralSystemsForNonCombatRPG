#include "Game/Debugging/DeveloperDebugInfoBox.hpp"
#include "Engine/Font/Font.hpp"
#include "Engine/Math/Vertex_PCT.hpp"
#include "Engine/2D/SpriteResource.hpp"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Static Variable
STATIC const DeveloperDebugInfoBox DeveloperDebugInfoBox::s_EmptyDevDebugInfoBox
		= DeveloperDebugInfoBox();

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors
DeveloperDebugInfoBox::DeveloperDebugInfoBox()
{

}

DeveloperDebugInfoBox::DeveloperDebugInfoBox(const Rgba& boxColor)
	: m_BoxColor(boxColor)
{

}

DeveloperDebugInfoBox::DeveloperDebugInfoBox(const Rgba& boxColor, 
	const std::vector<DeveloperDebugInfoTextBlurb>& textBlurbs)
	: m_BoxColor(boxColor),
	m_TextBlurbs(textBlurbs)
{

}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Update
float DeveloperDebugInfoBox::Update(Font* font, const Vector2& currentPosition,
	std::vector<Vertex_PCT>& BoxModels, std::vector<unsigned int>& boxIndices,
	std::vector<std::vector<Vertex_PCT>>& TextModel,
	std::vector<std::vector<unsigned int>>& TextIndices,
	float fontSize, float maxBoxWidth, const SpriteResource* whiteBoxResource,
	float& CurrentWidth)
{
	if (font == nullptr
		|| m_TextBlurbs.size() == 0)
	{
		m_BoxHeight = 1.f;
		AddQuadForBoxModel(BoxModels, boxIndices, currentPosition, 
			m_BoxHeight, maxBoxWidth, whiteBoxResource);
		return m_BoxHeight;
	}
	m_BoxHeight = 0.f;

	float textHeight = fontSize;
	float LengthOfSpace = 0.f;
	if (font != nullptr)
	{
		textHeight = font->CalcTextHeight("test", fontSize, false);
		LengthOfSpace = font->CalcTextWidth("   ", fontSize);
	}
	m_BoxHeight = textHeight;

	float totalLength = 0.f;
	float CurrentMaxWidth = maxBoxWidth;
	bool lastBlurbWantsToBeAlone = false;
	for (size_t texBlurbIdx = 0; texBlurbIdx < m_TextBlurbs.size(); texBlurbIdx++)
	{
		const DeveloperDebugInfoTextBlurb& blurb = m_TextBlurbs[texBlurbIdx];
		const Rgba& blurbColor = blurb.GetColor();
		const std::string& blurbText = blurb.GetText();
		bool thisBlurbWantsToBeAlone = blurb.GetGuaranteeAloneOnLine();
		float CurrentBlurbLengthFloat = 0.f;
		if (font != nullptr)
		{
			CurrentBlurbLengthFloat = font->CalcTextWidth(blurbText, fontSize);
		}
		Vector2 tlPosition = 
			Vector2(currentPosition.x, currentPosition.y - m_BoxHeight);
		float combinedWidth = CurrentBlurbLengthFloat + totalLength;
		if (totalLength > 0.f)
		{
			combinedWidth += LengthOfSpace;
		}
		if (!thisBlurbWantsToBeAlone && !lastBlurbWantsToBeAlone
			&& combinedWidth <= CurrentMaxWidth)
		{
			tlPosition.x += totalLength;
			totalLength += CurrentBlurbLengthFloat;
			if (totalLength > 0.f)
			{
				tlPosition.x += LengthOfSpace;
				totalLength += LengthOfSpace;
			}
		}
		else
		{
			tlPosition.y -= textHeight;
			m_BoxHeight += textHeight;
			totalLength = CurrentBlurbLengthFloat;
			if (CurrentBlurbLengthFloat > CurrentMaxWidth)
			{
				CurrentMaxWidth = CurrentBlurbLengthFloat;
			}
		}
		lastBlurbWantsToBeAlone = thisBlurbWantsToBeAlone;

		AddLineOfText(TextModel, TextIndices, blurbText, blurbColor, font, fontSize, tlPosition);
	}

	AddQuadForBoxModel(BoxModels, boxIndices, currentPosition, 
		m_BoxHeight, CurrentMaxWidth, whiteBoxResource);
	if (CurrentMaxWidth > CurrentWidth)
	{
		CurrentWidth = CurrentMaxWidth;
	}

	return m_BoxHeight;
}

void DeveloperDebugInfoBox::AddQuadForBoxModel(
	std::vector<Vertex_PCT>& BoxModels, std::vector<unsigned int>& boxIndices,
	const Vector2& tlPosition, float boxHeight, float maxBoxWidth, const SpriteResource* whiteBoxResource)
{
	unsigned int startIndice = BoxModels.size();
	BoxModels.reserve(startIndice + 4);

	const Vector2 topRight = Vector2(tlPosition.x + maxBoxWidth, tlPosition.y);
	const Vector2 bottomLeft = Vector2(tlPosition.x, tlPosition.y - boxHeight);
	AABB2 textureCoords = AABB2::ZERO_TO_ONE;
	if (whiteBoxResource != nullptr)
	{
		textureCoords = whiteBoxResource->GetTextureCoords();
	}
	Vertex_PCT vert;
	vert.m_color = m_BoxColor;

	//BL Position
	vert.m_pos = Vector2(bottomLeft.x, bottomLeft.y);
	vert.m_texCoords = Vector2(textureCoords.mins.x, textureCoords.mins.y);
	BoxModels.push_back(vert);

	//BR Position
	vert.m_pos = Vector2(topRight.x, bottomLeft.y);
	vert.m_texCoords = Vector2(textureCoords.maxs.x, textureCoords.mins.y);
	BoxModels.push_back(vert);

	//TR Position
	vert.m_pos = Vector2(topRight.x, topRight.y);
	vert.m_texCoords = Vector2(textureCoords.maxs.x, textureCoords.maxs.y);
	BoxModels.push_back(vert);

	//TL Position
	vert.m_pos = Vector2(bottomLeft.x, topRight.y);
	vert.m_texCoords = Vector2(textureCoords.mins.x, textureCoords.maxs.y);
	BoxModels.push_back(vert);

	boxIndices.reserve(boxIndices.size() + 6);
	boxIndices.push_back(startIndice + 0);
	boxIndices.push_back(startIndice + 1);
	boxIndices.push_back(startIndice + 2);
	boxIndices.push_back(startIndice + 0);
	boxIndices.push_back(startIndice + 2);
	boxIndices.push_back(startIndice + 3);
}


void DeveloperDebugInfoBox::AddLineOfText(std::vector<std::vector<Vertex_PCT>>& fontModel,
	std::vector<std::vector<unsigned int>>& fontIndices,
	const std::string& text, const Rgba& textColor,
	Font* font, float fontSize,
	const Vector2& currentPosition)
{

	std::vector<std::vector<Vertex_PCT>> textModel;
	std::vector<std::vector<unsigned int>> textIndices;
	font->GetTextToRender(textModel, textIndices, currentPosition, text,
		fontSize, Vector3(1.f, 0.f), Vector3(0.f, 1.f), textColor, Font_LeftHand_Aligned,
		true, 0.f);

	fontModel.resize(textModel.size());
	fontIndices.resize(textIndices.size());
	for (size_t pageIdx = 0; pageIdx < textModel.size(); pageIdx++)
	{
		size_t lastIndex = fontModel[pageIdx].size();
		fontModel[pageIdx].insert(fontModel[pageIdx].end(), 
			textModel[pageIdx].begin(), textModel[pageIdx].end());
		for (size_t i = 0; i < textIndices[pageIdx].size(); i++)
		{
			fontIndices[pageIdx].push_back(textIndices[pageIdx][i] + lastIndex);
		}
	}

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Setters
void DeveloperDebugInfoBox::AddTextBlurb(const DeveloperDebugInfoTextBlurb& blurb)
{
	m_TextBlurbs.push_back(blurb);
}

void DeveloperDebugInfoBox::SetBoxColor(const Rgba& color)
{
	m_BoxColor = color;
}

void DeveloperDebugInfoBox::SetTextBlurbText(size_t idx, std::string& text)
{
	if (idx >= m_TextBlurbs.size())
	{
		return;
	}
	m_TextBlurbs[idx].SetText(text);
}

void DeveloperDebugInfoBox::SetTextBlurbColor(size_t idx, const Rgba& color)
{
	if (idx >= m_TextBlurbs.size())
	{
		return;
	}
	m_TextBlurbs[idx].SetColor(color);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Getters
const std::vector<DeveloperDebugInfoTextBlurb>& DeveloperDebugInfoBox::GetTextBlurbs() const
{
	return m_TextBlurbs;
}

const Rgba& DeveloperDebugInfoBox::GetBoxColor() const
{
	return m_BoxColor;
}

const size_t DeveloperDebugInfoBox::GetNumberOfTextBlurbs() const
{
	return m_TextBlurbs.size();
}

const DeveloperDebugInfoTextBlurb& DeveloperDebugInfoBox::GetTextBlurb(size_t idx) const
{
	if (idx >= m_TextBlurbs.size())
	{
		return DeveloperDebugInfoTextBlurb::s_EmptyDevDebugInfoTextBlurb;
	}
	return m_TextBlurbs[idx];
}

const std::string DeveloperDebugInfoBox::GetTextFromTextBlurb(size_t idx) const
{
	if (idx >= m_TextBlurbs.size())
	{
		return "";
	}
	return m_TextBlurbs[idx].GetText();
}

const Rgba DeveloperDebugInfoBox::GetColorFromTextBlurb(size_t idx) const
{
	if (idx >= m_TextBlurbs.size())
	{
		return Rgba::s_White;
	}
	return m_TextBlurbs[idx].GetColor();
}