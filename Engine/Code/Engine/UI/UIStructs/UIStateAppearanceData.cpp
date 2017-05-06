#include "Engine/UI/UIStructs/UIStateAppearanceData.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/UI/UISystem.hpp"

STATIC const std::string UIStateAppearanceData::s_ActiveText = "active";
STATIC const std::string UIStateAppearanceData::s_HighlightedText = "highlighted";
STATIC const std::string UIStateAppearanceData::s_HoveredText = "hovered";
STATIC const std::string UIStateAppearanceData::s_PressedText = "pressed";
STATIC const std::string UIStateAppearanceData::s_DisabledText = "disabled";
STATIC const std::string UIStateAppearanceData::s_MarginColorText = "margincolor";
STATIC const std::string UIStateAppearanceData::s_FillColorText = "fillcolor";
STATIC const std::string UIStateAppearanceData::s_FontColorText = "fontcolor";
STATIC const std::string UIStateAppearanceData::s_MarginImageText = "marginimage";
STATIC const std::string UIStateAppearanceData::s_BackgroundImageText = "backgroundimage";
STATIC const std::string UIStateAppearanceData::s_FontText = "font";


//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIStateAppearanceData::UIStateAppearanceData(UIWidgetStates state)
	: m_WidgetState((int)state)
{
	UISystem* system = UISystem::GetUISystemPtr();
	if (system != nullptr)
	{
		m_BackgroundImage = system->GetDefaultBackgroundImageForWidgets();
		m_MarginImage = m_BackgroundImage;
		m_CheckedMarginImage = m_BackgroundImage;
		m_CheckedBackgroundImage = m_BackgroundImage;
	}
}

UIStateAppearanceData::UIStateAppearanceData(int state)
	: m_WidgetState(state)
{
	UISystem* system = UISystem::GetUISystemPtr();
	if (system != nullptr)
	{
		m_BackgroundImage = system->GetDefaultBackgroundImageForWidgets();
		m_MarginImage = m_BackgroundImage;
	}
}

UIStateAppearanceData::~UIStateAppearanceData()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//XML
void UIStateAppearanceData::ReadXMLNode(const XMLNode& node)
{

	std::string stateName = MakeLower(node.getName());
	bool isAValidState = ConvertStringToStateType(stateName);
	if (isAValidState == false)
	{
		GUARANTEE_RECOVERABLE(false, "Hit a Style Sheet type that is not valid. Please be more careful when defining Style Sheet appearance states.");
		m_WidgetState = -1;
		return;
	}
	ReadXMLNodeAttributes(node);
	ReadXMLNodeChildren(node);
}


void UIStateAppearanceData::ReadXMLNodeAttributes(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string lowerCaseName = MakeLower(attr.lpszName);
		std::string val = std::string(attr.lpszValue);
		if (lowerCaseName == s_MarginColorText)
		{
			m_backgroundMarginColor = ReadColorOutOfString(val);
		}
		else if (lowerCaseName == s_FillColorText)
		{
			m_backgroundColor = ReadColorOutOfString(val);
		}
		else if (lowerCaseName == s_FontColorText)
		{
			m_fontColor = ReadColorOutOfString(val);
		}
		else if (lowerCaseName == "fillbarcolor")
		{
			m_fillBarColor = ReadColorOutOfString(val);
		}
		else if (lowerCaseName == s_MarginImageText)
		{
			Texture* tex = Texture::CreateOrGetTexture(val);
			if (tex != nullptr)
			{
				m_MarginImage = tex;
			}
		}
		else if (lowerCaseName == s_BackgroundImageText)
		{
			Texture* tex = Texture::CreateOrGetTexture(val);
			if (tex != nullptr)
			{
				m_BackgroundImage = tex;
			}
		}
		else if (lowerCaseName == "backgroundcheckboxselected")
		{
			Texture* tex = Texture::CreateOrGetTexture(val);
			if (tex != nullptr)
			{
				m_CheckedBackgroundImage = tex;
			}
		}
		else if (lowerCaseName == "margincheckboxselected")
		{
			Texture* tex = Texture::CreateOrGetTexture(val);
			if (tex != nullptr)
			{
				m_CheckedMarginImage = tex;
			}
		}
		else if (lowerCaseName == "backgroundradiounselected")
		{
			Texture* tex = Texture::CreateOrGetTexture(val);
			if (tex != nullptr)
			{
				m_RadioBackgroundImage = tex;
			}
		}
		else if (lowerCaseName == "backgroundradiounselected")
		{
			Texture* tex = Texture::CreateOrGetTexture(val);
			if (tex != nullptr)
			{
				m_RadioSetBackgroundImage = tex;
			}
		}
		else if (lowerCaseName == "fillbarimage")
		{
			Texture* tex = Texture::CreateOrGetTexture(val);
			if (tex != nullptr)
			{
				m_FillBarImage = tex;
			}
		}
		else if (lowerCaseName == s_FontText)
		{
			//val
			std::string fontLoc = val;
			Font* font = Font::GetFontByNameWithDirectoryLoc(fontLoc);
			if (font != nullptr)
			{
				m_Font = font;
			}
		}
		else if (lowerCaseName == "fontsize" || lowerCaseName == "fontscale")
		{
			m_fontSize = (float)(atof(val.c_str()));
		}
	}
}


