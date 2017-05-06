#pragma once

#ifndef DEVELOPERDEBUGINFOTEXTBLURB_HPP
#define DEVELOPERDEBUGINFOTEXTBLURB_HPP
#include "Engine/Renderer/Rgba.hpp"
#include <string>

struct DeveloperDebugInfoTextBlurb
{
private:
protected:
	//Variables
	std::string m_text = "";
	Rgba m_color = Rgba::s_White;
	bool m_GuaranteeAloneOnLine = false;
public:
	//static variables
	static const DeveloperDebugInfoTextBlurb s_EmptyDevDebugInfoTextBlurb;

	//Constructors
	DeveloperDebugInfoTextBlurb();
	DeveloperDebugInfoTextBlurb(const std::string& text, const Rgba& color);

	//Setters
	void SetText(const std::string& text);
	void SetColor(const Rgba& color);
	void SetGuaranteeAloneOnLine(bool newLine);

	//Getters
	const Rgba& GetColor() const;
	const std::string& GetText() const;
	const size_t GetTextLength() const;
	const bool GetGuaranteeAloneOnLine() const;

};
#endif