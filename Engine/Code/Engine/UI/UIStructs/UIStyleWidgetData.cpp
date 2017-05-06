#include "Engine/UI/UIStructs/UIStyleWidgetData.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//Constructors
UIStyleWidgetData::UIStyleWidgetData()
{
	m_AppearancePerWidgetState.resize(UIWIDGETSTATE_NUM_TO_HOLD_DATA_FOR);

	for (size_t idx = 0; idx < m_AppearancePerWidgetState.size(); idx++)
	{
		m_AppearancePerWidgetState[idx].SetState(idx);
	}
}

UIStyleWidgetData::UIStyleWidgetData(std::string widgetType, const UIStateAppearanceData& defaultAppearance)
	: m_WidgetType(widgetType)
{
	m_defaultBackgroundImage = defaultAppearance.GetBackgroundImage();
	m_defaultMarginImage = defaultAppearance.GetMarginImage();
	m_defaultFont = defaultAppearance.GetFont();
	m_defaultFontColor = defaultAppearance.GetFontColor();
	m_defaultMarginColor = defaultAppearance.GetBackgroundMarginColor();
	m_defaultBackgroundColor = defaultAppearance.GetBackgroundColor();

	m_AppearancePerWidgetState.resize(UIWIDGETSTATE_NUM_TO_HOLD_DATA_FOR);
	for (size_t stateAppIdx = 0; stateAppIdx < m_AppearancePerWidgetState.size(); stateAppIdx++)
	{
		m_AppearancePerWidgetState[stateAppIdx] = defaultAppearance;
		m_AppearancePerWidgetState[stateAppIdx].SetState(stateAppIdx);
	}
}

UIStyleWidgetData::~UIStyleWidgetData()
{

}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//XML
void UIStyleWidgetData::ReadXMLNode(const XMLNode& node)
{
	std::vector<int> statesRead;
	std::string widgetTypeName = MakeLower(node.getName());
	SetWidgetType(widgetTypeName);
	for (int stateTypeIdx = 0; stateTypeIdx < node.nChildNode(); stateTypeIdx++)
	{
		UIStateAppearanceData stateAppearanceData;
		XMLNode stateTypeChild = node.getChildNode(stateTypeIdx);
		stateAppearanceData.ReadXMLNode(stateTypeChild);

		int stateID = stateAppearanceData.GetState();
		if (stateID >= 0)
		{
			SetStateAppearance(stateAppearanceData.GetState(), stateAppearanceData);
			statesRead.push_back(stateID);
		}
	}

	UIStateAppearanceData defaultData;
	for (size_t appIdx = 0; appIdx < m_AppearancePerWidgetState.size(); appIdx++)
	{
		UIStateAppearanceData& stateAppearanceData = m_AppearancePerWidgetState[appIdx];
		int stateID = stateAppearanceData.GetState();
		if (stateID == UIWIDGETSTATE_ACTIVE)
		{
			defaultData = stateAppearanceData;
			break;
		}
	}


	for (size_t appIdx = 0; appIdx < m_AppearancePerWidgetState.size(); appIdx++)
	{
		UIStateAppearanceData& stateAppearanceData = m_AppearancePerWidgetState[appIdx];
		bool setOnce = false;

		for (size_t statesReadIdx = 0; statesReadIdx < statesRead.size(); statesReadIdx++)
		{
			if (stateAppearanceData.GetState() == statesRead[statesReadIdx])
			{
				setOnce = true;
				break;
			}
		}
		if (setOnce == false)
		{
			int stateID = stateAppearanceData.GetState();
			defaultData.SetState(stateID);
			stateAppearanceData = defaultData;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//Set Alls
void UIStyleWidgetData::SetAllStatesBackgroundImages(const Texture* image)
{
	m_defaultBackgroundImage = image;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetBackgroundImage(m_defaultBackgroundImage);
	}
}

void UIStyleWidgetData::SetAllStatesCheckedBackgroundImages(const Texture* image)
{
	m_defaultCheckedBackgroundImage = image;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetCheckedBackgroundImage(m_defaultCheckedBackgroundImage);
	}
}

void UIStyleWidgetData::SetAllStatesRadioBackgroundImages(const Texture* image)
{
	m_defaultRadioBackgroundImage = image;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetRadioBackgroundImage(m_defaultRadioBackgroundImage);
	}
}

void UIStyleWidgetData::SetAllStatesRadioSetBackgroundImages(const Texture* image)
{
	m_defaultRadioSetBackgroundImage = image;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetRadioSetBackgroundImage(m_defaultRadioSetBackgroundImage);
	}
}