void UIStateAppearanceData::ReadXMLNodeChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "fontcolor" || childName == "fontcoloranimation")
		{
			ReadNodeDataForUIAnimatedProperties(child, m_FontColorAnimation);
			if (m_FontColorAnimation.GetNumberOfFrames() == 0)
			{
				m_hasFontColorAnimation = false;
			}
			else
			{
				m_hasFontColorAnimation = true;
			}
		}
		else if (childName == "margincolor" || childName == "margincoloranimation")
		{
			ReadNodeDataForUIAnimatedProperties(child, m_backgroundMarginColorAnimation);
			if (m_backgroundMarginColorAnimation.GetNumberOfFrames() == 0)
			{
				m_hasMarginAnimation = false;
			}
			else
			{
				m_hasMarginAnimation = true;
			}
		}
		else if (childName == "fillcolor" || childName == "fillcoloranimation"
			|| childName == "backgroundcolor" || childName == "backgroundcoloranimation")
		{
			ReadNodeDataForUIAnimatedProperties(child, m_backgroundFillColorAnimation);
			if (m_backgroundFillColorAnimation.GetNumberOfFrames() == 0)
			{
				m_hasFillColorAnimation = false;
			}
			else
			{
				m_hasFillColorAnimation = true;
			}
		}
	}
}


void UIStateAppearanceData::ReadNodeDataForUIAnimatedProperties(const XMLNode& child, UIAnimatedProperties<Rgba>& animation)
{
	animation.ClearFrames();
	float totalTime = 1.f;
	for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = child.getAttribute(attrIdx);
		std::string name = MakeLower(attr.lpszName);
		if (name == "time")
		{
			totalTime = (float)atof(attr.lpszValue);
		}
	}
	animation.SetEndTime(totalTime);

	for (int grandChildIdx = 0; grandChildIdx < child.nChildNode(); grandChildIdx++)
	{
		XMLNode grandChild = child.getChildNode(grandChildIdx);
		std::string grandChildName = MakeLower(grandChild.getName());
		if (grandChildName == "keyframe" || grandChildName == "frame")
		{
			bool setColor = false;
			bool setTime = false;
			float time = 0.f;
			Rgba colorVal = Rgba::s_White;
			for (int grandChildAttrIdx = 0; grandChildAttrIdx < grandChild.nAttribute(); grandChildAttrIdx++)
			{
				XMLAttribute attr = grandChild.getAttribute(grandChildAttrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "color" || attrName == "tint")
				{
					std::vector<std::string> parsedVals = ParseString(attr.lpszValue, ',');
					if (parsedVals.size() == 0)
					{
						continue;
					}
					setColor = true;
					if (parsedVals.size() > 0)
					{
						colorVal.Alpha = (float)atof(parsedVals[0].c_str());
					}
					if (parsedVals.size() > 1)
					{
						colorVal.Alpha = (float)atof(parsedVals[1].c_str());
					}
					if (parsedVals.size() > 2)
					{
						colorVal.Alpha = (float)atof(parsedVals[2].c_str());
					}
					if (parsedVals.size() > 3)
					{
						colorVal.Alpha = (float)atof(parsedVals[3].c_str());
					}

				}
				else if (attrName == "timepercentage")
				{
					setTime = true;
					time = (float)atof(attr.lpszValue);
				}
				else if (attrName == "time")
				{
					setTime = true;
					time = (float)atof(attr.lpszValue);
					if (totalTime != 0.f)
					{
						time = time / totalTime;
					}
				}
			}
			if (setColor && setTime)
			{
				UIKeyFrame<Rgba> frame(colorVal, time);
				animation.AddOrSetValue(frame);
			}
		}
	}
}


