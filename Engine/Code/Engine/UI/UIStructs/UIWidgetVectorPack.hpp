#pragma once

#ifndef UIWIDGETVECTORPACK_HPP
#define UIWIDGETVECTORPACK_HPP
#include "Engine/Math/Vertex_PCT.hpp"
#include "Engine/UI/UIStructs/UIWidgetEventCallback.hpp"
#include <vector>

class UIWidgetBase;
struct UIWidgetVectorPack
{
private:
protected:
public:
	//variables
	std::vector<UIWidgetEventCallback> m_eventCallbacks;
	std::vector<std::string> m_callbackEventsToFire;
	std::vector<UIWidgetBase*> m_children;
	std::vector<Vertex_PCT> m_BackgroundModel;
	std::vector<Vertex_PCT> m_MarginModel;
	std::vector<unsigned int> m_BackgroundIndices;
	std::vector<unsigned int> m_MarginIndices;

	//constructors
	UIWidgetVectorPack();
	virtual ~UIWidgetVectorPack();
};

#endif