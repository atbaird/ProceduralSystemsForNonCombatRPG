#include "Engine/UI/UIStructs/UIStyleSheet.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIStyleSheet::UIStyleSheet()
{
	m_stateDataPerWidgetType.resize(1);
}

UIStyleSheet::UIStyleSheet(const std::string& styleSheetName, const std::vector<UIStyleWidgetData>& data)
	: m_styleSheetName(styleSheetName),
	m_stateDataPerWidgetType(data)
{
	m_stateDataPerWidgetType.resize(1);
	if (GetStyleSheetHasADefault() == false)
	{
		m_stateDataPerWidgetType.resize(m_stateDataPerWidgetType.size() + 1);
		for (size_t widgetTypeIdx = m_stateDataPerWidgetType.size() - 1; widgetTypeIdx >= 1 ; widgetTypeIdx--)
		{
			m_stateDataPerWidgetType[widgetTypeIdx] = m_stateDataPerWidgetType[widgetTypeIdx - 1];
		}
		UIStyleWidgetData defaultVal = UIStyleWidgetData();
		m_stateDataPerWidgetType[0] = defaultVal;
	}
	else if (m_stateDataPerWidgetType[0].GetWidgetType() != "default")
	{
		size_t idxOfDefault = 0;
		for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
		{
			if (m_stateDataPerWidgetType[widgetTypeIdx].GetWidgetType() == "default")
			{
				idxOfDefault = widgetTypeIdx;
				break;
			}
		}
		UIStyleWidgetData defaultVal = m_stateDataPerWidgetType[idxOfDefault];

		for (size_t widgetTypeIdx = idxOfDefault; widgetTypeIdx >= 0; widgetTypeIdx--)
		{
			m_stateDataPerWidgetType[widgetTypeIdx] = m_stateDataPerWidgetType[widgetTypeIdx - 1];
		}
		m_stateDataPerWidgetType[0] = defaultVal;
	}
	m_changed = true;
}

UIStyleSheet::~UIStyleSheet()
{

}


//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------s
//XML
void UIStyleSheet::ReadXMLNodeData(const XMLNode& node)
{
	std::string name = node.getName();
	if (name == "")
	{
		return;
	}
	SetStyleSheetName(name);

	for (int widgetTypeIdx = 0; widgetTypeIdx < node.nChildNode(); widgetTypeIdx++)
	{
		//grand child level is by Widget type
		UIStyleWidgetData styleStateData;
		XMLNode widgetTypeChild = node.getChildNode(widgetTypeIdx);
		styleStateData.ReadXMLNode(widgetTypeChild);
		SetOrAddStateDataPerWidget(styleStateData);
	}
	m_changed = true;
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Setters
void UIStyleSheet::SetStyleSheetChanged(bool changed)
{
	m_changed = changed;
}

void UIStyleSheet::SetStyleSheetName(const std::string& styleSheetName)
{
	m_styleSheetName = styleSheetName;
	m_changed = true;
}

void UIStyleSheet::SetOrAddStateDataPerWidget(const UIStyleWidgetData& data)
{
	if (SetStateDataPerWidget(data) == false)
	{
		m_stateDataPerWidgetType.push_back(data);
	}
	m_changed = true;
}

const bool UIStyleSheet::SetStateDataPerWidget(const UIStyleWidgetData& data)
{
	std::string lowerCaseType = MakeLower(data.GetWidgetType());
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		std::string lowerCaseTypeCur = MakeLower(m_stateDataPerWidgetType[widgetTypeIdx].GetWidgetType());
		if (lowerCaseTypeCur == lowerCaseType)
		{
			m_stateDataPerWidgetType[widgetTypeIdx] = data;
			return true;
		}
	}

	return false;
}

