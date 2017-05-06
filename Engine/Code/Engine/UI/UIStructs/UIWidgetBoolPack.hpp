#pragma once

#ifndef UIWIDGETBOOLPACK_HPP
#define UIWIDGETBOOLPACK_HPP

struct UIWidgetBoolPack
{
private:
protected:
public:
	//Variables
	bool m_readFromXMLNode = false;
	bool m_StyleSheetChanged = true;
	bool m_PositionChanged = true;
	bool m_Render = true;
	bool m_RenderChildren = true;
	bool m_SizeChanged = true;
	bool m_UpdateMesh = true;
	bool m_UpdateText = true;
	bool m_stateChanged = true;
	bool m_OffsetSetByParent = false;
	bool m_currentlyHovered = false;
	bool m_amANamedWidget = false;
	bool m_AllowBuiltInMouseRelatedCodeToTrigger = true;
	bool m_animatingFontColor = false;
	bool m_animatingBackgroundColor = false;
	bool m_animatingMarginColor = false;
	bool m_amGroupingWidget = false;
	
	//Constructors();
	UIWidgetBoolPack();
	virtual ~UIWidgetBoolPack();
};

#endif