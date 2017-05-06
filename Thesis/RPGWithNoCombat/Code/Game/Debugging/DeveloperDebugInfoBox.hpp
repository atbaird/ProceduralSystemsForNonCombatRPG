#pragma once

#ifndef DEVELOPERDEBUGINFOBOX_HPP
#define DEVELOPERDEBUGINFOBOX_HPP
#include "Game/Debugging/DeveloperDebugInfoTextBlurb.hpp"
#include <vector>

class Font;
struct Vertex_PCT;
class SpriteResource;
struct DeveloperDebugInfoBox
{
private:
protected:
	//Variables
	std::vector<DeveloperDebugInfoTextBlurb> m_TextBlurbs;
	Rgba m_BoxColor = Rgba(0.1f, 0.1f, 0.1f, 0.5f);
	float m_BoxHeight = 1.f;
public:
	//Static Variable
	static const DeveloperDebugInfoBox s_EmptyDevDebugInfoBox;

	//Constructors
	DeveloperDebugInfoBox();
	DeveloperDebugInfoBox(const Rgba& boxColor);
	DeveloperDebugInfoBox(const Rgba& boxColor, const std::vector<DeveloperDebugInfoTextBlurb>& textBlurbs);

	//Update
	float Update(Font* font, const Vector2& currentPosition,
		std::vector<Vertex_PCT>& BoxModels, std::vector<unsigned int>& boxIndices,
		std::vector<std::vector<Vertex_PCT>>& TextModel, 
		std::vector<std::vector<unsigned int>>& TextIndices,
		float fontSize, float maxBoxWidth, const SpriteResource* whiteBoxResource,
		float& CurrentWidth);
protected:
	void AddQuadForBoxModel(std::vector<Vertex_PCT>& BoxModels, std::vector<unsigned int>& boxIndices,
		const Vector2& tlPosition, float boxHeight, float maxBoxWidth,
		const SpriteResource* whiteBoxResource);
	void AddLineOfText(std::vector<std::vector<Vertex_PCT>>& fontModel,
		std::vector<std::vector<unsigned int>>& fontIndices,
		const std::string& text, const Rgba& textColor,
		Font* font, float fontSize,
		const Vector2& currentPosition);
public:

	//Setters
	void AddTextBlurb(const DeveloperDebugInfoTextBlurb& blurb);
	void SetBoxColor(const Rgba& color);
	void SetTextBlurbText(size_t idx, std::string& text);
	void SetTextBlurbColor(size_t idx, const Rgba& color);

	//Getters
	const float GetBoxHeight() const { return m_BoxHeight; };
	const std::vector<DeveloperDebugInfoTextBlurb>& GetTextBlurbs() const;
	const Rgba& GetBoxColor() const;
	const size_t GetNumberOfTextBlurbs() const;
	const DeveloperDebugInfoTextBlurb& GetTextBlurb(size_t idx) const;
	const std::string GetTextFromTextBlurb(size_t idx) const;
	const Rgba GetColorFromTextBlurb(size_t idx) const;
};
#endif