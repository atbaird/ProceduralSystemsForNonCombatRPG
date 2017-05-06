#pragma once

#ifndef UIWIDGETREGISTRATIONDATAPACKET_HPP
#define UIWIDGETREGISTRATIONDATAPACKET_HPP
#include "Engine/UI/UIStructs/UIStyleSheet.hpp"

class UIWidgetBase;

struct UIWidgetRegistrationDataPacket
{
private:
protected:
	UIWidgetBase* m_UIWidgetParent = nullptr;
	UIStyleSheet* m_defaultStyleSheet = nullptr;
	size_t m_currentRootNodeID = 0;
public:
	//Constructors
	UIWidgetRegistrationDataPacket(UIWidgetBase* parent = nullptr, UIStyleSheet* defaultStyleSheet = nullptr, size_t currentRootNodeID = 0);
	virtual ~UIWidgetRegistrationDataPacket();

	//Setters
	void LoadDataValues(UIWidgetBase* parent);
	void CopyValuesOntoUIWidgetBase(UIWidgetBase* base) const;
	void SetCurrentRootNodeID(size_t rootNodeID);
	void SetDefaultStyleSheet(UIStyleSheet* styleSheet);

	//Getters
	UIWidgetBase* GetUIWidgetParent() const;
	UIStyleSheet* GetStyleSheet() const;
	size_t GetRootNodeID() const;
};
#endif