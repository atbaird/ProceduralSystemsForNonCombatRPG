#pragma once

#ifndef UIWIDGETCOLORPACK_HPP
#define UIWIDGETCOLORPACK_HPP
#include "Engine/Renderer/Rgba.hpp"

struct UIWidgetColorPack
{
private:
protected:
public:
	//Variables
	Rgba m_fontColor = Rgba::s_White;
	Rgba m_backgroundColor = Rgba::s_White;
	Rgba m_marginColor = Rgba::s_White;
	Rgba m_fontColorWhenOriginallyChangedState = Rgba::s_White;
	Rgba m_backgroundColorWhenOriginallyChangedState = Rgba::s_White;
	Rgba m_marginColorWhenOriginallyChangedState = Rgba::s_White;

	//Constructors
	UIWidgetColorPack();
	virtual ~UIWidgetColorPack();
};

#endif