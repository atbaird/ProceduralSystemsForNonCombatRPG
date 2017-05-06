#pragma once

#ifndef UISTYLESTATEDATA_HPP
#define UISTYLESTATEDATA_HPP
#include "Engine/UI/UIStructs/UIStateAppearanceData.hpp"

struct UIStyleWidgetData
{
private:
protected:
	std::vector<UIStateAppearanceData> m_AppearancePerWidgetState;
	std::string m_WidgetType = "default";
	const Texture* m_defaultBackgroundImage = nullptr;
	const Texture* m_defaultCheckedBackgroundImage = nullptr;
	const Texture* m_defaultMarginImage = nullptr;
	const Texture* m_defaultCheckedMarginImage = nullptr;
	const Texture* m_defaultRadioBackgroundImage = nullptr;
	const Texture* m_defaultRadioSetBackgroundImage = nullptr;
	const Texture* m_defaultFillBarImage = nullptr;
	Font* m_defaultFont = nullptr;
	Rgba m_defaultBackgroundColor = Rgba::s_White;
	Rgba m_defaultMarginColor = Rgba::s_White;
	Rgba m_defaultFontColor = Rgba::s_Black;
	Rgba m_defaultFillBarColor = Rgba(1.f, 0.f, 0.f, 1.f);

public:
	//Constructors
	UIStyleWidgetData();
	UIStyleWidgetData(std::string widgetType, const UIStateAppearanceData& defaultAppearance = UIStateAppearanceData());
	virtual ~UIStyleWidgetData();

	//XML
	void ReadXMLNode(const XMLNode& node);

	//Set Alls
	void SetAllStatesBackgroundImages(const Texture* image);
	void SetAllStatesCheckedBackgroundImages(const Texture* image);
	void SetAllStatesRadioBackgroundImages(const Texture* image);
	void SetAllStatesRadioSetBackgroundImages(const Texture* image);
	void SetAllStatesMarginImages(const Texture* image);
	void SetAllStatesCheckedMarginImages(const Texture* image);
	void SetAllStatesFillBarImages(const Texture* image);
	void SetAllStatesFonts(Font* font);
	void SetAllStatesBackgroundColors(const Rgba& Color);
	void SetAllStatesMarginColors(const Rgba& Color);
	void SetAllStatesFontColors(const Rgba& Color);
	void SetALlStatesFillBarColor(const Rgba& Color);
	void LoadDefaultsOntoUIStateAppearanceData(UIStateAppearanceData& data);

	//Setters
	void SetDefaultBackgroundImage(const Texture* defaultBackgroundImage);
	void SetDefaultCheckedBackgroundImage(const Texture* defaultBackgroundImage);
	void SetDefaultRadioBackgroundImage(const Texture* defaultBackgroundImage);
	void SetDefaultRadioSetBackgroundImage(const Texture* defaultBackgroundImage);
	void SetDefaultMarginImage(const Texture* defaultMarginImage);
	void SetDefaultCheckedMarginImage(const Texture* defaultMarginImage);
	void SetDefaultFillBarImage(const Texture* defaultMarginImage);
	void SetDefaultFont(Font* font);
	void SetDefaultBackgroundColor(const Rgba& color);
	void SetDefaultMarginColor(const Rgba& color);
	void SetDefaultFontColor(const Rgba& color);

	void SetStateBackgroundImage(UIWidgetStates state, const Texture* image);
	void SetStateMarginImage(UIWidgetStates state, const Texture* image);
	void SetStateFont(UIWidgetStates state, Font* font);
	void SetStateBackgroundColor(UIWidgetStates state, const Rgba& Color);
	void SetStateMarginColor(UIWidgetStates state, const Rgba& Color);
	void SetStateFontColor(UIWidgetStates state, const Rgba& Color);
	void SetStateBackgroundImage(int state, const Texture* image);
	void SetStateMarginImage(int state, const Texture* image);
	void SetStateFont(int state, Font* font);
	void SetStateBackgroundColor(int state, const Rgba& Color);
	void SetStateMarginColor(int state, const Rgba& Color);
	void SetStateFontColor(int state, const Rgba& Color);

	void SetWidgetType(const std::string& widgetType);
	void ResizeStatesToLargerSize(size_t newSize);
	void ShrinkBackToDefaultNumberOfStates();
	void SetStateAppearance(size_t idx, const UIStateAppearanceData& stateApp);

	//Getters
	const UIStateAppearanceData GetAppearanceDataForIdx(size_t idx) const;
	UIStateAppearanceData* GetEditableAppearanceDataForIdx(size_t idx);
	const std::string GetWidgetType() const;
	const std::vector<UIStateAppearanceData>* GetStateAppearanceData() const;
	std::vector<UIStateAppearanceData>* GetEditableStateAppearanceData();

	//Operators
	const bool operator==(const UIStyleWidgetData& other) const;


};
#endif