void UIStyleSheet::SetStateAppearanceOnStyleState(const std::string& widgetType, size_t widgetState, const UIStateAppearanceData& appearanceData)
{
	std::string lowerCaseType = MakeLower(widgetType);
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		if (m_stateDataPerWidgetType[widgetTypeIdx] == lowerCaseType)
		{
			m_stateDataPerWidgetType[widgetTypeIdx].SetStateAppearance(widgetState, appearanceData);
		}
	}
	m_changed = true;
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Getters
const bool UIStyleSheet::GetStyleSheetHasADefault() const
{
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		if (m_stateDataPerWidgetType[widgetTypeIdx].GetWidgetType() == "default")
		{
			return true;
		}
	}
	return false;
}

const std::string UIStyleSheet::GetStyleSheetName() const
{
	return m_styleSheetName;
}
const std::vector<UIStyleWidgetData>* UIStyleSheet::GetStateDataForAllWidgets() const
{
	return &m_stateDataPerWidgetType;
}

std::vector<UIStyleWidgetData>* UIStyleSheet::GetEditableStateDataForAllWidgets()
{
	return &m_stateDataPerWidgetType;
}

const UIStyleWidgetData* UIStyleSheet::GetStyleDataForWidgetType(const std::string& widgetType) const
{
	std::string lowerCaseType = MakeLower(widgetType);
	const UIStyleWidgetData* defaultData = nullptr;
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		std::string currentName = m_stateDataPerWidgetType[widgetTypeIdx].GetWidgetType();
		if (currentName == lowerCaseType)
		{
			return &m_stateDataPerWidgetType[widgetTypeIdx];
		}
		else if (currentName == "default")
		{
			defaultData = &m_stateDataPerWidgetType[widgetTypeIdx];
		}
	}

	return defaultData;
}

UIStyleWidgetData* UIStyleSheet::GetEditableStyleDataForWidgetType(const std::string& widgetType)
{
	UIStyleWidgetData* defaultData = nullptr;
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		std::string currentName = m_stateDataPerWidgetType[widgetTypeIdx].GetWidgetType();
		if (currentName == widgetType)
		{
			return &m_stateDataPerWidgetType[widgetTypeIdx];
		}
		else if (currentName == "default")
		{
			defaultData = &m_stateDataPerWidgetType[widgetTypeIdx];
		}
	}

	return defaultData;
}

const bool UIStyleSheet::GetStyleSheetChanged() const
{
	return m_changed;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Convenience Methods
void UIStyleSheet::SetBackgroundColorForAllStatesAndAllAppearances(const Rgba& color)
{
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		UIStyleWidgetData& data = m_stateDataPerWidgetType[widgetTypeIdx];
		data.SetAllStatesBackgroundColors(color);
	}
	m_changed = true;
}

void UIStyleSheet::SetMarginColorForAllStatesAndAllAppearances(const Rgba& color)
{
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		UIStyleWidgetData& data = m_stateDataPerWidgetType[widgetTypeIdx];
		data.SetAllStatesMarginColors(color);
	}
	m_changed = true;
}

void UIStyleSheet::SetFontColorForAllStatesAndAllAppearances(const Rgba& color)
{
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		UIStyleWidgetData& data = m_stateDataPerWidgetType[widgetTypeIdx];
		data.SetAllStatesFontColors(color);
	}
	m_changed = true;
}

void UIStyleSheet::SetBackgroundImagesForAllStatesAndAllAppearances(const Texture* tex)
{
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		UIStyleWidgetData& data = m_stateDataPerWidgetType[widgetTypeIdx];
		data.SetAllStatesBackgroundImages(tex);
	}
	m_changed = true;
}

void UIStyleSheet::SetMarginImagesForAllStatesAndAllAppearances(const Texture* tex)
{
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		UIStyleWidgetData& data = m_stateDataPerWidgetType[widgetTypeIdx];
		data.SetAllStatesMarginImages(tex);
	}
	m_changed = true;
}

void UIStyleSheet::SetFontForAllStatesAndAllAppearances(Font* font)
{
	for (size_t widgetTypeIdx = 0; widgetTypeIdx < m_stateDataPerWidgetType.size(); widgetTypeIdx++)
	{
		UIStyleWidgetData& data = m_stateDataPerWidgetType[widgetTypeIdx];
		data.SetAllStatesFonts(font);
	}
	m_changed = true;
}