#include "Game/Debugging/DeveloperDebugInfoTextBlurb.hpp"

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//static variables
STATIC const DeveloperDebugInfoTextBlurb DeveloperDebugInfoTextBlurb::s_EmptyDevDebugInfoTextBlurb
		= DeveloperDebugInfoTextBlurb();

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Constructors
DeveloperDebugInfoTextBlurb::DeveloperDebugInfoTextBlurb()
{

}

DeveloperDebugInfoTextBlurb::DeveloperDebugInfoTextBlurb(const std::string& text, const Rgba& color)
	: m_text(text),
	m_color(color)
{

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Setters
void DeveloperDebugInfoTextBlurb::SetText(const std::string& text)
{
	m_text = text;
}

void DeveloperDebugInfoTextBlurb::SetColor(const Rgba& color)
{
	m_color = color;
}

void DeveloperDebugInfoTextBlurb::SetGuaranteeAloneOnLine(bool newLine)
{
	m_GuaranteeAloneOnLine = newLine;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Getters
const Rgba& DeveloperDebugInfoTextBlurb::GetColor() const
{
	return m_color;
}

const std::string& DeveloperDebugInfoTextBlurb::GetText() const
{
	return m_text;
}

const size_t DeveloperDebugInfoTextBlurb::GetTextLength() const
{
	return m_text.length();
}

const bool DeveloperDebugInfoTextBlurb::GetGuaranteeAloneOnLine() const
{
	return m_GuaranteeAloneOnLine;
}