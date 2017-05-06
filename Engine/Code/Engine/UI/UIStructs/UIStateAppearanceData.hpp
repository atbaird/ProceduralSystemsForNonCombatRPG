#pragma once

#ifndef UISTATEDATA_HPP
#define UISTATEDATA_HPP
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Font/Font.hpp"
#include "Engine/UI/UIStructs/UIAnimations/UIAnimatedProperties.hpp"
#include "Engine/UI/UIStructs/UIWidgetStates.hpp"


struct XMLNode;
struct UIStateAppearanceData
{
private:
protected:
	static const std::string s_ActiveText;
	static const std::string s_HighlightedText;
	static const std::string s_HoveredText;
	static const std::string s_PressedText;
	static const std::string s_DisabledText;
	static const std::string s_MarginColorText;
	static const std::string s_FillColorText;
	static const std::string s_FontColorText;
	static const std::string s_MarginImageText;
	static const std::string s_BackgroundImageText;
	static const std::string s_FontText;

	UIAnimatedProperties<Rgba> m_backgroundMarginColorAnimation;
	UIAnimatedProperties<Rgba> m_backgroundFillColorAnimation;
	UIAnimatedProperties<Rgba> m_FontColorAnimation;
	Rgba m_backgroundMarginColor = Rgba(0.f, 0.f, 0.f, 1.f);
	Rgba m_backgroundColor = Rgba(1.f, 1.f, 1.f, 1.f);
	Rgba m_fontColor = Rgba(0.f, 0.f, 0.f, 1.f);
	Rgba m_fillBarColor = Rgba(1.f, 0.f, 0.f, 1.f);
	const Texture* m_BackgroundImage = nullptr;
	const Texture* m_CheckedBackgroundImage = nullptr;
	const Texture* m_MarginImage = nullptr;
	const Texture* m_CheckedMarginImage = nullptr;
	const Texture* m_RadioBackgroundImage = nullptr;
	const Texture* m_RadioSetBackgroundImage = nullptr;
	const Texture* m_FillBarImage = nullptr;
	Font* m_Font = nullptr;
	float m_fontSize = 0.01f;
	int m_WidgetState = (int)UIWIDGETSTATE_HIDDEN;
	bool m_hasMarginAnimation = false;
	bool m_hasFillColorAnimation = false;
	bool m_hasFontColorAnimation = false;
public:
	//Constructors
	UIStateAppearanceData(UIWidgetStates state = UIWIDGETSTATE_HIDDEN);
	UIStateAppearanceData(int state);
	virtual ~UIStateAppearanceData();

	//XML
	void ReadXMLNode(const XMLNode& node);
	void ReadXMLNodeAttributes(const XMLNode& node);
	void ReadXMLNodeChildren(const XMLNode& node);
	void ReadNodeDataForUIAnimatedProperties(const XMLNode& currentNode, UIAnimatedProperties<Rgba>& animation);

	//Setters
	void SetBackgroundMarginColorAnimation(const UIAnimatedProperties<Rgba>& color);
	void SetBackgroundFillColorAnimation(const UIAnimatedProperties<Rgba>& color);
	void SetTextColorAnimation(const UIAnimatedProperties<Rgba>& color);
	void SetBackgroundMarginColor(const Rgba& color);
	void SetBackgroundColor(const Rgba& color);
	void SetFillBarColor(const Rgba& color);
	void SetFont(Font* font);
	void SetFontSize(float size);
	void SetFontColor(const Rgba& color);
	void SetMarginImage(const Texture* tex);
	void SetBackgroundImage(const Texture* tex);
	void SetCheckedMarginImage(const Texture* tex);
	void SetCheckedBackgroundImage(const Texture* tex);
	void SetRadioBackgroundImage(const Texture* tex);
	void SetRadioSetBackgroundImage(const Texture* tex);
	void SetFillBarImage(const Texture* tex);
	void SetState(UIWidgetStates state);
	void SetState(int state);
	bool ConvertStringToStateType(const std::string& stateName);

	//Getters
	const UIAnimatedProperties<Rgba> GetBackgroundMarginColorAnimation() const;
	const UIAnimatedProperties<Rgba> GetBackgroundFillColorAnimation() const;
	const UIAnimatedProperties<Rgba> GetFontColorAnimation() const;
	const UIAnimatedProperties<Rgba>* GetPtrBackgroundMarginColorAnimation() const;
	const UIAnimatedProperties<Rgba>* GetPtrBackgroundFillColorAnimation() const;
	const UIAnimatedProperties<Rgba>* GetPtrFontColorAnimation() const;
	UIAnimatedProperties<Rgba>* GetEditablePtrBackgroundMarginColorAnimation();
	UIAnimatedProperties<Rgba>* GetEditablePtrBackgroundFillColorAnimation();
	UIAnimatedProperties<Rgba>* GetEditablePtrFontColorAnimation();
	float GetFontSize() const;
	bool GetHasMarginColorAnimation() const;
	bool GetHasFillColorAnimation() const;
	bool GetHasTextColorAnimation() const;
	Rgba GetBackgroundMarginColor() const;
	Rgba GetBackgroundColor() const;
	Rgba GetFillBarColor() const;
	Rgba GetFontColor() const;
	const Texture* GetMarginImage() const;
	const Texture* GetBackgroundImage() const;
	const Texture* GetCheckedMarginImage() const;
	const Texture* GetCheckedBackgroundImage() const;
	const Texture* GetRadioBackgroundImage() const;
	const Texture* GetRadioSetBackgroundImage() const;
	const Texture* GetFillBarImage() const;
	int GetState() const;
	Font* GetFont() const;
	UIWidgetStates GetStateAsWidgetState() const;

	//Operations
	const bool operator==(const UIStateAppearanceData& other) const;
};
#endif