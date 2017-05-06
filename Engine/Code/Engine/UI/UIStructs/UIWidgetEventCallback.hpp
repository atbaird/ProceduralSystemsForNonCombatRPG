#pragma once

#ifndef UIWIDGETEVENTS_HPP
#define UIWIDGETEVENTS_HPP
#include <string>

class UIWidgetBase;

struct UIWidgetEventCallback
{
private:
protected:
	std::string m_EnterEventToUse = "";
	std::string m_ExitEventToUse = "";
	std::string m_UpdateEventToUse = "";
	std::string m_name = "default";
	bool m_updateEventSet = false;
	bool m_enterEventSet = false;
	bool m_exitEventSet = false;
public:
	//Constructors
	UIWidgetEventCallback(const std::string& eventSystemCallToUse = "", const std::string& exitEventSystemCallToUse = "",
		const std::string& enterEventSystemCallToUse = "", const std::string& name = "");
	virtual ~UIWidgetEventCallback();

	//Callback
	void CallUpdateEvent(UIWidgetBase* widget);
	void CallExitEvent(UIWidgetBase* widget);
	void CallEnterEvent(UIWidgetBase* widget);

	//Setters
	void ChangeUpdateEventSystemCallToUse(const std::string& eventSystemCallToUse);
	void ChangeExitEventSystemCallToUse(const std::string& eventSystemCallToUse);
	void ChangeEnterEventSystemCallToUse(const std::string& eventSystemCallToUse);
	void SetName(const std::string& name);

	//Getters
	const std::string GetEventSystemCallToUse() const;
	const std::string GetExitEventSystemCallToUse() const;
	const std::string GetEnterEventSystemCallToUse() const;
	const std::string GetName() const;
	bool GetUpdateEventIsSet() const;
	bool GetExitEventIsSet() const;
	bool GetEnterEventIsSet() const;
};

#endif // !UIWIDGETEVENTS_HPP