//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Setters
void UIStateAppearanceData::SetBackgroundMarginColorAnimation(const UIAnimatedProperties<Rgba>& color)
{
	m_backgroundMarginColorAnimation = color;
	if (m_backgroundMarginColorAnimation.GetNumberOfFrames() == 0)
	{
		m_hasMarginAnimation = false;
	}
	else
	{
		m_hasMarginAnimation = true;
	}
}

void UIStateAppearanceData::SetBackgroundFillColorAnimation(const UIAnimatedProperties<Rgba>& color)
{
	m_backgroundFillColorAnimation = color;
	if (m_backgroundFillColorAnimation.GetNumberOfFrames() == 0)
	{
		m_hasFillColorAnimation = false;
	}
	else
	{
		m_hasFillColorAnimation = true;
	}
}

void UIStateAppearanceData::SetTextColorAnimation(const UIAnimatedProperties<Rgba>& color)
{
	m_FontColorAnimation = color;
	if (m_FontColorAnimation.GetNumberOfFrames() == 0)
	{
		m_hasFontColorAnimation = false;
	}
	else
	{
		m_hasFontColorAnimation = true;
	}
}

void UIStateAppearanceData::SetBackgroundMarginColor(const Rgba& color)
{
	m_backgroundMarginColor = color;
}

void UIStateAppearanceData::SetBackgroundColor(const Rgba& color)
{
	m_backgroundColor = color;
}

void UIStateAppearanceData::SetFillBarColor(const Rgba& color)
{
	m_fillBarColor = color;
}

void UIStateAppearanceData::SetFont(Font* font)
{
	m_Font = font;
}

void UIStateAppearanceData::SetFontSize(float size)
{
	m_fontSize = size;
}

void UIStateAppearanceData::SetFontColor(const Rgba& color)
{
	m_fontColor = color;
}

void UIStateAppearanceData::SetMarginImage(const Texture* tex)
{
	m_MarginImage = tex;
}

void UIStateAppearanceData::SetBackgroundImage(const Texture* tex)
{
	m_BackgroundImage = tex;
}

void UIStateAppearanceData::SetCheckedMarginImage(const Texture* tex)
{
	m_CheckedMarginImage = tex;
}

void UIStateAppearanceData::SetCheckedBackgroundImage(const Texture* tex)
{
	m_CheckedBackgroundImage = tex;
}

void UIStateAppearanceData::SetRadioBackgroundImage(const Texture* tex)
{
	m_RadioBackgroundImage = tex;
}

void UIStateAppearanceData::SetRadioSetBackgroundImage(const Texture* tex)
{
	m_RadioSetBackgroundImage = tex;
}

void UIStateAppearanceData::SetFillBarImage(const Texture* tex)
{
	m_FillBarImage = tex;
}

void UIStateAppearanceData::SetState(UIWidgetStates state)
{
	m_WidgetState = state;
}

void UIStateAppearanceData::SetState(int state)
{
	m_WidgetState = state;
}

