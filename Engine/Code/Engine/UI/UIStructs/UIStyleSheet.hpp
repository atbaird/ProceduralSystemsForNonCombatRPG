#pragma once

#ifndef UISTYLESHEET_HPP
#define UISTYLESHEET_HPP
#include "Engine/UI/UIStructs/UIStyleWidgetData.hpp"

struct XMLNode;
struct UIStyleSheet
{
private:
protected:
	std::vector<UIStyleWidgetData> m_stateDataPerWidgetType;
	std::string m_styleSheetName = "default";
	bool m_changed = true;
public:
	//Constructors
	UIStyleSheet();
	UIStyleSheet(const std::string& styleSheetName, const std::vector<UIStyleWidgetData>& data);
	virtual ~UIStyleSheet();

	//XML
	void ReadXMLNodeData(const XMLNode& node);

	//Setters
	void SetStyleSheetChanged(bool changed);
	void SetStyleSheetName(const std::string& styleSheetName);
	void SetOrAddStateDataPerWidget(const UIStyleWidgetData& data);
	const bool SetStateDataPerWidget(const UIStyleWidgetData& data);
	void SetStateAppearanceOnStyleState(const std::string& widgetType, size_t widgetState , const UIStateAppearanceData& appearanceData);
	

	//Getters
	const bool GetStyleSheetHasADefault() const;
	const std::string GetStyleSheetName() const;
	const std::vector<UIStyleWidgetData>* GetStateDataForAllWidgets() const;
	std::vector<UIStyleWidgetData>* GetEditableStateDataForAllWidgets();
	const UIStyleWidgetData* GetStyleDataForWidgetType(const std::string& widgetType) const;
	UIStyleWidgetData* GetEditableStyleDataForWidgetType(const std::string& widgetType);
	const bool GetStyleSheetChanged() const;

	//Convenience Methods
	void SetBackgroundColorForAllStatesAndAllAppearances(const Rgba& color);
	void SetMarginColorForAllStatesAndAllAppearances(const Rgba& color);
	void SetFontColorForAllStatesAndAllAppearances(const Rgba& color);
	void SetBackgroundImagesForAllStatesAndAllAppearances(const Texture* tex);
	void SetMarginImagesForAllStatesAndAllAppearances(const Texture* tex);
	void SetFontForAllStatesAndAllAppearances(Font* font);
};
#endif