void UIStyleWidgetData::SetAllStatesMarginImages(const Texture* image)
{
	m_defaultMarginImage = image;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetMarginImage(m_defaultMarginImage);
	}
}

void UIStyleWidgetData::SetAllStatesCheckedMarginImages(const Texture* image)
{
	m_defaultCheckedMarginImage = image;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetCheckedMarginImage(m_defaultCheckedMarginImage);
	}
}

void UIStyleWidgetData::SetAllStatesFillBarImages(const Texture* image)
{
	m_defaultFillBarImage = image;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetFillBarImage(m_defaultFillBarImage);
	}

}

void UIStyleWidgetData::SetAllStatesFonts(Font* font)
{
	m_defaultFont = font;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetFont(m_defaultFont);
	}
}

void UIStyleWidgetData::SetAllStatesBackgroundColors(const Rgba& Color)
{
	m_defaultBackgroundColor = Color;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetBackgroundColor(m_defaultBackgroundColor);
	}
}

void UIStyleWidgetData::SetAllStatesMarginColors(const Rgba& Color)
{
	m_defaultMarginColor = Color;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetBackgroundMarginColor(m_defaultMarginColor);
	}
}

void UIStyleWidgetData::SetAllStatesFontColors(const Rgba& Color)
{
	m_defaultFontColor = Color;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetFontColor(m_defaultFontColor);
	}
}

void UIStyleWidgetData::SetALlStatesFillBarColor(const Rgba& Color)
{
	m_defaultFillBarColor = Color;
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		data.SetFillBarColor(m_defaultFillBarColor);
	}
}

void UIStyleWidgetData::LoadDefaultsOntoUIStateAppearanceData(UIStateAppearanceData& data)
{
	data.SetBackgroundColor(m_defaultBackgroundColor);
	data.SetBackgroundMarginColor(m_defaultMarginColor);
	data.SetMarginImage(m_defaultMarginImage);
	data.SetBackgroundImage(m_defaultBackgroundImage);
	data.SetFont(m_defaultFont);
	data.SetCheckedBackgroundImage(m_defaultCheckedBackgroundImage);
	data.SetCheckedMarginImage(m_defaultCheckedMarginImage);
	data.SetRadioBackgroundImage(m_defaultRadioBackgroundImage);
	data.SetRadioSetBackgroundImage(m_defaultRadioSetBackgroundImage);
	data.SetFillBarImage(m_defaultFillBarImage);
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//Setters

void UIStyleWidgetData::SetDefaultBackgroundImage(const Texture* defaultBackgroundImage)
{
	m_defaultBackgroundImage = defaultBackgroundImage;
}

void UIStyleWidgetData::SetDefaultCheckedBackgroundImage(const Texture* defaultBackgroundImage)
{
	m_defaultCheckedBackgroundImage = defaultBackgroundImage;
}

void UIStyleWidgetData::SetDefaultRadioBackgroundImage(const Texture* defaultBackgroundImage)
{
	m_defaultRadioBackgroundImage = defaultBackgroundImage;
}

void UIStyleWidgetData::SetDefaultRadioSetBackgroundImage(const Texture* defaultBackgroundImage)
{
	m_defaultRadioSetBackgroundImage = defaultBackgroundImage;
}

void UIStyleWidgetData::SetDefaultMarginImage(const Texture* defaultMarginImage)
{
	m_defaultMarginImage = defaultMarginImage;
}

void UIStyleWidgetData::SetDefaultCheckedMarginImage(const Texture* defaultMarginImage)
{
	m_defaultCheckedMarginImage = defaultMarginImage;
}

void UIStyleWidgetData::SetDefaultFillBarImage(const Texture* defaultMarginImage)
{
	m_defaultFillBarImage = defaultMarginImage;
}

void UIStyleWidgetData::SetDefaultFont(Font* font)
{
	m_defaultFont = font;
}

void UIStyleWidgetData::SetDefaultBackgroundColor(const Rgba& color)
{
	m_defaultBackgroundColor = color;
}

void UIStyleWidgetData::SetDefaultMarginColor(const Rgba& color)
{
	m_defaultMarginColor = color;
}

void UIStyleWidgetData::SetDefaultFontColor(const Rgba& color)
{
	m_defaultFontColor = color;
}

void UIStyleWidgetData::SetStateBackgroundImage(UIWidgetStates state, const Texture* image)
{
	SetStateBackgroundImage((int)state, image);
}

void UIStyleWidgetData::SetStateMarginImage(UIWidgetStates state, const Texture* image)
{
	SetStateMarginImage((int)state, image);
}

void UIStyleWidgetData::SetStateFont(UIWidgetStates state, Font* font)
{
	SetStateFont((int)state, font);
}

void UIStyleWidgetData::SetStateBackgroundColor(UIWidgetStates state, const Rgba& Color)
{
	SetStateBackgroundColor((int)state, Color);
}

void UIStyleWidgetData::SetStateMarginColor(UIWidgetStates state, const Rgba& Color)
{
	SetStateMarginColor((int)state, Color);
}

void UIStyleWidgetData::SetStateFontColor(UIWidgetStates state, const Rgba& Color)
{
	SetStateFontColor((int)state, Color);
}

void UIStyleWidgetData::SetStateBackgroundImage(int state, const Texture* image)
{
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		if (data.GetState() == state)
		{
			data.SetBackgroundImage(image);
		}
	}
}