bool UIStateAppearanceData::ConvertStringToStateType(const std::string& stateName)
{
	/*
	UIWIDGETSTATE_HIDDEN = -1,
	UIWIDGETSTATE_ACTIVE,
	UIWIDGETSTATE_HIGHLIGHTED, //Hovered
	UIWIDGETSTATE_HOVERED = UIWIDGETSTATE_HIGHLIGHTED,
	UIWIDGETSTATE_PRESSED,
	UIWIDGETSTATE_DISABLED,
	UIWIDGETSTATE_NUM_TO_HOLD_DATA_FOR,
	UIWIDGETSTATE_NUM_OF_STATES
	*/
	std::string stateLower = MakeLower(stateName);
	if (stateLower == (s_ActiveText))
	{
		m_WidgetState = UIWIDGETSTATE_ACTIVE;
	}
	else if (stateLower == (s_HighlightedText) 
		|| stateLower == (s_HoveredText))
	{
		m_WidgetState = UIWIDGETSTATE_HOVERED;
	}
	else if (stateLower == (s_PressedText))
	{
		m_WidgetState = UIWIDGETSTATE_PRESSED;
	}
	else if (stateLower == (s_DisabledText))
	{
		m_WidgetState = UIWIDGETSTATE_DISABLED;
	}
	else
	{
		int val = atoi(stateLower.c_str());
		if (val >= 0)
		{
			//Need to do more for this; since would basically need custom registered states for this.
			m_WidgetState = val;
		}
		else
		{
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Getters

const UIAnimatedProperties<Rgba> UIStateAppearanceData::GetBackgroundMarginColorAnimation() const
{
	return m_backgroundMarginColorAnimation;
}

const UIAnimatedProperties<Rgba> UIStateAppearanceData::GetBackgroundFillColorAnimation() const
{
	return m_backgroundFillColorAnimation;
}

const UIAnimatedProperties<Rgba> UIStateAppearanceData::GetFontColorAnimation() const
{
	return m_FontColorAnimation;
}

const UIAnimatedProperties<Rgba>* UIStateAppearanceData::GetPtrBackgroundMarginColorAnimation() const
{
	return &m_backgroundMarginColorAnimation;
}

const UIAnimatedProperties<Rgba>* UIStateAppearanceData::GetPtrBackgroundFillColorAnimation() const
{
	return &m_backgroundFillColorAnimation;
}

const UIAnimatedProperties<Rgba>* UIStateAppearanceData::GetPtrFontColorAnimation() const
{
	return &m_FontColorAnimation;
}

UIAnimatedProperties<Rgba>* UIStateAppearanceData::GetEditablePtrBackgroundMarginColorAnimation()
{
	return &m_backgroundMarginColorAnimation;
}

UIAnimatedProperties<Rgba>* UIStateAppearanceData::GetEditablePtrBackgroundFillColorAnimation()
{
	return &m_backgroundFillColorAnimation;
}

UIAnimatedProperties<Rgba>* UIStateAppearanceData::GetEditablePtrFontColorAnimation()
{
	return &m_FontColorAnimation;
}

float UIStateAppearanceData::GetFontSize() const
{
	return m_fontSize;
}

bool UIStateAppearanceData::GetHasMarginColorAnimation() const
{
	return m_hasMarginAnimation;
}

bool UIStateAppearanceData::GetHasFillColorAnimation() const
{
	return m_hasFillColorAnimation;
}

bool UIStateAppearanceData::GetHasTextColorAnimation() const
{
	return m_hasFontColorAnimation;
}

Rgba UIStateAppearanceData::GetBackgroundMarginColor() const
{
	return m_backgroundMarginColor;
}

Rgba UIStateAppearanceData::GetBackgroundColor() const
{
	return m_backgroundColor;
}

Rgba UIStateAppearanceData::GetFillBarColor() const
{
	return m_fillBarColor;
}

Rgba UIStateAppearanceData::GetFontColor() const
{
	return m_fontColor;
}

const Texture* UIStateAppearanceData::GetMarginImage() const
{
	return m_MarginImage;
}

const Texture* UIStateAppearanceData::GetBackgroundImage() const
{
	return m_BackgroundImage;
}

const Texture* UIStateAppearanceData::GetCheckedMarginImage() const
{
	return m_CheckedMarginImage;
}

const Texture* UIStateAppearanceData::GetCheckedBackgroundImage() const
{
	return m_CheckedBackgroundImage;
}

const Texture* UIStateAppearanceData::GetRadioBackgroundImage() const
{
	return m_RadioBackgroundImage;
}

const Texture* UIStateAppearanceData::GetRadioSetBackgroundImage() const
{
	return m_RadioSetBackgroundImage;
}

const Texture* UIStateAppearanceData::GetFillBarImage() const
{
	return m_FillBarImage;
}

int UIStateAppearanceData::GetState() const
{
	return m_WidgetState;
}

Font* UIStateAppearanceData::GetFont() const
{
	return m_Font;
}

UIWidgetStates UIStateAppearanceData::GetStateAsWidgetState() const
{
	return (UIWidgetStates)m_WidgetState;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Operations
const bool UIStateAppearanceData::operator==(const UIStateAppearanceData& other) const
{
	if (other.m_WidgetState == m_WidgetState)
	{
		return true;
	}
	return false;
}