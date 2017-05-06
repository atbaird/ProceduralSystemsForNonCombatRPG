#include "Engine/UI/UIStructs/UIWidgetEventCallback.hpp"
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/EventSystem/EventSystem.hpp"

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIWidgetEventCallback::UIWidgetEventCallback(const std::string& eventSystemCallToUse,
	const std::string& exitEventSystemCallToUse,
	const std::string& enterEventSystemCallToUse,
	const std::string& name)
	: m_UpdateEventToUse(eventSystemCallToUse),
	m_ExitEventToUse(exitEventSystemCallToUse),
	m_EnterEventToUse(enterEventSystemCallToUse),
	m_name(name)
{
	if (m_UpdateEventToUse != "")
	{
		m_updateEventSet = true;
	}
	if (m_ExitEventToUse!= "")
	{
		m_exitEventSet = true;
	}
	if (m_EnterEventToUse != "")
	{
		m_enterEventSet = true;
	}
}

UIWidgetEventCallback::~UIWidgetEventCallback()
{
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Callback
void UIWidgetEventCallback::CallUpdateEvent(UIWidgetBase* widget)
{
	if (m_updateEventSet == false
		|| g_EventSystem == nullptr)
	{
		return;
	}
	NamedProperties params = NamedProperties();
	params.Set("widget", widget);
	g_EventSystem->FireEvent(m_UpdateEventToUse, params);
}


void UIWidgetEventCallback::CallExitEvent(UIWidgetBase* widget)
{
	if (m_exitEventSet == false
		|| g_EventSystem == nullptr)
	{
		return;
	}
	NamedProperties params = NamedProperties();
	params.Set("widget", widget);
	g_EventSystem->FireEvent(m_ExitEventToUse, params);
}

void UIWidgetEventCallback::CallEnterEvent(UIWidgetBase* widget)
{
	if (m_enterEventSet == false
		|| g_EventSystem == nullptr)
	{
		return;
	}
	NamedProperties params = NamedProperties();
	params.Set("widget", widget);
	g_EventSystem->FireEvent(m_EnterEventToUse, params);
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Setters
void UIWidgetEventCallback::ChangeUpdateEventSystemCallToUse(const std::string& eventSystemCallToUse)
{
	m_UpdateEventToUse = eventSystemCallToUse;
	if (m_UpdateEventToUse != "")
	{
		m_updateEventSet = true;
	}
	else
	{
		m_updateEventSet = false;
	}
}


void UIWidgetEventCallback::ChangeExitEventSystemCallToUse(const std::string& eventSystemCallToUse)
{
	m_ExitEventToUse = eventSystemCallToUse;
	if (m_ExitEventToUse != "")
	{
		m_exitEventSet = true;
	}
	else
	{
		m_exitEventSet = false;
	}
}

void UIWidgetEventCallback::ChangeEnterEventSystemCallToUse(const std::string& eventSystemCallToUse)
{
	m_EnterEventToUse = eventSystemCallToUse;
	if (m_EnterEventToUse != "")
	{
		m_enterEventSet = true;
	}
	else
	{
		m_enterEventSet = false;
	}
}

void UIWidgetEventCallback::SetName(const std::string& name)
{
	m_name = name;
}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//Getters
const std::string UIWidgetEventCallback::GetEventSystemCallToUse() const
{
	return m_UpdateEventToUse;
}

const std::string UIWidgetEventCallback::GetExitEventSystemCallToUse() const
{
	return m_ExitEventToUse;
}

const std::string UIWidgetEventCallback::GetEnterEventSystemCallToUse() const
{
	return m_EnterEventToUse;
}

const std::string UIWidgetEventCallback::GetName() const
{
	return m_name;
}

bool UIWidgetEventCallback::GetUpdateEventIsSet() const
{
	return m_updateEventSet;
}

bool UIWidgetEventCallback::GetExitEventIsSet() const
{
	return m_exitEventSet;
}

bool UIWidgetEventCallback::GetEnterEventIsSet() const
{
	return m_enterEventSet;
}