void UIStyleWidgetData::SetStateMarginImage(int state, const Texture* image)
{
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		if (data.GetState() == state)
		{
			data.SetMarginImage(image);
		}
	}
}

void UIStyleWidgetData::SetStateFont(int state, Font* font)
{
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		if (data.GetState() == state)
		{
			data.SetFont(font);
		}
	}
}

void UIStyleWidgetData::SetStateBackgroundColor(int state, const Rgba& Color)
{
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		if (data.GetState() == state)
		{
			data.SetBackgroundColor(Color);
		}
	}
}

void UIStyleWidgetData::SetStateMarginColor(int state, const Rgba& Color)
{
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		if (data.GetState() == state)
		{
			data.SetBackgroundMarginColor(Color);
		}
	}
}

void UIStyleWidgetData::SetStateFontColor(int state, const Rgba& Color)
{
	for (size_t widgetStateIdx = 0; widgetStateIdx < m_AppearancePerWidgetState.size(); widgetStateIdx++)
	{
		UIStateAppearanceData& data = m_AppearancePerWidgetState[widgetStateIdx];
		if (data.GetState() == state)
		{
			data.SetFontColor(Color);
		}
	}
}

void UIStyleWidgetData::SetWidgetType(const std::string& widgetType)
{
	m_WidgetType = widgetType;
}

void UIStyleWidgetData::ResizeStatesToLargerSize(size_t newSize)
{
	if (newSize > m_AppearancePerWidgetState.size())
	{
		m_AppearancePerWidgetState.resize(newSize);
		for (size_t appIdx = 0; appIdx < m_AppearancePerWidgetState.size(); appIdx++)
		{
			m_AppearancePerWidgetState[appIdx].SetState(appIdx);
		}
	}
}

void UIStyleWidgetData::ShrinkBackToDefaultNumberOfStates()
{
	m_AppearancePerWidgetState.resize(UIWIDGETSTATE_NUM_TO_HOLD_DATA_FOR);
}

void UIStyleWidgetData::SetStateAppearance(size_t idx, const UIStateAppearanceData& stateApp)
{
	if (idx >= m_AppearancePerWidgetState.size())
	{
		return;
	}
	m_AppearancePerWidgetState[idx] = stateApp;
	m_AppearancePerWidgetState[idx].SetState(idx);
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//Getters
const UIStateAppearanceData UIStyleWidgetData::GetAppearanceDataForIdx(size_t idx) const
{
	if (idx >= m_AppearancePerWidgetState.size())
	{
		m_AppearancePerWidgetState[0];
	}
	return m_AppearancePerWidgetState[idx];
}

UIStateAppearanceData* UIStyleWidgetData::GetEditableAppearanceDataForIdx(size_t idx)
{
	if (idx >= m_AppearancePerWidgetState.size())
	{
		return nullptr;
	}
	return &m_AppearancePerWidgetState[idx];
}

const std::string UIStyleWidgetData::GetWidgetType() const
{
	return m_WidgetType;
}

const std::vector<UIStateAppearanceData>* UIStyleWidgetData::GetStateAppearanceData() const
{
	return &m_AppearancePerWidgetState;
}

std::vector<UIStateAppearanceData>* UIStyleWidgetData::GetEditableStateAppearanceData()
{
	return &m_AppearancePerWidgetState;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//Operators
const bool UIStyleWidgetData::operator==(const UIStyleWidgetData& other) const
{
	if (other.m_WidgetType == m_WidgetType)
	{
		return true;
	}
	return false;
}